#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns the rounds a load costs in the active ammo mode: splits it, resolves each part to
 * the active round type and merges equal round types. In Basic and Simple mode a mix
 * therefore collapses to one entry. Entries with 0 rounds are left out.
 *
 * Arguments:
 * 0: Load [cartId, tracerCartId, tracerEvery, lastTracers] <ARRAY>
 * 1: Round count <NUMBER>
 *
 * Return Value:
 * [[roundType, n], ...] <ARRAY>
 *
 * Example:
 * [["556_ball", "556_tracer", 4, 4], 200] call cvo_redmag_core_fnc_loadCost
 * // Complex: [["556_ball", 147], ["556_tracer", 53]]   Basic: [["556x45", 200]]
 *
 * Public: Yes
 */

params [["_load", [], [[]], 4], ["_count", 0, [0]]];

private _cost = [];
{
    _x params ["_cart", "_rounds"];
    if (_rounds <= 0) then {continue};
    private _roundType = [_cart] call FUNC(resolveRoundType);
    private _index = _cost findIf {(_x select 0) == _roundType};
    if (_index < 0) then {
        _cost pushBack [_roundType, _rounds];
    } else {
        (_cost select _index) set [1, ((_cost select _index) select 1) + _rounds];
    };
} forEach ([_load, _count] call FUNC(splitLoad));

_cost
