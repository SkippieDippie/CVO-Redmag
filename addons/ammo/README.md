# `ammo` addon: coverage and vanilla survey

All items and magazine mappings of CVO Redmag. No code. The schema is in
[docs/04-config.md](../../docs/04-config.md).

| File | Contents |
|------|----------|
| `CfgCVORedmag.hpp` | Registry: categories, calibers, bodies, cartridges |
| `CfgMagazines.hpp` | Includes `calibers/*.hpp`, plus the category packs |
| `calibers/<family>.hpp` | Vanilla magazine mappings and the packs of one caliber family |
| `CfgWeapons.hpp` | Empty items |
| `script_component.hpp` | `CVO_SINGLE`, `CVO_MIX`, `CVO_PREFERRED`, `CVO_PACK`, `CVO_EMPTY` macros |

The survey below was made with [`tools/config_generator`](../../tools/config_generator/README.md)
from the vanilla `weapons_f*` PBOs (Arma 3 2.18, all DLC).

## Coverage

Loads: "last 4" = `lastRoundsTracer = 4`, "1:4" = `tracersEvery = 4`. The tracers of a
full magazine are given in brackets. Each finish is its own body; a `_sand`, `_green`,
`_khaki` or `_black` body inherits the base body and takes the same loads.

| Family | Body | Type | Magazine classes | Loads |
|--------|------|------|------------------|-------|
| 5.56×45 | `stanag30` | mag, stripper | `30Rnd_556x45_Stanag{,_green,_red,_Tracer_*}` (6) | ball + last 4 (4): `Stanag`, also `_green`, `_red`; tracer: `Tracer_Red`, also green, yellow |
| | `stanag30_sand` | mag, stripper | `30Rnd_556x45_Stanag_Sand*` (6) | as `stanag30` |
| | `stanag_drum150` | mag | `150Rnd_556x45_Drum_Mag{,_Tracer}_F` | ball + last 4 (4); tracer |
| | `stanag_drum150_sand` | mag | `150Rnd_556x45_Drum_Sand_Mag{,_Tracer}_F` | as `stanag_drum150` |
| | `stanag_drum150_green` | mag | `150Rnd_556x45_Drum_Green_Mag{,_Tracer}_F` | as `stanag_drum150` |
| | `m249box200` | box | `200Rnd_556x45_Box_*` (4) | ball + 1:4 and last 4 (53): `Box_F`, also `Box_Red_F`; tracer: `Box_Tracer_F`, also `Box_Tracer_Red_F` |
| | `sdar20` | mag | `20Rnd_556x45_UW_mag` | dual purpose (no tracers) |
| 6.5×39 caseless | `mx30` | mag | `30Rnd_65x39_caseless_mag{,_Tracer}` | ball + last 4 (4); tracer |
| | `mx30_khaki` | mag | `30Rnd_65x39_caseless_khaki_mag{,_Tracer}` | as `mx30` |
| | `mx30_black` | mag | `30Rnd_65x39_caseless_black_mag{,_Tracer}` | as `mx30` |
| | `mx100` | mag | `100Rnd_65x39_caseless_mag{,_Tracer}` | ball + 1:4 and last 4 (28); tracer |
| | `mx100_khaki` | mag | `100Rnd_65x39_caseless_khaki_mag{,_tracer}` | as `mx100` |
| | `mx100_black` | mag | `100Rnd_65x39_caseless_black_mag{,_tracer}` | as `mx100` |
| | `katiba30` | mag | `30Rnd_65x39_caseless_green{,_mag_Tracer}` | ball + last 4 (4); tracer |
| | `msbs30` | mag | `30Rnd_65x39_caseless_msbs_mag{,_Tracer}` | ball + last 4 (4); tracer |
| | `mk200box200` | box | `200Rnd_65x39_cased_Box*` (4) | ball + 1:4 and last 4 (53): `cased_Box`, also `_Red`; tracer: `cased_Box_Tracer`, also `_Tracer_Red` |
| 6.5×39 cased | `cmr20` | mag | `20Rnd_650x39_Cased_Mag_F` | ball (no tracers) |

**Excluded** (`cvo_redmag_body = ""`, vanilla behaviour): the blank magazines
`30Rnd_556x45_Stanag_Blank`, `30Rnd_65x39_caseless_blank_mag`, `100Rnd_65x39_caseless_blank_mag`,
`30Rnd_65x39_caseless_msbs_blank_mag`, `30Rnd_65x39_caseless_green_blank_mag`,
`200Rnd_65x39_cased_box_blank`. They inherit from mapped classes.

**Preferred classes:** all tracer colours use one tracer cartridge, so classes that
only differ by colour share a *(body, load)*. Refilling an Empty produces the preferred one,
listed first in the table above: the base class the other colours inherit from, and
`Tracer_Red` for the plain STANAG tracers, which are siblings. A magazine of another colour
can still be found, looted or topped off, and unloads like the preferred one.

