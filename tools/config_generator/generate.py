#!/usr/bin/env python3
"""
CVO Redmag config generator (draft tool, see docs/04-config.md §6).

Reads game or mod configs offline and drafts CVO Redmag mappings for every magazine an
infantry weapon can load:
  - bodies:     grouped by magazine well (or weapon, if the magazine is in no well) + count
                + appearance (modelSpecial and hiddenSelectionsTextures, not the class name)
  - cartridges: grouped by CfgAmmo; tracersEvery == 1 marks a tracer cartridge
  - mixes:      tracersEvery >= 2 or lastRoundsTracer > 0 (ball + the tracer of its ammo)

Inputs are PBOs (unpacked and derapified with HEMTT, which must be on PATH) and/or folders
that already hold derapified config.cpp files.

Output: a Markdown survey and a draft config. The draft always needs human review: rename
IDs, merge interchangeable bodies, and pick preferred classes. The preInit validator then
checks the result in-game.

Usage:
  python tools/config_generator/generate.py --pbo "<Arma 3>/Addons/weapons_f.pbo" ... \
      --ammo "556x45|65x39" --out drafts/
"""
import argparse, glob, os, re, subprocess, sys, tempfile
from collections import OrderedDict, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from cfgparse import load, get  # noqa: E402

INFANTRY_BASES = ('rifle_base_f', 'pistol_base_f', 'rifle_long_base_f', 'rifle_short_base_f')


