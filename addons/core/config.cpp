#include "script_component.hpp"

class CfgPatches {
    class ADDON {
        name = ADDON_NAME;
        author = "24th Chorni Voron";
        authors[] = {"Skippie [CVO]"};
        url = "https://github.com/SkippieDippie/CVO-Redmag";

        requiredVersion = REQUIRED_VERSION;
        requiredAddons[] = {
            "cvo_redmag_main",
            "ace_common",
            "ace_interact_menu",
            "ace_interaction",
            "ace_cargo",
            "ace_dragging",
            "ace_arsenal",
            "ace_magazinerepack",
            "ace_reload"
        };

        units[] = {};
        weapons[] = {};

        VERSION_CONFIG;
    };
};

#include "CfgFunctions.hpp"
