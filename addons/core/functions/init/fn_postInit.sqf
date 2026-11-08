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
    INFO_1("settings initialized, ammo mode: %1",GVAR(ammo_mode));
}] call CBA_fnc_addEventHandler;
