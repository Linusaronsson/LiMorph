#pragma once

#include <map>

namespace LiMorph {


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



enum class ShapeshiftForm {
	HUMANOID = 0,
	CAT = 1,
	TREE_OF_LIFE = 2,
	TRAVEL = 3,
	AQUATIC = 4,
	BEAR = 5,
	GHOST_WOLF = 16,
	SWIFT_FLIGHT = 27,
	SHADOW = 28,
	FLIGHT = 29,
	STEALTH = 30,
	MOONKIN = 31,
};

constexpr int N_SHAPESHIFTS = 12;

const ShapeshiftForm SHAPESHIFT_LIST[N_SHAPESHIFTS] = {
	ShapeshiftForm::HUMANOID,
	ShapeshiftForm::CAT,
	ShapeshiftForm::TREE_OF_LIFE,
	ShapeshiftForm::TRAVEL,
	ShapeshiftForm::AQUATIC,
	ShapeshiftForm::BEAR,
	ShapeshiftForm::GHOST_WOLF,
	ShapeshiftForm::SWIFT_FLIGHT,
	ShapeshiftForm::SHADOW,
	ShapeshiftForm::FLIGHT,
	ShapeshiftForm::STEALTH,
	ShapeshiftForm::MOONKIN,
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

const Items ITEMS_LIST[N_ITEMS] = {
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

namespace WoWUtils {

	inline int shapeshiftToIndex(ShapeshiftForm item) {
		switch (item) {
		case ShapeshiftForm::HUMANOID: return 0;
		case ShapeshiftForm::CAT: return 1;
		case ShapeshiftForm::TREE_OF_LIFE: return 2;
		case ShapeshiftForm::TRAVEL: return 3;
		case ShapeshiftForm::AQUATIC: return 4;
		case ShapeshiftForm::BEAR: return 5;
		case ShapeshiftForm::GHOST_WOLF: return 6;
		case ShapeshiftForm::SWIFT_FLIGHT: return 7;
		case ShapeshiftForm::SHADOW: return 7;
		case ShapeshiftForm::FLIGHT: return 9;
		case ShapeshiftForm::MOONKIN: return 10;
		case ShapeshiftForm::STEALTH: return 11;
		}
		// Unreachable
		return -1;
	}

	inline int itemToIndex(Items item) {
		switch (item) {
		case Items::HEAD: return 0;
		case Items::SHOULDERS: return 1;
		case Items::SHIRT: return 2;
		case Items::CHEST: return 3;
		case Items::BELT: return 4;
		case Items::LEGS: return 5;
		case Items::FEET: return 6;
		case Items::WRISTS: return 7;
		case Items::GLOVES: return 8;
		case Items::BACK: return 9;
		case Items::MAIN_HAND: return 10;
		case Items::OFF_HAND: return 11;
		case Items::TABARD: return 12;
		}
		// Unreachable
		return -1;
	}

	const std::map<RaceIDs, RaceMorphIDs> race_male = {
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

	const std::map<RaceIDs, RaceMorphIDs> race_female = {
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
}
};