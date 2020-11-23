#pragma once

namespace morph {

namespace Memory
{

    template<typename T>
    inline T readMemory(uintptr_t adr) {
        return *(reinterpret_cast<T*>(adr));
    }

    template<typename T>
    inline void writeMemory(uintptr_t adr, T val) {
        *(reinterpret_cast<T*>(adr)) = val;
    }

    template<typename T>
    inline T* pointMemory(uintptr_t adr) {
        return reinterpret_cast<T*>(adr);
    }

    template<typename T>
    inline DWORD protectMemory(uintptr_t adr, DWORD prot) {
        DWORD oldProt;
        VirtualProtect((void*)adr, sizeof(T), prot, &oldProt);
        return oldProt;
    }

    template<typename T>
    inline DWORD protectMemory(HANDLE proc, uintptr_t adr, DWORD prot) {
        DWORD oldProt;
        VirtualProtectEx(proc, adr, sizeof(T), prot, &oldProt);
        return oldProt;
    }

} //namespace Memory
} // namespace morph