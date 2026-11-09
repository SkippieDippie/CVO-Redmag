// IDs are config class names; the code lowercases them.
class CfgCVORedmag {
    // Simple-mode round types. Category packs have no real-world packaging: gameplay sizes.
    class Categories {
        class pistol {
            displayName = "Pistol rounds";
            packs[] = {"cvo_redmag_pack_pistol_box50"};
        };
        class intermediate {
            displayName = "Intermediate rounds";
            packs[] = {"cvo_redmag_pack_intermediate_box30"};
        };
        class rifle {
            displayName = "Rifle rounds";
            packs[] = {"cvo_redmag_pack_rifle_box20"};
        };
        class antimateriel {
            displayName = "Anti-materiel rounds";
            packs[] = {"cvo_redmag_pack_antimateriel_box10"};
        };
        class shotgun {
            displayName = "Shotgun shells";
            packs[] = {"cvo_redmag_pack_shotgun_box25"};
        };
    };

    class Calibers {
        class 556x45 {
            displayName = "5.56×45 mm";
            category = "intermediate";
            ammo[] = {"B_556x45_Ball", "B_556x45_Ball_Tracer_Red", "B_556x45_Ball_Tracer_Green", "B_556x45_Ball_Tracer_Yellow", "B_556x45_dual"};
            packs[] = {"cvo_redmag_pack_556x45_box20", "cvo_redmag_pack_556x45_clip10"};
        };
        class 65x39 {
            displayName = "6.5×39 mm Caseless";
            category = "intermediate";
            ammo[] = {"B_65x39_Caseless", "B_65x39_Caseless_green", "B_65x39_Caseless_yellow"};
            packs[] = {"cvo_redmag_pack_65x39_box20"};
        };
        class 65x39_cased {
            displayName = "6.5×39 mm";
            category = "intermediate";
            ammo[] = {"B_65x39_Case", "B_65x39_Case_green", "B_65x39_Case_yellow"};
            packs[] = {"cvo_redmag_pack_65x39_cased_box20"};
        };
    };

    class Bodies {
        // 5.56×45
        class stanag30 {
            caliber = "556x45";
            capacity = 30;
            type = "mag";
            emptyItem = "cvo_redmag_empty_stanag30";
            stripperCompatible = 1;
        };
        class stanag30_sand: stanag30 {
            emptyItem = "cvo_redmag_empty_stanag30_sand";
        };
        class stanag_drum150 {          // SPAR-16S drum (STANAG_556x45_Large)
            caliber = "556x45";
            capacity = 150;
            type = "mag";
            emptyItem = "cvo_redmag_empty_stanag_drum150";
        };
        class stanag_drum150_sand: stanag_drum150 {
            emptyItem = "cvo_redmag_empty_stanag_drum150_sand";
        };
        class stanag_drum150_green: stanag_drum150 {
            emptyItem = "cvo_redmag_empty_stanag_drum150_green";
        };
        class m249box200 {              // LIM-85
            caliber = "556x45";
            capacity = 200;
            type = "box";
            emptyItem = "cvo_redmag_empty_m249box200";
        };
        class sdar20 {                  // SDAR dual-purpose magazine
            caliber = "556x45";
            capacity = 20;
            type = "mag";
            emptyItem = "cvo_redmag_empty_sdar20";
        };

        // 6.5×39 caseless
        class mx30 {
            caliber = "65x39";
            capacity = 30;
            type = "mag";
            emptyItem = "cvo_redmag_empty_mx30";
        };
        class mx30_khaki: mx30 {
            emptyItem = "cvo_redmag_empty_mx30_khaki";
        };
        class mx30_black: mx30 {
            emptyItem = "cvo_redmag_empty_mx30_black";
        };
        class mx100 {                   // MX SW: a large magazine, not a belt box
            caliber = "65x39";
            capacity = 100;
            type = "mag";
            emptyItem = "cvo_redmag_empty_mx100";
        };
        class mx100_khaki: mx100 {
            emptyItem = "cvo_redmag_empty_mx100_khaki";
        };
        class mx100_black: mx100 {
            emptyItem = "cvo_redmag_empty_mx100_black";
        };
        class katiba30 {                // Katiba, Type 115 / ARX
            caliber = "65x39";
            capacity = 30;
            type = "mag";
            emptyItem = "cvo_redmag_empty_katiba30";
        };
        class msbs30 {                  // Promet
            caliber = "65x39";
            capacity = 30;
            type = "mag";
            emptyItem = "cvo_redmag_empty_msbs30";
        };
        class mk200box200 {             // Mk200
            caliber = "65x39";
            capacity = 200;
            type = "box";
            emptyItem = "cvo_redmag_empty_mk200box200";
        };

        // 6.5×39 cased
        class cmr20 {                   // CMR-76
            caliber = "65x39_cased";
            capacity = 20;
            type = "mag";
            emptyItem = "cvo_redmag_empty_cmr20";
        };
    };

    // Complex-mode round types. ammo is the CfgAmmo given to this cartridge's packs.
    class Cartridges {
        // 5.56×45. Real-world packaging: 20-round boxes; 10-round stripper clips
        // (7 clips per bandolier, 840 rounds per M2A1 can).
        class 556_ball {
            caliber = "556x45";
            ammo = "B_556x45_Ball";
            displayName = "5.56 mm Ball";
            packs[] = {"cvo_redmag_pack_556_ball_box20", "cvo_redmag_pack_556_ball_clip10"};
        };
        class 556_tracer {
            caliber = "556x45";
            ammo = "B_556x45_Ball_Tracer_Red";
            displayName = "5.56 mm Tracer";
            packs[] = {"cvo_redmag_pack_556_tracer_box20"};
        };
        class 556_dual {                // fictional, underwater; packed like 5.56 ball
            caliber = "556x45";
            ammo = "B_556x45_dual";
            displayName = "5.56 mm Dual Purpose";
            packs[] = {"cvo_redmag_pack_556_dual_box20"};
        };

        // 6.5×39 caseless: fictional. Closest real analogue 6.5 Grendel, 20-round boxes.
        class 65_ball {
            caliber = "65x39";
            ammo = "B_65x39_Caseless";
            displayName = "6.5 mm Caseless Ball";
            packs[] = {"cvo_redmag_pack_65_ball_box20"};
        };
        class 65_tracer {
            caliber = "65x39";
            ammo = "B_65x39_Caseless";
            displayName = "6.5 mm Caseless Tracer";
            packs[] = {"cvo_redmag_pack_65_tracer_box20"};
        };

        // 6.5×39 cased (6.5 Grendel analogue): 20-round boxes.
        class 65c_ball {
            caliber = "65x39_cased";
            ammo = "B_65x39_Case_green";
            displayName = "6.5 mm Ball";
            packs[] = {"cvo_redmag_pack_65c_ball_box20"};
        };
    };
};
