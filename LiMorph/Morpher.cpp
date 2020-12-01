#include "pch.h"
#include "Morpher.h"

#include <sstream>

#include "Logging.h"
#include "Functions.h"
#include "Token.h"

//extern uintptr_t base_address;
extern LiMorph::Morpher* morpher_ptr;

#define LIMORPH_LUA dummy;return

namespace LiMorph {

void Morpher::SendWoWMessage(const std::string& message,
    const std::string& color = "1,1,1") {
    std::string script = "DEFAULT_CHAT_FRAME:AddMessage('|cFF3480eb LiMorph:|r ";
    script += message;
    script += "'," + color + ")"; //RGB color of message.
    WoWFunctions::executeLUA(script);
}

void Morpher::reportParseError(const std::string& message) {
    SendWoWMessage(message, "1,0.1,0.1");
    m_lex.finish();
}

const char* Morpher::getMountEventLuaCode() {
    auto dummy = 0;
    #include "MountEvent.lua"
}

const char* Morpher::getShapeshiftEventLuaCode() {
    auto dummy = 0;
    #include "ShapeshiftEvent.lua"
}

const char* Morpher::getParseChatLuaCode() {
    auto dummy = 0;
    #include "ParseChat.lua"
}

void Morpher::forceUpdateModel() {
    m_last_morphed_id = m_player.getMorphID();
    m_player.setCurrentMorphIDInMemory(); //todo: remove since ran in updateModel right?
    WoWFunctions::updateModel(m_player_ptr);
}

void Morpher::updateModel() {
    m_player.setCurrentMorphIDInMemory();
    // todo: dont think this check is necessary if updateModel is always called 
    // correctly (i.e., not called when not necessary)
    if (m_last_morphed_id != m_player.getMorphID()) {
        forceUpdateModel();
    }
}

void Morpher::_morph(int morph_id) {
    m_player.setCurrentMorphID(morph_id);
    updateModel();
}

void Morpher::morphShapeshift(ShapeshiftForm form_id, int morph_id) {
    switch (form_id) {
    case ShapeshiftForm::HUMANOID:
    case ShapeshiftForm::CAT:
    case ShapeshiftForm::TREE_OF_LIFE:
    case ShapeshiftForm::TRAVEL:
    case ShapeshiftForm::AQUATIC:
    case ShapeshiftForm::BEAR:
    case ShapeshiftForm::GHOST_WOLF:
    case ShapeshiftForm::SWIFT_FLIGHT:
    case ShapeshiftForm::SHADOW:
    case ShapeshiftForm::STEALTH:
    case ShapeshiftForm::FLIGHT:
    case ShapeshiftForm::MOONKIN:
        m_player.setShapeshiftID(form_id, morph_id);
        WoWFunctions::executeLUA("player_shapeshift_event()");
        break;
    default:
        reportParseError("Invalid form id.");
        break;
    }
}

// todo: allow any shapeshift to become "transparent", meaning that 
// it will have the same morph as HUMANOID by default (e.g., .mapshapeshift 5 0 for 
// mapping bear form to humanoid form
void Morpher::morphTransparentShapeshift(ShapeshiftForm form_id, bool force_morph) {
    int current_id = m_player.getShapeshiftID(form_id);
    // shapeshift has been morphed, morph into it
    if (current_id) {
        // true transparent shapeshifts have the same original id as humanoid,
        // a fake transparent shapeshift wouldnt (fix if this is implemented)
        m_player.setCurrentOriginalShapeshiftID(ShapeshiftForm::HUMANOID);
        _morph(current_id);
    }
    //otherwise use humanoid morph
    else {
        if (force_morph) {
            // This is run if .morph is done while in a transparent shapeshift.
            // This will update the HUMANOID morph id, but we also want it to
            // update the model of the transparent shapeshift in that case since it
            // is the same as the HUMANOID by default.
            m_player.setCurrentOriginalShapeshiftID(ShapeshiftForm::HUMANOID); // probably not needed
            _morph(m_player.getShapeshiftID(ShapeshiftForm::HUMANOID));
        }
        else {
            // otherwise set current morph ID in memory for consistency 
            // note: probably not needed. (todo: check if actually needed, 
            // entering transparent shapeshift might actually set current morph id to 
            // original HUMANOID morph id)
            m_player.setCurrentMorphIDInMemory();
        }
    }
}

// callback invoked by the lua function player_shapeshift_event_() on a shapeshift event with force_morph = false
// also invoked by lua function player_shapeshift_event() with force_morph = true, which is called explicitly by
// ExecuteLua("player_shapeshift_event()") (e.g., on .morph and .shapeshift, see morphShapeshift(...) above)
void Morpher::smartMorphShapeshift(uintptr_t lua_state) {
    int top = WoWFunctions::luaGetTop(lua_state);
    if (top) {
        int id = WoWFunctions::luaToNumber(lua_state, top-1);
        int force_morph = WoWFunctions::luaToNumber(lua_state, top);

        //  SendWoWMessage("id: " + std::to_string(id));
        //  SendWoWMessage("ev: " + std::to_string(ev));

        auto shapeshift_form = static_cast<ShapeshiftForm>(id);
        int current_morph_id_in_memory = m_player.getMorphIDInMemory();

        // shapeshifts that only change transparancy (or nothing) by default need special treatment
        // since they should inherit humanoid behaviour if not shapeshifted. Also, they dont
        // set any morph id in memory (todo: check if true. update: think they do, but it is the 
        // original morph id of HUMANOID, so this special case is still needed)
        switch (shapeshift_form) {
        case ShapeshiftForm::SHADOW:
        case ShapeshiftForm::STEALTH:
            morphTransparentShapeshift(shapeshift_form, force_morph);
            return;
        default:
            break;
        }
        // All other shapeshifts will set the morph id in memory. Only run 
        // When the morph id has actually changed in memory: update the model accordingly.
        // This is needed because UPDATE_SHAPESHIFT_FORM is called multiple times when 
        // entering/exiting shapeshift forms. This makes the shapeshit animations look clean 
        // since it is only done once (i.e., probably at the last event).
        // Also ran if force_morph = true (so that the shapeshifts can be updated accordingly 
        // upon .morph and .shapeshift; these do not modify morph id in memory obviously)
        if (current_morph_id_in_memory != m_player.getMorphID() || force_morph) {
            switch (shapeshift_form) {
            case ShapeshiftForm::HUMANOID:
                m_player.setCurrentOriginalShapeshiftID(shapeshift_form); // note: will be current for shadow/stealth too (as wanted)
                _morph(m_player.getShapeshiftID(shapeshift_form));
                break;
            case ShapeshiftForm::CAT:
            case ShapeshiftForm::TREE_OF_LIFE:
            case ShapeshiftForm::TRAVEL:
            case ShapeshiftForm::AQUATIC:
            case ShapeshiftForm::BEAR:
            case ShapeshiftForm::GHOST_WOLF:
            case ShapeshiftForm::SWIFT_FLIGHT:
            case ShapeshiftForm::FLIGHT:
            case ShapeshiftForm::MOONKIN:
            {
                //  SendWoWMessage("NON HUMANOID FORM");
                m_player.setCurrentOriginalShapeshiftID(shapeshift_form);
                int current_form_id = m_player.getShapeshiftID(shapeshift_form);
                _morph(current_form_id ? current_form_id : current_morph_id_in_memory);
                break;
            }
            default:
                // This makes non-supported shapeshifts work as normal
                WoWFunctions::updateModel(m_player_ptr); // might be risky
                break;
            }
        }
    }
}

void Morpher::morphRace(int race_id) {
    m_player.setRaceID(race_id);
    RaceIDs id = static_cast<RaceIDs>(race_id);
    switch (id) {
    case RaceIDs::HUMAN:
    case RaceIDs::ORC:
    case RaceIDs::DWARF:
    case RaceIDs::NELF:
    case RaceIDs::UNDEAD:
    case RaceIDs::TAUREN:
    case RaceIDs::GNOME:
    case RaceIDs::TROLL:
    case RaceIDs::GOBLIN:
    case RaceIDs::BELF:
    case RaceIDs::DRAENEI:
    case RaceIDs::WORGEN:
    case RaceIDs::PANDAREN:
    case RaceIDs::NIGHTBORNE:
    case RaceIDs::HIGHMOUNTAIN_TAUREN:
    case RaceIDs::VOID_ELF:
    case RaceIDs::LIGHTFORGED_DRAENEI:
    case RaceIDs::ZANDALARI_TROLL:
    case RaceIDs::KUL_TIRAN:
    case RaceIDs::DARK_IRON_DWARF:
    case RaceIDs::VULPERA:
    case RaceIDs::MAGHAR_ORC:
    case RaceIDs::MECHAGNOME:
        if (!m_player.getGenderID())
            morphShapeshift(ShapeshiftForm::HUMANOID, 
                static_cast<int>(WoWUtils::race_male.at(id)));
        else
            morphShapeshift(ShapeshiftForm::HUMANOID, 
                static_cast<int>(WoWUtils::race_female.at(id)));
        break;
    default:
        reportParseError("Invalid race ID.");
        return;
    }
}

void Morpher::updateGender(int gender_id) {
    RaceIDs race_id = static_cast<RaceIDs>(m_player.getRaceID());
    if (!gender_id) {
        morphShapeshift(ShapeshiftForm::HUMANOID, 
            static_cast<int>(WoWUtils::race_male.at(race_id)));
        m_player.setGenderID(0);
    }
    else {
        morphShapeshift(ShapeshiftForm::HUMANOID,
            static_cast<int>(WoWUtils::race_female.at(race_id)));
        m_player.setGenderID(1);
    }
}

void Morpher::morphGender(int gender_id) {
    switch (gender_id) {
    case 0:
        updateGender(0);
        break;
    case 1:
        updateGender(1);
        break;
    case -1:
        if (!m_player.getGenderID())
            updateGender(1);
        else
            updateGender(0);
        break;
    default:
        reportParseError("Invalid gender ID.");
        return;
    }
}

// Replica Warlord's Satin Mantle: 77899
// Pauldrons of Arcane Rage: 24024
void Morpher::morphItem(int item, int item_id, int item_version = -1) {
    Items item_ = static_cast<Items>(item);
    switch (item_) {
    case Items::HEAD:
    case Items::SHOULDERS:
    case Items::BACK:
    case Items::CHEST:
    case Items::SHIRT:
    case Items::TABARD:
    case Items::WRISTS:
    case Items::MAIN_HAND:
    case Items::OFF_HAND:
    case Items::GLOVES:
    case Items::BELT:
    case Items::LEGS:
    case Items::FEET:
        m_player.setItemID(item_, item_id);
        if (item_version != -1)
            m_player.setItemVersionID(item_, item_version); // todo, maybe check that its a valid item version
        break;
    default:
        reportParseError("Invalid item ID.");
        return;
    }
    forceUpdateModel();
}

void Morpher::morphEnchant(int item, int enchant_id) {
    switch (item) {
    case 1:
        m_player.setItemEnchantID(Items::MAIN_HAND, enchant_id);
        break;
    case 2:
        m_player.setItemEnchantID(Items::OFF_HAND, enchant_id);
        break;
    default:
        reportParseError("Can not enchant that item.");
        return;
    }
    forceUpdateModel();
}

void Morpher::morphMount() {
    if (m_player.mountMorphed()) {
       WoWFunctions::updateMountModel(m_player_ptr, m_player.getMountID());
    }
   // SendWoWMessage("reached morph mount");
   // m_player.setCurrentMorphIDInMemory();
    updateModel();
}

void Morpher::morphTitle(int title_id) {
    m_player.setTitleID(title_id);
    forceUpdateModel();
}

/*
void UpdateModel() {
    WoWFunctions::updateModel(WoWFunctions::getUnitFromName("Player"));
    std::string msg = "HELLO THEREEE";
    const std::string& color = "0,1,1";
    std::string script = "DEFAULT_CHAT_FRAME:AddMessage('MORPHER: ";
    script += msg;
    script += "'," + color + ")"; //RGB color of message.
    WoWFunctions::executeLUA(script);
}
*/

void __fastcall Morpher::updateDisplayInfoHook(uintptr_t unit) {
    //__SendMessage("HOOKED DISPLAY.");

    //if (unit != WoWFunctions::getUnitFromName("Player")) {
    //    WoWFunctions::updateModel(unit);
    //}
}

void Morpher::registerFunctions() {
    WoWFunctions::registerFunction("ParseChat", (uintptr_t)chatCallback);
    WoWFunctions::registerFunction("MorphMount", (uintptr_t)mountCallback);
    WoWFunctions::registerFunction("MorphShapeshift", (uintptr_t)shapeshiftCallback);
}

void Morpher::registerLuaEvents() {
    WoWFunctions::executeLUA(getMountEventLuaCode());
    WoWFunctions::executeLUA(getShapeshiftEventLuaCode());
    WoWFunctions::executeLUA(getParseChatLuaCode());
}

void Morpher::hookUpdateDisplayInfo() {
    VMTHook* m_hook = new VMTHook(reinterpret_cast<void*>(m_player_ptr));
    m_hook->HookFunction(updateDisplayInfoHook, Offsets::update_display_info_vtable_offset);
}


void Morpher::initializeMorpher() {
    m_player_ptr = getPlayerPtr();
    m_player = Player(m_player_ptr);
    m_player.initializePlayer();
    m_last_morphed_id = m_player.getMorphID();

    hookUpdateDisplayInfo();
    registerFunctions();
    registerLuaEvents();

    SendWoWMessage("Loaded. Type .commands for usable commands.");
    
        /*
    std::stringstream stream;
    stream << std::hex << m_base_address;
    SendWoWMessage("Base address: " + stream.str());
    stream.str("");
    stream << std::hex << m_player_ptr;
    SendWoWMessage("Player_ptr address: " + stream.str());
    SendWoWMessage("MORPHER LOADED. Type .commands for usable commands.");
    SendWoWMessage("MorphID: " + std::to_string(m_player.getMorphID()));

    */
}

void Morpher::zoning() {
    hookUpdateDisplayInfo();
    m_player.restorePlayer(); // note: does not set current morph id in memory (its taken care of by player_shapeshift_event() below)

    //when zoning wow will call UpdateDisplayInfo with a new model (original model of whichever shapeshift),
    //pressumably with current morph id in memory as the model
    m_last_morphed_id = m_player.getMorphIDInMemory(); 

    WoWFunctions::executeLUA("player_shapeshift_event()");

    // needed incase mount state is different between enter and exit zoning
    // (in which case mount event is not triggered automatically)
    WoWFunctions::executeLUA("player_mount_event()");

   // forceUpdateModel();
}

void Morpher::zoningUpdateModel() {
    forceUpdateModel();
}

//undead male: 54041
//female NE: 54439

// fire bear: 66718
// ice bea: 66719

// cat 8: 66795
// cat 7:  66794

// swift flight form 21243

int __cdecl Morpher::chatCallback(uintptr_t lua_state) {
    morpher_ptr->parseChat(lua_state);
    return 0;
}

int __cdecl Morpher::mountCallback(uintptr_t lua_state) {
    morpher_ptr->morphMount();
    return 0;
}

int __cdecl Morpher::shapeshiftCallback(uintptr_t lua_state) {
    morpher_ptr->smartMorphShapeshift(lua_state);
    return 0;
}

void Morpher::initializeMorpherCallback() {
    morpher_ptr->initializeMorpher();
}

void Morpher::hookUpdateDisplayInfoCallback() {
    morpher_ptr->hookUpdateDisplayInfo();
}

void Morpher::zoningCallback() {
    morpher_ptr->zoning();
}

void Morpher::zoningUpdateModelCallback() {
    morpher_ptr->zoningUpdateModel();
}

void Morpher::startMorpher() {
    MainThread ts = MainThread();

    //Logging::Print("flag: " + std::to_string(Memory::readMemory<int>(m_base_address + Offsets::in_game_flag)) + "\n");
    // Login: 0, 1, 1037, 1545, 1561, 1553.
    // Reload: 1553, 1293, 1817, 1553
    // zoning: 1553, 1537, 1553
    // logout: 1553, 0

    bool morpher_loaded = false;
    uint16_t flag = Memory::readMemory<uint16_t>(m_base_address + Offsets::in_game_flag);
    uint16_t flag_updated = flag;
    while (true) {
        flag_updated = Memory::readMemory<uint16_t>(m_base_address + Offsets::in_game_flag);
        if (flag_updated != flag) {
            while (getPlayerPtr() == 0);
            m_player_ptr = getPlayerPtr();
            m_player.setPlayerPtr(m_player_ptr);
            if (flag_updated == 1553) {
                switch (flag) {
                case 1037:
                case 1545:
                case 1561:
                    // This is reached after a login loading screen
                    ts.invokeInMainThread(initializeMorpherCallback);
                    morpher_loaded = true;
                    break;
                case 1293:
                case 1809:
                    // This is reached after a /reload loading screen
                    // m_player.restorePlayer(); //not needed for /reload
                    ts.invokeInMainThread(hookUpdateDisplayInfoCallback); // not necessary to rehook on /reload i think
                    ts.invokeInMainThread(registerFunctions);
                    ts.invokeInMainThread(registerLuaEvents);
                    break;
                case 1537:
                    // This is reached after a loading screen cause by any form of zoning in game
                   ts.invokeInMainThread(zoningCallback);
                   Sleep(200);
                   ts.invokeInMainThread(zoningUpdateModelCallback);
                   Sleep(1000);
                   ts.invokeInMainThread(zoningUpdateModelCallback);
                    break;
                }
            }
            flag = flag_updated;
        }
        else {
            // already in world, load morpher if not loaded already
            if (flag_updated == 1553 && flag == 1553) {
                if (!morpher_loaded) { // todo: is this really needed? (wow doesnt allow the same dll to be injected again)
                    while (getPlayerPtr() == 0); 
                    ts.invokeInMainThread(initializeMorpherCallback);
                    morpher_loaded = true;
                }
            }
        }
    }
}

uintptr_t Morpher::getPlayerPtr() {
    return WoWFunctions::getUnitFromName("Player");
}

// Chat Parsing
void Morpher::parseChat(uintptr_t lua_state) {
    //SendWoWMessage("well that worked..");
    int top = WoWFunctions::luaGetTop(lua_state);
    if (top) {
        std::string msg = WoWFunctions::luaToString(lua_state, top, 0);
        m_lex.setText(msg);
        Token t = m_lex.nextToken();
        TokenType current = t.type();
        while (current != TokenType::END) {
            switch (current) {
            case TokenType::RESET:
                resetMorpher();
                return;
            case TokenType::COMMANDS:
                parseCommands();
                return;
            case TokenType::MORPH:
                parseMorph();
                return;
            case TokenType::RACE:
                parseRace();
                return;
            case TokenType::GENDER:
                parseGender();
                return;
            case TokenType::ITEM:
                parseItem();
                return;
            case TokenType::ENCHANT:
                parseEnchant();
                return;
            case TokenType::MOUNT:
                parseMount();
                return;
            case TokenType::TITLE:
                parseTitle();
                return;
            case TokenType::SHAPESHIFT:
                //reportParseError("Currently not supported.");
                parseShapeshift();
                return;
            case TokenType::NUMBER:
                reportParseError("Command can not start with number.");
                return;
            case TokenType::END:
                reportParseError("UNREACHABLE END");
                return;
            case TokenType::UNKNOWN:
                reportParseError("Unknown command. Type .commands to see available commands.");
                return;
            default:
                reportParseError("UNREACHABLE!!");
                //UnknownCommand(TokenType::UNKNOWN);
                return;
            }
            current = m_lex.nextToken().type();
        }
    }
}

void Morpher::parseCommands() {
    std::string required_color = "E74C3C";
    std::string optional_color = "28B463";
    SendWoWMessage("|cFF" + required_color + "Required|r |cFF" + optional_color + "Optional|r");
  //  SendWoWMessage("Existing commands: ");
    SendWoWMessage(".morph |cFF" + required_color +  "morph_id|r");
    SendWoWMessage(".race |cFF" + required_color + "race_id|r");
    SendWoWMessage(".gender |cFF" + optional_color + "[0-1]|r");
    SendWoWMessage(".item |cFF" + required_color + "item|r |cFF" + required_color + "item_id|r |cFF" + optional_color + "version_id|r");
    SendWoWMessage(".mount |cFF" + required_color + "mount_id|r");
    SendWoWMessage(".title |cFF" + required_color + "title_id|r");
    SendWoWMessage(".enchant |cFF" + required_color + "[1-2]|r |cFF" + required_color + "enchant_id|r");
    SendWoWMessage(".shapeshift |cFF" + required_color + "form_id|r |cFF" + required_color + "morph_id|r");
    SendWoWMessage(".reset");
    m_lex.finish();
}

void Morpher::parseMorph() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
        morphShapeshift(ShapeshiftForm::HUMANOID, next.toNumber());
    }
    else {
        reportParseError("Invalid .morph operand (must be number).");
    }
    m_lex.finish();
}

