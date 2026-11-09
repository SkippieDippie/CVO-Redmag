# Config generator

Drafts CVO Redmag mappings from game or mod configs, offline. See
[docs/04-config.md §6](../../docs/04-config.md#6-compat-patches).

Requirements: Python 3 and [HEMTT](https://github.com/BrettMayson/HEMTT) on `PATH` (used to
unpack PBOs and derapify `config.bin`).

```bash
python tools/config_generator/generate.py --pbo "F:/SteamLibrary/steamapps/common/Arma 3/Addons/weapons_f.pbo" "F:/SteamLibrary/steamapps/common/Arma 3/Expansion/Addons/weapons_f_exp.pbo" --ammo "556x45|65x39" --out drafts
```

| Option | Meaning |
|--------|---------|
| `--pbo` | PBO files to read. Pass every PBO that defines the weapons, magazine wells and magazines you need, since classes are merged across them. |
| `--dir` | Folders that already hold derapified `config.cpp` files (faster for repeated runs). |
| `--ammo` | Regex on the CfgAmmo class, to limit the output to some calibers. |
| `--out` | Output folder (default `drafts`). |

Output:

- `survey.md`: every magazine an infantry weapon can load, with parent, ammo, count,
  `tracersEvery`, `lastRoundsTracer`, magazine wells and the proposed body and load.
  `tracersEvery = 1` is proposed as a tracer cartridge; `tracersEvery ≥ 2` or
  `lastRoundsTracer > 0` as a mix of ball and the tracer of the magazine's ammo.
- `CfgMagazines.draft.hpp`: a draft patch with each class's real parent, parents first.

Bodies are proposed per magazine well + count + appearance (`modelSpecial` and
`hiddenSelectionsTextures`). The first finish of a well and count gets the plain ID; other
finishes get their texture file as a suffix, e.g. `mx_65x39_30_xmx_base_khk_co`.

The draft is a starting point: rename the body and cartridge IDs, merge interchangeable
bodies, choose preferred classes, and use the `CVO_SINGLE` / `CVO_MIX` macros from
`addons/ammo/script_component.hpp`. The preInit validator checks the result in-game.

Keep drafts out of the repository: `drafts/` is not part of any addon.
