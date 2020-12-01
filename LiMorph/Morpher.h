#pragma once

#include "Offsets.h"
#include "Memory.h"
#include "MainThread.h"
#include "Player.h"
#include "Lexer.h"
#include "WoWUtils.h"
#include "VMTHook.h"


//12E0

namespace LiMorph {

// { "maxWoWAddress", "48 8B 05 ? ? ? 01 48 85 D2 74 09 48 85 C0 0F 85 ? 00 00 00", SignatureType::NORMAL, 0x3, 0x0 },

class Morpher
{
public:
	Morpher() {}
	~Morpher() {}
	void startMorpher();

	inline void setBaseAddress(uintptr_t base_address) {
		m_base_address = base_address;
	}

	inline uintptr_t getBaseAddress() {
		return m_base_address;
	}

private:
	// load lua code into c++
	static const char* getMountEventLuaCode();
	static const char* getShapeshiftEventLuaCode();
	static const char* getParseChatLuaCode();

	// lua callbacks
	static int chatCallback(uintptr_t lua_state);
	static int mountCallback(uintptr_t lua_state);
	static int shapeshiftCallback(uintptr_t lua_state);

	// main thread callbacks
	static void initializeMorpherCallback();
	static void hookUpdateDisplayInfoCallback();
	static void registerFunctions();
	static void registerLuaEvents();
	static void zoningCallback();
	static void zoningUpdateModelCallback();


	// CGUnit_C__UpdateDisplayInfo callback
	static void __fastcall updateDisplayInfoHook(uintptr_t unit);


	void zoning();
	void zoningUpdateModel();
	void initializeMorpher();
	void hookUpdateDisplayInfo();
	void updateModel();
	void forceUpdateModel();

	void SendWoWMessage(const std::string& message, const std::string& color);
	uintptr_t getPlayerPtr();

	// Morphing
	void _morph(int morph_id);
	void morphRace(int race_id);
	void morphGender(int gender_id);
	void updateGender(int gender);
	void morphItem(int item, int item_id, int item_version);
	void morphEnchant(int item, int enchant_id);
	void morphMount();
	void morphTitle(int title_id);
	void morphShapeshift(ShapeshiftForm form_id, int morph_id);
	void smartMorphShapeshift(uintptr_t lua_state);
	void morphTransparentShapeshift(ShapeshiftForm form_id, bool force_morph);

	// Parsing
	void parseChat(uintptr_t lua_state);
	void reportParseError(const std::string& message);
	void parseMorph();
	void parseRace();
	void parseGender();
	void parseItem();
	void parseEnchant();
	void parseMount();
	void parseTitle();
	void parseShapeshift();
	void parseCommands();
	void resetMorpher();

	uintptr_t m_base_address;
	uintptr_t m_player_ptr;
	uintptr_t m_customization_ptr;
	Player m_player;
	Lexer m_lex;
	int m_last_morphed_id = 0;
};

} // namespace morph
