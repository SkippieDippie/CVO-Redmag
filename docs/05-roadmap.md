# 05 — Roadmap, Decisions & Open Questions

## Milestones

| Milestone | Goal | Contents | Exit criteria |
|-----------|------|----------|---------------|
| **M0 — Skeleton** | Builds and loads | HEMTT project (prefix `cvo_redmag`), `main` + `core` + `ammo` addons, CBA settings stub, CI build (GitHub Actions + HEMTT), `LICENSE` with the GPL-2.0 text | `hemtt build` is green. The mod loads with CBA+ACE without RPT errors. |
| **M1 — Data layer** | Knowledge of magazines | `CfgCVORedmag` schema (categories, calibers, cartridges), loads, `splitLoad`, `resolveRoundType`, `loadCost`, ammo-mode setting, lookup builder, validator, `ammo` config for 5.56 + 6.5, survey of vanilla mixed classes, real-world packaging references | Validator passes. The generator tool drafts vanilla configs. |
| **M2 — Empties** | Keep empty mags | Empty items, `Reloaded` and `MagazineUnloaded` EHs on the player unit, 0-round magazine count in nearby containers, `isEligibleUnit`, drop fallback, settings | Reloading an empty STANAG, or unloading one into the inventory, gives one Empty item; unloading it onto the ground or into a box leaves an Empty there instead of the 0-round magazine. Tested in SP and on a dedicated server. AI and Zeus remote-controlled units keep nothing. |
| **M3 — Packs & refill** | Core loop | Pack classes for all three round-type levels, refill/unload/convert/repack planners including mixed loads, progress bar, ACE self-menu, inventory context menu, ACE Repack disabled | Full loop for 5.56 and 6.5: empty → refill → fire → retain. Interrupting keeps partial progress. Mixed load-then-unload returns exactly what went in. The loop works in all three ammo modes. |
| **M4 — Ammo cans** | Logistics | Can objects, server-authoritative counts, two-can mixed reservations, Eden attributes, ACE cargo/drag | Two players draining one can on a dedicated server never over-draw. |
| **M5 — Belts** | MG support | Belt segments per load, link/unlink/join, box load/unload (no link items), ACE assisted belt linking disabled | M249/Mk200/Navid/Zafir workflows. The link menu only lists belts for weapons in the player's squad. |
| **M6 — Vanilla complete** | Coverage | All vanilla infantry calibers with categories, ACE Arsenal tab and pack hiding, icons, localisation (`stringtable.xml`) | Coverage script reports 100% of infantry magazines mapped or ignored. |
| **v1.0** | Release | Balance pass, docs, Workshop page (GPL-2.0, links to source), signing keys | Playtest sign-off. |
| **v1.1** | Expand | Refill from vehicle/crate cargo, compat_rhs, compat_cup | — |

## Risks

