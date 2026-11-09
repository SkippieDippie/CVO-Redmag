class CfgMagazines {
    class CA_Magazine;

    #include "calibers\556x45.hpp"
    #include "calibers\65x39.hpp"

    // Category packs (Simple mode): gameplay sizes, no real-world packaging.
    CVO_PACK(cvo_redmag_pack_pistol_box50,"Pistol rounds (box of 50)","B_9x21_Ball",50,13,"box",cvo_redmag_category,"pistol",PIC_556);
    CVO_PACK(cvo_redmag_pack_intermediate_box30,"Intermediate rounds (box of 30)","B_556x45_Ball",30,8,"box",cvo_redmag_category,"intermediate",PIC_556);
    CVO_PACK(cvo_redmag_pack_rifle_box20,"Rifle rounds (box of 20)","B_762x51_Ball",20,11,"box",cvo_redmag_category,"rifle",PIC_556);
    CVO_PACK(cvo_redmag_pack_antimateriel_box10,"Anti-materiel rounds (box of 10)","B_127x99_Ball",10,25,"box",cvo_redmag_category,"antimateriel",PIC_556);
    CVO_PACK(cvo_redmag_pack_shotgun_box25,"Shotgun shells (box of 25)","B_9x21_Ball",25,22,"box",cvo_redmag_category,"shotgun",PIC_556);
};
