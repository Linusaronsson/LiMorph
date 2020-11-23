#pragma once

#include "pch.h"
#include "Offsets.h"
#include "morpher.h"

#include <functional>

extern morph::Morpher* morpher_ptr;

namespace morph {
namespace WoWFunctions {
    typedef uintptr_t(__cdecl* GetUnitFromName)(const char*);
    typedef void(__fastcall* UpdateDisplayInfo)(uintptr_t);
    typedef void(__fastcall* UpdateMountDisplayInfo)(uintptr_t, int);
    typedef uintptr_t(__fastcall* FrameScriptExecute)(const char* script, const char* script_name, int64_t val);
    typedef bool(__fastcall* FrameScriptRegisterFunction)(const char*, uintptr_t);
    typedef uintptr_t(__cdecl* FrameScriptInvalidPtrCheck)(uintptr_t);
    typedef int(__fastcall* LuaGetTop)(uintptr_t);
    //typedef int(__fastcall* LuaPushNumber)(uintptr_t, 0); //not sure about this
    typedef const char* (__fastcall* LuaToString)(uintptr_t, int, int);
    typedef double(__fastcall* LuaToNumber)(uintptr_t, int);

    /*
    inline uintptr_t getUnitFromName(const char* unit);
    inline void updateModel(uintptr_t player);
    inline void executeLUA(const std::string& script);
    inline bool registerFunction(const char* name, uintptr_t callback);
    inline uintptr_t invalidPtrCheck(uintptr_t fn);
    inline int luaGetTop(uintptr_t lua_state);
    //void LuaPushNumber(uintptr_t lua_state);
    inline const char* luaToString(uintptr_t lua_state, int index, int length);
    inline double luaToNumber(uintptr_t lua_state, int index);
    */

    inline uintptr_t getUnitFromName(const char* unit) {
        GetUnitFromName _getUnitFromName =
            reinterpret_cast<GetUnitFromName>(morpher_ptr->getBaseAddress() + Offsets::get_unit_from_name);
        return _getUnitFromName(unit);
    }

    inline void updateModel(uintptr_t player) {
        UpdateDisplayInfo _updateModel =
            reinterpret_cast<UpdateDisplayInfo>(morpher_ptr->getBaseAddress() + Offsets::update_display_info);
        _updateModel(player);
    }

    inline void updateMountModel(uintptr_t player, int mountID) {
        UpdateMountDisplayInfo _updateMountModel =
            reinterpret_cast<UpdateMountDisplayInfo>(morpher_ptr->getBaseAddress() + Offsets::update_mount_display_info);
        _updateMountModel(player, mountID);
    }

    inline void executeLUA(const std::string& script) {
        FrameScriptExecute _executeLUA =
            reinterpret_cast<FrameScriptExecute>(morpher_ptr->getBaseAddress() + Offsets::frame_script_execute);
        _executeLUA(script.c_str(), "testxD", 0);
        //executeLUA(script, script, 1);
    }


    inline bool registerFunction(const char* name, uintptr_t callback) {
        const auto textSectionEnd = Memory::readMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit);
        if (callback >= textSectionEnd) {
            const auto diff = callback - textSectionEnd;
            //*reinterpret_cast<int64_t*>(base_address + Offsets::invalid_ptr_limit) = textSectionEnd + diff + 1;
            Memory::writeMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit, textSectionEnd + diff + 1);
        }

        FrameScriptRegisterFunction _registerFunction =
            reinterpret_cast<FrameScriptRegisterFunction>(morpher_ptr->getBaseAddress() + Offsets::frame_script_register_function);
        return _registerFunction(name, callback);
    }

    // Unused
    inline uintptr_t invalidPtrCheck(uintptr_t fn) {
        FrameScriptInvalidPtrCheck _invalidPtrCheck =
            reinterpret_cast<FrameScriptInvalidPtrCheck>(morpher_ptr->getBaseAddress() + Offsets::frame_script_invalid_ptr_check);
        return _invalidPtrCheck(fn);
    }

    inline int luaGetTop(uintptr_t lua_state) {
        LuaGetTop _luaGetTop =
            reinterpret_cast<LuaGetTop>(morpher_ptr->getBaseAddress() + Offsets::lua_gettop);
        return _luaGetTop(lua_state);
    }

    inline const char* luaToString(uintptr_t lua_state, int index, int length) {
        LuaToString _luaToString =
            reinterpret_cast<LuaToString>(morpher_ptr->getBaseAddress() + Offsets::lua_tolstring);
        return _luaToString(lua_state, index, length);
    }

    inline double luaToNumber(uintptr_t lua_state, int index) {
        LuaToNumber _luaToNumber =
            reinterpret_cast<LuaToNumber>(morpher_ptr->getBaseAddress() + Offsets::lua_tonumber);
        return _luaToNumber(lua_state, index);
    }

} // namespace WoWFunctions

} // namespace morph
