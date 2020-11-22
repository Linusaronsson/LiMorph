#pragma once

#include <map>

#include "Offsets.h"
#include "Memory.h"
#include "MainThread.h"
#include "Player.h"
#include "Lexer.h"
#include "WoWUtils.h"
#include "VMTHook.h"


//12E0

namespace morph {

	// { "maxWoWAddress", "48 8B 05 ? ? ? 01 48 85 D2 74 09 48 85 C0 0F 85 ? 00 00 00", SignatureType::NORMAL, 0x3, 0x0 },

	class Morpher
	{
	public:
		Morpher() {}
		~Morpher() {}
		void startMorpher();
		void parseChat(uintptr_t lua_state);

		inline void setBaseAddress(uintptr_t base_address) {
			m_base_address = base_address;
		}

		inline uintptr_t getBaseAddress() {
			return m_base_address;
		}

	private:
		static void chatCallback(uintptr_t lua_state);
		static void mountCallback(uintptr_t lua_state);

		static void __fastcall updateDisplayInfoHook(uintptr_t unit);
		static void run_in_main_thread();

		void initializeMorpher(int init_type);
		void initialLogin();

		uintptr_t getPlayerPtr();

		void SendWoWMessage(const std::string& message, const std::string& color);
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

		void _morph(int morph_id);
		void morphRace(int race_id);
		void morphGender(int gender_id);
		void updateGender(int gender);
		void morphItem(int item, int item_id, int item_version);
		void morphEnchant(int item, int enchant_id);
		void morphMount();
		void morphTitle(int title_id);
		void morphShapeshift(int form_id, int morph_id);

		// Normal instance variables
		uintptr_t m_base_address;
		uintptr_t m_player_ptr;
		uintptr_t m_customization_ptr;
		Player m_player;
		Lexer m_lex;

		const std::map<RaceIDs, RaceMorphIDs> m_race_male = {
		   {RaceIDs::HUMAN, RaceMorphIDs::HUMAN_M},
		   {RaceIDs::ORC, RaceMorphIDs::ORC_M},
		   {RaceIDs::DWARF, RaceMorphIDs::DWARF_M},
		   {RaceIDs::NELF, RaceMorphIDs::NELF_M},
		   {RaceIDs::UNDEAD, RaceMorphIDs::UNDEAD_M},
		   {RaceIDs::TAUREN, RaceMorphIDs::TAUREN_M},
		   {RaceIDs::GNOME, RaceMorphIDs::GNOME_M},
		   {RaceIDs::TROLL, RaceMorphIDs::TROLL_M},
		   {RaceIDs::GOBLIN, RaceMorphIDs::GOBLIN_M},
		   {RaceIDs::BELF, RaceMorphIDs::BELF_M},
		   {RaceIDs::DRAENEI, RaceMorphIDs::DRAENEI_M},
		   {RaceIDs::WORGEN, RaceMorphIDs::WORGEN_M},
		   {RaceIDs::PANDAREN, RaceMorphIDs::PANDAREN_M},
		   {RaceIDs::NIGHTBORNE, RaceMorphIDs::NIGHTBORNE_M},
		   {RaceIDs::HIGHMOUNTAIN_TAUREN, RaceMorphIDs::HIGHMOUNTAIN_TAUREN_M},
		   {RaceIDs::VOID_ELF, RaceMorphIDs::VOID_ELF_M},
		   {RaceIDs::LIGHTFORGED_DRAENEI, RaceMorphIDs::LIGHTFORGED_DRAENEI_M},
		   {RaceIDs::ZANDALARI_TROLL, RaceMorphIDs::ZANDALARI_TROLL_M},
		   {RaceIDs::KUL_TIRAN, RaceMorphIDs::KUL_TIRAN_M},
		   {RaceIDs::DARK_IRON_DWARF, RaceMorphIDs::DARK_IRON_DWARF_M},
		   {RaceIDs::VULPERA, RaceMorphIDs::VULPERA_M},
		   {RaceIDs::MAGHAR_ORC, RaceMorphIDs::MAGHAR_ORC_M},
		   {RaceIDs::MECHAGNOME, RaceMorphIDs::MECHAGNOME_M},
		};

		const std::map<RaceIDs, RaceMorphIDs> m_race_female = {
		   {RaceIDs::HUMAN, RaceMorphIDs::HUMAN_F},
		   {RaceIDs::ORC, RaceMorphIDs::ORC_F},
		   {RaceIDs::DWARF, RaceMorphIDs::DWARF_F},
		   {RaceIDs::NELF, RaceMorphIDs::NELF_F},
		   {RaceIDs::UNDEAD, RaceMorphIDs::UNDEAD_F},
		   {RaceIDs::TAUREN, RaceMorphIDs::TAUREN_F},
		   {RaceIDs::GNOME, RaceMorphIDs::GNOME_F},
		   {RaceIDs::TROLL, RaceMorphIDs::TROLL_F},
		   {RaceIDs::GOBLIN, RaceMorphIDs::GOBLIN_F},
		   {RaceIDs::BELF, RaceMorphIDs::BELF_F},
		   {RaceIDs::DRAENEI, RaceMorphIDs::DRAENEI_F},
		   {RaceIDs::WORGEN, RaceMorphIDs::WORGEN_F},
		   {RaceIDs::PANDAREN, RaceMorphIDs::PANDAREN_F},
		   {RaceIDs::NIGHTBORNE, RaceMorphIDs::NIGHTBORNE_F},
		   {RaceIDs::HIGHMOUNTAIN_TAUREN, RaceMorphIDs::HIGHMOUNTAIN_TAUREN_F},
		   {RaceIDs::VOID_ELF, RaceMorphIDs::VOID_ELF_F},
		   {RaceIDs::LIGHTFORGED_DRAENEI, RaceMorphIDs::LIGHTFORGED_DRAENEI_F},
		   {RaceIDs::ZANDALARI_TROLL, RaceMorphIDs::ZANDALARI_TROLL_F},
		   {RaceIDs::KUL_TIRAN, RaceMorphIDs::KUL_TIRAN_F},
		   {RaceIDs::DARK_IRON_DWARF, RaceMorphIDs::DARK_IRON_DWARF_F},
		   {RaceIDs::VULPERA, RaceMorphIDs::VULPERA_F},
		   {RaceIDs::MAGHAR_ORC, RaceMorphIDs::MAGHAR_ORC_F},
		   {RaceIDs::MECHAGNOME, RaceMorphIDs::MECHAGNOME_F},
		};
	};

} // namespace morph
