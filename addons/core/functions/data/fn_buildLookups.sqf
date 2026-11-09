#include "..\..\script_component.hpp"
/*
 * Author: Skippie [CVO]
 * Builds the read-only lookup hashmaps from CfgCVORedmag, CfgMagazines and CfgWeapons, and
 * validates the config while doing so. A broken entry is logged
 * to the RPT and skipped; it never stops the rest from loading.
 *
 * All IDs (categories, calibers, cartridges, bodies) and class-name keys are lowercase.
 *
 * Arguments:
 * None
 *
 * Return Value:
 * Number of problems found <NUMBER>
 *
 * Example:
 * call cvo_redmag_core_fnc_buildLookups
 *
 * Public: No
 */

private _problems = 0;
#define PROBLEM_1(MSG,A) _problems = _problems + 1; WARNING_1(MSG,A)
#define PROBLEM_2(MSG,A,B) _problems = _problems + 1; WARNING_2(MSG,A,B)
#define PROBLEM_3(MSG,A,B,C) _problems = _problems + 1; WARNING_3(MSG,A,B,C)
#define PROBLEM_4(MSG,A,B,C,D) _problems = _problems + 1; WARNING_4(MSG,A,B,C,D)

private _root = configFile >> "CfgCVORedmag";
private _cfgMagazines = configFile >> "CfgMagazines";

private _categories = createHashMap;    // id -> [displayName, packs]
private _calibers = createHashMap;      // id -> [category, displayName, packs, ammo]
private _cartridges = createHashMap;    // id -> [caliber, cfgAmmo, displayName, packs]
private _bodies = createHashMap;        // id -> [caliber, capacity, emptyItem, type, stripperCompatible]
private _roundTypeLevel = createHashMap;
private _emptyToBody = createHashMap;
private _packToRound = createHashMap;   // pack class -> [roundType, capacity]
private _magToBodyLoad = createHashMap; // magazine class -> [bodyId, load]
private _bodyLoadToMag = createHashMap; // "bodyId:loadKey" -> magazine class
private _bodyLoads = createHashMap;     // bodyId -> [load, ...]
private _beltToLoad = createHashMap;    // belt class -> [load, capacity]

private _fnc_addRoundType = {
    params ["_id", "_level"];
    if (_id in _roundTypeLevel) exitWith {
        PROBLEM_3("Round type '%1' (%2) is already a %3; skipped",_id,_level,_roundTypeLevel get _id);
        false
    };
    _roundTypeLevel set [_id, _level];
    true
};

// --- Categories ---------------------------------------------------------------------------
{
    private _id = toLowerANSI configName _x;
    if !([_id, "category"] call _fnc_addRoundType) then {continue};
    _categories set [_id, [getText (_x >> "displayName"), getArray (_x >> "packs")]];
} forEach ("true" configClasses (_root >> "Categories"));

// --- Calibers -----------------------------------------------------------------------------
{
    private _id = toLowerANSI configName _x;
    private _category = toLowerANSI getText (_x >> "category");
    if !(_category in _categories) then {
        PROBLEM_2("Caliber '%1': unknown category '%2'; skipped",_id,_category);
        continue;
    };
    if !([_id, "caliber"] call _fnc_addRoundType) then {continue};
    private _ammo = (getArray (_x >> "ammo")) apply {toLowerANSI _x};
    if (_ammo isEqualTo []) then {
        PROBLEM_1("Caliber '%1': ammo[] is empty; no magazine can map to it",_id);
    };
    _calibers set [_id, [_category, getText (_x >> "displayName"), getArray (_x >> "packs"), _ammo]];
} forEach ("true" configClasses (_root >> "Calibers"));

// --- Cartridges ---------------------------------------------------------------------------
{
    private _id = toLowerANSI configName _x;
    private _caliber = toLowerANSI getText (_x >> "caliber");
    if !(_caliber in _calibers) then {
        PROBLEM_2("Cartridge '%1': unknown caliber '%2'; skipped",_id,_caliber);
        continue;
    };
    private _ammo = getText (_x >> "ammo");
    if !(toLowerANSI _ammo in ((_calibers get _caliber) select 3)) then {
        PROBLEM_3("Cartridge '%1': ammo '%2' is not in caliber '%3' ammo[]; skipped",_id,_ammo,_caliber);
        continue;
    };
    if !([_id, "cartridge"] call _fnc_addRoundType) then {continue};
    _cartridges set [_id, [_caliber, _ammo, getText (_x >> "displayName"), getArray (_x >> "packs")]];
} forEach ("true" configClasses (_root >> "Cartridges"));

