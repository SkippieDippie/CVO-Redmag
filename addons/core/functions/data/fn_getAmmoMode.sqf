#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Returns the active ammo mode. It is fixed once CBA settings are initialized; before that,
 * the current setting value is returned.
 *
 * Arguments:
 * None
 *
 * Return Value:
 * "complex", "basic" or "simple" <STRING>
 *
 * Example:
 * call cvo_redmag_core_fnc_getAmmoMode
 *
 * Public: Yes
 */

missionNamespace getVariable [QGVAR(ammoMode), missionNamespace getVariable [QGVAR(ammo_mode), "basic"]]
