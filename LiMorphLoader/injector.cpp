
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "pathcch.h"


DWORD GetProcId(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_stricmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_stricmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

//7ff6e162c7ca

int main()
{

    char szFullPath[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, szFullPath);
    std::string dll = std::string(szFullPath) + "\\LiMorph.dll";
    //std::cout << std::string(szFullPath) + "\\LiMorph.dll" << std::endl;
    const char* dllPath = dll.c_str();
  //  const char* dllPath = "C:\\Users\\Linus\\Source\\Repos\\LiMorph\\x64\\Release\\LiMorph.dll";

     const char* process_name = "Wow.exe";
    // const char* procName = "SmallProgram.exe";
    DWORD process_id = 0;


    std::cout << "Will inject as soon as Wow.exe starts." << std::endl;
    while (!process_id)
    {
        process_id = GetProcId(process_name);
      //  std::cout << "asd" << std::endl;
        Sleep(30);

    }

    uintptr_t base_address = GetModuleBaseAddress(process_id, process_name);

   //ULONG_PTR ImageBase = (ULONG_PTR)GetModuleHandle("SmallProgram.exe");
   //ImageBase = (ULONG_PTR)GetModuleHandleW(NULL);
   //int remap_ret = RmpRemapImage(ImageBase);

    uintptr_t game_version = base_address + 0x2D0241C;
    uintptr_t game_build = base_address + 0x2CF1634;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
    char version[10];
    char build[10];
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {

        ReadProcessMemory(hProc, (LPVOID)game_version, &version, 10, 0);
        ReadProcessMemory(hProc, (LPVOID)game_build, &build, 10, 0);
        std::string current_verion = std::string(version) + "." + std::string(build);
        std::string supported_version = "9.0.5.37899";

        std::cout << "Current WoW version: " << current_verion << std::endl;
      //  std::cout << "LiMorph currently supports WoW version: " + supported_version << std::endl;

        if (current_verion == supported_version) {
            SetConsoleTextAttribute(hConsole, 10);

            std::cout << "LiMorph was successfully injected." << std::endl;


            void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);

            HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
      

            if (hThread)
            {
                CloseHandle(hThread);
            }
            else {
                std::cout << "ERROR WITH REMOTE THREAD" << std::endl;
            }

        }
        else {
            SetConsoleTextAttribute(hConsole, 12);

            std::cout << "LiMorph failed to inject. Currently supports WoW version: " + supported_version << std::endl;

        }

        } else {
            std::cout << "OPEN PROCESS ERROR" << std::endl;
        }



    if (hProc) {
            CloseHandle(hProc);
    } else {
        std::cout << "ERROR WITH PROCESS" << std::endl;
    }
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << "Press enter to exit window." << std::endl;

    std::getchar();
    
    //while (1);
    return 0;
}