def derapify_pbos(pbos, workdir):
    for pbo in pbos:
        name = os.path.splitext(os.path.basename(pbo))[0]
        dest = os.path.join(workdir, name)
        subprocess.run(['hemtt', 'utils', 'pbo', 'unpack', pbo, dest], check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        for binf in glob.glob(os.path.join(dest, '**', 'config.bin'), recursive=True):
            subprocess.run(['hemtt', 'utils', 'config', 'derapify', binf, binf[:-4] + '.cpp'],
                           check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return workdir


def ancestry(section, name):
    out, node = [], section['_c'].get(name.lower())
    while node is not None and len(out) < 80:
        out.append(node['_n'])
        node = section['_c'].get((node['_p'] or '').lower())
    return out


def num(v, default=0):
    try:
        return int(float(v))
    except (TypeError, ValueError):
        return default


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--pbo', nargs='*', default=[], help='PBO files to read')
    ap.add_argument('--dir', nargs='*', default=[], help='folders with derapified config.cpp')
    ap.add_argument('--ammo', default='', help='regex on CfgAmmo class to limit the output')
    ap.add_argument('--out', default='drafts', help='output folder')
    a = ap.parse_args()

    with tempfile.TemporaryDirectory() as tmp:
        dirs = list(a.dir)
        if a.pbo:
            dirs.append(derapify_pbos(a.pbo, tmp))
        files = []
        for d in dirs:
            files += sorted(glob.glob(os.path.join(d, '**', 'config.cpp'), recursive=True))
        root = load(files)

    mags = root['_c'].get('cfgmagazines', {'_c': {}})
    weps = root['_c'].get('cfgweapons', {'_c': {}})
    wells = root['_c'].get('cfgmagazinewells', {'_c': {}})
    ammo_re = re.compile(a.ammo, re.I) if a.ammo else None

    # magazine -> wells, magazine -> weapons
    mag_wells = defaultdict(set)
    for w in wells['_c'].values():
        for arr in w['_v'].values():
            if isinstance(arr, list):
                for m in arr:
                    mag_wells[str(m).lower()].add(w['_n'])
    mag_weps = defaultdict(set)
    for w in weps['_c'].values():
        anc = [x.lower() for x in ancestry(weps, w['_n'])]
        if not any(b in anc for b in INFANTRY_BASES):
            continue
        if num(get(weps, w['_n'], 'scope')) < 2:   # skips hidden helpers such as FakeWeapon_*
            continue
        ms = [str(m).lower() for m in (get(weps, w['_n'], 'magazines') or [])]
        for wl in get(weps, w['_n'], 'magazinewell') or []:
            wn = wells['_c'].get(str(wl).lower())
            if wn:
                for arr in wn['_v'].values():
                    if isinstance(arr, list):
                        ms += [str(m).lower() for m in arr]
        for m in ms:
            mag_weps[m].add(w['_n'])

    rows = []
    for key, m in mags['_c'].items():
        if m.get('_ext') or key not in mag_weps:
            continue
        ammo = get(mags, m['_n'], 'ammo') or ''
        if ammo_re and not ammo_re.search(ammo):
            continue
        count = num(get(mags, m['_n'], 'count'))
        tev = num(get(mags, m['_n'], 'tracersevery'))
        last = num(get(mags, m['_n'], 'lastroundstracer'))
        wl = sorted(mag_wells.get(key, [])) or ['weapon:' + sorted(mag_weps[key])[0]]
        # Appearance (D35): the 3D model and its textures. The icon is not used, because it
        # changes with the tracer load.
        tex = get(mags, m['_n'], 'hiddenselectionstextures') or []
        look = (str(get(mags, m['_n'], 'modelspecial') or '').lower(),
                tuple(str(t).lower() for t in tex))
        rows.append(OrderedDict(cls=m['_n'], parent=m['_p'] or '', ammo=ammo, count=count,
                                tracersEvery=tev, lastRoundsTracer=last, wells=wl, look=look,
                                texture=os.path.basename(str(tex[0]).replace('\\', '/')) if tex else '',
                                weapons=sorted(mag_weps[key])))

    # proposals: one body per magazine well + count + appearance. The first appearance of a
    # well + count gets the plain ID; others get a suffix from their texture, to be renamed
    # (e.g. mx_65x39_30_xmx_base_khk_co -> mx30_khaki).
    bodies = OrderedDict()
    looks = defaultdict(list)
    for r in rows:
        base = (r['wells'][0] + '_' + str(r['count'])).lower().replace('weapon:', '')
        if r['look'] not in looks[base]:
            looks[base].append(r['look'])
        index = looks[base].index(r['look'])
        suffix = re.sub(r'\.paa$', '', r['texture'].lower()) or f'v{index + 1}'
        bid = base if index == 0 else f'{base}_{suffix}'
        r['body'] = bid
        bodies.setdefault(bid, {'count': r['count'], 'wells': r['wells'], 'classes': []})['classes'].append(r['cls'])
    # Every vanilla bullet draws a tracer in its ammo's colour; the magazine decides which
    # rounds show it (D34): tracersEvery 1 = all, >= 2 = one in N, lastRoundsTracer = last L.
    for r in rows:
        n, l = r['tracersEvery'], r['lastRoundsTracer']
        if n == 1:
            r['load'] = f"{r['ammo']}_tracer"
        elif n >= 2 or l > 0:
            r['load'] = f"{r['ammo']}_ball + {r['ammo']}_tracer / {n} / {l}"
        else:
            r['load'] = f"{r['ammo']}_ball"

    os.makedirs(a.out, exist_ok=True)
    # survey
    with open(os.path.join(a.out, 'survey.md'), 'w', encoding='utf-8') as f:
        f.write('| Class | Parent | Ammo | Count | tracersEvery | lastRoundsTracer | Wells | Proposed body | Proposed load |\n')
        f.write('|---|---|---|---|---|---|---|---|---|\n')
        for r in sorted(rows, key=lambda r: (r['body'], r['cls'])):
            f.write(f"| `{r['cls']}` | `{r['parent']}` | `{r['ammo']}` | {r['count']} | {r['tracersEvery']} | {r['lastRoundsTracer']} | "
                    f"{', '.join(r['wells'])} | `{r['body']}` | {r['load']} |\n")
    # draft CfgMagazines patch, parents first
    by = {r['cls'].lower(): r for r in rows}
    emitted, order = set(), []

    def emit(cls):
        k = cls.lower()
        if k in emitted or k not in by:
            return
        p = by[k]['parent']
        if p and p.lower() in by:
            emit(p)
        emitted.add(k)
        order.append(by[k])
    for r in rows:
        emit(r['cls'])
    with open(os.path.join(a.out, 'CfgMagazines.draft.hpp'), 'w', encoding='utf-8') as f:
        f.write('// DRAFT from tools/config_generator. Review IDs, bodies and preferred classes.\n')
        f.write('class CfgMagazines {\n')
        externals = sorted({r['parent'] for r in order if r['parent'] and r['parent'].lower() not in by})
        for e in externals:
            f.write(f'    class {e};\n')
        for r in order:
            f.write(f"    // ammo {r['ammo']}, count {r['count']}, tracersEvery {r['tracersEvery']}, "
                    f"lastRoundsTracer {r['lastRoundsTracer']}\n")
            f.write(f"    class {r['cls']}: {r['parent']} {{\n")
            f.write(f"        cvo_redmag_body = \"{r['body']}\";\n")
            f.write(f"        // load: {r['load']}\n")
            f.write('    };\n')
        f.write('};\n')
    print(f"{len(rows)} magazines, {len(bodies)} proposed bodies -> {a.out}")


if __name__ == '__main__':
    main()
