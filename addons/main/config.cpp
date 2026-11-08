#include "script_component.hpp"

class CfgPatches {
    class ADDON {
        name = ADDON_NAME;
        author = "24th Chorni Voron";
        authors[] = {"Skippie [CVO]"};
        url = "https://github.com/SkippieDippie/CVO-Redmag";

        requiredVersion = REQUIRED_VERSION;
        requiredAddons[] = {"cba_main", "ace_main"};

        units[] = {};
        weapons[] = {};

        VERSION_CONFIG;
    };
};