void Morpher::parseRace() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
        morphRace(next.toNumber());
    }
    else {
        reportParseError("Invalid .race operand (must be number).");
    }
    m_lex.finish();
}

void Morpher::parseGender() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
        morphGender(next.toNumber());
    }
    else {
        morphGender(-1);
    }
    m_lex.finish();
}

void Morpher::parseItem() {
    Token next = m_lex.nextToken();
    int item;
    int item_id;
    if (next.type() == TokenType::NUMBER) {
        item = next.toNumber();
        next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            item_id = next.toNumber();
            next = m_lex.nextToken();
            if (next.type() == TokenType::NUMBER) {
                int item_version_id = next.toNumber();
                morphItem(item, item_id, item_version_id);
            }
            else {
                morphItem(item, item_id);
            }
        }
        else {
            reportParseError("Invalid .item second operand (must be number).");
        }
    }
    else {
        reportParseError("Invalid .item first operand (must be number).");
    }
    m_lex.finish();
}

void Morpher::parseEnchant() {
    Token next = m_lex.nextToken();
    int item;
    int enchant_id;
    if (next.type() == TokenType::NUMBER) {
        item = next.toNumber();
        next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            enchant_id = next.toNumber();
            morphEnchant(item, enchant_id);
        }
        else {
            reportParseError("Invalid .enchant second operand (must be number).");
        }
    }
    else {
        reportParseError("Invalid .enchant first operand (must be number).");
    }
    m_lex.finish();
}

