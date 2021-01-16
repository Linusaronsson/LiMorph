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

/*
public enum ShapeshiftForm
{
	Normal = 0,
	Cat = 1,
	TreeOfLife = 2,
	Travel = 3,
	Aqua = 4,
	Bear = 5,
	Ambient = 6,
	Ghoul = 7,
	DireBear = 8,
	CreatureBear = 14,
	CreatureCat = 15,
	GhostWolf = 16,
	BattleStance = 17,
	DefensiveStance = 18,
	BerserkerStance = 19,
	EpicFlightForm = 27,
	Shadow = 28,
	Stealth = 30,
	Moonkin = 31,
	SpiritOfRedemption = 32
}
*/

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
	FEL_ORC = 12,
	NAGA = 13,
	BROKEN = 14,
	SKELETON = 15,
	VRYKUL = 16,
	TUSKARR = 17,
	FOREST_TROLL = 18,
	TAUNKA = 19,
	NORTHREND_SKELETON = 20,
	ICE_TROLL = 21,
	WORGEN = 22,
	GILNEAN = 23,
	PANDAREN = 24,
	PANDAREN_ALLIANCE = 25,
	PANDAREN_HORDE = 26,
	// other pandarens 25-26 (but same display id so not needed)
	NIGHTBORNE = 27,
	HIGHMOUNTAIN_TAUREN = 28,
	VOID_ELF = 29,
	LIGHTFORGED_DRAENEI = 30,
	ZANDALARI_TROLL = 31,
	KUL_TIRAN = 32,
	THIN_HUMAN = 33,
	DARK_IRON_DWARF = 34,
	VULPERA = 35,
	MAGHAR_ORC = 36,
	MECHAGNOME = 37
};

constexpr uint8_t N_RACES = 23;

// only usable races have customizations
const RaceIDs RACES[N_RACES] = {
	RaceIDs::HUMAN, 
	RaceIDs::ORC, 
	RaceIDs::DWARF,
	RaceIDs::NELF, 
	RaceIDs::UNDEAD,
	RaceIDs::TAUREN, 
	RaceIDs::GNOME, 
	RaceIDs::TROLL,
	RaceIDs::GOBLIN,
	RaceIDs::BELF, 
	RaceIDs::DRAENEI,
	RaceIDs::WORGEN,
	RaceIDs::PANDAREN, 
	RaceIDs::NIGHTBORNE,
	RaceIDs::HIGHMOUNTAIN_TAUREN,
	RaceIDs::VOID_ELF,
	RaceIDs::LIGHTFORGED_DRAENEI,
	RaceIDs::ZANDALARI_TROLL,
	RaceIDs::KUL_TIRAN,
	RaceIDs::DARK_IRON_DWARF, 
	RaceIDs::VULPERA,
	RaceIDs::MAGHAR_ORC, 
	RaceIDs::MECHAGNOME
};

#define USE_NATIVE_MORPH_IDS

#ifndef USE_NATIVE_MORPH_IDS

enum class RaceMorphIDs {
	HUMAN_M = 57899,
	HUMAN_F = 56658,

	ORC_M = 51894,
	ORC_F = 53762,

	DWARF_M = 49242,
	DWARF_F = 53768,

	NELF_M = 54918,
	NELF_F = 54439,

	UNDEAD_M = 54041,
	UNDEAD_F = 56327,

	TAUREN_M = 55077,
	TAUREN_F = 56316,

	GNOME_M = 51877,
	GNOME_F = 53291,

	TROLL_M = 59071,
	TROLL_F = 59223,

	GOBLIN_M = 6894,
	GOBLIN_F = 6895,

	BELF_M = 62127,
	BELF_F = 62128,

	DRAENEI_M = 57027,
	DRAENEI_F = 58232,

	FEL_ORC_M = 16981,
	FEL_ORC_F = 16980,

	NAGA_M = 17402,
	NAGA_F = 17403,

