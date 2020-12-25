#pragma once

#include <array>

#include "Memory.h"
#include "Offsets.h"
#include "WoWUtils.h"
#include <vector>
#include <unordered_map>

namespace LiMorph {

// Customization logic:
// Get ChrModelID from chrmodel.csv.
// Get all customization options (i.e. a set of ChrCustomizationOptionIDs) for this ChrModelID in chrcustomizationoption.csv
// For each of these ChrCustomizationOptionIDs retrieve all ChrCustomizationChoiceIDs from chrcustomizationchoice.csv

// All indexing will actually be done through chrcustomizationconversion.csv:
// Given race/gender: Get all unique OptionIDs from chrcustomizationconversion.csv.
// For each OptionID -> get name_lang from chrcustomizationoption 


class Player
{
public:
	Player() {}
	Player(uintptr_t player) : m_player_ptr{ player } {}
	~Player() {}
	void initializePlayer();

	// reset player to original IDs
	void resetPlayer();

	// restore player to current morph (used after zoning, i.e not realod or initial login)
	void restorePlayer();

	// getters
	int getCurrentMorphID();
	int getOriginalMorphID();
	uint8_t getGenderID();
	uint8_t getRaceID();
	int getMountID();
	bool mountMorphed();
	bool isShapeshiftTransparent(ShapeshiftForm form);
	bool isShapeshiftTransparentByDefault(ShapeshiftForm form);
	int getShapeshiftID(ShapeshiftForm form);
	int getOriginalShapeshiftID(ShapeshiftForm form);
	const std::unordered_map<std::string, int>& getCustomizations();
	int getNumberOfChoices(int option);
	int getChoiceID(int option, int choice);
	bool getDisableMeta();

	// get from wow mem
	int getMorphIDFromMemory();
	uint8_t getShapeshiftFormIDFromMemory();
	int getNativeMorphID();
	uintptr_t getCustomizationPtr();

	// setters
	void setCurrentMorphID(int id);
	void setGenderID(uint8_t gender_id);
	void setRaceID(uint8_t id);
	void setMountID(int id);
	void setShapeshiftID(ShapeshiftForm form, int form_id);
	void setShapeshiftTransparency(ShapeshiftForm form, bool transparent);
	void setCurrentOriginalShapeshiftID(ShapeshiftForm form, int morph_id=0);
	void setPlayerPtr(uintptr_t player_ptr);
	void setMetaDisabled(bool meta);


	// set values in wow mem (items/title also set instance variables)
	void setMorphIDInMemory(int morph_id);
	void setCurrentMorphIDInMemory();
	void setItemID(Items item, int id);
	void setItemVersionID(Items item, int id);
	void setItemEnchantID(Items item, int id);
	void setTitleID(int id);
	void setCustomizationChoice(int option, int choice);
	void setCustomizationChoices();

	// copy player
	void copyPlayerItems(uintptr_t unit);
	void copyPlayerItem(uintptr_t unit, int item);


private:
	uintptr_t m_player_ptr;
	uintptr_t m_customization_ptr;

	// Morphing
	int m_current_morph_id; // this is whats morphed (written to memory)
	int m_current_original_morph_id; // if reset: always morph into this! (tracks correct original id of current shapeshift)

	//shapeshifting
	std::array<int, N_SHAPESHIFTS> m_shapeshift_ids;
	std::array<int, N_SHAPESHIFTS> m_original_shapeshift_ids;
	std::array<bool, N_SHAPESHIFTS> m_is_shapeshift_transparent;


	//items (all stored in array because they exist at executive bytes in memory)
	std::array<int, N_ITEMS * 3> m_item_ids;
	std::array<int, N_ITEMS * 3> m_original_item_ids;

	// customizations
	std::map<RaceMorphIDs, std::array<uint32_t, 32>> m_customizations;
	std::array<uint32_t, 32> m_native_customizations;

	// things not related to morphing the player model
	uint8_t m_race_id;
	uint8_t m_gender_id;
	int m_mount_id;
	int m_title_id;
	uint8_t m_original_race_id;
	uint8_t m_original_gender_id;
	int m_original_mount_id;
	int m_original_title_id;

	// other stuff
	bool m_mount_morphed;
	bool m_meta_disabled;
};


} // namespace morph