void Morpher::parseMount() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
        m_player.setMountID(next.toNumber());
        // WoWFunctions::executeLUA("last_id = -1");
        WoWFunctions::executeLUA("player_mount_event()");
    }
    else {
        reportParseError("Invalid .mount operand (must be number).");
    }
    m_lex.finish();
}

void Morpher::parseTitle() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
        morphTitle(next.toNumber());
    }
    else {
        reportParseError("Invalid .title operand (must be number).");
    }
    m_lex.finish();
}


void Morpher::parseShapeshift() {
    Token next = m_lex.nextToken();
    int form_id;
    int morph_id;
    if (next.type() == TokenType::NUMBER) {
        form_id = next.toNumber();
        next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            morph_id = next.toNumber();
            morphShapeshift(static_cast<ShapeshiftForm>(form_id), morph_id);
        }
        else {
            reportParseError("Invalid .shapeshift second operand.");
        }
    }
    else {
        reportParseError("Invalid .shapeshift first operand.");
    }
    m_lex.finish();
}

void Morpher::resetMorpher() {
    m_player.resetPlayer();
    WoWFunctions::executeLUA("player_shapeshift_event()");
    //WoWFunctions::updateModel(m_player_ptr);

    // TODO: see if running the line below works well (i.e resetting mount while on it)
    //WoWFunctions::executeLUA("player_mount_event()");
    m_lex.finish();
}

