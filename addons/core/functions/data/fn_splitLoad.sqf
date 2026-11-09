#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Splits a round count of a load into its cartridges.
 *
 * A mix holds tracers in two places, as the engine fires them:
 * - the last L rounds (lastRoundsTracer): the first L rounds loaded, at the bottom;
 * - one round in every N (tracersEvery) among the rest, if N >= 2.
 * With c rounds: tracers = min(c, L) + floor(max(0, c - L) / N), the rest ball.
 * The split depends only on c, so loading and then unloading always gives back exactly what
 * went in, and topping off from c to c + 1 adds one round of a known type.
 *
 * Arguments:
 * 0: Load [cartId, tracerCartId, tracerEvery, lastTracers] <ARRAY>
 * 1: Round count <NUMBER>
 *
 * Return Value:
 * [[cartId, n]] for a single cartridge, [[cartId, n], [tracerCartId, m]] for a mix <ARRAY>
 *
 * Example:
 * [["556_ball", "556_tracer", 4, 4], 200] call cvo_redmag_core_fnc_splitLoad
 * // [["556_ball", 147], ["556_tracer", 53]]
 * [["556_ball", "556_tracer", 0, 4], 30] call cvo_redmag_core_fnc_splitLoad
 * // [["556_ball", 26], ["556_tracer", 4]]
 *
 * Public: Yes
 */

params [["_load", [], [[]], 4], ["_count", 0, [0]]];
_load params ["_cart", "_tracer", "_every", "_last"];

if (_tracer == "") exitWith {[[_cart, _count]]};

private _tracers = _count min _last;
if (_every >= 2) then {
    _tracers = _tracers + floor ((0 max (_count - _last)) / _every);
};
[[_cart, _count - _tracers], [_tracer, _tracers]]
