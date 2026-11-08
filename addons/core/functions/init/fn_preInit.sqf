#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Runs at preInit (CfgFunctions preInit = 1) on every machine.
 * Registers the CBA settings.
 *
 * Arguments:
 * None
 *
 * Return Value:
 * None
 *
 * Public: No
 */

[
    QGVAR(ammo_mode),
    "LIST",
    ["Complexity mode", "The ammo complexity mode.\nComplex: each magazine uses specific cartridge caliber and type.\nBasic: magazines of the same caliber use a common cartridge of that caliber. Simple: magazines of different calibers are grouped into categories like pistol, intermediary, rifle, antimateriel, so 5.56x45 and 5.45x39 will use intermediary cartridge, while 7.62x51 and 7.62x54 will use rifle cartridge."],
    [QUOTE(PREFIX_BEAUTIFIED), "Ammo"],
    [["complex", "basic", "simple"], ["Complex", "Basic", "Simple"], 1],
    CBA_SETTING_GLOBAL,
    {},
    true // needs mission restart
] call CBA_fnc_addSetting;

INFO_1("preInit done, version %1",QUOTE(VERSION_STR));
