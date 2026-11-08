// Every function is cvo_redmag_core_fnc_<name>, which matches FUNC(<name>) in this addon.
// One category per feature; the engine loads functions\<category>\fn_<name>.sqf.
class CfgFunctions {
    class ADDON {
        class init {
            file = QPATHTOF(functions\init);
            class preInit { preInit = 1; };     // CBA settings
            class postInit { postInit = 1; };
        };
    };
};
