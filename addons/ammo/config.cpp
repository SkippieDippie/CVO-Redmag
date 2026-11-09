#include "script_component.hpp"

class CfgPatches {
    class ADDON {
        name = ADDON_NAME;
        author = "24th Chorni Voron";
        authors[] = {"Skippie [CVO]"};
        url = "https://github.com/SkippieDippie/CVO-Redmag";

        requiredVersion = REQUIRED_VERSION;
        // Vanilla load-order addons, so magazine patches apply after vanilla config.
        requiredAddons[] = {"cvo_redmag_main", "cba_jr", "A3_Data_F_Decade_Loadorder", "A3_Data_F_Mod_Loadorder"};

        units[] = {};
        weapons[] = {
            "cvo_redmag_empty_stanag30",
            "cvo_redmag_empty_stanag30_sand",
            "cvo_redmag_empty_stanag_drum150",
            "cvo_redmag_empty_stanag_drum150_sand",
            "cvo_redmag_empty_stanag_drum150_green",
            "cvo_redmag_empty_m249box200",
            "cvo_redmag_empty_sdar20",
            "cvo_redmag_empty_mx30",
            "cvo_redmag_empty_mx30_khaki",
            "cvo_redmag_empty_mx30_black",
            "cvo_redmag_empty_mx100",
            "cvo_redmag_empty_mx100_khaki",
            "cvo_redmag_empty_mx100_black",
            "cvo_redmag_empty_katiba30",
            "cvo_redmag_empty_msbs30",
            "cvo_redmag_empty_mk200box200",
            "cvo_redmag_empty_cmr20"
        };

        VERSION_CONFIG;
    };
};

#include "CfgCVORedmag.hpp"
#include "CfgMagazines.hpp"
#include "CfgWeapons.hpp"
