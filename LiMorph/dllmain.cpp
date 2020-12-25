// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "Morpher/Morpher.h"

LiMorph::Morpher morpher;
LiMorph::Morpher* morpher_ptr;

DWORD WINAPI runMorpher(LPVOID lpParam) {
    //ThreadSynchronizer ts = ThreadSynchronizer();
   // ts.InvokeInMainThread(run_in_main
    uintptr_t base_address = (uintptr_t)GetModuleHandle(NULL);
    morpher.setBaseAddress(base_address);
    morpher_ptr = &morpher;
    morpher.startMorpher();
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved) {


    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    {
        auto thread = CreateThread(NULL, 0, &runMorpher, NULL, 0, NULL);
        CloseHandle(thread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

