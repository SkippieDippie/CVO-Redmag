// 5.56×45 mm: vanilla magazine mappings and packs. Included inside CfgMagazines.
// Survey: addons/ammo/README.md. Parents must match vanilla exactly.
//
// Vanilla notes:
// - Every vanilla bullet draws a tracer; the magazine decides which rounds show it. A "ball"
//   magazine with lastRoundsTracer = 4 holds 4 tracers at the bottom, so it is a mix of ball
//   + tracer. tracersEvery and lastRoundsTracer are read from each class.
// - All tracer colours use one cartridge, 556_tracer: the colour belongs to the
//   magazine class. Classes that only differ by colour (e.g. the "reload tracer" _green and
//   _red STANAGs) share a (body, load); the preferred class is the one refilling makes.
// - The drum and LIM-85 ball classes use tracer-coloured ammo; the cartridges come from the
//   mapping, not from the ammo class.
// - Each finish is its own body: Sand STANAGs and Sand / Green drums use other
//   textures (hiddenSelectionsTextures) than the base classes.

// --- STANAG 30 (lastRoundsTracer = 4) -----------------------------------------------------
class 30Rnd_556x45_Stanag: CA_Magazine {
    CVO_MIX("stanag30","556_ball","556_tracer");
};
class 30Rnd_556x45_Stanag_green: 30Rnd_556x45_Stanag {
    CVO_MIX("stanag30","556_ball","556_tracer");
};
class 30Rnd_556x45_Stanag_red: 30Rnd_556x45_Stanag {
    CVO_MIX("stanag30","556_ball","556_tracer");
    CVO_PREFERRED;
};
class 30Rnd_556x45_Stanag_Tracer_Red: 30Rnd_556x45_Stanag {
    CVO_SINGLE("stanag30","556_tracer");
    CVO_PREFERRED;
};
class 30Rnd_556x45_Stanag_Tracer_Green: 30Rnd_556x45_Stanag {
    CVO_SINGLE("stanag30","556_tracer");
};
class 30Rnd_556x45_Stanag_Tracer_Yellow: 30Rnd_556x45_Stanag {
    CVO_SINGLE("stanag30","556_tracer");
};

// --- STANAG 30, sand ----------------------------------------------------------------------
class 30Rnd_556x45_Stanag_Sand: 30Rnd_556x45_Stanag {
    CVO_MIX("stanag30_sand","556_ball","556_tracer");
};
class 30Rnd_556x45_Stanag_Sand_green: 30Rnd_556x45_Stanag_Sand {
    CVO_MIX("stanag30_sand","556_ball","556_tracer");
};
class 30Rnd_556x45_Stanag_Sand_red: 30Rnd_556x45_Stanag_Sand {
    CVO_MIX("stanag30_sand","556_ball","556_tracer");
    CVO_PREFERRED;
};
class 30Rnd_556x45_Stanag_Sand_Tracer_Red: 30Rnd_556x45_Stanag_Sand {
    CVO_SINGLE("stanag30_sand","556_tracer");
    CVO_PREFERRED;
};
class 30Rnd_556x45_Stanag_Sand_Tracer_Green: 30Rnd_556x45_Stanag_Sand_Tracer_Red {
    CVO_SINGLE("stanag30_sand","556_tracer");
};
class 30Rnd_556x45_Stanag_Sand_Tracer_Yellow: 30Rnd_556x45_Stanag_Sand_Tracer_Red {
    CVO_SINGLE("stanag30_sand","556_tracer");
};

// Blanks keep vanilla behaviour.
class 30Rnd_556x45_Stanag_Blank: 30Rnd_556x45_Stanag {
    cvo_redmag_body = "";
};

// --- SDAR 20 (inherits from the STANAG, so every attribute is overridden) -----------------
class 20Rnd_556x45_UW_mag: 30Rnd_556x45_Stanag {
    CVO_SINGLE("sdar20","556_dual");
};

