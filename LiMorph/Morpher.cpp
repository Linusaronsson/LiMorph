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

const char* Morpher::getParseChatLuaCode() {
    auto dummy = 0;
    #include "ParseChat.lua"
}

const char* Morpher::getClickMorphingCode() {
    auto dummy = 0;
    #include "ClickMorphing.lua"
}

const char* Morpher::getClickMountMorphingCode() {
    auto dummy = 0;
    #include "ClickMountMorphing.lua"
}

int __cdecl Morpher::chatCallback(uintptr_t lua_state) {
    morpher_ptr->parseChat(lua_state);
    return 0;
}

int __cdecl Morpher::mountCallback(uintptr_t lua_state) {
    morpher_ptr->morphMount();
    return 0;
}

int __cdecl Morpher::morphItemCallback(uintptr_t lua_state) {
    int top = WoWFunctions::luaGetTop(lua_state);
    if (top) {
        int item = WoWFunctions::luaToNumber(lua_state, top - 2);
        int item_id = WoWFunctions::luaToNumber(lua_state, top - 1);
        int item_version = WoWFunctions::luaToNumber(lua_state, top);
        morpher_ptr->morphItem(item, item_id, item_version);
    }
    return 0;
}

int __cdecl Morpher::morphEnchantCallback(uintptr_t lua_state) {
    int top = WoWFunctions::luaGetTop(lua_state);
    if (top) {
        int item = WoWFunctions::luaToNumber(lua_state, top - 1);
        int enchant_id = WoWFunctions::luaToNumber(lua_state, top);
        morpher_ptr->morphEnchant(item, enchant_id);
    }
    return 0;
}

int __cdecl Morpher::morphMountCallback(uintptr_t lua_state) {
    int top = WoWFunctions::luaGetTop(lua_state);
    if (top) {
        int mount_id = WoWFunctions::luaToNumber(lua_state, top);
        morpher_ptr->morphMountByID(mount_id);
    }
    return 0;
}

void Morpher::initializeMorpherCallback() {
    morpher_ptr->initializeMorpher();
}

void Morpher::hookUpdateDisplayInfoCallback() {
    morpher_ptr->hookUpdateDisplayInfo();
}

void Morpher::registerFunctions() {
    WoWFunctions::registerFunction("ParseChat", (uintptr_t)chatCallback);
    WoWFunctions::registerFunction("MorphMount_", (uintptr_t)mountCallback);
   //WoWFunctions::registerFunction("MorphTransparentShapeshift", (uintptr_t)shapeshiftCallback);
    WoWFunctions::registerFunction("MorphItem", (uintptr_t)morphItemCallback);
    WoWFunctions::registerFunction("MorphEnchant", (uintptr_t)morphEnchantCallback);
    WoWFunctions::registerFunction("MorphMount", (uintptr_t)morphMountCallback);
    //WoWFunctions::registerFunction("PlayerPtr", (uintptr_t)WoWFunctions::getUnitFromName("Player"));

}

void Morpher::registerLuaEvents() {
    WoWFunctions::executeLUA(getMountEventLuaCode());
    //WoWFunctions::executeLUA(getShapeshiftEventLuaCode());
    WoWFunctions::executeLUA(getParseChatLuaCode());
    WoWFunctions::executeLUA(getClickMorphingCode());
    WoWFunctions::executeLUA(getClickMountMorphingCode());
}

void Morpher::zoningCallback() {
    morpher_ptr->zoning();
}

void Morpher::zoning() {
    hookUpdateDisplayInfo();
    m_player.restorePlayer(); // note: does not set current morph id in memory (its taken care of by player_shapeshift_event() below)

    //when zoning wow will call UpdateDisplayInfo (while unhooked) with a new model (original model of whichever shapeshift),
    //pressumably with current morph id in memory as the model
    m_last_morphed_id = m_player.getMorphIDFromMemory();

    //WoWFunctions::executeLUA("player_shapeshift_event()");

    // needed incase mount state is different between enter and exit zoning
    // (in which case mount event is not triggered automatically)
    WoWFunctions::executeLUA("player_mount_event()");

    smartMorphShapeshift();
    updateModel();
}