// --- Bodies -------------------------------------------------------------------------------
{
    private _id = toLowerANSI configName _x;
    private _caliber = toLowerANSI getText (_x >> "caliber");
    private _capacity = getNumber (_x >> "capacity");
    private _type = toLowerANSI getText (_x >> "type");
    private _empty = getText (_x >> "emptyItem");
    if !(_caliber in _calibers) then {
        PROBLEM_2("Body '%1': unknown caliber '%2'; skipped",_id,_caliber);
        continue;
    };
    if (_capacity < 1) then {
        PROBLEM_1("Body '%1': capacity must be at least 1; skipped",_id);
        continue;
    };
    if !(_type in ["mag", "box", "belt"]) then {
        PROBLEM_2("Body '%1': unknown type '%2'; skipped",_id,_type);
        continue;
    };
    if (_type == "belt") then {
        if (_empty != "") then {
            PROBLEM_1("Body '%1': a belt body has no emptyItem; it is ignored",_id);
            _empty = "";
        };
    } else {
        if !(isClass (configFile >> "CfgWeapons" >> _empty)) then {
            PROBLEM_2("Body '%1': emptyItem '%2' does not exist; skipped",_id,_empty);
            continue;
        };
        _emptyToBody set [toLowerANSI _empty, _id];
    };
    _bodies set [_id, [_caliber, _capacity, _empty, _type, getNumber (_x >> "stripperCompatible") == 1]];
} forEach ("true" configClasses (_root >> "Bodies"));

// --- Loads (shared by magazines and belts) ------------------------------------------------
// Returns [load, caliber] or [] after logging why the class is skipped.
private _fnc_readLoad = {
    params ["_cfg"];
    private _class = configName _cfg;
    private _cart = toLowerANSI getText (_cfg >> "cvo_redmag_cartridge");
    private _tracer = toLowerANSI getText (_cfg >> "cvo_redmag_tracerCartridge");
    if !(_cart in _cartridges) exitWith {
        PROBLEM_2("Magazine '%1': unknown cartridge '%2'; skipped",_class,_cart);
        []
    };
    private _caliber = (_cartridges get _cart) select 0;
    if (_tracer == "") exitWith {[[_cart, "", 0, 0], _caliber]};
    if !(_tracer in _cartridges) exitWith {
        PROBLEM_2("Magazine '%1': unknown tracer cartridge '%2'; skipped",_class,_tracer);
        []
    };
    if (((_cartridges get _tracer) select 0) != _caliber) exitWith {
        PROBLEM_3("Magazine '%1': cartridges '%2' and '%3' have different calibers; skipped",_class,_cart,_tracer);
        []
    };
    private _every = if (isNumber (_cfg >> "cvo_redmag_tracerEvery")) then {
        getNumber (_cfg >> "cvo_redmag_tracerEvery")
    } else {
        getNumber (_cfg >> "tracersEvery")
    };
    private _last = if (isNumber (_cfg >> "cvo_redmag_lastRoundsTracer")) then {
        getNumber (_cfg >> "cvo_redmag_lastRoundsTracer")
    } else {
        getNumber (_cfg >> "lastRoundsTracer")
    };
    // tracerEvery 1 is all tracers: map that as a single tracer cartridge instead.
    if (_every == 1 || {_every < 0} || {_last < 0}) exitWith {
        PROBLEM_3("Magazine '%1': a mixed load needs tracerEvery 0 or at least 2 and lastRoundsTracer of at least 0 (got %2 and %3); skipped",_class,_every,_last);
        []
    };
    if (_every == 0 && {_last == 0}) exitWith {
        PROBLEM_1("Magazine '%1': a mixed load with tracerEvery 0 and lastRoundsTracer 0 holds no tracers; skipped",_class);
        []
    };
    [[_cart, _tracer, _every, _last], _caliber]
};

// --- Packs --------------------------------------------------------------------------------
{
    private _class = configName _x;
    private _set = [
        ["cartridge", toLowerANSI getText (_x >> "cvo_redmag_cartridge")],
        ["caliber", toLowerANSI getText (_x >> "cvo_redmag_caliber")],
        ["category", toLowerANSI getText (_x >> "cvo_redmag_category")]
    ] select {(_x select 1) != ""};
    if (count _set != 1) then {
        PROBLEM_2("Pack '%1': must set exactly one of cartridge/caliber/category (has %2); skipped",_class,count _set);
        continue;
    };
    (_set select 0) params ["_level", "_roundType"];
    if ((_roundTypeLevel getOrDefault [_roundType, ""]) != _level) then {
        PROBLEM_3("Pack '%1': unknown %2 '%3'; skipped",_class,_level,_roundType);
        continue;
    };
    private _packType = toLowerANSI getText (_x >> "cvo_redmag_packType");
    if !(_packType in ["box", "clip"]) then {
        PROBLEM_2("Pack '%1': packType must be 'box' or 'clip' (got '%2'); skipped",_class,_packType);
        continue;
    };
    _packToRound set [toLowerANSI _class, [_roundType, getNumber (_x >> "count")]];
} forEach ("isText (_x >> 'cvo_redmag_packType')" configClasses _cfgMagazines);

