#include "pch.h"
#include "Player.h"

#include "WoWUtils.h"

namespace LiMorph {

void Player::initializePlayer() {
	m_customization_ptr = m_player_ptr + Offsets::customization_ptr;

	m_current_morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
	m_original_race_id = m_race_id = Memory::readMemory<int>(m_player_ptr + Offsets::race_id);
	m_original_gender_id = m_gender_id = Memory::readMemory<int>(m_player_ptr + Offsets::gender_id);
	m_original_title_id = m_title_id = Memory::readMemory<int>(m_player_ptr + Offsets::title_id);

	// initialzie humanoid shapeshift from memory
	int morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::original_morph_id);
	m_current_original_morph_id = morph_id;
	m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)] = morph_id;
	m_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)] = morph_id;

	// Initialize all other shapeshifts to 0 for now
	for (int i = 1; i < m_shapeshift_ids.size(); i++) {
		m_original_shapeshift_ids[i] = 0;
		m_shapeshift_ids[i] = 0;
	}

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

	m_mount_id = 0;
	m_mount_morphed = false;
}

void Player::resetPlayer() {
	// write original item related things
	for (int i = 0, j = 0; i < m_original_item_ids.size(); i += 3, j++) {
		setItemID(ITEMS_LIST[j], m_original_item_ids[i]);
		setItemVersionID(ITEMS_LIST[j], m_original_item_ids[i + 1]);
		setItemEnchantID(ITEMS_LIST[j], m_original_item_ids[i + 2]);
	}

	m_current_morph_id = m_current_original_morph_id; // or use setMorphID()
	m_gender_id = m_original_gender_id;
	m_race_id = m_original_race_id;
	m_title_id = m_original_title_id;

	for (int i = 0; i < m_shapeshift_ids.size(); i++) {
		m_shapeshift_ids[i] = m_original_shapeshift_ids[i];
	}

	m_mount_morphed = false;
	 
	setCurrentMorphIDInMemory();
	setTitleID(m_original_title_id);
}

void Player::restorePlayer() {
	for (int i = 0, j = 0; i < m_item_ids.size(); i += 3, j++) {
		setItemID(ITEMS_LIST[j], m_item_ids[i]);
		setItemVersionID(ITEMS_LIST[j], m_item_ids[i + 1]);
		setItemEnchantID(ITEMS_LIST[j], m_item_ids[i + 2]);
	}
	setCurrentMorphIDInMemory();
	setTitleID(m_title_id);
}

int Player::getMorphID() {
	return m_current_morph_id;
}

int Player::getMorphIDInMemory() {
	return Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
}

int Player::getMountIDInMemory() {
	return Memory::readMemory<int>(m_player_ptr + Offsets::mount_id1);
}

int Player::getOriginalMorphID() {
	return m_current_original_morph_id;
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


int Player::getShapeshiftID(ShapeshiftForm form) {
	return m_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)];
}

int Player::getOriginalShapeshiftID(ShapeshiftForm form) {
	return m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)];
}

void Player::setCurrentOriginalShapeshiftID(ShapeshiftForm form) {
	if (form == ShapeshiftForm::HUMANOID) {
		m_current_original_morph_id = m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)];
	}
	else {
		int morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
		m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)] = morph_id;
		m_current_original_morph_id = morph_id;
	}
}

void Player::setShapeshiftID(ShapeshiftForm form, int form_id) {
	m_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)] = form_id;
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

void Player::setCurrentMorphID(int morph_id) {
	m_current_morph_id = morph_id;
}

// unused
void Player::setMorphIDInMemory(int morph_id) {
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, morph_id);
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id2, morph_id);
}

void Player::setCurrentMorphIDInMemory() {
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, m_current_morph_id);
	//Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id2, m_current_morph_id);
}


void Player::setItemID(Items item, int item_id) {
	m_item_ids[WoWUtils::itemToIndex(item) * 3] = item_id;
	Memory::writeMemory<int32_t>(
		m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12), item_id);
}

void Player::setItemVersionID(Items item, int item_version) {
	m_item_ids[WoWUtils::itemToIndex(item) * 3 + 1] = item_version;
	Memory::writeMemory<int8_t>(
		m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 8, item_version);
}

void Player::setItemEnchantID(Items item, int enchant_id) {
	m_item_ids[WoWUtils::itemToIndex(item) * 3 + 2] = enchant_id;
	Memory::writeMemory<int16_t>(
		m_player_ptr + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 10, enchant_id);
}

void Player::setTitleID(int title_id) {
	m_title_id = title_id;
	Memory::writeMemory<int>(m_player_ptr + Offsets::title_id, title_id);
}


} // namespace morph