# 01 — Vision & Scope

## Pitch

In vanilla Arma 3, a magazine is a single item: when it is empty and you reload, it disappears,
and an ammo box only refills you with whole magazines. **CVO Redmag** turns ammunition into
something you manage. You keep your empties, carry loose rounds, top off magazines during a
lull, unload tracers to swap in ball, and link belts for the gunner from a crate of loose rounds.

The goal is not to simulate gunsmithing. It is to make resupply, rationing and the downtime
between fights matter, and to give logistics roles something to do.

## Design pillars

1. **Everything is a real item.**
   All state lives in engine-native magazines and items: no hidden variables on the player.
   Arsenal loadouts, persistence frameworks, ACE weight, ground holders and JIP all keep
   working without extra code.
2. **Fast in combat, deliberate out of it.**
   A reload during a firefight behaves exactly like vanilla. Refilling, unloading and
   belt-linking are deliberate actions that take time and can be interrupted.
3. **Data-driven and mod-friendly.**
   Compatibility is declared in config (bodies, cartridges, calibers, categories). Supporting RHS, CUP or
   any weapon pack is a config patch, not code.
4. **Plays well with ACE.**
   Use ACE's interaction menu, progress bars and weight system. Where an ACE feature would
   destroy magazine bodies, CVO Redmag replaces it: ACE Magazine Repack is replaced by
   CVO Redmag's repack, and ACE's assisted belt linking is disabled in favour of CVO Redmag's
   belt actions.
5. **Cheap at runtime.**
   No per-frame loops. All lookups are built once at `preInit` into hashmaps, and the work
   happens only on events or when the player triggers an action.

## Target experience

- **Rifleman:** after a firefight, has three empty mags and one half-mag. Opens the ACE self
  menu, picks *Refill magazines*, and spends about a minute loading rounds from a box.
- **Machine gunner:** is low on belts. The assistant gunner links 200 rounds of ball and
  tracer from ammo cans into a belt (one tracer every five rounds) and hands it over.
- **Logistics:** carries ammo cans and packs instead of pre-loaded magazines, which is lighter
  and fits more weapons. The trade-off is time, because someone has to load them.
- **Mission maker:** picks an ammo mode (Basic, Complex or Simple) for how detailed ammo
  should be, places ammo cans and sets their contents in Eden attributes, and uses CBA
  settings to tune timings.

## Scope (v1.0)

| In scope | Notes |
|----------|-------|
| Empty magazine retention | Units controlled by a player, for infantry small-arms magazines that have a body defined in config. |
| Loose rounds as packs | Pack sizes follow real-world packaging where it exists. Pack *splitting* and *combining*. |
| Refill magazines | Fill an Empty, or top off a partial magazine, from packs or an ammo can. **A magazine always needs a body:** loose rounds alone never become a magazine. |
| Ammo modes | **Basic** (rounds by caliber, the default), **Complex** (by cartridge, opt-in) or **Simple** (by category, e.g. all intermediate calibers share rounds). Chosen per mission. |
| ACE Arsenal | A custom CVO Redmag tab with all the mod's items. Packs that don't belong to the active mode are hidden. |
| Unload magazines | Rounds go back into packs. The Empty is kept. |
| Repack magazines | Replaces ACE Magazine Repack: merges partial magazines, and every magazine it empties becomes an Empty. |
| Ammo-type conversion | Load any same-caliber cartridge into an Empty, and the result is the matching magazine class (ball → tracer, etc.). |
| Mixed magazines | Magazine classes that mix ball and tracer are filled from **both** ball and tracer rounds, in the class's ratio. There is no separate "mixed" cartridge. |
| Belts | Link loose rounds into a belt (no link items needed). Unlink a belt. Join belt segments. Load a belt into a box body. |
| Ammo cans | World containers with large round counts and ACE interactions. Eden-configurable. |
| Vanilla coverage | All vanilla infantry rifle, pistol, SMG, LMG and MMG magazines. |
| CBA settings | Server-enforceable tuning (timings, strictness). |

## Non-goals (v1.0)

- **AI and Zeus-controlled units.** The system is for players only. AI and units a Zeus is
  remote-controlling keep vanilla behaviour: no retained Empties and no actions.
- **Vehicle and static-weapon magazines.** ACE Rearm already handles these.
- **Consumable belt links.** Belts are linked from rounds alone.
- **Arbitrary round sequences** (e.g. "two tracers, then three ball"). A mixed magazine
  uses the fixed ratio its class defines.
- **Custom animations.** Actions use the ACE progress bar only.
- **Reloading from components** (powder, primers, cases) and brass collection.
- **Grenades, launchers, under-barrel grenades and disposable launchers.**
- **Changing weapon handling or ballistics.** That is ACE Ballistics' job.

## Success criteria

- A full vanilla infantry loadout works end to end: retain, refill, unload, convert, belts.
- No measurable frame-time cost while not performing an action.
- Disabling the mod leaves a loadout that is still valid. Empties and packs simply become
  inert items.
- A compat patch for a mid-size weapon pack can be produced in under an hour, with help from
  the generator tool.
