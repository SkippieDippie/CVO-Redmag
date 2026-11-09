#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns the string key of a load, used in hashmap keys.
 *
 * Arguments:
 * 0: Load [cartId, tracerCartId, tracerEvery, lastTracers] <ARRAY>
 *
 * Return Value:
 * Load key: "556_ball", or "ball+tracer/tracerEvery/lastTracers" for a mix,
 * e.g. "556_ball+556_tracer/4/4" <STRING>
 *
 * Example:
 * [["556_ball", "556_tracer", 0, 4]] call cvo_redmag_core_fnc_loadKey
 *
 * Public: No
 */

params [["_load", [], [[]], 4]];
_load params ["_cart", "_tracer", "_every", "_last"];

if (_tracer == "") exitWith {_cart};
format ["%1+%2/%3/%4", _cart, _tracer, _every, _last]
