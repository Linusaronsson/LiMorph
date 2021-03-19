#include "../pch.h"
#include "Player.h"

#include "../Customizations/Customizations.h"


namespace LiMorph {


void Player::initializePlayer() {
	m_customization_ptr = m_player_ptr + Offsets::customization_ptr;

	m_original_race_id = m_race_id = Memory::readMemory<uint8_t>(m_player_ptr + Offsets::race_id);
	m_original_gender_id = m_gender_id = Memory::readMemory<uint8_t>(m_player_ptr + Offsets::gender_id);
	m_original_title_id = m_title_id = Memory::readMemory<int>(m_player_ptr + Offsets::title_id);
	m_original_scale = m_scale = Memory::readMemory<float>(m_player_ptr + Offsets::scale);

	// initialzie humanoid shapeshift from memory
	m_original_morph_id = Memory::readMemory<int>(m_player_ptr + Offsets::original_morph_id);
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
		int32_t c1 = Memory::readMemory<int32_t>(getCustomizationPtr() + (i * 8));
		m_native_customizations[i*2] = c1;
		m_customizations[race_gender][i*2] = c1; 
	}
	// customizations init
	//m_customizations[race_gender] = Customizations::getNativeCustomizations(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		int32_t c2 = Memory::readMemory<int32_t>(getCustomizationPtr() + 4 + (i * 8));
		m_native_customizations[(i*2)+1] = c2;
		m_customizations[race_gender][(i*2)+1] = c2;
	}

	setCustomizationPtr((uintptr_t)m_customizations[race_gender].data());

	m_mount_id = 0;
	m_mount_morphed = false;
	m_meta_disabled = false;
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
	m_scale = m_original_scale;

	setScale(m_original_scale);

	for (int i = 0; i < N_RACES; i++) {
		int race = static_cast<int>(RACES[i]);
		for (int j = 0; j < 2; j++) {
			RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(race, j);
			m_customizations[race_gender] = Customizations::getNativeCustomizations(race, j);
		}
	}

	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	m_customizations[race_gender] = m_native_customizations;
	setCustomizationPtr((uintptr_t)m_customizations[race_gender].data());

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
	m_meta_disabled = false;
	setTitleID(m_original_title_id);
#ifdef USE_NATIVE_MORPH_IDS
	resetMorphIDInMemory();
#endif
}