| Risk | Impact | Mitigation |
|------|--------|-----------|
| `Reloaded` EH misses some reload paths (scripted reloads, ACE, weapon swaps) | Empties occasionally lost | Write a test matrix in M2. Fallback: compare `magazinesAmmoFull` before and after on `Reloaded`/`Take`. |
| `MagazineUnloaded` count check misses the magazine (wrong container list, timing, or two players unloading into the same container at once) | Player gets an Empty while the 0-round magazine also stays in the container (one extra body) | Search the open inventory's containers one frame later; test each container type in M2. |
| Zeus remote-control is not detected reliably | Zeus-controlled units keep Empties or show actions | One eligibility function used everywhere. Zeus checklist in the test mission (M2). |
| Remove-and-re-add disturbs magazine order | Annoying for players | Only changed magazines are touched: each is removed on its own with `CBA_fnc_removeMagazine` (class + round count), and new ones are added with `CBA_fnc_addMagazine`. Nothing else is reshuffled. |
| Reused CBA or ACE code loses its attribution | GPL non-compliance | Keep the original copyright and author lines in the header of every reused or adapted file. |
| Vanilla mixed classes don't use `tracersEvery` consistently | Wrong ratios | The M1 survey records the ratio for every mixed class ([addons/ammo/README.md](../addons/ammo/README.md)). `cvo_redmag_tracerEvery` and `cvo_redmag_lastRoundsTracer` override it. |
| Fixed per-class mass (a near-empty pack weighs as much as a full one) | Minor weight inaccuracy | Real-world packs are small (10–50 rounds), so the error is small. |
| Real-world packaging data is missing for some calibers | Arbitrary sizes | Use the closest real analogue and record the source in config comments. |
| Huge third-party magazine counts make compat work heavy | Slow coverage | The generator tool, plus "unmapped magazine = vanilla behaviour" so nothing breaks. |
| ACE updates change interaction or progress bar APIs | Breakage | Pin a minimum ACE version and wrap ACE calls in `core`. |
| A CBA or ACE update breaks the removal of ACE's repack context menu entry (it reads CBA's internal `cba_ui_ItemContextMenuOptions`) | ACE's repack reappears in the inventory menu and deletes bodies | A warning is logged when the entry is not found; switch to ACE's fixed ID once a release has it; repack checklist in the test mission. |
| Three levels of pack items (cartridge, caliber, category) clutter the Arsenal | Long item lists, wrong packs picked | ACE Arsenal hides packs of other modes and has a CVO Redmag tab (D18). The vanilla Arsenal is not filtered. |
| Interaction menu cost with large inventories | FPS drops when opening the menu | Cached child generation, invalidated on `cvo_redmag_inventoryChanged` / `Take` / `Put`. |

## Decision log