void Morpher::initializeMorpher() {
    m_player_ptr = getPlayerPtr();
    m_player = Player(m_player_ptr);
    m_player.initializePlayer();
    m_last_morphed_id = m_player.getCurrentMorphID(); // maybe not needed or wrong
    //m_hook = new VMTHook(reinterpret_cast<void*>(m_player_ptr));
   // m_hook->PrintFunctions(m_base_address);
    hookUpdateDisplayInfo();
    registerFunctions();
    registerLuaEvents();

    morphRace(m_player.getRaceID());
    smartMorphShapeshift();

    //smartMorphShapeshift();
    SendWoWMessage("Loaded. Type .commands for usable commands.");
    
  
    /*
    std::stringstream stream;
    stream << std::hex << m_base_address;
    SendWoWMessage("Base address: " + stream.str());
    stream.str("");
    stream << std::hex << m_player_ptr;
    SendWoWMessage("Player_ptr address: " + stream.str());
    SendWoWMessage("raceID: " + std::to_string(m_player.getRaceID()));
    SendWoWMessage("genderID: " + std::to_string(m_player.getGenderID()));
    */
    

}

void Morpher::hookUpdateDisplayInfo() {
    VMTHook* hook = new VMTHook(reinterpret_cast<void*>(m_player_ptr));
    hook->HookFunction(updateDisplayInfoHook, Offsets::update_display_info_vtable_offset);
}

typedef   int(__fastcall* testFuncT)(uintptr_t, uint32_t arg, uint32_t arg2, uint32_t arg3);
void __fastcall Morpher::updateDisplayInfoHook(uintptr_t unit) {
    morpher_ptr->updateDisplayInfoCustom(unit);

     //morpher_ptr->SendWoWMessage("HOOKED DISPLAY: " + std::to_string(arg) + ",   " + std::to_string(arg2) + ",     " + std::to_string(arg3));
    // return morpher_ptr->m_hook->GetOriginalFunction<testFuncT>(morpher_ptr->m_func)(unit, 0, 27, arg3);
  //   morpher_ptr->SendWoWMessage(ret);
     //morpher_ptr->SendWoWMessage("Return val: " + std::to_string(ret));

     //if (unit != WoWFunctions::getUnitFromName("Player")) {
     //    WoWFunctions::updateModel(unit);
     //}

    // return 0;
    //return unit;
}

void Morpher::updateDisplayInfoCustom(uintptr_t unit) {
    //SendWoWMessage("Hello?");
    smartMorphShapeshift();
}

void Morpher::forceUpdateModel() {
    m_player.setCurrentMorphIDInMemory();
    m_last_morphed_id = m_player.getCurrentMorphID();
    WoWFunctions::updateModel(m_player_ptr);
}

void Morpher::updateModel() {
    m_player.setCurrentMorphIDInMemory();
    if (m_last_morphed_id != m_player.getCurrentMorphID()) {
        m_last_morphed_id = m_player.getCurrentMorphID();
        WoWFunctions::updateModel(m_player_ptr);
    }
}

uintptr_t Morpher::getPlayerPtr() {
    return WoWFunctions::getUnitFromName("Player");
}

int Morpher::getTargetMorphID() {
    uintptr_t target_ptr = WoWFunctions::getUnitFromName("Target");
    if (target_ptr) {
        return Memory::readMemory<int>(target_ptr + Offsets::morph_id);
    }
    else {
        reportParseError("You need to target something first.");
    }
    return 0;
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
    {
        if (morph_id == 0) 
            m_player.setShapeshiftTransparency(form_id, true);
         m_player.setShapeshiftID(form_id, morph_id);
        
        ShapeshiftForm current_shapeshift_form =
            static_cast<ShapeshiftForm>(m_player.getShapeshiftFormIDFromMemory());
        if (current_shapeshift_form == form_id ||
            (form_id == ShapeshiftForm::HUMANOID && m_player.isShapeshiftTransparent(current_shapeshift_form))) {
            smartMorphShapeshift(false);
        }
        break;
    }
    default:
        reportParseError("Invalid form id.");
        break;
    }
}

