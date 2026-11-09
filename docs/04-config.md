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

    // Basic-mode round types. ammo[]: every CfgAmmo a magazine of this caliber may use (D30).
    class Calibers {
        class 556x45 {
            displayName = "5.56×45 mm"; category = "intermediate";
            ammo[] = {"B_556x45_Ball", "B_556x45_Ball_Tracer_Red", "B_556x45_Ball_Tracer_Green", "B_556x45_Ball_Tracer_Yellow", "B_556x45_dual"};
            packs[] = {"cvo_redmag_pack_556x45_box20", "cvo_redmag_pack_556x45_clip10"};
        };
        class 545x39 { displayName = "5.45×39 mm"; category = "intermediate"; packs[] = {"cvo_redmag_pack_545x39_box30"}; };
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
        // Another finish of the same magazine: inherits everything but the Empty.
        class stanag30_sand: stanag30 {
            emptyItem = "cvo_redmag_empty_stanag30_sand";
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
            ammo        = "B_556x45_Ball";        // CfgAmmo given to this cartridge's packs; must be in the caliber's ammo[]
            displayName = "5.56 mm Ball";
            // Real-world packaging: 20-round box, 10-round stripper clip
            packs[]     = {"cvo_redmag_pack_556_ball_box20", "cvo_redmag_pack_556_ball_clip10"};
        };
        // One tracer cartridge for every tracer colour (D36).
        class 556_tracer {
            caliber     = "556x45";
            ammo        = "B_556x45_Ball_Tracer_Red";
            displayName = "5.56 mm Tracer";
            packs[]     = {"cvo_redmag_pack_556_tracer_box20"};
        };
    };
};
```

**Tracer colour is not a cartridge.** Each caliber has one tracer cartridge, whatever
colour its magazines draw. The colour belongs to the magazine class, through its ammo.
Classes that only differ by tracer colour (red, green and yellow tracer STANAGs; the yellow
and red LIM-85 boxes) therefore map to the same *(body, load)*, and `cvo_redmag_preferred`
picks the one that refilling an Empty produces.

**A body includes its finish.** Magazines that look different are different bodies,
each with its own Empty, so a sand Empty only makes sand magazines and a khaki MX Empty only
khaki MX magazines. "Looks different" means another 3D model (`modelSpecial`) or other
textures (`hiddenSelectionsTextures`). Class names are not used, because their naming is
inconsistent, especially across mods, and the inventory icon is not used either, because it
changes with the tracer load. A colour variant inherits from the base body and only sets its
own `emptyItem`. Loads are not part of the body: the vanilla reload-tracer STANAGs look like
the plain one and share `stanag30`.

**Ammo is checked per caliber, not per cartridge.** In vanilla the tracer behaviour
comes from the magazine's `tracersEvery`, not from its ammo class: MX ball and tracer
magazines share `B_65x39_Caseless`, the Katiba ball magazine uses the green-tracer ammo, and
the LIM-85 ball box fires only `B_556x45_Ball_Tracer_Yellow` with `tracersEvery = 4`. The
cartridge is therefore taken from the mapping, and the validator only checks that the
magazine's ammo belongs to the cartridge's caliber.

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
    // Ball with the last 4 rounds tracer (engine: tracersEvery = 0, lastRoundsTracer = 4).
    class 30Rnd_556x45_Stanag : CA_Magazine {
        cvo_redmag_body             = "stanag30";
        cvo_redmag_cartridge        = "556_ball";    // ball part
        cvo_redmag_tracerCartridge  = "556_tracer";  // tracer part; presence makes it a mix
        cvo_redmag_preferred        = 1;             // wins over the _green / _red variants
    };
    // All tracer (tracersEvery = 1): a single tracer cartridge.
    class 30Rnd_556x45_Stanag_Tracer_Red : 30Rnd_556x45_Stanag {
        cvo_redmag_cartridge        = "556_tracer";  // body inherited
        cvo_redmag_tracerCartridge  = "";            // clears the inherited mix
        cvo_redmag_preferred        = 1;             // wins over the green and yellow tracers
    };
    // One tracer in 4 plus the last 4 (tracersEvery = 4, lastRoundsTracer = 4).
    class 200Rnd_556x45_Box_F : CA_Magazine {
        cvo_redmag_body             = "m249box200";
        cvo_redmag_cartridge        = "556_ball";
        cvo_redmag_tracerCartridge  = "556_tracer";
        // cvo_redmag_tracerEvery      = 4;   // optional; defaults to the class's tracersEvery
        // cvo_redmag_lastRoundsTracer = 4;   // optional; defaults to the class's lastRoundsTracer
    };
};
```

The `ammo` addon writes these with the `CVO_SINGLE(body, cartridge)` and
`CVO_MIX(body, ball, tracer)` macros, which set every attribute.

