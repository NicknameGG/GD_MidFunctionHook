// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "pch.h"
#include<iostream>
#include<Windows.h>

void placeJMP(BYTE* address, DWORD jumpTo, DWORD length)
{
    DWORD oldProtect, newProtect, relativeAddress;
    VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    relativeAddress = (DWORD)(jumpTo - (DWORD)address) - 5;
    *address = 0xE9;
    *((DWORD*)(address + 0x1)) = relativeAddress;
    for (DWORD x = 0x5; x < length; x++)
    {
        *(address + x) = 0x90;
    }
    VirtualProtect(address, length, oldProtect, &newProtect);
}

DWORD base = (DWORD)GetModuleHandleA(0);
DWORD addu = (DWORD)base + 0x1E29C0;
DWORD retJMP = base + 0x1E279C;



DWORD number = 0x29E1F4;
DWORD text = base + 0x2D59D8;


__declspec(naked) void myMidfuncHook() {


    __asm {
        mov eax, text
        push eax
        mov eax, number
        push eax
        mov eax, text
        push eax
        mov ECX,EBX
        call addu
        push eax
        mov eax, 0x00000259
        jmp[retJMP]
    }
}


void mainHack() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Base: " << base << std::endl;


  placeJMP((BYTE*)base + 0x1E2797, (DWORD)myMidfuncHook, 0x5);


}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainHack, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:       


    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