	BROKEN_M = 17576,
	BROKEN_F = 17577,

	SKELETON_M = 17578,
	SKELETON_F = 17579,

	VRYKUL_M = 21685,
	VRYKUL_F = 21686,

	TUSKARR_M = 21780,
	TUSKARR_F = 21781,

	FOREST_TROLL_M = 21963,
	FOREST_TROLL_F = 21964,

	TAUNKA_M = 26316,
	TAUNKA_F = 26317,

	NORTHREND_SKELETON_M = 26871,
	NORTHREND_SKELETON_F = 26872,

	ICE_TROLL_M = 26873,
	ICE_TROLL_F = 26874,

	//WORGEN_M = 29422,
	//WORGEN_F = 29423,

	WORGEN_F = 37914,
	WORGEN_M = 37915,

	GILNEAN_M = 57899, //same as human
	GILNEAN_F = 56658, //same as human

	PANDAREN_M = 38551,
	PANDAREN_F = 38552,

	NIGHTBORNE_M = 75078,
	NIGHTBORNE_F = 75079,

	HIGHMOUNTAIN_TAUREN_M = 75080,
	HIGHMOUNTAIN_TAUREN_F = 75081,

	VOID_ELF_M = 75082,
	VOID_ELF_F = 75083,

	LIGHTFORGED_DRAENEI_M = 75084,
	LIGHTFORGED_DRAENEI_F = 75085,


	ZANDALARI_TROLL_M = 79100,
	ZANDALARI_TROLL_F = 79101,

	KUL_TIRAN_M = 80387,
	KUL_TIRAN_F = 80388,

	THIN_HUMAN_M = 82317,
	THIN_HUMAN_F = 82318,

	DARK_IRON_DWARF_M = 83910,
	DARK_IRON_DWARF_F = 83911,

	VULPERA_M = 83913,
	VULPERA_F = 83914,

	MAGHAR_ORC_M = 84558,
	MAGHAR_ORC_F = 84560,

	MECHAGNOME_M = 90786,
	MECHAGNOME_F = 90787

};

#else

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
	NELF_F = 54439,

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