| Attribute | Required | Meaning |
|-----------|----------|---------|
| `cvo_redmag_body` | yes | Body ID. `""` excludes the class. |
| `cvo_redmag_cartridge` | yes | The cartridge, or the ball part of a mix. |
| `cvo_redmag_tracerCartridge` | no | The tracer part of a mix: the caliber's tracer cartridge, whatever the tracer colour (D36). Leave it out for a single-cartridge load. |
| `cvo_redmag_tracerEvery` | no | One tracer every *N* rounds (0 = none). Defaults to the magazine's engine `tracersEvery`. A mix with *N* = 1 or *N* < 0 fails validation. |
| `cvo_redmag_lastRoundsTracer` | no | The last *L* rounds are tracers. Defaults to the magazine's engine `lastRoundsTracer`. A mix with *N* = 0 and *L* = 0 has no tracers and fails validation. |
| `cvo_redmag_preferred` | no | Breaks ties when two classes map to the same *(body, load)*. Only counts on the class that sets it: children do not inherit it (D31). |

> ⚠️ **Inheritance pitfall:** subclasses inherit `cvo_redmag_*` attributes. A child magazine with a
> different `ammo` or `count` inherits a wrong mapping unless it is overridden. The preInit
> validator catches mismatches (wrong ammo, wrong count) and **skips** them with an RPT
> warning. To exclude a class explicitly, set `cvo_redmag_body = "";`. A child of a mixed
> class also inherits `cvo_redmag_tracerCartridge`; set it to `""` to make the child a
> single-cartridge load.
>
> Vanilla has several such cases: the SDAR magazine inherits from the STANAG, the Katiba,
> Promet and Mk200 magazines from MX magazines, and the blank magazines from the mapped
> classes. The `ammo` addon therefore sets every attribute on every class (the `CVO_SINGLE`
> and `CVO_MIX` macros) and excludes the blanks.

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
    cvo_redmag_packType = "box";           // "box" | "clip"
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
    cvo_redmag_tracerCartridge = "556_tracer";
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

**Workflow with `tools/config_generator`** (usage in its [README](../tools/config_generator/README.md))
1. The generator is a Python script that reads configs **offline**: it unpacks and
   derapifies the given PBOs with HEMTT, so the game does not need to run. It iterates over
   the `CfgMagazines` entries used by scope-2 infantry weapons (descendants of
   `Rifle_Base_F` / `Pistol_Base_F`).
2. It groups them by `magazineWell` + `count` + appearance (`modelSpecial` and
   `hiddenSelectionsTextures`), which gives proposed bodies, and by `ammo`, which gives
   proposed cartridges. The first appearance of a well and count gets the plain body ID, and
   other finishes get a suffix from their texture file, to be renamed. Classes with `tracersEvery = 1` are proposed as tracer
   cartridges, and classes with `tracersEvery ≥ 2` or `lastRoundsTracer > 0` as mixes. It writes a Markdown survey and
   a draft `CfgMagazines` patch with every class's real parent, in an order where parents
   come first.
3. A human reviews names (e.g. `mx_65x39_30_xmx_base_khk_co` → `mx30_khaki`), merges
   duplicate bodies (a mod's magazine that uses the same model and textures as a vanilla
   one belongs on the vanilla body) and commits the result.
4. The preInit validator confirms the mapping in-game.

## 7. Vanilla coverage checklist (`ammo` addon)

| Family | Bodies (initial list) |
|--------|----------------------|
| 5.56×45 | ✅ `stanag30` (+ `_sand`), `stanag_drum150` (+ `_sand`, `_green`), `m249box200` (box), `sdar20` |
| 6.5×39 caseless | ✅ `mx30` (+ `_khaki`, `_black`), `mx100` (+ `_khaki`, `_black`), `katiba30`, `msbs30`, `mk200box200` (box) |
| 6.5×39 cased | ✅ `cmr20` |
| 5.8×42 | `cpt30` (Type 115 / CAR-95), `car95box100` |
| 7.62×51 | `mk18_20`, `spar17_20`, `mk1_20`, `navid150` (belt?) |
| 7.62×39 | `akm30`, `rpk75`? |
| 5.45×39 | `ak12_30`, `ak12_75`? |
| 9.3×64 / .338 / 12.7 | marksman and sniper magazines |
| Pistol/SMG | `p07_16`, `rook_17`, `acp45_11`, `vermin30`, `sting30`, `protector30`, … |

✅ marks families that are mapped. Entries marked `?` need checking against the game's
config. The final list is produced by the generator and recorded in
[`addons/ammo/README.md`](../addons/ammo/README.md), together with the survey of vanilla
mixed classes.

`mx100` is a `mag`, not a `box`: the MX SW's 100-round magazine is refilled like any
other magazine, since caseless rounds cannot be belted.
