#include "pch.h"
#include "Player.h"

namespace morph {
	void Player::initializePlayer() {
		m_customization_ptr = m_player_ptr + Offsets::customization_ptr;
		m_original_morph_id = m_morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
		m_original_race_id = m_race_id = Memory::readMemory<int>(m_player_ptr + Offsets::race_id);
		m_original_gender_id = m_gender_id = Memory::readMemory<int>(m_player_ptr + Offsets::gender_id);
		m_original_title_id = m_title_id = Memory::readMemory<int>(m_player_ptr + Offsets::title_id);

		// read item related things
		for (int i = 0, j = 0; i < m_original_item_ids.size(); i += 3, j++) {
			uintptr_t item_offset_j = Offsets::transmog_display_ids + (static_cast<int>(ITEMS_LIST[j]) * 12);
			m_item_ids[i] = m_original_item_ids[i] =
				Memory::readMemory<int32_t>(m_player_ptr + item_offset_j); // item_id
			m_item_ids[i + 1] = m_original_item_ids[i + 1] =
				Memory::readMemory<int8_t>(m_player_ptr + item_offset_j + 8); // item_version
			m_item_ids[i + 2] = m_original_item_ids[i + 2] =
				Memory::readMemory<int16_t>(m_player_ptr + item_offset_j + 10); //item_enchant
		}

		m_mount_morphed = false;
	}

	void Player::resetPlayer() {
		setMorphID(m_original_morph_id);
		setTitleID(m_original_title_id);

		// write original item related things
		for (int i = 0, j = 0; i < m_original_item_ids.size(); i += 3, j++) {
			setItemID(ITEMS_LIST[j], m_original_item_ids[i]);
			setItemVersionID(ITEMS_LIST[j], m_original_item_ids[i + 1]);
			setItemEnchantID(ITEMS_LIST[j], m_original_item_ids[i + 2]);
		}

		m_morph_id = m_original_morph_id;
		m_gender_id = m_original_gender_id;
		m_race_id = m_original_race_id;
		m_title_id = m_original_title_id;
		m_mount_morphed = false;
	}

	void Player::restorePlayer() {
		for (int i = 0, j = 0; i < m_item_ids.size(); i += 3, j++) {
			setItemID(ITEMS_LIST[j], m_item_ids[i]);
			setItemVersionID(ITEMS_LIST[j], m_item_ids[i + 1]);
			setItemEnchantID(ITEMS_LIST[j], m_item_ids[i + 2]);
		}
		setMorphID(m_morph_id);
		setTitleID(m_title_id);


	}

	int Player::getMorphID() {
		return m_morph_id;
	}

	int Player::getRaceID() {
		return m_race_id;
	}

	int Player::getGenderID() {
		return m_gender_id;
	}

	int Player::getMountID() {
		return m_mount_id;
	}

	void Player::setGenderID(int gender_id) {
		m_gender_id = gender_id;
	}


	void Player::setRaceID(int race_id) {
		m_race_id = race_id;
	}



	void Player::setMountID(int mount_id) {
		m_mount_id = mount_id;
		m_mount_morphed = true;
	}

	bool Player::mountMorphed() {
		return m_mount_morphed;
	}

	void Player::setMorphID(int morph_id) {
		m_morph_id = morph_id;
		Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, morph_id);
	}

	int itemToIndex(Items item) {
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

	void Player::setItemID(Items item, int item_id) {
		m_item_ids[itemToIndex(item) * 3] = item_id;
		Memory::writeMemory<int32_t>(
			m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12), item_id);
	}

	void Player::setItemVersionID(Items item, int item_version) {
		m_item_ids[itemToIndex(item) * 3 + 1] = item_version;
		Memory::writeMemory<int8_t>(
			m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 8, item_version);
	}

	void Player::setItemEnchantID(Items item, int enchant_id) {
		m_item_ids[itemToIndex(item) * 3 + 2] = enchant_id;
		Memory::writeMemory<int16_t>(
			m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 10, enchant_id);
	}

	void Player::setTitleID(int title_id) {
		m_title_id = title_id;
		Memory::writeMemory<int>(m_player_ptr + Offsets::title_id, title_id);
	}


} // namespace morph