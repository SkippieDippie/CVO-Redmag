#define COMPONENT ammo
#define COMPONENT_BEAUTIFIED Ammo

#include "\cvo\cvo_redmag\addons\main\script_mod.hpp"
#include "\cvo\cvo_redmag\addons\main\script_macros.hpp"

// Load attributes on a magazine class. Every attribute is set
// explicitly, so nothing wrong is inherited from the parent class.
#define CVO_SINGLE(BODY,CART) \
    cvo_redmag_body = BODY; \
    cvo_redmag_cartridge = CART; \
    cvo_redmag_tracerCartridge = ""

#define CVO_MIX(BODY,BALL,TRACER) \
    cvo_redmag_body = BODY; \
    cvo_redmag_cartridge = BALL; \
    cvo_redmag_tracerCartridge = TRACER

// Wins ties when several classes map to the same (body, load).
#define CVO_PREFERRED cvo_redmag_preferred = 1

// A pack of loose rounds. ROUNDATTR is cvo_redmag_cartridge,
// cvo_redmag_caliber or cvo_redmag_category.
#define CVO_PACK(NAME,DNAME,AMMO,COUNT,MASS,PACKTYPE,ROUNDATTR,ROUNDTYPE,PIC) \
    class NAME: CA_Magazine { \
        scope = 2; \
        scopeArsenal = 2; \
        author = "24th Chorni Voron"; \
        displayName = DNAME; \
        descriptionShort = "Loose rounds for refilling magazines."; \
        picture = PIC; \
        model = "\A3\weapons_F\ammo\mag_univ.p3d"; \
        ammo = AMMO; \
        count = COUNT; \
        mass = MASS; \
        cvo_redmag_packType = PACKTYPE; \
        ROUNDATTR = ROUNDTYPE; \
    }

// An Empty item.
#define CVO_EMPTY(NAME,DNAME,DESC,PIC,MASS) \
    class NAME: CBA_MiscItem { \
        scope = 2; \
        scopeArsenal = 2; \
        author = "24th Chorni Voron"; \
        displayName = DNAME; \
        descriptionShort = DESC; \
        picture = PIC; \
        model = "\A3\weapons_F\ammo\mag_univ.p3d"; \
        class ItemInfo: CBA_MiscItem_ItemInfo { \
            mass = MASS; \
        }; \
    }

#define PIC_556 "\a3\Weapons_F\MagazineProxies\data\UI\icon_30Rnd_556x45_Stanag_CA.paa"
#define PIC_65 "\A3\weapons_f\data\ui\m_30stanag_caseless_CA.paa"
#define PIC_65C "\A3\Weapons_F_Exp\Data\UI\icon_20Rnd_650x39_Cased_Mag_F_ca.paa"
