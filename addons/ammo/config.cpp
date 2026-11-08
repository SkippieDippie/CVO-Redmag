#include "script_component.hpp"

class CfgPatches {
    class ADDON {
        name = ADDON_NAME;
        author = "24th Chorni Voron";
        authors[] = {"Skippie [CVO]"};
        url = "https://github.com/SkippieDippie/CVO-Redmag";

        requiredVersion = REQUIRED_VERSION;
        // Vanilla load-order addon, so magazine patches apply after vanilla config.
        requiredAddons[] = {"cvo_redmag_main", "A3_Data_F_Decade_Loadorder"};

        units[] = {};
        weapons[] = {};

        VERSION_CONFIG;
    };
};
