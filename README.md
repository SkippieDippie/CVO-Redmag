# CVO Redmag
[![Build](https://github.com/SkippieDippie/CVO-Redmag/actions/workflows/validate_hemtt.yml/badge.svg)](https://github.com/SkippieDippie/CVO-Redmag/actions/workflows/validate_hemtt.yml)
[![Steam Subscriptions](https://img.shields.io/steam/subscriptions/3821740168)](https://steamcommunity.com/sharedfiles/filedetails/?id=3821740168)
[![Licence: GPL-2.0](https://img.shields.io/badge/licence-GPL--2.0-blue)](LICENSE)
[![Requires CBA_A3](https://img.shields.io/badge/requires-CBA__A3-orange)](https://steamcommunity.com/workshop/filedetails/?id=450814997)
[![Requires ACE3](https://img.shields.io/badge/requires-ACE3-orange)](https://steamcommunity.com/workshop/filedetails/?id=463939057)

An Arma 3 mod that makes small-arms ammunition physical. Empty magazines are kept as items, loose rounds are carried in packs, magazines are refilled, unloaded and repacked by hand, and machine-gun belts are linked from loose rounds.

Reloading stays as fast as vanilla. Refilling, unloading, repacking and belt linking are timed ACE actions that keep their progress when interrupted. Everything you carry is a normal item or magazine, so ACE Arsenal, ACE weight, persistence mods and JIP keep working.

> **Status:** rewrite in progress. See the [roadmap](docs/05-roadmap.md) for what is done.

## Features
- **Empty magazines:** reloading over an empty magazine, or dragging one out of a weapon in the inventory, gives you an Empty. A new magazine always needs an Empty or a partial magazine.
- **Loose rounds in packs:** boxes, packets and stripper clips sized after real packaging, with combine and split actions.
- **Refill, unload, swap load and repack:** from the ACE self-menu and the inventory context menu. Replaces ACE Magazine Repack, so repacking never destroys magazines.
- **Mixed ball and tracer magazines:** filled from both cartridges in the magazine's tracer ratio. Unloading returns exactly what went in.
- **Belts:** link, unlink and join belts from loose rounds, and load or unload box magazines. Replaces ACE assisted belt linking.
- **Ammo cans:** placeable containers set up in Eden, shared by everyone nearby. They can be dragged, carried and loaded as ACE cargo.
- **ACE Arsenal tab** with every CVO Redmag item for the active ammo mode.

## Ammo modes
A server setting, fixed for the mission. It decides how finely loose rounds are told apart; magazines themselves never change.

| Mode | Rounds told apart by | Example |
|------|---------------------|---------|
| **Complex** | Cartridge | M855 and M855A1 magazines use different rounds. |
| **Basic** (default) | Caliber | All 5.56×45 magazines use *5.56×45* rounds. |
| **Simple** | Category | 5.56×45 and 5.45×39 both use *intermediate* rounds. |

## Requirements
- [CBA_A3](https://steamcommunity.com/workshop/filedetails/?id=450814997)
- [ACE3](https://steamcommunity.com/workshop/filedetails/?id=463939057)

Applies to players only. AI and units a Zeus is remote-controlling keep vanilla behaviour.

## Documentation
The design documents are in [`docs/`](docs/):
[vision](docs/01-vision.md), [gameplay](docs/02-gameplay.md), [technical design](docs/03-technical.md), [config](docs/04-config.md) and [roadmap](docs/05-roadmap.md).

## Building
Built with [HEMTT](https://github.com/BrettMayson/HEMTT):

```bash
hemtt build
```

## Licence
[GPL-2.0](LICENSE), the same licence as CBA_A3 and ACE3.

## Links
- [GitHub Repository: CVO Redmag](https://github.com/SkippieDippie/CVO-Redmag)
- [Steam Workshop: CVO Redmag](https://steamcommunity.com/sharedfiles/filedetails/?id=3821740168)