// Only responsible for setting the correct morph id as the current morph id
void Morpher::smartMorphShapeshift(bool set_original) {
    ShapeshiftForm form_id =
        static_cast<ShapeshiftForm>(m_player.getShapeshiftFormIDFromMemory());

    int morph_id_in_mem = m_player.getMorphIDFromMemory();
    int orig = morph_id_in_mem;
    if (morph_id_in_mem == m_player.getNativeMorphID()) {
        morph_id_in_mem = m_player.getOriginalShapeshiftID(ShapeshiftForm::HUMANOID);
        m_player.setMorphIDInMemory(morph_id_in_mem); // make sure UpdateModel is called even for transparent shapeshifts

        // native morph in mem but not humanoid - means its a transparent shapeshift (shadow, stealth or astral)
        if (form_id != ShapeshiftForm::HUMANOID) {
            m_player.setShapeshiftTransparency(form_id, true);
            is_moonkin_transparent = true; // astral form special case stuff
        }
      
    }
    else {
        if (form_id == ShapeshiftForm::HUMANOID && set_original) {
            m_last_morphed_id = morph_id_in_mem;
            WoWFunctions::updateModel(m_player_ptr);
            return;
        }
    }

    switch (form_id) {
    case ShapeshiftForm::MOONKIN:
        // astral form special case stuff
        if (orig != m_player.getNativeMorphID() && set_original && is_moonkin_transparent) {
            m_player.setShapeshiftTransparency(form_id, false);
            is_moonkin_transparent = false;
        }
    case ShapeshiftForm::SHADOW:
    case ShapeshiftForm::STEALTH:
    case ShapeshiftForm::HUMANOID:
    case ShapeshiftForm::CAT:
    case ShapeshiftForm::TREE_OF_LIFE:
    case ShapeshiftForm::TRAVEL:
    case ShapeshiftForm::AQUATIC:
    case ShapeshiftForm::BEAR:
    case ShapeshiftForm::GHOST_WOLF:
    case ShapeshiftForm::SWIFT_FLIGHT:
    case ShapeshiftForm::FLIGHT:
    {
        int original = morph_id_in_mem;
        if (m_player.isShapeshiftTransparent(form_id)) 
            morph_id_in_mem = m_player.getShapeshiftID(ShapeshiftForm::HUMANOID);
        
        if (set_original)
            m_player.setCurrentOriginalShapeshiftID(form_id, original); // note: will be current for shadow/stealth too (as wanted)
        int form_morph_id = m_player.getShapeshiftID(form_id);
        m_player.setCurrentMorphID(form_morph_id ? form_morph_id : morph_id_in_mem);
        updateModel();
        break;
    }   
    default:
        m_last_morphed_id = morph_id_in_mem;
        WoWFunctions::updateModel(m_player_ptr);
       // SendWoWMessage("UNREACHABLE: smartMorphShapeshift()");
        break;
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
    Items item_ = static_cast<Items>(item);
    switch (item_) {
    case Items::MAIN_HAND:
    case Items::OFF_HAND:
        m_player.setItemEnchantID(item_, enchant_id);
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
    updateModel();
}

void Morpher::morphMountByID(int mount_id) {
    m_player.setMountID(mount_id);
    WoWFunctions::executeLUA("player_mount_event()");
}

void Morpher::morphTitle(int title_id) {
    m_player.setTitleID(title_id);
    forceUpdateModel();
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
                   // iterateObjMgr();
                    morpher_loaded = true;
                }
            }
        }
    }

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
            case TokenType::NPC:
                parseMorphNPC();
                return;
            case TokenType::NPCID:
                parseNPCID();
                return;
            case TokenType::MOUNT:
                parseMount();
                return;
            case TokenType::TITLE:
                parseTitle();
                return;
            case TokenType::SHAPESHIFT:
                parseShapeshift();
                return;
            case TokenType::NUMBER:
                reportParseError("Command can not start with number.");
                return;
            case TokenType::END:
                reportParseError("UNREACHABLE END");
                return;
            case TokenType::STRING:
                reportParseError("Unknown command. Type .commands to see available commands.");
                return;
            default:
                reportParseError("UNREACHABLE: parseChat()");
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
    SendWoWMessage(".morph |cFF" + required_color +  "<morph_id>|r");
    SendWoWMessage(".morph |cFF" + required_color + "target|r");
    SendWoWMessage(".race |cFF" + required_color + "<race_id>|r");
    SendWoWMessage(".gender |cFF" + optional_color + "[0-1]|r");
    SendWoWMessage(".item |cFF" + required_color + "<item>|r |cFF" + required_color + "<item_id>|r |cFF" + optional_color + "<version_id>|r");
    SendWoWMessage(".item |cFF" + required_color + "target|r");
    SendWoWMessage(".item |cFF" + required_color + "<item>|r |cFF" + required_color + "target|r");
    SendWoWMessage(".mount |cFF" + required_color + "<mount_id>|r");
    SendWoWMessage(".title |cFF" + required_color + "<title_id>|r");
    SendWoWMessage(".enchant |cFF" + required_color + "[1-2]|r |cFF" + required_color + "<enchant_id>|r");
    SendWoWMessage(".shapeshift |cFF" + required_color + "<form_id>|r |cFF" + required_color + "<morph_id>|r");
    SendWoWMessage(".shapeshift |cFF" + required_color + "<form_id>|r |cFF" + required_color + "target|r");
    SendWoWMessage(".shapeshift |cFF" + required_color + "<form_id>|r |cFF" + required_color + "0|r");

    SendWoWMessage(".npcid");
    SendWoWMessage(".reset");

    SendWoWMessage("Note: You can alt-click items and mounts in the collection tab in order to morph them.");

    m_lex.finish();
}

