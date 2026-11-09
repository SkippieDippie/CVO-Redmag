// Every function is cvo_redmag_core_fnc_<name>, which matches FUNC(<name>) in this addon.
// One category per feature; the engine loads functions\<category>\fn_<name>.sqf.
class CfgFunctions {
    class ADDON {
        class init {
            file = QPATHTOF(functions\init);
            class preInit { preInit = 1; };     // CBA settings, lookups and validation
            class postInit { postInit = 1; };
        };
        class data {
            file = QPATHTOF(functions\data);
            class buildLookups {};
            class compatibleLoads {};
            class getAmmoMode {};
            class getBodyInfo {};
            class loadCost {};
            class loadKey {};
            class magazineFor {};
            class resolveRoundType {};
            class splitLoad {};
        };
    };
};
