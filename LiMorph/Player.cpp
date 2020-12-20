#include "pch.h"
#include "Player.h"
#include "Customizations.h"


namespace LiMorph {


void Player::initializePlayer() {
	m_customization_ptr = m_player_ptr + Offsets::customization_ptr;

	m_original_race_id = m_race_id = Memory::readMemory<uint8_t>(m_player_ptr + Offsets::race_id);
	m_original_gender_id = m_gender_id = Memory::readMemory<uint8_t>(m_player_ptr + Offsets::gender_id);
	m_original_title_id = m_title_id = Memory::readMemory<int>(m_player_ptr + Offsets::title_id);

	// initialzie humanoid shapeshift from memory
	//int morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::original_morph_id);
	RaceIDs id = static_cast<RaceIDs>(m_original_race_id);
	int morph_id;
	if (!m_original_gender_id)
		morph_id = static_cast<int>(WoWUtils::race_male.at(id));
	else
		morph_id = static_cast<int>(WoWUtils::race_female.at(id));

	if (getMorphIDFromMemory() == getNativeMorphID()) {
		m_current_morph_id = morph_id;
	}
	else {
		m_current_morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
	}

	m_current_original_morph_id = m_current_morph_id;

	m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)] = morph_id;
	m_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)] = morph_id;

	// Initialize all other shapeshifts to 0 for now
	for (int i = 1; i < m_shapeshift_ids.size(); i++) {
		m_original_shapeshift_ids[i] = 0;
		m_shapeshift_ids[i] = 0;
		m_is_shapeshift_transparent[i] = false;
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

	for (int i = 0; i < N_RACES; i++) {
		int race = static_cast<int>(RACES[i]);
		for (int j = 0; j < 2; j++) {
			RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(race, j);
			m_customizations[race_gender] = Customizations::getNativeCustomizations(race, j);
		}
	}

	// customizations init
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	//m_customizations[race_gender] = Customizations::getNativeCustomizations(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		int32_t c = Memory::readMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8));
		m_native_customizations[i] = c;
		m_customizations[race_gender][i] = c; // item_id
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

	for (int i = 0; i < N_RACES; i++) {
		int race = static_cast<int>(RACES[i]);
		for (int j = 0; j < 2; j++) {
			RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(race, j);
			m_customizations[race_gender] = Customizations::getNativeCustomizations(race, j);
		}
	}

	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		Memory::writeMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8), m_native_customizations[i]);

		auto& options_ids = Customizations::getCustomizationOptionIDs(m_race_id, m_gender_id);
		Memory::writeMemory<int32_t>(getCustomizationPtr() + (i * 8), options_ids[i]); 

		m_customizations[race_gender][i] = m_native_customizations[i]; 
	}

	
	m_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)] =
		m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(ShapeshiftForm::HUMANOID)];

	for (int i = 1; i < m_shapeshift_ids.size(); i++) {
		m_original_shapeshift_ids[i] = 0;
		m_shapeshift_ids[i] = 0;
		m_is_shapeshift_transparent[i] = false;
	}

	if (m_current_morph_id == getOriginalShapeshiftID(ShapeshiftForm::HUMANOID))
		setMorphIDInMemory(getNativeMorphID());
	else
		setCurrentMorphIDInMemory();

	m_mount_morphed = false;
	setTitleID(m_original_title_id);
}

void Player::restorePlayer() {
	for (int i = 0, j = 0; i < m_item_ids.size(); i += 3, j++) {
		setItemID(ITEMS_LIST[j], m_item_ids[i]);
		setItemVersionID(ITEMS_LIST[j], m_item_ids[i + 1]);
		setItemEnchantID(ITEMS_LIST[j], m_item_ids[i + 2]);
	}

	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		Memory::writeMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8), m_customizations[race_gender][i]); // item_id
		auto& options_ids = Customizations::getCustomizationOptionIDs(m_race_id, m_gender_id);
		Memory::writeMemory<int32_t>(getCustomizationPtr() + (i * 8), options_ids[i]);
	}
	setTitleID(m_title_id);
}

int Player::getCurrentMorphID() {
	return m_current_morph_id;
}

int Player::getOriginalMorphID() {
	return m_current_original_morph_id;
}

uint8_t Player::getRaceID() {
	return m_race_id;
}

uint8_t Player::getGenderID() {
	return m_gender_id;
}

int Player::getMountID() {
	return m_mount_id;
}

bool Player::mountMorphed() {
	return m_mount_morphed;
}

bool Player::isShapeshiftTransparent(ShapeshiftForm form_id) {
	return m_is_shapeshift_transparent[WoWUtils::shapeshiftToIndex(form_id)];
}

