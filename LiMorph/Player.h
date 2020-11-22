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

		// restore player to current morph (used after loading screen etc)
		void restorePlayer();

		int getMorphID();
		int getGenderID();
		int getRaceID();
		int getMountID();

		// set Player instance variable
		void setGenderID(int id);
		void setRaceID(int id);
		void setMountID(int id);


		// check if mount was morphed
		bool mountMorphed();

		// set values in wow mem
		void setMorphID(int id);
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
		int m_original_morph_id;
		int m_original_race_id;
		int m_original_gender_id;
		int m_original_mount_id;
		int m_original_title_id;

		int m_morph_id;
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
