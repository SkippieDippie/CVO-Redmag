# 04 — Config Reference

All of CVO Redmag's knowledge about weapons comes from config, and all of it lives in the
**`ammo`** addon (vanilla) or a compat optional (other mods). Supporting another mod's
weapons means writing a config patch. There is **no SQF** involved.

## 1. Overview

```
CfgCVORedmag              ← registry: categories, calibers, bodies, cartridges
CfgMagazines             ← vanilla/mod magazines get cvo_redmag_body + load attributes
                           + CVO Redmag's own pack and belt magazine classes
CfgWeapons               ← Empty items (CBA_MiscItem)
CfgVehicles              ← Ammo cans
```

## 2. `CfgCVORedmag`

```cpp
class CfgCVORedmag {
    // Simple-mode round types
    class Categories {
        class intermediate { displayName = "Intermediate rounds"; packs[] = {"cvo_redmag_pack_intermediate_box30"}; };
        class rifle        { displayName = "Rifle rounds";        packs[] = {"cvo_redmag_pack_rifle_box20"}; };
        class pistol       { displayName = "Pistol rounds";       packs[] = {"cvo_redmag_pack_pistol_box50"}; };
        class antimateriel { displayName = "Anti-materiel rounds"; packs[] = {"cvo_redmag_pack_antimateriel_box10"}; };
        class shotgun      { displayName = "Shotgun shells";      packs[] = {"cvo_redmag_pack_shotgun_box25"}; };
    };

    // Basic-mode round types
    class Calibers {
        class 556x45 { displayName = "5.56×45 mm"; category = "intermediate"; packs[] = {"cvo_redmag_pack_556x45_box20", "cvo_redmag_pack_556x45_clip10"}; };
        class 545x39 { displayName = "5.45×39 mm"; category = "intermediate"; packs[] = {"cvo_redmag_pack_545x39_packet30"}; };
        class 762x39 { displayName = "7.62×39 mm"; category = "intermediate"; packs[] = {"cvo_redmag_pack_762x39_box20"}; };
        class 762x51 { displayName = "7.62×51 mm"; category = "rifle";        packs[] = {"cvo_redmag_pack_762x51_box20"}; };
        class 9x21   { displayName = "9×21 mm";    category = "pistol";       packs[] = {"cvo_redmag_pack_9x21_box50"}; };
    };

    class Bodies {
        class stanag30 {
            caliber   = "556x45";
            capacity  = 30;
            type      = "mag";           // "mag" | "box" | "belt"
            emptyItem = "cvo_redmag_empty_stanag30";
            stripperCompatible = 1;      // faster loading with stripper clips
        };
        class m249box200 {
            caliber   = "556x45";
            capacity  = 200;
            type      = "box";           // filled by loading a belt segment
            emptyItem = "cvo_redmag_empty_m249box200";
        };
    };

    // Complex-mode round types
    class Cartridges {
        class 556_ball {
            caliber     = "556x45";
            ammo        = "B_556x45_Ball";        // CfgAmmo; validated against mapped mags
            displayName = "5.56 mm Ball";
            // Real-world packaging: 20-round box, 10-round stripper clip
            packs[]     = {"cvo_redmag_pack_556_ball_box20", "cvo_redmag_pack_556_ball_clip10"};
        };
        class 556_tracer_red {
            caliber     = "556x45";
            ammo        = "B_556x45_Ball_Tracer_Red";
            displayName = "5.56 mm Tracer (Red)";
            packs[]     = {"cvo_redmag_pack_556_tracer_red_box20"};
        };
    };
};
```