| # | Decision | Date | Effect on the design |
|---|----------|------|----------------------|
| D1 | Name **CVO Redmag**, prefix **`cvo_redmag`** | 2026-11-08 | All classes, functions, settings and events use the prefix. |
| D2 | Mixed magazines are filled from **both ball and tracer cartridges**. There is no "mixed" cartridge. | 2026-11-08 | Introduces *loads* (single or mix), the split rule ([02 §3.1](02-gameplay.md#31-mixed-magazines)) and the `cvo_redmag_tracerCartridge` attribute. |
| D3 | **Players only.** AI and Zeus-controlled units are out of scope. | 2026-11-08 | Event handler on the player's unit only, `isEligibleUnit` check, AI settings removed. |
| D4 | Empty magazines are **items**. | 2026-11-08 | `CBA_MiscItem` classes, unchanged. |
| D5 | Packs and cans use **real-world packaging** where possible. | 2026-11-08 | Packaging table in [02 §2](02-gameplay.md#2-loose-rounds-packs), to be confirmed against references in M1. |
| D6 | Refilling **requires an Empty** (or a partial magazine). | 2026-11-08 | Loose rounds never become a magazine without a body. |
| D7 | **No consumable links.** Belts are linked from rounds alone. | 2026-11-08 | `requireLinks` setting and link items removed. |
| D8 | **Animations are out of scope for v1.** | 2026-11-08 | Actions use the ACE progress bar with no custom animation. |
| D9 | ~~Licence: Arma Public License – Share Alike (APL-SA).~~ Superseded by D19. | 2026-11-08 | — |
| D10 | Short on one cartridge in a mixed refill: **stop at the highest count that keeps the ratio**. | 2026-11-08 | As described in [02 §3.1](02-gameplay.md#31-mixed-magazines). |
| D11 | A mixed refill or belt link **can draw from two ammo cans at once** (ball can + tracer can). | 2026-11-08 | Server reserves from both cans in one request ([03 §4](03-technical.md#4-multiplayer--locality)). |
| D12 | **No bandoliers** for now. | 2026-11-08 | Packs are boxes and stripper clips only (D37). |
| D13 | **Three addons:** `main`, `core` (all code), `ammo` (all items and magazine mappings). | 2026-11-08 | Layout in [03 §1](03-technical.md#1-build--project-layout). Functions are `cvo_redmag_core_fnc_*`. |
| D14 | Functions are declared with **`CfgFunctions`**, not CBA `PREP`/XEH. | 2026-11-08 | `core/CfgFunctions.hpp`, one category per feature, `preInit`/`postInit` attributes for init. CBA script macros (`GVAR`, `FUNC`, ...) are kept. |
| D15 | Belts can be **linked without an Empty**, but the menu only offers belts for weapons used by the player or their squadmates. | 2026-11-08 | Exception to D6. Filter described in [02 §5.1](02-gameplay.md#51-which-belts-the-menu-offers). |
| D16 | **Ammo modes:** Basic (rounds by caliber, **default**), Complex (by cartridge, opt-in), Simple (by category). Server setting, fixed per mission. | 2026-11-08 | Round types, `resolveRoundType`/`loadCost`, packs per level ([02 §0](02-gameplay.md#0-ammo-modes)). |
| D17 | Simple-mode categories: `pistol`, `intermediate`, `rifle`, `antimateriel`, `shotgun`. 7.62×39 is `intermediate`. | 2026-11-08 | Category table in [02 §0](02-gameplay.md#0-ammo-modes). |
| D18 | ACE Arsenal **hides packs** that don't belong to the active mode, and gets a **custom right-panel tab** with all CVO Redmag items. | 2026-11-08 | [02 §0.1](02-gameplay.md#01-ace-arsenal), [03 §5.1](03-technical.md#51-ace-arsenal). `ace_arsenal` added to `core`'s dependencies. |
| D19 | Licence: **GPL-2.0** (replaces APL-SA, D9). | 2026-11-08 | Matches CBA and ACE, so their code may be reused with attribution. `LICENSE` file in M0. |
| D20 | ~~Items are added vest → uniform → backpack.~~ **Reverted:** items use the engine's order (uniform → vest → backpack). | 2026-11-08 | No custom container logic. |
| D21 | Magazines are removed one at a time with **`CBA_fnc_removeMagazine`** (class + round count). | 2026-11-08 | The rest of the inventory keeps its order ([03 §3.2](03-technical.md#32-inventory-manipulation)). |
| D22 | Magazines are added with **`CBA_fnc_addMagazine`** (`_verify = true`): engine order when there is room, otherwise vehicle cargo or the ground. | 2026-11-08 | Used for every added magazine, pack and belt ([03 §3.2](03-technical.md#32-inventory-manipulation)). |
| D23 | Manual unloads are caught with **`MagazineUnloaded`**. Where the 0-round magazine went is found by **counting 0-round magazines** (`magazinesAmmoCargo`) in the candidate containers against a baseline taken when the inventory opened. Found: it is **replaced with the Empty in that container**. Not found: the Empty is added to the player. Only while the inventory screen is open: unloads by scripts are ignored. | 2026-11-08 | `magazinesDetail` returns nothing for boxes and ground holders (tested in-game) and only weapon magazines for vehicles, so no ID search ([03 §3.1.1](03-technical.md#311-manual-unload-magazineunloaded)). |
| D24 | **ACE Magazine Repack is replaced** by CVO Redmag's repack, which turns every emptied magazine into an Empty. ACE's self-action is hidden (`condition = "false"`) and its inventory context menu entry is removed with `CBA_fnc_removeItemContextMenuOption`. | 2026-11-08 | [02 §4.1](02-gameplay.md#41-repacking-magazines), [03 §5.2](03-technical.md#52-replacing-ace-magazine-repack). `ace_magazinerepack` added to `core`'s dependencies. |
| D25 | **ACE assisted belt linking is disabled** for all weapons (`ace_reload_linkBelt` condition set to `"false"`). CVO Redmag's belt actions replace it. | 2026-11-08 | [03 §5.3](03-technical.md#53-disabling-ace-assisted-belt-linking). `ace_reload` added to `core`'s dependencies. |
| D26 | Rewritten rounds (combine, split, unload, return to a can) come out as the **active round type**, and every can is **converted to the active round type at mission start**. | 2026-11-08 | [02 §0](02-gameplay.md#0-ammo-modes), [03 §4](03-technical.md#4-multiplayer--locality). |
| D27 | **Refill all only tops off partial magazines.** Empties are filled from the body's own entry, where the load is chosen. | 2026-11-08 | [02 §3](02-gameplay.md#3-refilling-magazines). |
| D28 | **No *Give rounds* action.** Packs are shared through the inventory screen. | 2026-11-08 | Removed from [02 §2](02-gameplay.md#2-loose-rounds-packs). |
| D29 | The second can for a mixed load is the **nearest can holding the other cartridge within 5 m** of the player, picked automatically. | 2026-11-08 | [02 §6](02-gameplay.md#6-ammo-cans). |
| D30 | Magazine **ammo is validated per caliber** (`ammo[]` on each caliber), not per cartridge. | 2026-11-08 | The M1 survey showed that vanilla ball and tracer magazines often share one ammo class and that ball magazines may use tracer-coloured ammo; the load comes from `tracersEvery`. [04 §2](04-config.md#2-cfgcvoredmag). |
| D31 | `cvo_redmag_preferred` **is not inherited**: it only counts on the class that sets it. | 2026-11-08 | A base class can be preferred without every child becoming preferred too. [04 §3](04-config.md#3-mapping-magazines-cfgmagazines-attributes). |
| D32 | The config generator is an **offline Python tool** that reads PBOs through HEMTT, not an in-game script. | 2026-11-08 | No game launch needed; any mod's PBOs can be surveyed. [04 §6](04-config.md#6-compat-patches). |
| D33 | Vanilla specifics: **`mx100` is a `mag`**, not a `box`; **blank magazines** are excluded. | 2026-11-08 | `addons/ammo/calibers/*.hpp`, survey in `addons/ammo/README.md`. |
| D34 | **Tracer counts use both `tracersEvery` (N) and `lastRoundsTracer` (L):** a mix with *c* rounds holds `min(c, L) + floor(max(0, c − L) / N)` tracers. Every vanilla bullet has a tracer, so a "ball" magazine with `lastRoundsTracer = 4` is a mix of ball and tracer; the "reload tracer" classes only differ by tracer colour. | 2026-11-08 | Loads become `[cart, tracerCart, tracerEvery, lastTracers]`, keys `ball+tracer/N/L`; new optional attribute `cvo_redmag_lastRoundsTracer`. In Complex mode, refilling most magazines needs a few tracer rounds: with no tracer rounds, an Empty whose `lastRoundsTracer` > 0 cannot be filled. This is intended; there is no ball-only fallback. [02 §3.1](02-gameplay.md#31-mixed-magazines), [03 §2](03-technical.md#loads). |
| D35 | **A body includes its finish:** magazines with another model (`modelSpecial`) or other textures (`hiddenSelectionsTextures`) are different bodies with their own Empty. Class names and icons are not used to decide this. | 2026-11-08 | Vanilla gains `stanag30_sand`, `stanag_drum150_sand` / `_green`, `mx30_khaki` / `_black`, `mx100_khaki` / `_black` (17 bodies). Variants inherit the base body. The generator groups by appearance. [04 §2](04-config.md#2-cfgcvoredmag). |
| D36 | **One tracer cartridge per caliber**, whatever the tracer colour. The colour belongs to the magazine class. | 2026-11-08 | `556_tracer` and `65_tracer` replace the red / green / yellow cartridges, with one tracer pack each. Classes that only differ by colour share a *(body, load)*; the preferred class is the base class the others inherit from, and red for the plain STANAG tracers. [04 §2](04-config.md#2-cfgcvoredmag). |
| D37 | **Pack types are `box` and `clip` only.** `packet` is dropped: it behaved exactly like `box`. | 2026-11-09 | Packs sold in paper packets (e.g. 5.45×39) are `box` packs. The validator rejects any other `cvo_redmag_packType`. [04 §4](04-config.md#4-cvo-redmag-item-classes). |

## Open questions

None at the moment. New questions are added here as they come up.
