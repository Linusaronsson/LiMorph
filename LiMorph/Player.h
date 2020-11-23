#pragma once

#include <array>

#include "Memory.h"
#include "Offsets.h"
#include "WoWUtils.h"

namespace morph {

class Player
{
public:
	Player() {}
	Player(uintptr_t player) : m_player_ptr{ player } {}
	~Player() {}
	void initializePlayer();

	// reset player to original IDs
	void resetPlayer();

	// restore player to current morph (used after loading screen 
	void restorePlayer();

	int getMorphID();
	int getMorphIDInMemory();
	int getOriginalMorphID();

	int getGenderID();
	int getRaceID();
	int getMountID();

	int getShapeshiftID(ShapeshiftForm form);
	int getOriginalShapeshiftID(ShapeshiftForm form);


	// set Player instance variable
	void setGenderID(int id);
	void setRaceID(int id);
	void setMountID(int id);
	void setShapeshiftID(ShapeshiftForm form, int form_id);

	void setCurrentOriginalShapeshiftID(ShapeshiftForm form);

	// check if mount was morphed
	bool mountMorphed();

	// set values in wow mem
	void setCurrentMorphID(int id);
	void setMorphIDInMemory(int morph_id);
	void setCurrentMorphIDInMemory();


	void setItemID(Items item, int id);
	void setItemVersionID(Items item, int id);
	void setItemEnchantID(Items item, int id);
	void setTitleID(int id);

	void setPlayerPtr(uintptr_t player_ptr) {
		m_player_ptr = player_ptr;
	}


private:
	uintptr_t m_player_ptr;
	uintptr_t m_customization_ptr;




	// morphing

	// this is whats morphed (written memory)
	int m_current_morph_id;

	// if reset: always morph in to this!
	int m_current_original_morph_id;

	//shapeshifting
	int m_humanoid_form_id;
	int m_bear_form_id;
	int m_cat_form_id;
	int m_flight_form_id;
	int m_travel_form_id;

	int m_original_humanoid_form_id;
	int m_original_bear_form_id;
	int m_original_cat_form_id;
	int m_original_flight_form_id;
	int m_original_travel_form_id;


	// things not related to morphing the player model
	int m_original_race_id;
	int m_original_gender_id;
	int m_original_mount_id;
	int m_original_title_id;

	int m_race_id;
	int m_gender_id;
	int m_mount_id;
	int m_title_id;

	//items (all stored in array because they exist at executive bytes in memory)
	std::array<int, N_ITEMS * 3> m_original_item_ids;
	std::array<int, N_ITEMS * 3> m_item_ids;

	// other stuff
	bool m_mount_morphed;
};


} // namespace morph