#endif

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

		/*
		{RaceIDs::FEL_ORC, RaceMorphIDs::FEL_ORC_M},
		{RaceIDs::NAGA, RaceMorphIDs::NAGA_M},
		{RaceIDs::BROKEN, RaceMorphIDs::BROKEN_M},
		{RaceIDs::SKELETON, RaceMorphIDs::SKELETON_M},
		{RaceIDs::VRYKUL, RaceMorphIDs::VRYKUL_M},
		{RaceIDs::TUSKARR, RaceMorphIDs::TUSKARR_M},
		{RaceIDs::FOREST_TROLL, RaceMorphIDs::FOREST_TROLL_M},
		{RaceIDs::TAUNKA, RaceMorphIDs::TAUNKA_M},
		{RaceIDs::NORTHREND_SKELETON, RaceMorphIDs::NORTHREND_SKELETON_M},
		{RaceIDs::ICE_TROLL, RaceMorphIDs::ICE_TROLL_M},
		*/
		{RaceIDs::WORGEN, RaceMorphIDs::WORGEN_M},

	//	{RaceIDs::GILNEAN, RaceMorphIDs::GILNEAN_M},

		{RaceIDs::PANDAREN, RaceMorphIDs::PANDAREN_M},
		{RaceIDs::PANDAREN_ALLIANCE, RaceMorphIDs::PANDAREN_M},
		{RaceIDs::PANDAREN_HORDE, RaceMorphIDs::PANDAREN_M},

		{RaceIDs::NIGHTBORNE, RaceMorphIDs::NIGHTBORNE_M},
		{RaceIDs::HIGHMOUNTAIN_TAUREN, RaceMorphIDs::HIGHMOUNTAIN_TAUREN_M},
		{RaceIDs::VOID_ELF, RaceMorphIDs::VOID_ELF_M},
		{RaceIDs::LIGHTFORGED_DRAENEI, RaceMorphIDs::LIGHTFORGED_DRAENEI_M},
		{RaceIDs::ZANDALARI_TROLL, RaceMorphIDs::ZANDALARI_TROLL_M},
		{RaceIDs::KUL_TIRAN, RaceMorphIDs::KUL_TIRAN_M},

		//{RaceIDs::THIN_HUMAN, RaceMorphIDs::THIN_HUMAN_M},

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

		/*
		{RaceIDs::FEL_ORC, RaceMorphIDs::FEL_ORC_F},
		{RaceIDs::NAGA, RaceMorphIDs::NAGA_F},
		{RaceIDs::BROKEN, RaceMorphIDs::BROKEN_F},
		{RaceIDs::SKELETON, RaceMorphIDs::SKELETON_F},
		{RaceIDs::VRYKUL, RaceMorphIDs::VRYKUL_F},
		{RaceIDs::TUSKARR, RaceMorphIDs::TUSKARR_F},
		{RaceIDs::FOREST_TROLL, RaceMorphIDs::FOREST_TROLL_F},
		{RaceIDs::TAUNKA, RaceMorphIDs::TAUNKA_F},
		{RaceIDs::NORTHREND_SKELETON, RaceMorphIDs::NORTHREND_SKELETON_F},
		{RaceIDs::ICE_TROLL, RaceMorphIDs::ICE_TROLL_F},
		*/
		{RaceIDs::WORGEN, RaceMorphIDs::WORGEN_F},

		//{RaceIDs::GILNEAN, RaceMorphIDs::GILNEAN_F},

		{RaceIDs::PANDAREN, RaceMorphIDs::PANDAREN_F},
		{RaceIDs::PANDAREN_ALLIANCE, RaceMorphIDs::PANDAREN_F},
		{RaceIDs::PANDAREN_HORDE, RaceMorphIDs::PANDAREN_F},
		{RaceIDs::NIGHTBORNE, RaceMorphIDs::NIGHTBORNE_F},
		{RaceIDs::HIGHMOUNTAIN_TAUREN, RaceMorphIDs::HIGHMOUNTAIN_TAUREN_F},
		{RaceIDs::VOID_ELF, RaceMorphIDs::VOID_ELF_F},
		{RaceIDs::LIGHTFORGED_DRAENEI, RaceMorphIDs::LIGHTFORGED_DRAENEI_F},
		{RaceIDs::ZANDALARI_TROLL, RaceMorphIDs::ZANDALARI_TROLL_F},
		{RaceIDs::KUL_TIRAN, RaceMorphIDs::KUL_TIRAN_F},

		//{RaceIDs::THIN_HUMAN, RaceMorphIDs::THIN_HUMAN_F},

		{RaceIDs::DARK_IRON_DWARF, RaceMorphIDs::DARK_IRON_DWARF_F},
		{RaceIDs::VULPERA, RaceMorphIDs::VULPERA_F},
		{RaceIDs::MAGHAR_ORC, RaceMorphIDs::MAGHAR_ORC_F},
		{RaceIDs::MECHAGNOME, RaceMorphIDs::MECHAGNOME_F},
	};


	inline RaceMorphIDs getRaceMorphID(int race_id, int gender) {
		if (gender)
			return race_female.at(static_cast<RaceIDs>(race_id));
		else
			return race_male.at(static_cast<RaceIDs>(race_id));
	}

	inline bool isRaceMorphID(int morph_id) {
		switch (static_cast<RaceMorphIDs>(morph_id)) {
		case RaceMorphIDs::HUMAN_M:
		case RaceMorphIDs::HUMAN_F:
		case RaceMorphIDs::ORC_M:
		case RaceMorphIDs::ORC_F:
		case RaceMorphIDs::DWARF_M:
		case RaceMorphIDs::DWARF_F:
		case RaceMorphIDs::NELF_M:
		case RaceMorphIDs::NELF_F:
		case RaceMorphIDs::UNDEAD_M:
		case RaceMorphIDs::UNDEAD_F:
		case RaceMorphIDs::TAUREN_M:
		case RaceMorphIDs::TAUREN_F:
		case RaceMorphIDs::GNOME_M:
		case RaceMorphIDs::GNOME_F:
		case RaceMorphIDs::TROLL_M:
		case RaceMorphIDs::TROLL_F:
		case RaceMorphIDs::GOBLIN_M:
		case RaceMorphIDs::GOBLIN_F:
		case RaceMorphIDs::BELF_M:
		case RaceMorphIDs::BELF_F:
		case RaceMorphIDs::DRAENEI_M:
		case RaceMorphIDs::DRAENEI_F:
			/*
		case RaceMorphIDs::FEL_ORC_M:
		case RaceMorphIDs::FEL_ORC_F:
		case RaceMorphIDs::NAGA_M:
		case RaceMorphIDs::NAGA_F:
		case RaceMorphIDs::BROKEN_M:
		case RaceMorphIDs::BROKEN_F:
		case RaceMorphIDs::SKELETON_M:
		case RaceMorphIDs::SKELETON_F:
		case RaceMorphIDs::VRYKUL_M:
		case RaceMorphIDs::VRYKUL_F:
		case RaceMorphIDs::TUSKARR_M:
		case RaceMorphIDs::TUSKARR_F:
		case RaceMorphIDs::FOREST_TROLL_M:
		case RaceMorphIDs::FOREST_TROLL_F:
		case RaceMorphIDs::TAUNKA_M:
		case RaceMorphIDs::TAUNKA_F:
		case RaceMorphIDs::NORTHREND_SKELETON_M:
		case RaceMorphIDs::NORTHREND_SKELETON_F:
		case RaceMorphIDs::ICE_TROLL_M:
		case RaceMorphIDs::ICE_TROLL_F:
		*/
		case RaceMorphIDs::WORGEN_M:
		case RaceMorphIDs::WORGEN_F:
		//case RaceMorphIDs::GILNEAN_M:
	//	case RaceMorphIDs::GILNEAN_F:
		case RaceMorphIDs::PANDAREN_M:
		case RaceMorphIDs::PANDAREN_F:
		case RaceMorphIDs::NIGHTBORNE_M:
		case RaceMorphIDs::NIGHTBORNE_F:
		case RaceMorphIDs::HIGHMOUNTAIN_TAUREN_M:
		case RaceMorphIDs::HIGHMOUNTAIN_TAUREN_F:
		case RaceMorphIDs::VOID_ELF_M:
		case RaceMorphIDs::VOID_ELF_F:
		case RaceMorphIDs::LIGHTFORGED_DRAENEI_M:
		case RaceMorphIDs::LIGHTFORGED_DRAENEI_F:
		case RaceMorphIDs::ZANDALARI_TROLL_M:
		case RaceMorphIDs::ZANDALARI_TROLL_F:
		case RaceMorphIDs::KUL_TIRAN_M:
		case RaceMorphIDs::KUL_TIRAN_F:
	//	case RaceMorphIDs::THIN_HUMAN_M:
	//	case RaceMorphIDs::THIN_HUMAN_F:
		case RaceMorphIDs::DARK_IRON_DWARF_M:
		case RaceMorphIDs::DARK_IRON_DWARF_F:
		case RaceMorphIDs::VULPERA_M:
		case RaceMorphIDs::VULPERA_F:
		case RaceMorphIDs::MAGHAR_ORC_M:
		case RaceMorphIDs::MAGHAR_ORC_F:
		case RaceMorphIDs::MECHAGNOME_M:
		case RaceMorphIDs::MECHAGNOME_F:
			return true;
		default:
			return false;
		}
	}

}
};