// remember 93
//remember 103
// remember 109 ??
// 110 (on spell press (end of spell or interrupt spell channel))
// 114 (on model change)
// 119 cast spell probly
// 123 does something on shapeshift
// 125 (triggered on move key)
// 127 (on move key release)
// 130 (consistent on spell press)

typedef   bool(__fastcall* testFunc)(uint32_t, int, int64_t, int64_t, int64_t);
// 21 - get type?
// 49 seems to be type
void Morpher::parseMorph() {
    Token next = m_lex.nextToken();
    if (next.type() == TokenType::NUMBER) {
      // m_func = next.toNumber();
      // m_hook->HookFunction(updateDisplayInfoHook, next.toNumber());
        //testFunc _getUnitFromName =
        //    reinterpret_cast<testFunc>(morpher_ptr->getBaseAddress() + 0x1CDEC0);
       // _getUnitFromName(next.toNumber(), 0, 0, 0, 0);
       // SendWoWMessage("yoyo");

       //uint16_t ret = morpher_ptr->m_hook->GetOriginalFunction<testFunc>(morpher_ptr->m_func)(m_player_ptr);
       //morpher_ptr->SendWoWMessage("Returned: " + std::to_string(ret));
       //return;
       morphShapeshift(ShapeshiftForm::HUMANOID, next.toNumber());
    }
    else if(next.type() == TokenType::STRING) {
        std::string arg = next.toString();
        if (arg == "target") {
            int target_morph_id = getTargetMorphID();
            if(target_morph_id)
                morphShapeshift(ShapeshiftForm::HUMANOID, target_morph_id);
        }
        else
            reportParseError("Invalid .morph operand.");
    } else {
        reportParseError("Invalid .morph operand.");
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
    // m_hook->UnhookAllFunctions();
    //return;
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

               
                // m_hook->HookFunction(updateDisplayInfoHook, Offsets::update_display_info_vtable_offset);
                // m_hook->HookFunction(updateDisplayInfoHook, Offsets::update_display_info_vtable_offset+1);

                // m_hook->PrintFunctions(m_base_address, m_player_ptr);
                //uintptr_t* oring_funcs = m_hook->GetOriginalFunctions();
                //  for (int i = 0; i < m_hook->GetTotalFunctions(); i++) {
                    //  SendWoWMessage(std::to_string(i));
              //  uintptr_t func = oring_funcs[item] - m_base_address;
               // WoWFunctions::setVisualKit(WoWFunctions::getUnitFromName("Player"), func, item_id, item_version_id, item_version_id, item_version_id);



                morphItem(item, item_id, item_version_id);
            }
            else {
                morphItem(item, item_id);
            }
        }
        else if(next.type() == TokenType::STRING) {
            std::string arg = next.toString();
            if (arg == "target") {
                uintptr_t target_ptr = WoWFunctions::getUnitFromName("Target");
                if (target_ptr) {
                    m_player.copyPlayerItem(target_ptr, item);
                    forceUpdateModel();
                }
                else {
                    reportParseError("You need to target something first.");
                }
            }
            else
                reportParseError("Invalid .shapeshift second operand.");
        }
        else {
            reportParseError("Invalid .item second operand (must be number).");
        }
    }
    else if (next.type() == TokenType::STRING) {
        std::string arg = next.toString();
        if (arg == "target") {
            uintptr_t target_ptr = WoWFunctions::getUnitFromName("Target");
            if (target_ptr) {
                m_player.copyPlayerItems(target_ptr);
                forceUpdateModel();
            }
            else {
                reportParseError("You need to target something first.");
            }
        }
        else
            reportParseError("Invalid .shapeshift second operand.");
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
            switch (item) {
            case 1:
                morphEnchant(static_cast<int>(Items::MAIN_HAND), enchant_id);
                break;
            case 2:
                morphEnchant(static_cast<int>(Items::OFF_HAND), enchant_id);
                break;
            default:
                reportParseError("Can not enchant that item.");
                return;
            }
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
        morphMountByID(next.toNumber());
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
        } else if (next.type() == TokenType::STRING) {
            std::string arg = next.toString();
            if (arg == "target") {
                int target_morph_id = getTargetMorphID();
                if (target_morph_id)
                    morphShapeshift(static_cast<ShapeshiftForm>(form_id), target_morph_id);
            }
            else
                reportParseError("Invalid .shapeshift second operand.");
        } else {
            reportParseError("Invalid .shapeshift second operand.");
        }
    }
    else {
        reportParseError("Invalid .shapeshift first operand.");
    }
    m_lex.finish();
}

