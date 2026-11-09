#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Maps a round type up to the level the active ammo mode uses.
 * A cartridge becomes its caliber in Basic mode and its category in Simple mode. A round
 * type more generic than the mode cannot be resolved down and returns "" (inert).
 *
 * Arguments:
 * 0: Round type: cartridge, caliber or category ID <STRING>
 *
 * Return Value:
 * Round type of the active mode, or "" <STRING>
 *
 * Example:
 * ["556_ball"] call cvo_redmag_core_fnc_resolveRoundType // "556x45" in Basic mode
 *
 * Public: Yes
 */

params [["_roundType", "", [""]]];
_roundType = toLowerANSI _roundType;

private _level = ["cartridge", "caliber", "category"] find (GVAR(roundTypeLevel) getOrDefault [_roundType, ""]);
private _target = ["complex", "basic", "simple"] find (call FUNC(getAmmoMode));
if (_level < 0 || {_target < 0} || {_level > _target}) exitWith {""};

if (_level == 0 && {_target > 0}) then {
    _roundType = (GVAR(cartridges) get _roundType) select 0;
    _level = 1;
};
if (_level == 1 && {_target > 1}) then {
    _roundType = (GVAR(calibers) get _roundType) select 0;
};

_roundType
