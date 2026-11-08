# 03 — Technical Design

## 1. Build & project layout

- **Build tool:** [HEMTT](https://github.com/BrettMayson/HEMTT) (`hemtt build` / `hemtt release`).
- **Structure:** three addons (PBOs). Code and data are kept apart: `core` holds all code
  and defines no items, and `ammo` holds all item and magazine config and contains no code.

```
cvo_redmag/
├── .hemtt/
│   └── project.toml              # name, prefix "cvo_redmag", version, signing
├── addons/
│   ├── main/                     # CfgPatches, script_macros.hpp, version, mod.cpp data
│   ├── core/                     # ALL code: functions, event handlers, CBA settings, ACE interactions
│   │   ├── CfgFunctions.hpp      # every function, one category per feature (see below)
│   │   ├── CfgVehicles.hpp       # config patch: hides ACE's repack and belt-linking actions
│   │   └── functions/
│   │       ├── init/             # fn_preInit.sqf (settings, lookups, validation), fn_postInit.sqf
│   │       ├── data/             # fn_buildLookups.sqf, fn_splitLoad.sqf, fn_isEligibleUnit.sqf, ...
│   │       ├── empties/          # fn_onReloaded.sqf, fn_addItemOrDrop.sqf, ...
│   │       ├── packs/  refill/  belts/  cans/  interaction/  arsenal/
│   │   └── ui/                   # ACE Arsenal tab icon
│   └── ammo/                     # ALL items and their ties to in-game magazines; no code
│       ├── CfgCVORedmag.hpp      # registry: categories, calibers, bodies, cartridges
│       ├── CfgMagazines.hpp      # load attributes on vanilla magazines + pack and belt classes
│       ├── CfgWeapons.hpp        # Empty items
│       ├── CfgVehicles.hpp       # ammo cans (Eden attributes, ACE cargo/drag properties)
│       ├── calibers/             # one .hpp per caliber family (556x45.hpp, 65x39.hpp, ...)
│       └── ui/                   # item icons
├── optionals/
│   ├── compat_rhs/               # skipWhenMissingDependencies = 1
│   └── compat_cup/
├── tools/
│   └── config_generator/         # dev-time script: proposes body/cartridge config for a mod
├── docs/
└── include/                      # \z\ace, \x\cba headers for HEMTT includes
```

| Addon | Contents | `requiredAddons` |
|-------|----------|------------------|
| `main` | CfgPatches, shared macros (`script_mod.hpp`, `script_macros.hpp`), version | `cba_main`, `ace_main` |
| `core` | All functions and event handlers, CBA settings, ACE interaction menus (self and on cans), inventory context menu entries, ACE Arsenal tab and filtering, the patch that disables ACE Magazine Repack and ACE assisted belt linking, server-side can state | `cvo_redmag_main`, `ace_common`, `ace_interact_menu`, `ace_interaction`, `ace_cargo`, `ace_dragging`, `ace_arsenal`, `ace_magazinerepack`, `ace_reload` |
| `ammo` | The `CfgCVORedmag` registry, load attributes on vanilla magazines, Empty items, packs, belt segments, ammo cans | `cvo_redmag_main`, plus the vanilla load-order addon so magazine patches apply after vanilla config |

`ammo` does not depend on `core`, and `core` does not depend on `ammo`. `core` reads the
registry at `preInit`, after all config is loaded, so whatever `ammo` and the compat
optionals define is picked up. ACE actions on ammo cans are added by `core` in script
(`ace_interact_menu_fnc_addActionToClass` on `cvo_redmag_can_base`), so all interaction
wiring stays in one place.

**Functions** are declared with the engine's **`CfgFunctions`**, not CBA's `PREP` macros or
Extended Event Handlers (XEH). The tag is `cvo_redmag_core`, so every function is
`cvo_redmag_core_fnc_<name>`. Each feature is one category with its own folder, and the
engine finds the file as `functions\<category>\fn_<name>.sqf`:

```cpp
class CfgFunctions {
    class cvo_redmag_core {                       // tag → cvo_redmag_core_fnc_*
        class init {
            file = "\z\cvo_redmag\addons\core\functions\init";
            class preInit  { preInit  = 1; };     // CBA settings, buildLookups, validation
            class postInit { postInit = 1; };     // player event handlers, can actions, server events
        };
        class data {
            file = "\z\cvo_redmag\addons\core\functions\data";
            class buildLookups {};
            class splitLoad {};
            class resolveRoundType {};
            class loadCost {};
            class isEligibleUnit {};
        };
        class empties {
            file = "\z\cvo_redmag\addons\core\functions\empties";
            class onReloaded {};
            class onMagazineUnloaded {};
            class findUnloadedMagazine {};
            class addItemOrDrop {};
        };
        // packs, refill, belts, cans, interaction, arsenal: same pattern
    };
};
```

- `preInit = 1` and `postInit = 1` replace the XEH init scripts. There is no
  `CfgEventHandlers` / `XEH_*` in the project.
- CBA settings are registered with `CBA_fnc_addSetting` from `fn_preInit.sqf`. *To verify in
  M0:* settings registered this way show up and sync correctly on a dedicated server.
- CBA functions (settings, events, player event handlers) are still **called**, and CBA's
  **script macros** are used throughout (`GVAR`, `QGVAR`, `FUNC`, `QFUNC`, `EFUNC`, ...).
  Only CBA's function compilation (`PREP`) and XEH init are not used.
- `main` provides `script_mod.hpp` (`PREFIX cvo_redmag`) and `script_macros.hpp`
  (includes `\x\cba\addons\main\script_macros_common.hpp`). Each addon sets
  `COMPONENT` in its own `script_component.hpp`, and every `.sqf` file includes it.
- The macros and `CfgFunctions` agree on names: in `core`, `FUNC(splitLoad)` expands to
  `cvo_redmag_core_fnc_splitLoad`, which is exactly the name `CfgFunctions` creates with the
  tag `cvo_redmag_core`. The `file` paths in `CfgFunctions.hpp` can be written with
  `QPATHTOF(functions\data)`.
- Settings are named `cvo_redmag_core_<feature>_<name>`.

**Licence:** the mod is released under the
[GNU General Public License v2.0 (GPL-2.0)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html),
the same licence as CBA_A3 and ACE3.
- The full licence text goes in `LICENSE` at the repository root, and `mod.cpp` and the
  Workshop page name the licence.
- Because the licences match, code from CBA_A3 or ACE3 may be reused or adapted. Reused code
  keeps its original copyright and author credit in the file header.
- The GPL requires the source to be available to anyone who receives the mod. The public
  repository satisfies this; the Workshop page links to it.
- Third-party assets (icons, models, sounds) are only used if their licence allows
  distribution under the GPL.

---

## 2. Data model

The runtime concepts are categories, calibers, bodies, cartridges and loads. They are all defined in
config (schema in [04](04-config.md)) and loaded into hashmaps at `preInit` (from `fn_preInit.sqf`) by
`cvo_redmag_core_fnc_buildLookups`.

```
Category "intermediate"                         ← Simple-mode round type
Caliber "556x45"  (category "intermediate")     ← Basic-mode round type
  ├── Body  "stanag30"   (capacity 30, emptyItem "cvo_redmag_empty_stanag30", type "mag")
  ├── Body  "m249box200" (capacity 200, type "box")
  ├── Cartridge "556_ball"       (ammo "B_556x45_Ball")             ← Complex-mode round types
  └── Cartridge "556_tracer_red" (ammo "B_556x45_Ball_Tracer_Red")

MagazineClass "30Rnd_556x45_Stanag"            → (stanag30,   load 556_ball)
MagazineClass "30Rnd_556x45_Stanag_Tracer_Red" → (stanag30,   load 556_tracer_red)
MagazineClass "200Rnd_556x45_Box_F" (example)  → (m249box200, load 556_ball+556_tracer_red/5)
PackClass     "cvo_redmag_pack_556_ball_box20" → (556_ball, 20)
BeltClass     "cvo_redmag_belt_556_ball_tr5"   → (load 556_ball+556_tracer_red/5, belt)
```

### Loads

A load is stored as `[cartId, tracerCartId, tracerEvery]`:

- single cartridge: `["556_ball", "", 0]`;
- mix: `["556_ball", "556_tracer_red", 5]`.

Its **load key** is a string used in hashmap keys: `"556_ball"` or `"556_ball+556_tracer_red/5"`.
The split rule from [02 §3.1](02-gameplay.md#31-mixed-magazines) lives in one pure function,
`cvo_redmag_core_fnc_splitLoad`:

```
[load, count] → [[cartId, n], [tracerCartId, m]]
```

Every planner (refill, top-off, unload, belts) uses it, so they all agree.

### Ammo modes and round types

Every cartridge belongs to one caliber, and every caliber to one category, so each cartridge
has a round type at all three levels:

```
cartridge "556_ball"  →  caliber "556x45"  →  category "intermediate"
   Complex                  Basic                 Simple
```

- `FUNC(resolveRoundType)`: `[roundType] → roundType or ""`. Maps any round type up to the
  active mode's level. A cartridge in Basic mode becomes its caliber; a caliber in Complex
  mode cannot be resolved down and returns `""` (inert).
- `FUNC(loadCost)`: `[load, count] → [[roundType, n], ...]`. Runs `splitLoad`, resolves each
  part, and merges equal round types. In Basic and Simple mode a mixed load therefore
  collapses to one entry. **All planners use `loadCost`**, so they never need to know the mode.
- The mode is read once, after CBA settings are final (`CBA_settingsInitialized`), into
  `GVAR(ammoMode)`. The setting is registered with *needs mission restart*, so it cannot
  change mid-mission and the lookups never need rebuilding.

All three levels are built at preInit regardless of mode, so the active mode costs nothing
extra at runtime: resolving is one hashmap lookup per level.

### Lookup tables (`cvo_redmag_core_*`, global, read-only after preInit)

| Hashmap | Key | Value |
|---------|-----|-------|
| `magToBodyLoad` | magazine class (lowercase) | `[bodyId, load]` |
| `bodyLoadToMag` | `bodyId + ":" + loadKey` | magazine class |
| `bodyLoads` | bodyId | `[load, ...]`: every load that has a magazine class for this body |
| `bodies` | bodyId | `[caliber, capacity, emptyItem, type, flags]` |
| `cartridges` | cartId | `[caliber, cfgAmmo, displayName, packClasses[]]` |
| `calibers` | caliber | `[category, displayName, packClasses[]]` |
| `categories` | category | `[displayName, packClasses[]]` |
| `roundTypeLevel` | round type | `"cartridge"`, `"caliber"` or `"category"` |
| `emptyToBody` | empty item class | bodyId |
| `packToRound` | pack class | `[roundType, capacity]` |
| `beltToLoad` | belt segment class | `[load, capacity]` |

At preInit, **validation** logs a warning (and skips the entry) when:

- a mapped magazine class doesn't exist;
- its `count` doesn't match the body capacity;
- its `ammo` doesn't match the cartridge's `CfgAmmo` (for a mix: the ball or the tracer
  cartridge's `CfgAmmo`);
- a mix has `tracerEvery < 2`, or its two cartridges have different calibers;
- a `mag` or `box` body has no empty item (a `belt` body has none by design);
- a caliber has no category, or a cartridge, caliber or category has no pack class (that
  round type would have no loose-round item in its mode).

This catches broken compat patches early.

---

## 3. Engine hooks

### 3.1 Empty retention: `Reloaded` and `MagazineUnloaded` event handlers

Two engine event handlers catch an emptied magazine leaving a weapon:

- `Reloaded`: the player reloads over an empty magazine.
- `MagazineUnloaded`: the player drags a magazine out of a weapon in the inventory screen.
  `Reloaded` does not fire in this case. Handled in §3.1.1.

Both handlers are added **only to the player's own unit**, never as class event handlers on
`CAManBase`. AI therefore costs nothing. CBA's player event handler
(`CBA_fnc_addPlayerEventHandler ["unit", ...]`) moves them to the new unit on
respawn or team switch.

**Eligibility** is checked in one place, `cvo_redmag_core_fnc_isEligibleUnit`. It is used
by the event handler and by every interaction condition:

```sqf
params ["_unit"];
// Local, a player, and not a unit a Zeus is remote-controlling.
local _unit && {[_unit, true] call ace_common_fnc_isPlayer}   // true = exclude remote-controlled
```

> To verify in M2: when a Zeus remote-controls a unit, CBA's `"unit"` player event may fire
> for the controlled unit. The eligibility check must reject it, and retention must resume
> when control returns to the player's own unit.

```sqf
params ["_unit", "_weapon", "_muzzle", "_newMagazine", "_oldMagazine"];
// _oldMagazine = [class, ammoCount, magazineId, creator] or [] if none
if (_oldMagazine isEqualTo [] || {(_oldMagazine # 1) > 0}) exitWith {};
if !([_unit] call FUNC(isEligibleUnit)) exitWith {};
private _entry = GVAR(magToBodyLoad) getOrDefault [toLowerANSI (_oldMagazine # 0), []];
if (_entry isEqualTo []) exitWith {};
[_unit, (GVAR(bodies) get (_entry # 0)) # 2] call FUNC(addItemOrDrop);
```

- `addItemOrDrop`: if `_unit canAdd _item`, add it with `addItem`, which uses the engine's
  order (uniform → vest → backpack). If nothing has room, the setting
  `noSpace` decides: `drop` puts the Empty where `CBA_fnc_addMagazine` would put a magazine
  (vehicle cargo when in a vehicle, otherwise a weapon holder on the ground), and `delete`
  discards it. Empties are items, not magazines, so they cannot go through
  `CBA_fnc_addMagazine`. *To check in M2:* whether `CBA_fnc_addItem` offers the same
  `_verify` fallback for items; if it does, use it here too.
- **Edge cases to verify in testing:**
  - ACE "quick reload" paths;
  - weapons with multiple muzzles (the UGL muzzle is ignored because it has no body);
  - `reload` triggered by script;
  - magazines emptied while the weapon is dropped;
  - one emptied magazine firing both handlers, e.g. when the player reloads by dragging a new
    magazine onto the weapon in the inventory screen. Handled magazine IDs are kept for a few
    seconds and a second event for the same ID is ignored.

#### 3.1.1 Manual unload: `MagazineUnloaded`

```sqf
params ["_unit", "_weapon", "_muzzle", "_magazine"];
_magazine params ["_class", "_ammoCount", "_magazineId", "_creator"];
```

Per the wiki, the event fires when a magazine is removed from a weapon **manually or by a
script**. `_magazineId` is the global magazine ID and `_creator` the owner of the magazine's
creator; the count method below does not need either.

**Script unloads are ignored.** The handler only acts while the player has the inventory
screen open (a baseline exists, see below). An unload by a script (another mod, a mission)
happens with the screen closed: the script decides what happens to the magazine, and
counting nearby containers without a baseline could mistake an unrelated 0-round magazine
on the ground for this one. CVO Redmag's own code never unloads a weapon.

Only 0-round magazines of a mapped body are handled; partial ones are left to the engine.
Where the player drops the empty magazine decides what the engine does with it:

| Unloaded into | Engine | CVO Redmag |
|---------------|--------|------------|
| Uniform, vest or backpack | Deletes the 0-round magazine | Adds the Empty to the player (`addItemOrDrop`) |
| Ground, box, vehicle or other container | Keeps the 0-round magazine there | Replaces the 0-round magazine with the Empty, in that container |

The handler cannot tell from its arguments where the magazine went. No command returns
magazine IDs for a container's cargo: `magazinesDetail` returns nothing for boxes and ground
holders (tested in-game) and only the gunner's weapon magazines for vehicles. The handler
therefore **counts 0-round magazines** in the containers the magazine could have gone to,
and compares with a count taken earlier.

`FUNC(findUnloadedMagazine)` `[unit, class] → container or objNull`:

1. **Baseline.** When the inventory screen opens (`InventoryOpened`), store, for each
   candidate container, how many 0-round magazines of each mapped class its cargo holds,
   from `magazinesAmmoCargo _container`, which lists 0-round magazines too (tested in-game).
   `InventoryClosed` clears the baseline.
2. **Compare.** On `MagazineUnloaded` (one frame later, see *Timing*), count 0-round
   magazines of the unloaded class in each candidate container again. A container with one
   more than its baseline is where the magazine went. A container with no baseline (e.g. a
   ground holder the engine created during the drag) counts as a baseline of 0.
3. **Act:**
   - **Found:** the engine kept it. Remove it with `[_container, _class, 0] call CBA_fnc_removeMagazine`,
     which removes one 0-round magazine of that class from the container's cargo
     (`addMagazineAmmoCargo [_class, -1, 0]`), and put the Empty in the same container with
     `_container addItemCargoGlobal [_emptyItem, 1]`.
   - **Not found:** the engine deleted it, because it went into the uniform, vest or
     backpack. Add the Empty to the player with `addItemOrDrop`.
4. **Keep the baseline valid.** Every 0-round magazine that arrives is replaced straight
   away, so the stored counts stay correct for the next unload while the screen is open.

`CBA_fnc_removeMagazine` removes by class and round count, not by ID. If the container
already held another 0-round magazine of the same class, either one may be removed; they are
identical, so the result is the same.

**Limitation:** counting is not tied to the magazine's ID. If another player drops a
0-round magazine of the same class into the same container between the baseline and the
check, it can be mistaken for ours. The magazines are identical, so at worst one player gets
an Empty they should not have and another 0-round magazine stays in the container. This is
accepted.

`magazinesDetail` and `CBA_fnc_getMagazineIndex` are **not** used for this.

**Candidate containers:**

1. The containers of the inventory screen that is open, from the `InventoryOpened`
   arguments: a box, a vehicle, or the ground holder the screen opened with.
2. Weapon holders within a few metres of the player (`nearestObjects`), for a ground holder
   created while the screen was already open.
3. The player's vehicle (`objectParent`), if any.

**Timing:** the magazine may not be in its new container yet when the event fires, so the
check runs one frame later (`CBA_fnc_execNextFrame`). *To confirm in M2.*

**To verify in M2:**
- whether a magazine dragged onto a backpack or vest lying in a box goes into that nested
  container (`everyContainer`) rather than the box's own cargo;
- that removing the magazine and adding the Empty works on a remote container (a box or
  vehicle owned by another machine), since both commands have global effect.

### 3.2 Inventory manipulation

The engine cannot edit the round count of one specific magazine in place, so a changed
magazine is removed and re-added. Removing **only the magazines that change**, one at a
time, keeps every other magazine where it is:

1. Snapshot `magazinesAmmoFull _unit`, filtered to classes involved.
2. Compute the new state in pure SQF: which magazines go away (class + round count) and which
   are added (class + round count). Unchanged magazines are not in the plan.
3. **Remove** each magazine that changes with
   [`CBA_fnc_removeMagazine`](https://github.com/CBATeam/CBA_A3/blob/master/addons/common/fnc_removeMagazine.sqf)
   `[_unit, _class, _ammo]`. With a round count given, it finds one magazine with that class
   and count in the uniform, vest or backpack and removes just that one with
   `addMagazineAmmoCargo [_class, -1, _ammo]` (a negative quantity removes). The rest of the
   inventory is not reshuffled, unlike `removeMagazines`, which removes every magazine of the
   class. The round count must always be passed: without it the function falls back to
   `removeMagazineGlobal`, which can remove a different magazine of the class.
4. **Add** each new magazine with
   [`CBA_fnc_addMagazine`](https://github.com/CBATeam/CBA_A3/blob/master/addons/common/fnc_addMagazine.sqf)
   `[_unit, _class, _ammo, true, true]`:
   - When there is room, it uses the engine's `addMagazine`, which places the magazine in
     the usual order (uniform → vest → backpack).
   - With `_verify = true` and a full inventory, it puts the magazine in the **vehicle's
     cargo** if the player is in a vehicle, or in a weapon holder on the ground within 2 m
     (reusing a nearby one or creating one).
   - `_skipAnim = true` stops it from playing its pick-up animation with `switchMove`.
   - The round count must always be passed; `-1` would add a full magazine.
5. Loaded magazines (in a weapon) are never touched.
6. Emit CBA event `cvo_redmag_inventoryChanged` (local) for UI refresh and for other mods.

`CBA_fnc_removeMagazine` searches the uniform first. If two identical magazines (same class
and round count) sit in different containers, it may remove the one in the uniform rather
than the one in the vest. The magazines are identical, so only the position of the gap
differs. This is accepted.

Snapshot → compute → apply is done **once at the end** of an action, or on interrupt.
Nothing is applied per round, which avoids inventory churn and network traffic. The
computation is a pure function (`cvo_redmag_core_fnc_planRefill`), so it can be unit-tested
outside the game.

**Container side** (ammo cans hold a count variable, not cargo). For crates and vehicles
holding packs (v1.1), the same technique works: `CBA_fnc_removeMagazine` on a vehicle removes
one magazine of a given class and count from its cargo with a negative
`addMagazineAmmoCargo`, so the cargo is never cleared and re-filled. This runs on the server,
under a per-container lock (see §4), so two players cannot take the same pack.

### 3.3 Timed actions

`ace_common_fnc_progressBar` with:

- a condition function (see [02 §3](02-gameplay.md#3-refilling-magazines));
- an `onSuccess` that applies the full plan;
- an `onFailure` that applies a partial plan computed from `elapsed / timePerRound`.

Animations are **out of scope for v1**. Actions use the ACE progress bar with no custom
animation.

---

## 4. Multiplayer & locality

| Operation | Runs on | Why |
|-----------|---------|-----|
| Retention EHs (`Reloaded`, `MagazineUnloaded`) | Player's machine, on the player's own unit only | Both fire where the unit is local; `addItem` is local-arg, global-effect. No handlers on AI, the server or headless clients. |
| Refill / unload / packs / belts on self | Player's machine | Inventory commands need a local unit; the effects replicate. |
| Ammo can count | **Server authoritative** | `cvo_redmag_can_rounds` is set by the server with `setVariable [.., true]`. |
| Take from / return to can | Client asks with CBA `serverEvent "cvo_redmag_can_request"` → server checks the count, deducts it, replies with `targetEvent` | Prevents two players taking the same rounds. |
| Refill from can | Server **reserves** rounds up front. On cancel, the unused rounds are returned. A mixed load reserves from both cans in one request, and fails as a whole if either is short. The second can is the nearest one holding the other cartridge within 5 m of the player. | The progress bar takes time, so the reservation stops other players taking those rounds mid-fill. |
| Container cargo edits (v1.1) | Server, with lock variable `cvo_redmag_busy` on the container | Checking the cargo and removing a pack must not interleave between two players. |

**Cans at mission start.** Once the ammo mode is known (`CBA_settingsInitialized`), the
server converts every can's contents to the active round type with `resolveRoundType`. Cans
whose round type cannot be resolved (more generic than the mode) are left unchanged and
inert. Cans created later (Zeus, scripts) are converted by `setContents`.

**Reservations expire.** A reservation is returned to the can if the client does not confirm
it within the progress bar's duration plus a margin, or if the player disconnects
(`HandleDisconnect` on the server). Rounds are never lost because a player crashed mid-refill.

JIP needs nothing extra: all player state is engine items, and can counts are public
variables.

---

## 5. ACE integration

| ACE component | Use |
|---------------|-----|
| `ace_interact_menu` | All actions. Self-menu path `ACE_SelfActions > ACE_Equipment > cvo_redmag_ammo`. Object actions go on cans. Children are built dynamically with `insertChildren`, based on the current inventory. |
| `ace_common` | `progressBar`, `displayTextStructured` hints, `canInteractWith`. |
| ACE Magazine Repack (`ace_magazinerepack`) | **Disabled and replaced** by CVO Redmag's repack, because ACE deletes the magazines it empties. See §5.2. |
| ACE belt linking (`ace_reload`) | **Disabled**: ACE's assisted linking uses up the assistant's magazine and would destroy box bodies. See §5.3. ACE's *Check ammo* actions from the same component are untouched. |
| ACE weight | Comes for free from `mass` on items and magazines. |
| `ace_cargo` / `ace_dragging` | Cans are draggable, carryable and loadable as cargo, through config properties. |
| `ace_arsenal` | Custom right-panel tab with all CVO Redmag items for the active mode, and hiding of packs from other modes. See §5.1. |

**Belt filter** ([02 §5.1](02-gameplay.md#51-which-belts-the-menu-offers)):
`FUNC(getLinkableBelts)` collects the weapons of `units group _player`, takes the union of
their compatible magazines (`compatibleMagazines`, which includes magazine wells), and maps
it through `magToBodyLoad` and `beltToLoad` to a list of belt segments and box loads. The
result is cached per player and rebuilt only when the group's membership or loadouts change
(CBA `"loadout"` player event, `Take`/`Put`, group change), or after a short timeout, so that
squadmates' weapon swaps are picked up.

**Menu performance:** use cached condition results with `ace_interact_menu` caching and
inventory-change events to invalidate them. Never scan the inventory on every menu frame.

### 5.1 ACE Arsenal

Both parts run once, after `CBA_settingsInitialized`, when the ammo mode is known
(`arsenal` category in `CfgFunctions`).

- **Tab:** `ace_arsenal_fnc_addRightPanelButton` registers one tab with the mod's icon
  (`core/ui/arsenal_tab_ca.paa`) and the item list from `FUNC(getArsenalItems)`: every Empty,
  every belt segment, and the packs whose round type is at the active mode's level.
- **Hiding:** packs from other levels are removed from the lists ACE Arsenal shows. The hook
  must use ACE's public events and functions, not its internal variables, which change
  between ACE versions. Candidates are `ace_arsenal_fnc_removeVirtualItems` on the arsenal
  object when it is opened, or removing rows in the `ace_arsenal_rightPanelFilled` event.
  *To decide in M6* by testing both against full and limited arsenals.
- Hidden packs keep `scope = 2`: hiding happens at runtime, because config cannot depend on
  a setting. They therefore still appear in the vanilla Arsenal and in Eden, which is
  accepted.

### 5.2 Replacing ACE Magazine Repack

ACE offers repacking in two places. Both are removed, and CVO Redmag adds its own entries in
the same places ([02 §4.1](02-gameplay.md#41-repacking-magazines)).

**1. ACE self-interaction.** ACE defines it as
`CfgVehicles >> CAManBase >> ACE_SelfActions >> ACE_RepackMagazines` with
`condition = "true"`. `core/CfgVehicles.hpp` overrides the condition, and `core` lists
`ace_magazinerepack` in `requiredAddons` so the patch loads after ACE's config:

```cpp
class CfgVehicles {
    class Man;
    class CAManBase: Man {
        class ACE_SelfActions {
            class ACE_RepackMagazines {
                condition = "false";   // replaced by CVO Redmag's repack
            };
        };
    };
};
```

**2. Inventory context menu.** ACE registers a CBA item context menu option on `"#Magazine"`
in its `XEH_postInit`, with the display name `STR_ace_magazinerepack_RepackMagazines`.
`fn_postInit` removes it on clients (`hasInterface`), one frame later so that ACE has
registered it first:

```sqf
// Remove ACE's option for repacking magazines from the inventory context menu.
// The option ID is generated by CBA at registration, so it is looked up by display name.
[{
    private _options = (missionNamespace getVariable ["cba_ui_ItemContextMenuOptions", createHashMap]) getOrDefault ["#Magazine", createHashMap];
    private _id = "";

    {
        // _y = [slots, displayName, ...]
        if ((_y select 1) == "STR_ace_magazinerepack_RepackMagazines") exitWith {
            _id = _x;
        };
    } forEach _options;

    if (_id == "") exitWith {
        WARNING("ACE inventory repack option not found, cannot remove it");
    };

    if !(["#Magazine", _id] call CBA_fnc_removeItemContextMenuOption) then {
        WARNING_1("Failed to remove ACE inventory repack option (id %1)",_id);
    };
}] call CBA_fnc_execNextFrame;
```

- `CBA_fnc_removeItemContextMenuOption` `[item, id]` deletes the option from
  `cba_ui_ItemContextMenuOptions` and returns `true`, or `false` if the item or ID is not
  registered (checked against CBA's source).
- ACE's current `master` branch passes a **fixed ID**, `ace_magazinerepack_contextMenuRepackAction`,
  as the `_id` argument of `CBA_fnc_addItemContextMenuOption`. Released versions may not
  yet, which is why the option is looked up by display name. Once a release with the fixed ID
  is the minimum supported ACE version, the lookup can try that ID first and keep the
  display-name search as a fallback.
- The code reads CBA's internal variable `cba_ui_ItemContextMenuOptions`, so a CBA update
  could break the lookup. If that happens, the warning is logged and ACE's entry stays
  visible; nothing else breaks.

**CVO Redmag's entries:** a self-action under *Ammunition*, and
`CBA_fnc_addItemContextMenuOption` on `"#Magazine"` with slots `["CONTAINER", "MAGAZINE"]`
and the fixed ID `cvo_redmag_core_repack`. Its show condition is true only for magazines of a
mapped body with another partial magazine they can merge with.

### 5.3 Disabling ACE assisted belt linking

ACE defines the action on the gunner as
`CfgVehicles >> CAManBase >> ACE_Actions >> ACE_Weapon >> ace_reload_linkBelt`, with a
condition that checks whether the assistant has ammo to link. ACE then removes the
assistant's magazine and adds its rounds to the gunner's loaded belt with `setAmmo`, so a
box magazine's body would be lost. The same `core/CfgVehicles.hpp` patch sets its condition
to `"false"`, and `core` lists `ace_reload` in `requiredAddons`:

```cpp
class CAManBase: Man {
    class ACE_Actions {
        class ACE_Weapon {
            class ace_reload_linkBelt {
                condition = "false";   // replaced by CVO Redmag's belt actions
            };
        };
    };
};
```

It is disabled for **all** weapons, including belt-fed weapons that have no CVO Redmag
mapping. ACE's *Check ammo* actions, defined next to it, are not changed.

---

## 6. CBA settings

Category **"CVO Redmag"**. All of these are server-enforceable (`isGlobal = 1`) unless marked
*client*.

| Setting | Type | Default | Notes |
|---------|------|---------|-------|
| `cvo_redmag_core_ammo_mode` | list | `basic` | `basic` / `complex` / `simple`. Needs a mission restart. |
| `cvo_redmag_core_empties_enabled` | bool | true | Master switch for retention |
| `cvo_redmag_core_empties_noSpace` | list | `drop` | `drop` (vehicle cargo or ground) / `delete`. Applies to Empties only; magazines, packs and belts are never deleted. |
| `cvo_redmag_core_empties_hint` | bool *(client)* | false | Hint when an Empty is kept |
| `cvo_redmag_core_refill_timePerRound` | slider 0.1–3.0 | 0.6 | |
| `cvo_redmag_core_refill_stripperMultiplier` | slider 0.1–1.0 | 0.4 | |
| `cvo_redmag_core_refill_unloadMultiplier` | slider 0.1–1.0 | 0.5 | |
| `cvo_redmag_core_refill_allowMoving` | bool | true | Walking allowed during refill |
| `cvo_redmag_core_refill_holsterWhileLoading` | bool | false | |
| `cvo_redmag_core_refill_allowInVehicle` | bool | true | Passengers only |
| `cvo_redmag_core_packs_combineTime` | slider | 1.0 | Seconds per pack |
| `cvo_redmag_core_belts_linkTimePerRound` | slider | 0.3 | |
| `cvo_redmag_core_belts_unlinkMultiplier` | slider 0.1–1.0 | 0.5 | Unlink time = link time × this |
| `cvo_redmag_core_belts_joinTime` | slider | 3.0 | Seconds |
| `cvo_redmag_core_belts_boxTime` | slider | 4.0 | Seconds to load or unload a box |
| `cvo_redmag_core_cans_defaultInfinite` | bool | false | |

---

## 7. Public API & events

These are stable from v1.0 so that other mods and missions can integrate.

| Function | Purpose |
|----------|---------|
| `cvo_redmag_core_fnc_getBodyInfo` | `[magOrEmptyClass] → [bodyId, caliber, capacity, ...]` |
| `cvo_redmag_core_fnc_compatibleLoads` | `[bodyId] → [load, ...]` |
| `cvo_redmag_core_fnc_magazineFor` | `[bodyId, load] → class or ""` |
| `cvo_redmag_core_fnc_splitLoad` | `[load, count] → [[cartId, n], ...]` |
| `cvo_redmag_core_fnc_isEligibleUnit` | `[unit] → bool` |
| `cvo_redmag_core_fnc_getAmmoMode` | `[] → "complex"`, `"basic"` or `"simple"` |
| `cvo_redmag_core_fnc_resolveRoundType` | `[roundType] → roundType or ""` (active mode) |
| `cvo_redmag_core_fnc_loadCost` | `[load, count] → [[roundType, n], ...]` (active mode) |
| `cvo_redmag_core_fnc_addRounds` | `[unit, roundType, count] → leftover` (gives packs of the active round type) |
| `cvo_redmag_core_fnc_setContents` | `[can, roundType, rounds, infinite]` (server) |

| CBA event | Args | Scope |
|-----------|------|-------|
| `cvo_redmag_emptyRetained` | `[unit, emptyClass, magClass]` | local |
| `cvo_redmag_refillCompleted` | `[unit, plan, interrupted]` | local |
| `cvo_redmag_inventoryChanged` | `[unit]` | local |
| `cvo_redmag_canChanged` | `[can, roundType, rounds]` | global |

---

## 8. Testing strategy

- **Pure-function tests:**
  - `splitLoad`, `loadCost`, `resolveRoundType`, `planRefill`, `planUnload`, `planRepack`, `planCombinePacks` and belt planners are
    written so that they take arrays and return arrays.
  - Mixed-load tests check that load-then-unload returns exactly the rounds that went in,
    for every count from 0 to capacity, including top-offs from every partial count.
  - They are tested with CBA's test framework (`CBA_fnc_test` / `test_*.sqf`) in a test
    mission. This needs the game to run, but no interaction.
- **Config validation:** a preInit validator, plus a HEMTT lint step and a tool script that
  checks every vanilla infantry magazine is either mapped in `ammo` or explicitly ignored.
- **All three modes:** the planner tests and the manual checklist run once per ammo mode.
- **Manual test mission** (`tests/cvo_redmag_test.VR`):
  - preset loadouts for each weapon family;
  - an ammo can of each type;
  - a dedicated-server checklist (locality, JIP, two players draining one can);
  - an Arsenal checklist per mode: the CVO Redmag tab lists the right items, and packs
    from other modes are hidden in full and limited ACE arsenals;
  - a repack checklist: ACE's self-action and context menu entry are gone, CVO Redmag's
    entries appear, and every emptied magazine becomes an Empty;
  - ACE's *Link belt* action on a gunner no longer appears; ACE's *Check ammo* still does;
  - a Zeus checklist: remote-control a unit, reload it, and confirm that nothing is
    retained and no actions are shown.
- **Performance:** with the script profiler, confirm that 40 AI firing adds no CVO Redmag
  script time, and that a player's reload costs one short handler call.