**Finishes** are told apart by `modelSpecial` and `hiddenSelectionsTextures`, not by class
name. The Katiba magazine uses the MX base texture, but it is its own body anyway
because it sits in another magazine well (`Katiba_65x39`).

## What the survey showed

- **Tracers are a magazine property.** Every vanilla bullet class has a tracer model (even
  `B_556x45_Ball` draws yellow). Which rounds show it comes from the magazine:
  `tracersEvery` (1 = all, ≥ 2 = one in *N*) and `lastRoundsTracer` (the last *L* rounds).
  The ammo class only gives the tracer colour, and ball classes often use tracer-coloured
  ammo (the SPAR drum, the Katiba, the LIM-85 box). Ammo is therefore only checked against
  the caliber.
- **Most "ball" magazines are mixes.** Almost every vanilla magazine has
  `lastRoundsTracer = 4`, so it holds 4 tracers at the bottom. The "reload tracer"
  classes (`30Rnd_556x45_Stanag_green` / `_red` and their Sand variants) only differ from the
  plain STANAG by the tracer colour, which is not a cartridge. Only the SDAR and CMR-76 magazines
  have no tracers at all.
- **Inheritance traps:** `20Rnd_556x45_UW_mag` inherits from the STANAG; the Katiba,
  Promet and Mk200 classes inherit from MX classes; the Mk200 box is named "cased" but fires
  caseless ammo. Every attribute is set explicitly on every class.

## Vanilla mixed classes

All vanilla infantry magazines with `tracersEvery ≥ 2`:

| Class | Ammo | Count | 1 tracer every | Magazine well / weapon | Mapped |
|-------|------|-------|----------------|------------------------|--------|
| `100Rnd_65x39_caseless_mag` (+ khaki, black) | `B_65x39_Caseless` | 100 | 4 | `MX_65x39_Large` | ✅ |
| `200Rnd_556x45_Box_F` | `B_556x45_Ball_Tracer_Yellow` | 200 | 4 | `M249_556x45` | ✅ |
| `200Rnd_556x45_Box_Red_F` | `B_556x45_Ball_Tracer_Red` | 200 | 4 | `M249_556x45` | ✅ |
| `200Rnd_65x39_cased_Box` | `B_65x39_Caseless_yellow` | 200 | 4 | `Mk200_65x39` | ✅ |
| `200Rnd_65x39_cased_Box_Red` | `B_65x39_Caseless` | 200 | 4 | `Mk200_65x39` | ✅ |
| `150Rnd_762x54_Box` | `B_762x54_Tracer_Green` | 150 | 5 | Zafir | M6 |
| `150Rnd_93x64_Mag` | `B_93x64_Ball` | 150 | 3 | Navid | M6 |
| `130Rnd_338_Mag` | `B_338_NM_Ball` | 130 | 5 | `SPMG_338` | M6 |

## Packs

| Pack | Round type | Rounds | Mass | Real-world reference |
|------|-----------|--------|------|----------------------|
| `cvo_redmag_pack_556_ball_box20` | `556_ball` | 20 | 6 | M855, 20-round carton (≈ 250 g) |
| `cvo_redmag_pack_556_ball_clip10` | `556_ball` | 10 | 3 | M855 on a 10-round stripper clip; 7 clips per bandolier, 840 rounds per M2A1 can |
| `cvo_redmag_pack_556_tracer_box20` | `556_tracer` | 20 | 6 | M856 tracer, 20-round carton; every tracer colour |
| `cvo_redmag_pack_556_dual_box20` | `556_dual` | 20 | 6 | Fictional; packed like ball |
| `cvo_redmag_pack_556x45_box20` / `_clip10` | `556x45` | 20 / 10 | 6 / 3 | As M855 |
| `cvo_redmag_pack_65_ball_box20` / `cvo_redmag_pack_65_tracer_box20` | `65_ball` / `65_tracer` | 20 | 6 | Fictional; 6.5 Grendel 20-round box, lighter without cases |
| `cvo_redmag_pack_65x39_box20` | `65x39` | 20 | 6 | As above |
| `cvo_redmag_pack_65c_ball_box20` / `cvo_redmag_pack_65x39_cased_box20` | 6.5 cased | 20 | 8 | 6.5 Grendel, 20-round box (≈ 360 g) |
| `cvo_redmag_pack_<category>_*` | categories | 50 / 30 / 20 / 10 / 25 | — | Gameplay sizes (no real-world packaging) |

Mass is in Arma units of about 45 g (ACE: 1 unit = 0.1 lb). Pack icons are placeholders
(vanilla magazine icons) until M6.