// --- SPAR-16S drum 150 (red-tracer ammo, lastRoundsTracer = 4) ---------------------------
class 150Rnd_556x45_Drum_Mag_F: CA_Magazine {
    CVO_MIX("stanag_drum150","556_ball","556_tracer");
};
class 150Rnd_556x45_Drum_Mag_Tracer_F: 150Rnd_556x45_Drum_Mag_F {
    CVO_SINGLE("stanag_drum150","556_tracer");
};
class 150Rnd_556x45_Drum_Sand_Mag_F: 150Rnd_556x45_Drum_Mag_F {
    CVO_MIX("stanag_drum150_sand","556_ball","556_tracer");
};
class 150Rnd_556x45_Drum_Sand_Mag_Tracer_F: 150Rnd_556x45_Drum_Sand_Mag_F {
    CVO_SINGLE("stanag_drum150_sand","556_tracer");
};
class 150Rnd_556x45_Drum_Green_Mag_F: 150Rnd_556x45_Drum_Mag_F {
    CVO_MIX("stanag_drum150_green","556_ball","556_tracer");
};
class 150Rnd_556x45_Drum_Green_Mag_Tracer_F: 150Rnd_556x45_Drum_Green_Mag_F {
    CVO_SINGLE("stanag_drum150_green","556_tracer");
};

// --- LIM-85 box 200 (mixes: tracersEvery = 4, lastRoundsTracer = 4) -----------------------
class 200Rnd_556x45_Box_F: CA_Magazine {
    CVO_MIX("m249box200","556_ball","556_tracer");
};
class 200Rnd_556x45_Box_Red_F: 200Rnd_556x45_Box_F {
    CVO_MIX("m249box200","556_ball","556_tracer");
    CVO_PREFERRED;
};
class 200Rnd_556x45_Box_Tracer_F: 200Rnd_556x45_Box_F {
    CVO_SINGLE("m249box200","556_tracer");
};
class 200Rnd_556x45_Box_Tracer_Red_F: 200Rnd_556x45_Box_Tracer_F {
    CVO_SINGLE("m249box200","556_tracer");
    CVO_PREFERRED;
};

// --- Packs --------------------------------------------------------------------------------
// Mass: Arma units of ~45 g (ACE: 1 unit = 0.1 lb). M855 ≈ 12.3 g per round.
// M855, 20-round carton: ≈ 250 g.
CVO_PACK(cvo_redmag_pack_556_ball_box20,"5.56 mm Ball (box of 20)","B_556x45_Ball",20,6,"box",cvo_redmag_cartridge,"556_ball",PIC_556);
// M855, 10-round stripper clip: ≈ 125 g.
CVO_PACK(cvo_redmag_pack_556_ball_clip10,"5.56 mm Ball (stripper clip of 10)","B_556x45_Ball",10,3,"clip",cvo_redmag_cartridge,"556_ball",PIC_556);
// M856 tracer, 20-round carton. One pack for every tracer colour (D36).
CVO_PACK(cvo_redmag_pack_556_tracer_box20,"5.56 mm Tracer (box of 20)","B_556x45_Ball_Tracer_Red",20,6,"box",cvo_redmag_cartridge,"556_tracer",PIC_556);
// Fictional dual-purpose round: packed like ball.
CVO_PACK(cvo_redmag_pack_556_dual_box20,"5.56 mm Dual Purpose (box of 20)","B_556x45_dual",20,6,"box",cvo_redmag_cartridge,"556_dual",PIC_556);
// Basic mode: same packaging as ball.
CVO_PACK(cvo_redmag_pack_556x45_box20,"5.56x45 mm (box of 20)","B_556x45_Ball",20,6,"box",cvo_redmag_caliber,"556x45",PIC_556);
CVO_PACK(cvo_redmag_pack_556x45_clip10,"5.56×45 mm (stripper clip of 10)","B_556x45_Ball",10,3,"clip",cvo_redmag_caliber,"556x45",PIC_556);