Categories, calibers and cartridges each list their own `packs[]`, because each is a round
type in one ammo mode (see [02 §0](02-gameplay.md#0-ammo-modes)). A compat addon that adds a
new caliber only has to give it a `category` to make it work in Simple mode.

**IDs** are config class names. Compat addons can add bodies and cartridges to the existing
classes, which merge like any other config. IDs from compat addons should carry a prefix,
e.g. `rhs_pmag30`, to avoid collisions.

## 3. Mapping magazines: `CfgMagazines` attributes

```cpp
class CfgMagazines {
    class CA_Magazine;
    class 30Rnd_556x45_Stanag : CA_Magazine {
        cvo_redmag_body      = "stanag30";
        cvo_redmag_cartridge = "556_ball";
    };
    class 30Rnd_556x45_Stanag_Tracer_Red : 30Rnd_556x45_Stanag {
        cvo_redmag_cartridge = "556_tracer_red";       // body inherited
    };

    // Mixed load: filled from ball AND tracer rounds. Example class; the vanilla
    // mixed classes are identified in M1.
    class 200Rnd_556x45_Box_F : CA_Magazine {
        cvo_redmag_body             = "m249box200";
        cvo_redmag_cartridge        = "556_ball";        // ball part
        cvo_redmag_tracerCartridge  = "556_tracer_red";  // tracer part; presence makes it a mix
        // cvo_redmag_tracerEvery   = 5;                 // optional; defaults to the class's tracersEvery
    };
};
```

| Attribute | Required | Meaning |
|-----------|----------|---------|
| `cvo_redmag_body` | yes | Body ID. `""` excludes the class. |
| `cvo_redmag_cartridge` | yes | The cartridge, or the ball part of a mix. |
| `cvo_redmag_tracerCartridge` | no | The tracer part of a mix. Leave it out for a single-cartridge load. |
| `cvo_redmag_tracerEvery` | no | One tracer every *N* rounds. Defaults to the magazine's engine `tracersEvery`. A mix with no usable value (< 2) fails validation. |
| `cvo_redmag_preferred` | no | Breaks ties when two classes map to the same *(body, load)*. |

> ⚠️ **Inheritance pitfall:** subclasses inherit `cvo_redmag_*` attributes. A child magazine with a
> different `ammo` or `count` inherits a wrong mapping unless it is overridden. The preInit
> validator catches mismatches (wrong ammo, wrong count) and **skips** them with an RPT
> warning. To exclude a class explicitly, set `cvo_redmag_body = "";`. A child of a mixed
> class also inherits `cvo_redmag_tracerCartridge`; set it to `""` to make the child a
> single-cartridge load.

**Reverse lookup:** `bodyLoadToMag` is built from these attributes. If two magazine classes
map to the same *(body, load)*, the one with `cvo_redmag_preferred = 1` wins. Otherwise the
first one in config order wins and a warning is logged. Refilling always produces the
preferred class.

## 4. CVO Redmag item classes

### Empty items (`CfgWeapons`)

```cpp
class CfgWeapons {
    class CBA_MiscItem;
    class CBA_MiscItem_ItemInfo;
    class cvo_redmag_empty_stanag30 : CBA_MiscItem {
        scope = 2;
        author = "CVO Redmag";
        displayName = "Empty STANAG Magazine (30)";
        descriptionShort = "Empty 5.56 mm magazine. Refill via ACE self-interaction.";
        picture = "\cvo\cvo_redmag\addons\ammo\ui\stanag30_empty_ca.paa";
        model = "\A3\Weapons_F\MagazineProxies\mag_univ.p3d";
        class ItemInfo : CBA_MiscItem_ItemInfo { mass = 3; };
    };
};
```

### Packs (`CfgMagazines`)

A magazine class that no weapon accepts: it is not listed in any weapon's `magazines[]` or
`magazineWell[]`.

```cpp
class cvo_redmag_pack_556_ball_box20 : CA_Magazine {
    scope = 2;
    displayName = "5.56 mm Ball (box of 20)";
    ammo  = "B_556x45_Ball";
    count = 20;
    mass  = 6;                             // real-world weight, converted to Arma mass units (TBC)
    cvo_redmag_packType = "box";           // "box" | "packet" | "clip"
    cvo_redmag_cartridge = "556_ball";     // Complex-mode pack
};

class cvo_redmag_pack_556x45_box20 : cvo_redmag_pack_556_ball_box20 {
    displayName = "5.56×45 mm (box of 20)";
    cvo_redmag_cartridge = "";
    cvo_redmag_caliber   = "556x45";       // Basic-mode pack
};

class cvo_redmag_pack_intermediate_box30 : CA_Magazine {
    scope = 2;
    displayName = "Intermediate rounds (box of 30)";
    ammo  = "B_556x45_Ball";               // unused: packs are never fired
    count = 30;
    mass  = 8;
    cvo_redmag_packType = "box";
    cvo_redmag_category = "intermediate";  // Simple-mode pack
};
```

A pack sets **exactly one** of `cvo_redmag_cartridge`, `cvo_redmag_caliber` or
`cvo_redmag_category`, which says which round type it holds. Validation rejects packs with
none or more than one. Watch inheritance: a child pack must clear the attribute it inherits.

Pack sizes and weights follow real-world packaging where it exists (cartridge and caliber
packs). Category packs have no real-world equivalent and use gameplay sizes. The reference for each
pack (e.g. *M855, 10-round stripper clip*) is recorded in a comment next to the class.

### Belt segments (`CfgMagazines`)

```cpp
class cvo_redmag_belt_556_ball_tr5 : CA_Magazine {
    scope = 2;
    displayName = "5.56 mm Belt, Ball + Tracer 1:5 (up to 200)";
    ammo  = "B_556x45_Ball_Tracer_Red";
    count = 200;
    tracersEvery = 5;
    mass  = 25;
    cvo_redmag_isBelt = 1;
    cvo_redmag_cartridge       = "556_ball";
    cvo_redmag_tracerCartridge = "556_tracer_red";
};
```

Belt segments use the same load attributes as magazines. There is one segment class per load
that a box body of that caliber accepts. Belts are made from rounds alone, so there are no
link item classes.

For **belt-only weapons** (the magazine *is* the belt), map the weapon's own belt magazine
with a body of `type = "belt"`. Linking then produces that class directly, with no separate
segment class. A `belt` body has no `emptyItem`, so nothing is kept when it is emptied.

## 5. Ammo cans (`CfgVehicles`)

```cpp
class cvo_redmag_can_base : ReammoBox_F {
    scope = 1;
    cvo_redmag_isCan = 1;
    cvo_redmag_defaultRoundType = "";   // cartridge, caliber or category ID
    cvo_redmag_defaultRounds    = 0;
    maximumLoad = 0;                // no regular inventory; contents tracked by variable
    ace_cargo_size = 1;  ace_cargo_canLoad = 1;
    ace_dragging_canDrag = 1;  ace_dragging_canCarry = 1;
    class Attributes { /* Eden: round type (combo), rounds (edit), infinite (checkbox) */ };
};
class cvo_redmag_can_556_ball_840 : cvo_redmag_can_base {
    scope = 2;
    displayName = "M2A1 Ammo Can (5.56 mm Ball, 840)";   // real-world M2A1 load
    cvo_redmag_defaultRoundType = "556_ball";
    cvo_redmag_defaultRounds = 840;
};
```

Cans hold round types of any level. A can of cartridges works in every mode, because it
resolves up to the active one; a can of a caliber or category is inert in more detailed
modes. In Eden, the round type combo marks entries that are inert in the current
`cvo_redmag_core_ammo_mode`.

## 6. Compat patches

Each third-party mod gets an optional addon under `optionals/compat_<mod>/`:

```cpp
class CfgPatches {
    class cvo_redmag_compat_rhs {
        requiredAddons[] = {"cvo_redmag_ammo", "rhsusf_c_weapons"};
        skipWhenMissingDependencies = 1;   // loads only when RHS is present
        units[] = {}; weapons[] = {};
    };
};
```

**Workflow with `tools/config_generator`**
1. The generator runs in-game from the debug console. It iterates over `CfgMagazines`
   entries used by infantry weapons.
2. It groups them by `magazineWell` + `count`, which gives proposed bodies, and by `ammo`,
   which gives proposed cartridges. Classes with `tracersEvery ≥ 2` are flagged as proposed
   mixes. Then it emits a draft `config.cpp`.
3. A human reviews names, merges duplicate bodies (e.g. mod PMAGs onto vanilla `stanag30`
   if they are interchangeable) and commits the result.
4. The preInit validator confirms the mapping in-game.

## 7. Vanilla coverage checklist (`ammo` addon)

| Family | Bodies (initial list) |
|--------|----------------------|
| 5.56×45 | `stanag30`, `stanag30_mx`?, `m249box200` |
| 6.5×39 | `mx30`, `mx100` (box), `katiba30` |
| 5.8×42 | `cpt30` (Type 115 / CAR-95), `car95box100` |
| 7.62×51 | `mk18_20`, `spar17_20`, `mk1_20`, `navid150` (belt?) |
| 7.62×39 | `akm30`, `rpk75`? |
| 5.45×39 | `ak12_30`, `ak12_75`? |
| 9.3×64 / .338 / 12.7 | marksman and sniper magazines |
| Pistol/SMG | `p07_16`, `rook_17`, `acp45_11`, `vermin30`, `sting30`, `protector30`, … |

Entries marked `?` need checking against the game's config. The final list is produced by
the generator and recorded in `addons/ammo/README.md`.
