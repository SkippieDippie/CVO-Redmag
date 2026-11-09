// 6.5×39 mm caseless and cased: vanilla magazine mappings and packs.
// Included inside CfgMagazines. Parents must match vanilla exactly.
//
// Vanilla notes:
// - MX ball and tracer magazines share B_65x39_Caseless (red tracer); Katiba ball uses the
//   green-tracer ammo. The cartridges come from the mapping, not from the ammo class.
// - Ball magazines have lastRoundsTracer = 4: 4 tracers at the bottom, so they are mixes of
//   ball + tracer. The CMR-76 has none.
// - All tracer colours use one cartridge, 65_tracer. The Mk200 yellow and red classes
//   share a (body, load); the yellow base class is preferred.
// - Katiba, Promet and Mk200 classes inherit from MX magazines, so every attribute is set.
// - Each finish is its own body: khaki and black MX magazines use other textures
//   (hiddenSelectionsTextures) than the base classes.

// --- MX 30 --------------------------------------------------------------------------------
class 30Rnd_65x39_caseless_mag: CA_Magazine {
    CVO_MIX("mx30","65_ball","65_tracer");
};
class 30Rnd_65x39_caseless_mag_Tracer: 30Rnd_65x39_caseless_mag {
    CVO_SINGLE("mx30","65_tracer");
};
class 30Rnd_65x39_caseless_khaki_mag: 30Rnd_65x39_caseless_mag {
    CVO_MIX("mx30_khaki","65_ball","65_tracer");
};
class 30Rnd_65x39_caseless_khaki_mag_Tracer: 30Rnd_65x39_caseless_mag_Tracer {
    CVO_SINGLE("mx30_khaki","65_tracer");
};
class 30Rnd_65x39_caseless_black_mag: 30Rnd_65x39_caseless_mag {
    CVO_MIX("mx30_black","65_ball","65_tracer");
};
class 30Rnd_65x39_caseless_black_mag_Tracer: 30Rnd_65x39_caseless_mag_Tracer {
    CVO_SINGLE("mx30_black","65_tracer");
};

// --- Katiba 30 ----------------------------------------------------------------------------
class 30Rnd_65x39_caseless_green: 30Rnd_65x39_caseless_mag {
    CVO_MIX("katiba30","65_ball","65_tracer");
};
class 30Rnd_65x39_caseless_green_mag_Tracer: 30Rnd_65x39_caseless_green {
    CVO_SINGLE("katiba30","65_tracer");
};

// --- Promet 30 ----------------------------------------------------------------------------
class 30Rnd_65x39_caseless_msbs_mag: 30Rnd_65x39_caseless_mag {
    CVO_MIX("msbs30","65_ball","65_tracer");
};
class 30Rnd_65x39_caseless_msbs_mag_Tracer: 30Rnd_65x39_caseless_msbs_mag {
    CVO_SINGLE("msbs30","65_tracer");
};

// --- MX SW 100 (mix: tracersEvery = 4, lastRoundsTracer = 4) -----------------------------
class 100Rnd_65x39_caseless_mag: CA_Magazine {
    CVO_MIX("mx100","65_ball","65_tracer");
};
class 100Rnd_65x39_caseless_mag_Tracer: 100Rnd_65x39_caseless_mag {
    CVO_SINGLE("mx100","65_tracer");
};
class 100Rnd_65x39_caseless_khaki_mag: 100Rnd_65x39_caseless_mag {
    CVO_MIX("mx100_khaki","65_ball","65_tracer");
};
class 100Rnd_65x39_caseless_khaki_mag_tracer: 100Rnd_65x39_caseless_mag_Tracer {
    CVO_SINGLE("mx100_khaki","65_tracer");
};
class 100Rnd_65x39_caseless_black_mag: 100Rnd_65x39_caseless_mag {
    CVO_MIX("mx100_black","65_ball","65_tracer");
};
class 100Rnd_65x39_caseless_black_mag_tracer: 100Rnd_65x39_caseless_mag_Tracer {
    CVO_SINGLE("mx100_black","65_tracer");
};

// --- Mk200 box 200 (inherits from the MX SW magazine; mix: 1 in 4 and the last 4) --------
class 200Rnd_65x39_cased_Box: 100Rnd_65x39_caseless_mag {
    CVO_MIX("mk200box200","65_ball","65_tracer");
};
class 200Rnd_65x39_cased_Box_Red: 100Rnd_65x39_caseless_mag {
    CVO_MIX("mk200box200","65_ball","65_tracer");
    CVO_PREFERRED;
};
class 200Rnd_65x39_cased_Box_Tracer: 200Rnd_65x39_cased_Box {
    CVO_SINGLE("mk200box200","65_tracer");
};
class 200Rnd_65x39_cased_Box_Tracer_Red: 200Rnd_65x39_cased_Box {
    CVO_SINGLE("mk200box200","65_tracer");
    CVO_PREFERRED;
};

// --- CMR-76 20 (6.5 cased) ---------------------------------------------------------------
class 20Rnd_650x39_Cased_Mag_F: CA_Magazine {
    CVO_SINGLE("cmr20","65c_ball");
};

// --- Blanks keep vanilla behaviour ------------------------------------------------------
class 30Rnd_65x39_caseless_blank_mag: 30Rnd_65x39_caseless_mag {
    cvo_redmag_body = "";
};
class 100Rnd_65x39_caseless_blank_mag: 30Rnd_65x39_caseless_mag {
    cvo_redmag_body = "";
};
class 30Rnd_65x39_caseless_msbs_blank_mag: 30Rnd_65x39_caseless_mag {
    cvo_redmag_body = "";
};
class 30Rnd_65x39_caseless_green_blank_mag: 30Rnd_65x39_caseless_green {
    cvo_redmag_body = "";
};
class 200Rnd_65x39_cased_box_blank: 200Rnd_65x39_cased_Box {
    cvo_redmag_body = "";
};

// --- Packs --------------------------------------------------------------------------------
// 6.5 caseless is fictional: 20-round boxes after 6.5 Grendel, lighter without cases (≈ 260 g).
CVO_PACK(cvo_redmag_pack_65_ball_box20,"6.5 mm Caseless Ball (box of 20)","B_65x39_Caseless",20,6,"box",cvo_redmag_cartridge,"65_ball",PIC_65);
CVO_PACK(cvo_redmag_pack_65_tracer_box20,"6.5 mm Caseless Tracer (box of 20)","B_65x39_Caseless",20,6,"box",cvo_redmag_cartridge,"65_tracer",PIC_65);
CVO_PACK(cvo_redmag_pack_65x39_box20,"6.5×39 mm Caseless (box of 20)","B_65x39_Caseless",20,6,"box",cvo_redmag_caliber,"65x39",PIC_65);
// 6.5 Grendel, 20-round box: ≈ 360 g.
CVO_PACK(cvo_redmag_pack_65c_ball_box20,"6.5 mm Ball (box of 20)","B_65x39_Case_green",20,8,"box",cvo_redmag_cartridge,"65c_ball",PIC_65C);
CVO_PACK(cvo_redmag_pack_65x39_cased_box20,"6.5×39 mm (box of 20)","B_65x39_Case_green",20,8,"box",cvo_redmag_caliber,"65x39_cased",PIC_65C);