/*

// iterating object manager (not used)
uintptr_t Morpher::getPlayerPtr() {
    uintptr_t obj_manager =
        Memory::readMemory<uintptr_t>(m_base_address + Offsets::object_manager);
    //outfile << "objManger: " << std::hex << objManager << "\n";
    WGUID active_player_GUID =
        Memory::readMemory<WGUID>(obj_manager + Offsets::active_player_GUID);
    //outfile << "activePlayerGUID_high: " << activePlayerGUID.m_high << "\n";
    //outfile << "activePlayerGUID_low: " << activePlayerGUID.m_low << "\n";

    uintptr_t first_object =
        Memory::readMemory<uintptr_t>(obj_manager + Offsets::first_object);
    uintptr_t current_object = first_object;

    while (current_object != 0x0) {
        //outfile << "currentObj: " << std::hex << currentObj << "\n";
        WGUID currentGUID =
            Memory::readMemory<WGUID>(current_object + Offsets::guid);

        if (currentGUID.m_high == active_player_GUID.m_high &&
            currentGUID.m_low == active_player_GUID.m_low)
        {
            //outfile << "currentObj: " << std::hex << currentObj << "\n";
            //char type = *((char*)(currentObj + standardOffset + 0x10));
            //outfile << "type: " << (int)type << "\n";
            //outfile << "GUID_high: " << activePlayerGUID.m_high << "\n";
            //outfile << "GUID_low: " << activePlayerGUID.m_low << "\n";
            return current_object + Offsets::standard_offset;
        }
        current_object = Memory::readMemory<uintptr_t>(current_object);
    }
    return 0;
}

*/

} //namespace morph