// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"


DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Og for a fee, stay sippin' fam\n";
     //get module base

    uintptr_t* directionAddr = 0,
                localPlayerPtr = 0,
                healthAddr = 0,
                speedAddr = 0,
                grenadeAddr = 0,
                entityPtr = 0,
                localPlayerYawAddr = 0,
                playersNumAddr = 0,
                localPlayerPitchAddr = 0,
                localPlayerXAddr = 0,
                localPlayerYAddr = 0,
                localPlayerZAddr = 0,
                localPlayerTeamAddr = 0;


   
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
    bool bHealth = false, bAmmo = false, bRecoil = false;
    uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);
    
    
    entityPtr = (uintptr_t*)(moduleBase + 0x110D90);
    healthAddr = (uintptr_t*)(localPlayerPtr + 0xF8);
    speedAddr = (uintptr_t*)(localPlayerPtr + 0x80);
    localPlayerYawAddr = (uintptr_t*)(localPlayerPtr + 0x40);
    localPlayerPitchAddr = (uintptr_t*)(localPlayerPtr + 0x44);
    localPlayerTeamAddr = (uintptr_t*)(localPlayerPtr + 0x32C);
    playersNumAddr = (uintptr_t*)((mem::FindDMAAddy(moduleBase + 0x1170, { 0x42C }));
    directionAddr = (uintptr_t*)(mem::FindDMAAddy(moduleBase + 0x109b74, { 0x80 }));

    
    //hack loop
    while(true)
    {
        //key input
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        //godmode
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;

            if(bRecoil)
            {
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
            }
            else
            {
                //write back original instructions
                mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }
        
        
        if(GetAsyncKeyState(VK_NUMPAD7) & 1)
            {
                vec3 self = mem::GetSelfCoords((uintptr_t)localPlayerPtr);
                std::cout << "x = " << self.x << "\ny = " << self.y << "\nz = " << self.z << std::endl;
            }

        //continuous write/freeze
       

        if (localPlayerPtr)
        {
            if (bHealth)
            {
                *(int*)(*localPlayerPtr + 0xf8) = 1337;
            }
            
            if (bAmmo)
            {
                //uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 });
                //int* ammo = (int*)ammoAddr;
                //*ammo = 1337;
                
                //or just 
                
                *(int*)mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 }) = 1337;
            }

            
        }
        Sleep(5);
    }

    //cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));

    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