bool Player::isShapeshiftTransparentByDefault(ShapeshiftForm form) {
	//return getMorphIDFromMemory() == getNativeMorphID() && form != ShapeshiftForm::HUMANOID;
	return form == ShapeshiftForm::HUMANOID || form == ShapeshiftForm::SHADOW || form == ShapeshiftForm::STEALTH;

}


int Player::getShapeshiftID(ShapeshiftForm form) {
	return m_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)];
}

int Player::getOriginalShapeshiftID(ShapeshiftForm form) {
	return m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)];
}

std::map<std::string, int>& Player::getCustomizations() {
	return Customizations::getCustomizations(m_race_id, m_gender_id);
}

int Player::getNumberOfChoices(int option) {
	return Customizations::getNumberOfChoices(m_race_id, m_gender_id, option);
}

int Player::getChoiceID(int option, int choice) {
	return Customizations::getChoiceID(m_race_id, m_gender_id, option, choice);
}

int Player::getMorphIDFromMemory() {
	return Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
}

uint8_t Player::getShapeshiftFormIDFromMemory() {
	return Memory::readMemory<uint8_t>(m_player_ptr + Offsets::shapeshift_id);
}

int Player::getNativeMorphID() {
	return Memory::readMemory<int>(m_player_ptr + Offsets::original_morph_id);
}

uintptr_t Player::getCustomizationPtr() {
	return Memory::readMemory<uintptr_t>(m_player_ptr + Offsets::customization_ptr);
}



void Player::setCurrentMorphID(int morph_id) {
	m_current_morph_id = morph_id;
}

void Player::setGenderID(uint8_t gender_id) {
	m_gender_id = gender_id;
	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::gender_id, gender_id);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		Memory::writeMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8), m_customizations[race_gender][i]); // item_id
		auto& options_ids = Customizations::getCustomizationOptionIDs(m_race_id, m_gender_id);
		Memory::writeMemory<int32_t>(getCustomizationPtr() + (i * 8), options_ids[i]);
	}
}

void Player::setRaceID(uint8_t race_id) {
	m_race_id = race_id;
	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::race_id, race_id);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		Memory::writeMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8), m_customizations[race_gender][i]); // item_id
		auto& options_ids = Customizations::getCustomizationOptionIDs(m_race_id, m_gender_id);
		Memory::writeMemory<int32_t>(getCustomizationPtr() + (i * 8), options_ids[i]);
	}
}

void Player::setMountID(int mount_id) {
	m_mount_id = mount_id;
	m_mount_morphed = true;
}

void Player::setShapeshiftID(ShapeshiftForm form, int form_id) {
	m_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)] = form_id;
}

void Player::setShapeshiftTransparency(ShapeshiftForm form, bool transparent) {
	m_is_shapeshift_transparent[WoWUtils::shapeshiftToIndex(form)] = transparent;
}


void Player::setCurrentOriginalShapeshiftID(ShapeshiftForm form, int morph_id) {
	m_original_shapeshift_ids[WoWUtils::shapeshiftToIndex(form)] = morph_id; //todo: this is not needed if HUMANOID since the original is set from initialization
	m_current_original_morph_id = morph_id;
}

void Player::setPlayerPtr(uintptr_t player_ptr) {
	m_player_ptr = player_ptr;
}

// unused
void Player::setMorphIDInMemory(int morph_id) {
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, morph_id);
}

void Player::setCurrentMorphIDInMemory() {
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, m_current_morph_id);
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

void Player::setCustomizationChoice(int option, int choice) {
	uintptr_t customization_ptr = Memory::readMemory<uintptr_t>(m_player_ptr + Offsets::customization_ptr);
	Memory::writeMemory<uint32_t>(customization_ptr + 4 + (option * 8), choice);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	m_customizations[race_gender][option] = choice; // item_id
}

void Player::setCustomizationChoices() {

}

void Player::copyPlayerItem(uintptr_t unit, int item) {
	Items item_ = static_cast<Items>(item);
	int32_t unit_item = Memory::readMemory<int32_t>(unit + Offsets::transmog_display_ids + (static_cast<int>(item) * 12));
	int8_t unit_version = Memory::readMemory<int8_t>(unit + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 8);
	int16_t unit_enchant = Memory::readMemory<int16_t>(unit + Offsets::transmog_display_ids + (static_cast<int>(item) * 12) + 12);
	setItemID(item_, unit_item);
	setItemVersionID(item_, unit_version);
	setItemEnchantID(item_, unit_enchant);
	
}

void Player::copyPlayerItems(uintptr_t unit) {
	for (int i = 0, j = 0; i < m_item_ids.size(); i += 3, j++) {
		copyPlayerItem(unit, static_cast<int>(ITEMS_LIST[j]));
	}
	
}

} // namespace morph