#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns the body of a mapped magazine class or an Empty item.
 *
 * Arguments:
 * 0: Magazine class or Empty item class <STRING>
 *
 * Return Value:
 * [bodyId, caliber, capacity, emptyItem, type, stripperCompatible], or [] if unmapped <ARRAY>
 *
 * Example:
 * ["30Rnd_556x45_Stanag"] call cvo_redmag_core_fnc_getBodyInfo
 *
 * Public: Yes
 */

params [["_class", "", [""]]];
_class = toLowerANSI _class;

private _body = (GVAR(magToBodyLoad) getOrDefault [_class, [""]]) select 0;
if (_body == "") then {_body = GVAR(emptyToBody) getOrDefault [_class, ""]};
if (_body == "") exitWith {[]};

[_body] + (GVAR(bodies) get _body)