// Every round type lists packs that exist and hold that round type.
{
    _x params ["_map", "_packIndex"];
    {
        private _roundType = _x;
        private _entry = _y;
        private _valid = (_entry select _packIndex) select {
            private _pack = toLowerANSI _x;
            private _ok = ((_packToRound getOrDefault [_pack, [""]]) select 0) == _roundType;
            if (!_ok) then {
                PROBLEM_2("Round type '%1': pack '%2' does not exist or holds another round type; removed",_roundType,_x);
            };
            _ok
        };
        if (_valid isEqualTo []) then {
            PROBLEM_1("Round type '%1' has no pack class; it has no loose-round item",_roundType);
        };
        _entry set [_packIndex, _valid apply {toLowerANSI _x}];
    } forEach _map;
} forEach [[_categories, 1], [_calibers, 2], [_cartridges, 3]];  // index of packs[] in each entry

// --- Magazines ----------------------------------------------------------------------------
private _candidates = createHashMap;    // "bodyId:loadKey" -> [[class, preferred], ...]
{
    private _class = configName _x;
    private _body = toLowerANSI getText (_x >> "cvo_redmag_body");
    if (_body == "") then {continue};   // explicitly excluded
    if !(_body in _bodies) then {
        PROBLEM_2("Magazine '%1': unknown body '%2'; skipped",_class,_body);
        continue;
    };
    (_bodies get _body) params ["_bodyCaliber", "_capacity"];
    private _count = getNumber (_x >> "count");
    if (_count != _capacity) then {
        PROBLEM_4("Magazine '%1': count %2 does not match body '%3' capacity %4; skipped",_class,_count,_body,_capacity);
        continue;
    };
    private _read = [_x] call _fnc_readLoad;
    if (_read isEqualTo []) then {continue};
    _read params ["_load", "_caliber"];
    if (_caliber != _bodyCaliber) then {
        PROBLEM_3("Magazine '%1': cartridge caliber '%2' does not match body caliber '%3'; skipped",_class,_caliber,_bodyCaliber);
        continue;
    };
    private _ammo = getText (_x >> "ammo");
    if !(toLowerANSI _ammo in ((_calibers get _caliber) select 3)) then {
        PROBLEM_3("Magazine '%1': ammo '%2' is not in caliber '%3' ammo[]; skipped",_class,_ammo,_caliber);
        continue;
    };

    _magToBodyLoad set [toLowerANSI _class, [_body, _load]];
    private _key = _body + ":" + ([_load] call FUNC(loadKey));
    // cvo_redmag_preferred only counts on the class that sets it, not on its children.
    private _preferred = getNumber (_x >> "cvo_redmag_preferred") == 1
        && {(configProperties [_x, "configName _x == 'cvo_redmag_preferred'", false]) isNotEqualTo []};
    (_candidates getOrDefault [_key, [], true]) pushBack [_class, _preferred];
    (_bodyLoads getOrDefault [_body, [], true]) pushBackUnique _load;
} forEach ("isText (_x >> 'cvo_redmag_body')" configClasses _cfgMagazines);

// Reverse lookup: the preferred class wins, otherwise the first in config order.
{
    private _key = _x;
    private _list = _y;
    private _preferred = _list select {_x select 1};
    if (count _preferred > 1) then {
        PROBLEM_2("Several preferred classes for '%1': %2; the first one is used",_key,_preferred apply {_x select 0});
    };
    if (_preferred isEqualTo [] && {count _list > 1}) then {
        PROBLEM_2("No preferred class for '%1' among %2; the first one is used",_key,_list apply {_x select 0});
    };
    private _winner = if (_preferred isEqualTo []) then {_list select 0} else {_preferred select 0};
    _bodyLoadToMag set [_key, _winner select 0];
} forEach _candidates;

// --- Belt segments -------------------------------------------------------------------
{
    private _read = [_x] call _fnc_readLoad;
    if (_read isEqualTo []) then {continue};
    _beltToLoad set [toLowerANSI configName _x, [_read select 0, getNumber (_x >> "count")]];
} forEach ("getNumber (_x >> 'cvo_redmag_isBelt') == 1" configClasses _cfgMagazines);

// --- Publish (read-only) ------------------------------------------------------------------
GVAR(categories) = compileFinal _categories;
GVAR(calibers) = compileFinal _calibers;
GVAR(cartridges) = compileFinal _cartridges;
GVAR(bodies) = compileFinal _bodies;
GVAR(roundTypeLevel) = compileFinal _roundTypeLevel;
GVAR(emptyToBody) = compileFinal _emptyToBody;
GVAR(packToRound) = compileFinal _packToRound;
GVAR(magToBodyLoad) = compileFinal _magToBodyLoad;
GVAR(bodyLoadToMag) = compileFinal _bodyLoadToMag;
GVAR(bodyLoads) = compileFinal _bodyLoads;
GVAR(beltToLoad) = compileFinal _beltToLoad;

INFO_4("Lookups built: %1 bodies / %2 cartridges / %3 magazines / %4 packs",count _bodies,count _cartridges,count _magToBodyLoad,count _packToRound);
if (_problems > 0) then {
    WARNING_1("Config validation found %1 problems; see the warnings above",_problems);
} else {
    INFO("Config validation passed");
};

_problems