void Player::restorePlayer() {
	for (int i = 0, j = 0; i < m_item_ids.size(); i += 3, j++) {
		setItemID(ITEMS_LIST[j], m_item_ids[i]);
		setItemVersionID(ITEMS_LIST[j], m_item_ids[i + 1]);
		setItemEnchantID(ITEMS_LIST[j], m_item_ids[i + 2]);
	}
	setScale(m_scale);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	setCustomizationPtr((uintptr_t)m_customizations[race_gender].data());
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

const std::unordered_map<std::string, int>& Player::getCustomizations() {
	return Customizations::getCustomizations(m_race_id, m_gender_id);
}

int Player::getNumberOfChoices(int option) {
	return Customizations::getNumberOfChoices(m_race_id, m_gender_id, option);
}

int Player::getChoiceID(int option, int choice) {
	return Customizations::getChoiceID(m_race_id, m_gender_id, option, choice);
}

bool Player::getDisableMeta() {
	return m_meta_disabled;
}

float Player::getScale() {
	return m_scale;
}


int Player::getMorphIDFromMemory() {
	return Memory::readMemory<int>(m_player_ptr + Offsets::morph_id);
}

uint8_t Player::getShapeshiftFormIDFromMemory() {
	return Memory::readMemory<uint8_t>(m_player_ptr + Offsets::shapeshift_id);
}

int Player::getNativeMorphID() {
	//return Memory::readMemory<int>(m_player_ptr + Offsets::original_morph_id);
	return m_original_morph_id;
}

uintptr_t Player::getCustomizationPtr() {
	return Memory::readMemory<uintptr_t>(m_player_ptr + Offsets::customization_ptr);
}

float Player::getScaleFromMemory() {
	return Memory::readMemory<float>(m_player_ptr + Offsets::scale);
}

void Player::setCurrentMorphID(int morph_id) {
	m_current_morph_id = morph_id;
}

std::vector<uintptr_t> dat = {
0xde3,
0xe04,
0x13b2,
0x195e,
0x198a,
0x1c7b,
0x1d4d,
0x21ac,
0x21f9,
0x2466,
0x2468,
0x2470,
0x4597,
0x4599,
0x479c,
0x47d8,
0x47f4,
0x47fc,
0x47fe,
0x4814,
0x4816,
0x4818,
0x481a,
0x481c,
0x481e,
0x4864,
0x486e,
0x48ae,
0x4969,
0x496b,
0x4985,
0x498b,
0x4993,
0x499c,
0x49a0,
0x49a2,
0x49af,
0x49b4,
0x49b6,
0x49bf,
0x49c6,
0x49d8,
0x49f4,
0x49ff,
0x4a1f,
0x4a64,
0x4a6e,
0x4aae,
0x4ab2,
0x4b69,
0x4b71,
0x4b75,
0x4be3,
0x4d69,
0x4d6d,
0x4d71,
0x4d75,
0x4d78,
0x4d7d,
0x4d7f,
0x4d87,
0x4d8b,
0x4d95,
0x4da0,
0x4da2,
0x4daf,
0x4db1,
0x4db6,
0x4dbf,
0x4dc1,
0x4dc7,
0x4ddd,
0x4de3,
0x4dff,
0x4e1f,
0x4e6f,
0x4eaf,
0x4eb3,
0x4eb5,
0x5452,
0x668f,
0x66a5,
0x671b,
0x6724,
0x6748,
0x6824,
0x684f,
0x6850,
0x6866,
0x68ac,
0x6ead,
0x6ec5,
0x6edd,
0x6f14,
0x6fb3,
0x6fbe,
0x6fe2,
0x7035,
0x7080,
0x7083,
0x70fb,
0x712a,
0x7146,
0x717f,
0x7186,
0x71ad,
0x71b4,
0x71be,
0x71c6,
0x7276,
0x728b,
0x72c4,
0x72ca,
0x72ea,
0x72ee,
0x72f1,
0x7346,
0x735a,
0x7375,
0x738f,
0x73ad,
0x73cf,
0x73d3,
0x73e0,
0x7417,
0x741b,
0x7494,
0x74a7,
0x75bf,
0x7609,
0x7630,
0x7632,
0x7633,
0x7637,
0x7681,
0x769c,
0x76b2,
0x76c8,
0x76e0,
0x76ec,
0x76f9,
0x76fc,
0x76fe,
0x774c,
0x77ab,
0xc332,
0xc932,
0xc9f2,
0xcaf2,
0xf7f2,
0xf9b2,
0xfb32,
};


void Player::setGenderID(uint8_t gender_id) {
	m_gender_id = gender_id;
	//Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::gender_id, m_gender_id);
	//Memory::writeMemory<uint8_t>(m_player_ptr + dat[8], m_gender_id);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	setCustomizationPtr((uintptr_t)m_customizations[race_gender].data());
}

void Player::setRaceID(uint8_t race_id) {
	m_race_id = race_id;
	//Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::race_id, m_race_id);
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	setCustomizationPtr((uintptr_t)m_customizations[race_gender].data());
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

void Player::setMetaDisabled(bool meta) {
	m_meta_disabled = meta;
}

// unused
void Player::setMorphIDInMemory(int morph_id) {
	Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, morph_id);

#ifdef USE_NATIVE_MORPH_IDS
	if (WoWUtils::isRaceMorphID(morph_id)) {
		Memory::writeMemory<int>(m_player_ptr + Offsets::original_morph_id, morph_id);
		//	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::gender_id, m_gender_id);
			//for(auto t : dat) {
		Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::race_id, m_race_id);
		//Memory::writeMemory<int>(m_player_ptr + Offsets::original_morph_id, 1);
		//Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, 0);

		Memory::writeMemory<uint8_t>(m_player_ptr + dat[8]+0x18, m_gender_id);
		//	}
	//	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::race_id, m_race_id);
	}
#endif
}

void Player::setCurrentMorphIDInMemory() {
	setMorphIDInMemory(m_current_morph_id);
}

void Player::resetMorphIDInMemory() {
	//Memory::writeMemory<int>(m_player_ptr + Offsets::morph_id, m_original_morph_id);
	//Memory::writeMemory<int>(m_player_ptr + Offsets::original_morph_id, m_original_morph_id);
	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::gender_id, m_original_gender_id);
	Memory::writeMemory<uint8_t>(m_player_ptr + dat[8] + 0x18, m_original_gender_id);
	Memory::writeMemory<uint8_t>(m_player_ptr + Offsets::race_id, m_original_race_id);
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
	setCurrentMorphIDInMemory();
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	m_customizations[race_gender][(option*2)+1] = choice; // item_id
}

void Player::setCustomizationChoices() {

}

void Player::setCustomizationPtr(uintptr_t ptr) {
	
	/*
	// customizations init
	RaceMorphIDs race_gender = WoWUtils::getRaceMorphID(m_race_id, m_gender_id);
	//m_customizations[race_gender] = Customizations::getNativeCustomizations(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		int32_t c1 = Memory::readMemory<int32_t>(ptr + (i * 8));
		m_customizations[race_gender][i * 2] = c1;
	}
	// customizations init
	//m_customizations[race_gender] = Customizations::getNativeCustomizations(m_race_id, m_gender_id);
	for (int i = 0; i < Customizations::getNumberOfCustomizations(m_race_id, m_gender_id); i++) {
		int32_t c2 = Memory::readMemory<int32_t>(ptr + 4 + (i * 8));
		m_customizations[race_gender][(i * 2) + 1] = c2;
	}
	*/
	Memory::writeMemory<uintptr_t>(m_player_ptr + Offsets::customization_ptr, ptr);
}

void Player::setScale(float scale) {
	m_scale = scale;
	Memory::writeMemory<float>(m_player_ptr + Offsets::scale, scale);
}

void Player::setNativeMorphID(int id) {
	Memory::writeMemory<float>(m_player_ptr + Offsets::original_morph_id, m_original_morph_id);

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