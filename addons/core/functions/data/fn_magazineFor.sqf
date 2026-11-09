#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns the magazine class for a body and a load: the preferred class when several map to
 * the same pair.
 *
 * Arguments:
 * 0: Body ID <STRING>
 * 1: Load [cartId, tracerCartId, tracerEvery, lastTracers] <ARRAY>
 *
 * Return Value:
 * Magazine class, or "" if none <STRING>
 *
 * Example:
 * ["stanag30", ["556_ball", "556_tracer", 0, 4]] call cvo_redmag_core_fnc_magazineFor
 * // "30Rnd_556x45_Stanag"
 *
 * Public: Yes
 */

params [["_body", "", [""]], ["_load", [], [[]], 4]];

GVAR(bodyLoadToMag) getOrDefault [toLowerANSI _body + ":" + ([_load] call FUNC(loadKey)), ""]
