#pragma once

namespace morph {

	struct WGUID {
		unsigned long long m_high;
		unsigned long long m_low;
	};

	enum class ObjectType : uint8_t
	{
		None = 0x0,
		Item = 0x1,
		NPC = 0x5,
		OtherPlayers = 0x6,
		Player = 0x7,
	};



	enum class Items {
		HEAD = 1,
		SHOULDERS = 3,
		SHIRT = 4,
		CHEST = 5,
		BELT = 6,
		LEGS = 7,
		FEET = 8,
		WRISTS = 9,
		GLOVES = 10,
		BACK = 15,
		MAIN_HAND = 16,
		OFF_HAND = 17,
		TABARD = 19,
	};

	enum class ItemVersion {
		NORMAL = 0,
		HEROIC = 1,
		MYTHIC = 3,
		RAID_FINDER = 4,
		STAGE1 = 6,
		STAGE2 = 17,
		STAGE3 = 28,
	};

	constexpr int N_ITEMS = 13;

	const Items ITEMS_LIST[13] = {
		Items::HEAD,
		Items::SHOULDERS,
		Items::SHIRT,
		Items::CHEST,
		Items::BELT,
		Items::LEGS,
		Items::FEET,
		Items::WRISTS,
		Items::GLOVES,
		Items::BACK,
		Items::MAIN_HAND,
		Items::OFF_HAND,
		Items::TABARD
	};


	enum class RaceIDs {
		HUMAN = 1,
		ORC = 2,
		DWARF = 3,
		NELF = 4,
		UNDEAD = 5,
		TAUREN = 6,
		GNOME = 7,
		TROLL = 8,
		GOBLIN = 9,
		BELF = 10,
		DRAENEI = 11,
		WORGEN = 22,
		PANDAREN = 24,
		NIGHTBORNE = 27,
		HIGHMOUNTAIN_TAUREN = 28,
		VOID_ELF = 29,
		LIGHTFORGED_DRAENEI = 30,
		ZANDALARI_TROLL = 31,
		KUL_TIRAN = 32,
		DARK_IRON_DWARF = 34,
		VULPERA = 35,
		MAGHAR_ORC = 36,
		MECHAGNOME = 37
	};



	enum class RaceMorphIDs {
		BELF_M = 20578,
		BELF_F = 20579,

		GNOME_M = 20580,
		GNOME_F = 20581,

		TAUREN_M = 20585,
		TAUREN_F = 20584,

		HUMAN_M = 19723,
		HUMAN_F = 19724,

		GOBLIN_M = 37927,
		GOBLIN_F = 37928,

		DRAENEI_M = 37916,
		DRAENEI_F = 20323,

		TROLL_M = 20321,
		TROLL_F = 37922,

		UNDEAD_M = 37923,
		UNDEAD_F = 37924,

		DWARF_M = 20317,
		DWARF_F = 37918,

		ORC_F = 20316,
		ORC_M = 37920,

		NELF_M = 20318,
		NELF_F = 37919,

		WORGEN_F = 37914,
		WORGEN_M = 37915,

		PANDAREN_M = 55249,
		PANDAREN_F = 55248,


		NIGHTBORNE_M = 82375,
		NIGHTBORNE_F = 82376,


		HIGHMOUNTAIN_TAUREN_M = 82377,
		HIGHMOUNTAIN_TAUREN_F = 82378,

		VOID_ELF_M = 82371,
		VOID_ELF_F = 82372,

		LIGHTFORGED_DRAENEI_M = 82373,
		LIGHTFORGED_DRAENEI_F = 82374,


		ZANDALARI_TROLL_M = 88417,
		ZANDALARI_TROLL_F = 82411,

		KUL_TIRAN_M = 88414,
		KUL_TIRAN_F = 88413,

		DARK_IRON_DWARF_M = 88409,
		DARK_IRON_DWARF_F = 88408,

		VULPERA_M = 94999,
		VULPERA_F = 95001,

		MAGHAR_ORC_M = 88420,
		MAGHAR_ORC_F = 88410,

		MECHAGNOME_M = 94998,
		MECHAGNOME_F = 95000
	};

};