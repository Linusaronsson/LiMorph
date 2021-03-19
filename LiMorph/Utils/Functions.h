#pragma once

#include "../pch.h"
#include "Offsets.h"
#include "../Morpher/Morpher.h"

#include <functional>

extern LiMorph::Morpher* morpher_ptr;

namespace LiMorph {
namespace WoWFunctions {
    typedef uintptr_t(__cdecl* GetUnitFromName)(const char*);
    typedef void(__fastcall* UpdateDisplayInfo)(uintptr_t);
    typedef void(__fastcall* SetVisualKit)(uintptr_t, int64_t);
    typedef void(__fastcall* SetVisualKitTwo)(uintptr_t);

    typedef void(__fastcall* UpdateMountDisplayInfo)(uintptr_t, unsigned int);
    typedef uintptr_t(__fastcall* FrameScriptExecute)(const char* script, const char* script_name, int64_t val);

    typedef void(__fastcall* FrameScriptExecuteWrapper)(uintptr_t lua_state);

    typedef bool(__fastcall* FrameScriptRegisterFunction)(const char*, uintptr_t);
    typedef bool(__fastcall* FrameScriptRegisterWrapperFunction)();

    typedef uintptr_t(__cdecl* FrameScriptInvalidPtrCheck)(uintptr_t);
    typedef int(__fastcall* LuaGetTop)(uintptr_t);
    //typedef int(__fastcall* LuaPushNumber)(uintptr_t, 0); //not sure about this
    typedef const char* (__fastcall* LuaToString)(uintptr_t, int, int);
    typedef double(__fastcall* LuaToNumber)(uintptr_t, int);
    typedef void(__fastcall* LuaPushLString)(uintptr_t, const char*, int);
    typedef void(__cdecl* LuaSetTop)(uintptr_t, int);

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

    inline void setVisualKit(uintptr_t player, uintptr_t func, int visual_spell_id, int arg2, int arg3, int arg4) {
        SetVisualKit _setVisualKit =
            reinterpret_cast<SetVisualKit>(morpher_ptr->getBaseAddress() + func);
        _setVisualKit(player, (int64_t)visual_spell_id);
    }

    inline void setVisualKitTwo(uintptr_t player, uintptr_t func) {
        SetVisualKitTwo _setVisualKitTwo =
            reinterpret_cast<SetVisualKitTwo>(morpher_ptr->getBaseAddress() + func);
        _setVisualKitTwo(player);
    }

    inline void updateMountModel(uintptr_t player, int mountID) {
        UpdateMountDisplayInfo _updateMountModel =
            reinterpret_cast<UpdateMountDisplayInfo>(morpher_ptr->getBaseAddress() + Offsets::update_mount_display_info);
        _updateMountModel(player, mountID);

    }

    inline void luaSetTop(uintptr_t lua_state, int index) {
        WoWFunctions::LuaSetTop f =
            reinterpret_cast<WoWFunctions::LuaSetTop>(morpher_ptr->getBaseAddress() + Offsets::lua_settop);
        f(lua_state, index);
    }

    inline void executeWrapperLUA(uintptr_t lua_state) {
        WoWFunctions::FrameScriptExecuteWrapper f =
            reinterpret_cast<WoWFunctions::FrameScriptExecuteWrapper>(morpher_ptr->getBaseAddress() + Offsets::frame_script_execute_wrapper);
        f(lua_state);
    }

    inline void executeLUA(const std::string& script) {
        const auto textSectionEnd = Memory::readMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit);
        if ((uintptr_t)executeLUA >= textSectionEnd) {
            const auto diff = (uintptr_t)executeLUA - textSectionEnd;
            //*reinterpret_cast<int64_t*>(base_address + Offsets::invalid_ptr_limit) = textSectionEnd + diff + 1;
            Memory::writeMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit, textSectionEnd + diff + 100);
        }
        FrameScriptExecute _executeLUA =
            reinterpret_cast<FrameScriptExecute>(morpher_ptr->getBaseAddress() + Offsets::frame_script_execute);
        _executeLUA(script.c_str(), "testxD", 0);
        //executeLUA(script, script, 1);
    }


    inline void registerFunction(const char* name, uintptr_t callback) {
        const auto textSectionEnd = Memory::readMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit);
        if (callback >= textSectionEnd) {
            const auto diff = callback - textSectionEnd;
            //*reinterpret_cast<int64_t*>(base_address + Offsets::invalid_ptr_limit) = textSectionEnd + diff + 1;
            Memory::writeMemory<int64_t>(morpher_ptr->getBaseAddress() + Offsets::invalid_ptr_limit, textSectionEnd + diff + 1);
        }

        //FrameScriptRegisterFunction _registerFunction =
        //    reinterpret_cast<FrameScriptRegisterFunction>(morpher_ptr->getBaseAddress() + Offsets::frame_script_register_function);

        FrameScriptRegisterWrapperFunction _registerFunction =
            reinterpret_cast<FrameScriptRegisterWrapperFunction>(morpher_ptr->getBaseAddress() + Offsets::frame_script_register_wrapper_function);

        const char* tempBlizzD1 = *reinterpret_cast<const char**>(morpher_ptr->getBaseAddress() + 0x319EAF0);
        int64_t tempBlizzD2 = *reinterpret_cast<int64_t*>(morpher_ptr->getBaseAddress() + 0x319EAF0 + 8);

        *reinterpret_cast<int64_t*>(morpher_ptr->getBaseAddress() + 0x319EAF0) = (int64_t)name;
        *reinterpret_cast<int64_t*>(morpher_ptr->getBaseAddress() + 0x319EAF0 + 8) = callback;

        _registerFunction();

        *reinterpret_cast<int64_t*>(morpher_ptr->getBaseAddress() + 0x319EAF0) = (int64_t)tempBlizzD1;
        *reinterpret_cast<int64_t*>(morpher_ptr->getBaseAddress() + 0x319EAF0 + 8) = tempBlizzD2;

        //return true;
       // return _registerFunction(name, callback);
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

    inline void luaPushLString(uintptr_t lua_state, const char* str, int len) {
        LuaPushLString _lua_pushlstring =
            reinterpret_cast<LuaPushLString>(morpher_ptr->getBaseAddress() + Offsets::lua_pushlstring);

        _lua_pushlstring(lua_state, str, len);
    }

} // namespace WoWFunctions

} // namespace morph
