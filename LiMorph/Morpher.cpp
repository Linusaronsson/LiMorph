#include "pch.h"
#include "Morpher.h"

#include <sstream>

#include "Logging.h"
#include "Functions.h"
#include "Token.h"

//extern uintptr_t base_address;
extern morph::Morpher* morpher_ptr;

namespace morph {

    void Morpher::SendWoWMessage(const std::string& message,
        const std::string& color = "0,1,1") {
        std::string script = "DEFAULT_CHAT_FRAME:AddMessage('MORPHER: ";
        script += message;
        script += "'," + color + ")"; //RGB color of message.
        WoWFunctions::executeLUA(script);
    }

    void Morpher::reportParseError(const std::string& message) {
        SendWoWMessage(message, "1,0.1,0.1");
        m_lex.finish();
    }

    void Morpher::_morph(int morph_id) {
        m_player.setMorphID(morph_id);
        WoWFunctions::updateModel(m_player_ptr);
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
                _morph(static_cast<int>(m_race_male.at(id)));
            else
                _morph(static_cast<int>(m_race_female.at(id)));

            break;
        default:
            reportParseError("Invalid race ID.");
            return;
        }
    }

    void Morpher::updateGender(int gender_id) {
        RaceIDs race_id = static_cast<RaceIDs>(m_player.getRaceID());
        if (!gender_id) {
            _morph(static_cast<int>(m_race_male.at(race_id)));
            m_player.setGenderID(0);
        }
        else {
            _morph(static_cast<int>(m_race_female.at(race_id)));
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
        WoWFunctions::updateModel(m_player_ptr);
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
        WoWFunctions::updateModel(m_player_ptr);
    }

    void Morpher::morphMount() {
        if (m_player.mountMorphed()) {
            WoWFunctions::updateMountModel(m_player_ptr, m_player.getMountID());
        }

        // WoWFunctions::updateModel(m_player_ptr);
    }

    void Morpher::morphTitle(int title_id) {
        m_player.setTitleID(title_id);
        WoWFunctions::updateModel(m_player_ptr);
    }

    void Morpher::morphShapeshift(int form_id, int morph_id) {

    }

    void Morpher::run_in_main_thread() {
        Logging::Print("IN MAIN THREAD\nThread id: ");
        Logging::Print(std::to_string(GetCurrentThreadId()));
    }

    void Morpher::chatCallback(uintptr_t lua_state) {
        morpher_ptr->parseChat(lua_state);
    }

    void Morpher::mountCallback(uintptr_t lua_state) {
        morpher_ptr->morphMount();
    }

    void UpdateModel() {
        WoWFunctions::updateModel(WoWFunctions::getUnitFromName("Player"));
        std::string msg = "HELLO THEREEE";
        const std::string& color = "0,1,1";
        std::string script = "DEFAULT_CHAT_FRAME:AddMessage('MORPHER: ";
        script += msg;
        script += "'," + color + ")"; //RGB color of message.
        WoWFunctions::executeLUA(script);
    }


    void Morpher::initializeMorpher(int init_type = 0) {

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


        // Hook methods that update display info
        VMTHook* m_hook = new VMTHook((void*)m_player_ptr);
        m_hook->HookFunction(updateDisplayInfoHook, Offsets::update_display_info_vtable_offset);

        if (init_type == -1) {
            WoWFunctions::registerFunction("ParseChat", (uintptr_t)chatCallback);
            WoWFunctions::registerFunction("MorphMount", (uintptr_t)mountCallback);
            WoWFunctions::registerFunction("UpdateModel", (uintptr_t)UpdateModel);

            std::string s =
                #include "ParseChat.lua";
                std::string s2 =
                #include "MountEvent.lua";


                WoWFunctions::executeLUA(s);
            WoWFunctions::executeLUA(s2);
        }
    }

    void __fastcall Morpher::updateDisplayInfoHook(uintptr_t unit) {
        //__SendMessage("HOOKED DISPLAY.");

        //if (unit != WoWFunctions::getUnitFromName("Player")) {
        //    WoWFunctions::updateModel(unit);
        //}
    }


    void Morpher::initialLogin() {
        m_player = Player(m_player_ptr);
        m_player.initializePlayer();
        initializeMorpher(-1);
        //SendWoWMessage("MORPHER LOADED. Type .commands for usable commands.");
    }

    void Morpher::startMorpher() {
        // ThreadSynchronizer ts = ThreadSynchronizer();
        // ts.InvokeInMainThread(run_in_main_thread);

         //Logging::Print("flag: " + std::to_string(Memory::readMemory<int>(m_base_address + Offsets::in_game_flag)) + "\n");
         // Login: 0, 1, 1037, 1545, 1561, 1553.
         // Reload: 1553, 1293, 1817, 1553
         // zoning: 1553, 1537, 1553
         // logout: 1553, 0
        bool logged_in = false;
        uint16_t flag = Memory::readMemory<uint16_t>(m_base_address + Offsets::in_game_flag);
        uint16_t flag_updated = flag;
        while (true) {
            flag_updated = Memory::readMemory<uint16_t>(m_base_address + Offsets::in_game_flag);
            if (flag_updated != flag) {
                while (getPlayerPtr() == 0);
                m_player_ptr = getPlayerPtr();
                if (flag_updated == 1553) {
                    switch (flag) {
                    case 1037:
                    case 1545:
                    case 1561:
                        initialLogin();
                        logged_in = true;
                        //  WoWFunctions::executeLUA("print('you just logged in!')");
                        break;
                    case 1293:
                    case 1809:
                        m_player.setPlayerPtr(m_player_ptr);
                        m_player.restorePlayer();
                        WoWFunctions::updateModel(m_player_ptr);
                        initializeMorpher(-1); // /reload requires reregistering functions and events
                     //   WoWFunctions::executeLUA("print('you just did /reload!')");
                        break;
                    case 1537:
                        m_player.setPlayerPtr(m_player_ptr);
                        m_player.restorePlayer();
                        WoWFunctions::updateModel(m_player_ptr);
                        initializeMorpher(0);
                        //    WoWFunctions::executeLUA("print('you just zoned!')");
                        break;
                    }
                }


                flag = flag_updated;
            }
            else {
                if (flag_updated == 1553 && flag == 1553) {
                    if (!logged_in) {
                        while (getPlayerPtr() == 0);
                        initialLogin();
                        logged_in = true;
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
        SendWoWMessage("Existing commands: ");
        SendWoWMessage(".morph [id]");
        SendWoWMessage(".race [id]");
        SendWoWMessage(".item [item] [item_id]");
        SendWoWMessage(".gender [id]");
        m_lex.finish();
    }

    void Morpher::parseMorph() {
        Token next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            _morph(next.toNumber());
        }
        else {
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
            reportParseError("Invalid race ID.");
        }
        m_lex.finish();
    }


    // Think about logic here abit (should invert if no arg etc)
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
                reportParseError("Invalid .item second operand.");
            }
        }
        else {
            reportParseError("Invalid .item first operand.");
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
                reportParseError("Invalid .enchant second operand.");
            }
        }
        else {
            reportParseError("Invalid .enchant first operand.");
        }
        m_lex.finish();
    }

    void Morpher::parseMount() {
        Token next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            m_player.setMountID(next.toNumber());
            WoWFunctions::executeLUA("player_mount_event()");
        }
        else {
            reportParseError("Invalid .mount operand.");
        }
        m_lex.finish();
    }

    void Morpher::parseTitle() {
        Token next = m_lex.nextToken();
        if (next.type() == TokenType::NUMBER) {
            morphTitle(next.toNumber());
        }
        else {
            reportParseError("Invalid .title operand.");
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
                morphShapeshift(form_id, morph_id);
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
        WoWFunctions::updateModel(m_player_ptr);
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