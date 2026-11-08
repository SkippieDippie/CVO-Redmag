# 02 — Gameplay Design

All numbers below are **starting defaults**. Each is exposed as a CBA setting (see
[03 §Settings](03-technical.md#6-cba-settings)) and will be tuned in playtests.

**Who it applies to:** only *eligible units*, meaning units a player controls as their own.
AI units and units a Zeus is remote-controlling behave exactly as in vanilla: they keep no
Empties and get no CVO Redmag actions.

---

## 0. Ammo modes

The mission maker (or server admin) picks an **ammo mode** with the CBA setting
`cvo_redmag_core_ammo_mode`. The mode decides how finely loose rounds are told apart, from
very detailed to very forgiving. It never changes the magazines themselves: a STANAG Empty is
still a STANAG Empty and only fits STANAG magazine classes.

| Mode | Loose rounds are told apart by | Example |
|------|-------------------------------|---------|
| **Complex** (opt-in) | **Cartridge**. Each magazine class uses its own cartridge. | A STANAG loaded with M855 uses *5.56×45 M855* rounds. A STANAG loaded with M855A1 uses *5.56×45 M855A1* rounds. Tracer magazines use tracer rounds. |
| **Basic** (default) | **Caliber**. Every magazine of one caliber uses one common cartridge. | STANAG magazines with M855, M855A1 or tracer all use *5.56×45* rounds. |
| **Simple** | **Category**. Calibers of the same kind share one common cartridge. | 5.56×45 STANAG and 5.45×39 AK magazines both use *intermediate* rounds. 7.62×51 and 7.62×54R magazines both use *rifle* rounds. |

The kind of loose round the active mode uses is called the **round type**: a cartridge in
Complex, a caliber in Basic, a category in Simple. In the rest of this document, "cartridge"
in the context of packs, cans and refilling means "round type" in Basic and Simple mode.

**What changes between modes**

- **Refilling an Empty:** the player still chooses which magazine class to make (M855, M855A1,
  tracer, mixed, …). In Basic and Simple mode every choice costs the same common rounds, so
  switching ammo type is free. Only the magazine body has to match.
- **Mixed magazines** (§3.1) only need two kinds of rounds in Complex mode. In Basic and
  Simple mode ball and tracer resolve to the same round type, so a mixed magazine is filled
  from that one type. The two-can draw (§6) and the "running short" rule (D10) therefore only
  matter in Complex mode.
- **Unloading** gives back rounds of the active round type. In Basic mode, unloading an
  M855A1 magazine gives *5.56×45* rounds.

**Simple-mode categories**

| Category | Calibers |
|----------|----------|
| `pistol` | 9×21, .45 ACP, 5.7×28, other pistol and SMG calibers |
| `intermediate` | 5.56×45, 5.45×39, 5.8×42, 6.5×39 caseless, 7.62×39 |
| `rifle` | 7.62×51, 7.62×54R, .338 Norma/Lapua, 9.3×64, other full-power rifle calibers |
| `antimateriel` | 12.7 mm and larger sniper and anti-materiel calibers |
| `shotgun` | 12 gauge and other shotgun gauges (mostly third-party weapons) |

Every caliber must name one of these categories. Compat addons may add categories, but
the five above are the standard set.

**Packs and cans across modes**

- Each round type has its own pack items: *5.56×45 M855 (box of 20)* for Complex,
  *5.56×45 (box of 20)* for Basic, *Intermediate rounds (box of 30)* for Simple. Basic packs
  use the caliber's real-world packaging. Categories have no real-world packaging, so Simple
  packs use gameplay sizes.
- **Detailed items still work in simpler modes.** A pack or can holding a more detailed round
  type counts as the active one: in Simple mode, an *M855* pack counts as intermediate rounds.
  This keeps loadouts and crates made for Complex mode usable in every mode.
- **Whatever is rewritten becomes the active round type.** When a player combines, splits or
  unloads, or puts rounds back into a can, the rounds that come out are packs of the active
  round type. In Basic mode, combining an *M855* box with a *5.56×45* box gives *5.56×45*
  boxes.
- **Cans are converted when the mission starts.** The server converts every can's contents to
  the active round type: in Basic mode, an *M855* can becomes a *5.56×45* can with the same
  round count. Cans whose round type is more generic than the mode stay inert. The mode is
  fixed per mission, so no information that matters is lost.
- **Generic items don't work in more detailed modes.** In Complex mode, an *Intermediate*
  pack has no single cartridge, so it is inert and its tooltip says it is not used in this
  mode.
- **The mode is fixed for a mission.** The setting only takes effect after a mission restart,
  so packs and cans never change meaning mid-mission.

### 0.1 ACE Arsenal

- **Only the active mode's packs are shown.** ACE Arsenal hides every pack that is not of the
  active level: in Basic mode it shows *5.56×45* packs and hides *M855*, *M855A1*, *tracer*
  and *Intermediate* packs. This keeps the Arsenal short. Detailed packs that are already in
  a loadout still work (see above); they just aren't offered.
- **CVO Redmag tab:** ACE Arsenal gets a custom tab in the right-side panel that lists all of
  the mod's items for the active mode: Empties, packs and belt segments.
- The vanilla (Bohemia) Arsenal is not filtered and gets no tab.

---

## 1. Empty magazine retention

**Triggers:**
- a player **reloads** a weapon whose current magazine has 0 rounds, or
- a player **manually unloads** an empty magazine from a weapon through the inventory screen
  (dragging it out of the weapon).

| Situation | Result |
|-----------|--------|
| Magazine has a body defined in config, and there is inventory space | The matching **Empty** item is added to the inventory in the engine's usual order: uniform → vest → backpack. |
| No inventory space | The Empty goes into the vehicle's cargo if the player is in a vehicle, otherwise it is dropped at their feet (setting: `drop` / `delete`). |
| Magazine has no body defined | Vanilla behaviour: the magazine is deleted. |
| Partial magazine swapped out or unloaded | Vanilla behaviour: the engine keeps it with its round count. Nothing extra happens. |
| Empty magazine unloaded into the uniform, vest or backpack | The engine deletes it. The Empty is added to the player instead, as for a reload. |
| Empty magazine unloaded onto the ground, into a box or into a vehicle | The engine keeps a 0-round magazine in that container. CVO Redmag **replaces it with the Empty item** in the same container, so the player finds the Empty where they put it. |
| AI unit, or a unit a Zeus is remote-controlling | Vanilla behaviour. Out of scope. |

**Container order:** everything CVO Redmag adds to a player (Empties, refilled magazines,
packs, belts) goes where the engine puts it: the uniform first, then the vest, then the
backpack. If none of them has room, a refilled magazine, pack
or belt goes into the vehicle's cargo when the player is in a vehicle, and otherwise onto the
ground at their feet. Nothing is lost.

**Feedback:** no popup during combat. An optional subtle hint can be enabled with a client setting.

**Design notes**
- Empties are items, not magazines, so they can never be loaded into a weapon by accident.
- Empties weigh about the real weight of an empty body, which is far less than a loaded one.
  Players are encouraged to keep them but can drop them when weight matters.
- Empties are listed in the CVO Redmag tab of ACE Arsenal (§0.1), so mission makers can hand them out.
- Magazines looted from AI are ordinary magazines. A player who empties one keeps its Empty.

---

## 2. Loose rounds (packs)

Loose rounds of one cartridge are carried as **packs**. A pack is a magazine class that no
weapon accepts. Because the engine tracks round count for magazines, a half-used box
"just works".

**Pack sizes follow real-world packaging** where a real equivalent exists. Fictional
cartridges (e.g. the 6.5 mm caseless used by the MX and Katiba) use the packaging of the
closest real cartridge. Starting list, to be confirmed against references in M1:

| Caliber | Real-world packaging | Pack items | Ammo can |
|---------|----------------------|------------|----------|
| 5.56×45 | 20-round boxes; 10-round stripper clips; M2A1 can | box 20, clip 10 | 840 |
| 7.62×51 | 20-round boxes | box 20 | TBC |
| 7.62×39 | 20-round boxes; sealed tins | box 20 | TBC |
| 5.45×39 | 30-round paper packets; sealed tins | packet 30 | TBC |
| 9 mm / .45 ACP | 50-round boxes | box 50 | TBC |
| Marksman and sniper calibers | 10- or 20-round boxes | TBC | TBC |
| Fictional calibers | Closest real analogue | TBC | TBC |

Pack and can masses also use real-world weights. **Bandoliers are not included** for now.

**Actions** (ACE self-interaction → *Ammunition*)
- **Combine packs:** merges all partial packs of the same cartridge into as few full packs as
  possible. Takes 1 s per pack.
- **Split pack:** takes N rounds out into a new pack. Useful for sharing.

There is no *Give rounds* action: players share packs through the inventory screen, like any
other item.

**Weight:** engine magazine mass is fixed per class, so a pack with 1 round left weighs the
same as a full one. Real-world packs are small (10–50 rounds), so this is a minor issue.

---

## 3. Refilling magazines

**A new magazine always needs a body.** Rounds go into an **Empty** or into a **partial
magazine**. Loose rounds alone never become a magazine.

**Entry points**
- ACE self-interaction → *Ammunition* → *Refill magazines* → submenu listing each body type
  with refillable magazines, plus a **Refill all** entry that tops off partial magazines
  only.
- ACE interaction on an **ammo can** → *Refill from can*.
- ACE interaction on a vehicle or crate that holds packs → *Refill from cargo* (v1.1).

**Flow**
1. The player chooses a body, or *Refill all*. **Refill all never fills Empties:** an Empty
   has no load yet, so the player fills Empties from the body's own entry, where they choose
   the load.
2. When filling Empties and more than one load is possible, a submenu picks the load. The
   list contains every magazine class mapped to that body whose cartridges the player has,
   e.g. *Ball*, *Tracer (red)*, *Ball + tracer (red), 1 in 5*.
3. An ACE progress bar runs for `rounds × timePerRound`.
   - Default `timePerRound`: **0.6 s** for magazines, and **0.24 s** (0.6 × 0.4) with a stripper clip on a
     compatible body.
   - The player can walk slowly. Sprinting, swimming, falling unconscious or getting in as
     a driver cancels it.
4. **When it is interrupted, the rounds loaded so far are kept.** Progress is calculated from
   elapsed time, so the player ends up with a partial magazine and a partial pack.
5. Fill order: top off the fullest partial magazines first, then fill Empties (body entry only).

**Rules**
- A partial magazine can only be topped off with **its own load**: the same cartridge, or
  for a mixed magazine, the same ball and tracer cartridges in the same ratio. To change the
  load, unload it first.
- An Empty can be filled with **any load mapped to its body**. The result is the magazine
  class for *(body, load)*. A load that has no magazine class is not offered.
- Refilling is a **two-handed** action, so the weapon is lowered while it runs. Fully
  holstering the weapon is optional, through the `holsterWhileLoading` setting.

### 3.1 Mixed magazines

Some magazine classes mix ball and tracer, e.g. a belt with one tracer every 5 rounds. These
are loaded from **both** a ball cartridge and a tracer cartridge. There is no separate
"mixed" round.

- **Ratio:** one tracer every *N* rounds. *N* comes from the magazine class (see
  [04 §3](04-config.md#3-mapping-magazines-cfgmagazines-attributes)).
- **Split rule:** a mixed magazine holding *c* rounds contains `floor(c / N)` tracers and
  the rest ball. For example, 200 rounds at 1 in 5 is 40 tracer and 160 ball.
- **Topping off** from *a* to *b* rounds uses `split(b) − split(a)` of each cartridge.
- **Unloading** a mixed magazine with *c* rounds returns `split(c)`. Load-then-unload
  always returns exactly what went in, so there is nothing to exploit.
- **Running short:** if one of the two cartridges runs out, filling stops at the highest
  round count the remaining rounds can make in the correct ratio (decision D10).

In the engine a mixed magazine fires one ammo class and draws a tracer every *N* rounds. The
ball/tracer split is CVO Redmag's bookkeeping of what went in and what comes out.

---

## 4. Unloading & ammo-type conversion

- **Unload magazine:** removes all rounds from a magazine into packs of its cartridge or
  cartridges, and the Empty is kept. Takes `rounds × timePerRound × 0.5` (unloading is faster).
- **Swap load:** shortcut that runs *unload* and then *refill with load X* in one
  progress bar.
- **Example:** before a night op, a team unloads its ball magazines and reloads them as
  *ball + tracer*, or reloads every fourth magazine with all-tracer.

### 4.1 Repacking magazines

CVO Redmag **replaces ACE Magazine Repack**. ACE's repack deletes every magazine it empties,
which would destroy the body. Its actions are removed (see
[03 §5.2](03-technical.md#52-replacing-ace-magazine-repack)) and CVO Redmag's repack takes their place, in
the same two spots players know:

- ACE self-interaction → *Ammunition* → *Repack magazines* → one entry per body type that
  has two or more partial magazines;
- the **inventory context menu** on a magazine (right-click) → *Repack magazines*.

**What it does:** moves rounds between partial magazines of the same body so that as many
as possible end up full. **Every magazine it empties becomes an Empty**, in the same
container.

**Which magazines can be merged:** rounds can only move into a magazine that could be topped
off with them (§3):
- **Complex mode:** only magazines with the same load (same class).
- **Basic and Simple mode:** any magazines of the same body, because their rounds resolve to
  the same round type. The magazine being filled keeps its class: M855A1 rounds moved into
  an M855 magazine count as M855 from then on.

**Order:** the fullest magazine is filled first, from the emptiest ones.

**Time:** `timePerRound` for each round moved, in one ACE progress bar. Interrupting keeps
the rounds moved so far, like refilling.

---

## 5. Belts

Belt-fed weapons (LMG box mags, MMG belts) use **bodies of type `box`** and loose **belt
segment** items. Belts are made from **rounds alone**: there are no link items to carry
or collect.

| Action | Input | Output | Default time |
|--------|-------|--------|-------------|
| **Link belt** | Loose rounds (packs or can), of one cartridge or a ball/tracer mix | Belt segment (up to the body's capacity) | 0.3 s / round |
| **Unlink belt** | Belt segment | Loose rounds | 0.15 s / round |
| **Join segments** | Two belt segments with the same load | One segment, up to max length | 3 s |
| **Load box** | Empty box body + belt segment | Loaded box magazine | 4 s |
| **Unload box** | Box magazine | Belt segment + empty box | 4 s |

Mixed belts follow the same split rule as mixed magazines (§3.1).

**Belt-only weapons** (an MMG with no box, where the magazine *is* the belt) skip the box step,
because the belt segment class is the loadable magazine. A disintegrating-link belt has no
reusable body, so linking one does not need an Empty. This is the one exception to the
"a magazine needs a body" rule (decision D15). For the same reason, emptying a belt-only
weapon keeps **no** Empty: the links fall away as it fires.

### 5.1 Which belts the menu offers

Linking creates a belt from nothing but rounds, so the number of possible belts is large.
To keep the interaction menu short, **Link belt** only offers belts for weapons that are
actually in use:

- the player's own weapons, and
- the weapons carried by the player's **squadmates** (the units in the player's group).

A belt is offered when its class, or a box magazine it can be loaded into, is in the
compatible magazines (`magazines[]` and `magazineWell[]`) of one of those weapons. For
example, a rifleman whose squad has an M249 gunner can link 5.56 belts for the M249, but is
not offered 7.62 belts unless someone in the squad carries a 7.62 MG.

The filter only decides what the menu lists: squadmates themselves get no actions. AI
squadmates' weapons count too, because the player may link a belt to hand to them.

**ACE assisted belt linking is disabled.** In ACE, an assistant can feed their own belt into
the gunner's weapon; ACE then uses up the assistant's magazine, which would destroy a box
body. CVO Redmag's belt actions replace it: the assistant links or joins a belt and hands it
over through the inventory.

---

## 6. Ammo cans

- Placeable objects sized after real containers, e.g. *M2A1 can (5.56 mm ball, 840 rds)*.
- Each can stores **one round type** (of any mode: cartridge, caliber or category) and a
  round count as an object variable, synced from the server. Contents resolve to the active
  mode like packs do (§0).
- Eden attributes: round type, round count, infinite (true/false).
- **ACE interactions:**
  - *Check contents*
  - *Take pack* (pack size picker)
  - *Refill magazines from can*
  - *Link belt from can*
  - *Return rounds*: put packs back
- In Complex mode, a mixed load draws from two cans at once: the can the player is using,
  plus the **nearest can holding the other cartridge within 5 m of the player**, picked
  automatically. If there is no such can, the other cartridge comes from packs the player
  carries.
- Cans can be carried and loaded as ACE cargo, using `ace_cargo_size` and `ace_dragging`.
- An empty can stays as a prop and can be refilled by putting in packs that resolve to its round type.

---

## 7. AI and Zeus

Out of scope. AI units and Zeus-controlled units use vanilla magazines and behaviour.
CVO Redmag adds no event handlers to AI and shows no actions to a unit while a Zeus is
remote-controlling it.

---

## 8. Balance levers

| Lever | Effect |
|-------|--------|
| Ammo mode | The biggest lever: Complex makes supply of each cartridge matter, Simple makes almost any loose round useful. |
| `timePerRound` | The main cost of using loose ammo versus loaded mags. |
| Pack and can mass | How attractive loose ammo is for logistics (real-world weights by default). |
| `empties_enabled` off | Turns the mod into "loose ammo + top-off" only. |
| Tracer supply | Mixed magazines need tracer rounds, so mission makers control tracer use through crate contents. |
| Arsenal/crate contents | Mission makers choose loaded mags, Empties, or loose rounds. |
