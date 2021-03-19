#pragma once

#include "../pch.h"
#include "WoWUtils.h"

namespace LiMorph {

	namespace Offsets {

		// Function offsets
		constexpr uintptr_t update_display_info = 0x1599930;
		constexpr uintptr_t update_mount_display_info = 0x1585620;
		constexpr uintptr_t frame_script_execute = 0x8036D0;
		constexpr uintptr_t frame_script_execute_wrapper = 0x8A3E10;

		constexpr uintptr_t frame_script_register_function = 0x8050F0;
		constexpr uintptr_t frame_script_register_wrapper_function = 0x88ED80;

		constexpr uintptr_t frame_script_invalid_ptr_check = 0x14B1980;
		constexpr uintptr_t lua_gettop = 0x42E9C0;
		constexpr uintptr_t lua_pushnumber = 0x42F3B0;
		constexpr uintptr_t lua_tolstring = 0x4300E0;
		constexpr uintptr_t lua_tonumber = 0x430180;
		constexpr uintptr_t lua_settop = 0x42FE03;
		constexpr uintptr_t lua_pushlstring = 0x42F310;
		constexpr uintptr_t get_unit_from_name = 0x2104450;

		constexpr uintptr_t wow_lua_state = 0x21C0E40;


		// POINTER TO END OF .TEXT (change value at this address to make frame_script_register_function work)
		constexpr uintptr_t invalid_ptr_limit = 0x3569AE8;

		// Generally useful offsets
		constexpr uintptr_t game_version = 0x2D0241C;
		constexpr uintptr_t in_game_flag = 0x369274C;

		// Object manager
		constexpr uintptr_t object_manager = 0x35E0210;


		// Object manager offsets
		constexpr uintptr_t standard_offset = -0x28;
		constexpr uintptr_t active_player_GUID = 0x140;
		constexpr uintptr_t first_object = 0x120; // NOTE: actual object is at firstObject + standardOffset!! (todo: update offsets based on this)

		// Player object offsets
		constexpr uintptr_t next_object = 0x0;
		constexpr uintptr_t morph_id = 0x1BA0;
		constexpr uintptr_t morph_id2 = 0x8C0;

		constexpr uintptr_t shapeshift_id = 0x1D63;

		constexpr uintptr_t original_morph_id = 0x1D00;

		//constexpr uintptr_t bear_form_id = 0x19F8;

		//constexpr uintptr_t bear_form_id = 0x19F8;

		//constexpr uintptr_t race_id = 0x1CBC;

		constexpr uintptr_t gender_id = 0x1C93;
		constexpr uintptr_t race_id = 0x1C90;
		constexpr uintptr_t class_id = 0x1C91;
		constexpr uintptr_t scale = 0x908;

		constexpr uintptr_t title_id = 0x2258;

		//constexpr uintptr_t mount_id1 = 0x93C;
		constexpr uintptr_t mount_id1 = 0x938;

		constexpr uintptr_t guid = standard_offset + 0x18;
		constexpr uintptr_t object_type = 0x10;

		// Item related offsets
		//constexpr uintptr_t transmog_display_ids = 0x43EC - 12;

		constexpr uintptr_t transmog_display_ids = 0x4404 - 12;

		/*

		constexpr uintptr_t transmog_head = transmog_display_ids + (static_cast<int>(Items::HEAD)*12);
		constexpr uintptr_t head_version = transmog_head + 8;

		constexpr uintptr_t transmog_shoulders = transmog_display_ids + (static_cast<int>(Items::SHOULDERS) * 12);
		constexpr uintptr_t shoulders_version = transmog_shoulders + 8;

		constexpr uintptr_t transmog_chest = transmog_display_ids + (static_cast<int>(Items::CHEST) * 12);
		constexpr uintptr_t chest_version = transmog_chest + 8;

		constexpr uintptr_t transmog_legs = transmog_display_ids + (static_cast<int>(Items::LEGS) * 12);
		constexpr uintptr_t legs_version = transmog_legs + 8;

		constexpr uintptr_t transmog_wrists = transmog_display_ids + (static_cast<int>(Items::WRISTS) * 12);
		constexpr uintptr_t wrists_version = transmog_wrists + 8;

		constexpr uintptr_t transmog_back = transmog_display_ids + (static_cast<int>(Items::BACK) * 12);
		constexpr uintptr_t back_version = transmog_back + 8;

		constexpr uintptr_t transmog_shirt = transmog_display_ids + (static_cast<int>(Items::SHIRT) * 12);
		constexpr uintptr_t shirt_version = transmog_shirt + 8;

		constexpr uintptr_t transmog_tabard = transmog_display_ids + (static_cast<int>(Items::TABARD) * 12);
		constexpr uintptr_t tabard_version = transmog_tabard + 8;

		constexpr uintptr_t transmog_gloves = transmog_display_ids + (static_cast<int>(Items::GLOVES) * 12);
		constexpr uintptr_t gloves_version = transmog_gloves + 8;

		constexpr uintptr_t transmog_belt = transmog_display_ids + (static_cast<int>(Items::BELT) * 12);
		constexpr uintptr_t belt_version = transmog_belt + 8;

		constexpr uintptr_t transmog_feet = transmog_display_ids + (static_cast<int>(Items::FEET) * 12);
		constexpr uintptr_t feet_version = transmog_feet + 8;

		constexpr uintptr_t transmog_main_hand = transmog_display_ids + (static_cast<int>(Items::MAIN_HAND) * 12);
		constexpr uintptr_t main_hand_version = transmog_main_hand + 8;
		constexpr uintptr_t main_hand_enchant = transmog_main_hand + 10;

		constexpr uintptr_t transmog_off_hand = transmog_display_ids + (static_cast<int>(Items::OFF_HAND) * 12);
		constexpr uintptr_t off_hand_version = transmog_main_hand + 8;
		constexpr uintptr_t off_hand_enchant = transmog_main_hand + 10;
		*/


		/*
		constexpr uintptr_t transmog_head = 0x43EC;
		constexpr uintptr_t transmog_shoulders = 0x4404;
		constexpr uintptr_t transmog_chest = 0x441C;
		constexpr uintptr_t transmog_legs = 0x4434;
		constexpr uintptr_t transmog_wrists = 0x444C;
		constexpr uintptr_t transmog_back = 0x4494;
		constexpr uintptr_t transmog_shirt = 0x4410;
		constexpr uintptr_t transmog_tabard = 0x44C4;
		constexpr uintptr_t transmog_gloves = 0x4458;
		constexpr uintptr_t transmog_belt = 0x4428;
		constexpr uintptr_t transmog_feet = 0x4440;
		constexpr uintptr_t transmog_main_hand = 0x44A0;
		constexpr uintptr_t transmog_off_hand = 0x44AC;
		*/

		// skin 19
		//tattoo 827

		//constexpr uintptr_t customization_ptr = 0x21C0;
		constexpr uintptr_t customization_ptr = 0x21D8;

		// Custimization offsets

		constexpr uintptr_t player_buffs_ptr = 0x1808;



		// CGUnit vtable offsets
		constexpr uintptr_t update_display_info_vtable_offset = 55;
		constexpr uintptr_t update_scale_vtable_offset = 11;

		//constexpr uintptr_t update_mount_display_info_vtable_offset = 54;


	} // namespace offsets
} // namespace morph