void Morpher::parseMorphNPC() {
    //morphShapeshift(ShapeshiftForm::HUMANOID, getTargetMorphID());
    m_lex.finish();
}

void Morpher::parseNPCID() {
    int target_morph_id = getTargetMorphID();
    if (target_morph_id) {
        SendWoWMessage(std::to_string(target_morph_id));
    }
    m_lex.finish();
}

void Morpher::resetMorpher() {
    m_player.resetPlayer();
    smartMorphShapeshift();
    forceUpdateModel(); 

    // TODO: see if running the line below works well (i.e resetting mount while on it)
    //WoWFunctions::executeLUA("player_mount_event()");
    m_lex.finish();
}



// iterating object manager (not used)
uintptr_t Morpher::iterateObjMgr() {
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
        uint8_t type =
            Memory::readMemory<uint8_t>(current_object + Offsets::object_type);
       
       // Logging::Print("Object_type: " + std::to_string(type) + "\n");

        if (type == 8) {
            //::Print("Object_type: " + std::to_string(type) + "\n");
            SendWoWMessage("obj ptr: " + std::to_string(current_object));
            SendWoWMessage("obj ptr2: " + std::to_string(getPlayerPtr()));
          //  Logging::Print("obj ptr: " + std::to_string(current_object));
          // Logging::Print("obj ptr2: " + std::to_string(getPlayerPtr()));


            return 0;
        }

        /*
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
        */
        current_object = Memory::readMemory<uintptr_t>(current_object);
    }
    return 0;
}



} //namespace morph