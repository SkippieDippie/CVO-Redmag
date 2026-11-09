#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns every load that has a magazine class for a body.
 *
 * Arguments:
 * 0: Body ID <STRING>
 *
 * Return Value:
 * [load, ...] (a copy) <ARRAY>
 *
 * Example:
 * ["stanag30"] call cvo_redmag_core_fnc_compatibleLoads
 *
 * Public: Yes
 */

params [["_body", "", [""]]];

+(GVAR(bodyLoads) getOrDefault [toLowerANSI _body, []])
