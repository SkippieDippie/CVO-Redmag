#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Runs at postInit (CfgFunctions postInit = 1) on every machine.
 * Placeholder for player event handlers, can actions and server events.
 *
 * Arguments:
 * None
 *
 * Return Value:
 * None
 *
 * Public: No
 */

["CBA_settingsInitialized", {
    // The setting needs a mission restart, so the mode is fixed from here on.
    GVAR(ammoMode) = GVAR(ammo_mode);
    INFO_1("settings initialized, ammo mode: %1",GVAR(ammoMode));
}] call CBA_fnc_addEventHandler;
