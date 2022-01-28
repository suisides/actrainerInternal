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
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
   
    uintptr_t localPlayerPtr = moduleBase + 0x10F4F4;
    uintptr_t entityPtr = moduleBase + 0x110D90;
    uintptr_t superJumpAddr = mem::FindDMAAddy(localPlayerPtr, { 0x18 });
    uintptr_t healthAddr = mem::FindDMAAddy(localPlayerPtr, { 0xF8 });
    uintptr_t speedAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x80 });
    uintptr_t localPlayerYawAddr = mem::FindDMAAddy(localPlayerPtr, { 0x40 });
    uintptr_t localPlayerPitchAddr = mem::FindDMAAddy(localPlayerPtr, { 0x44 });
    uintptr_t localPlayerTeamAddr = mem::FindDMAAddy(localPlayerPtr, { 0x32C });
    uintptr_t playersNumAddr = mem::FindDMAAddy(moduleBase + 0x1170, { 0x42C });
    uintptr_t directionAddr = mem::FindDMAAddy(moduleBase + 0x109b74, { 0x80 });
    uintptr_t grenadeAmmoAddr = mem::FindDMAAddy(moduleBase + 0x109B74, { 0x158 });
    uintptr_t currentAmmoPtr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14 , 0x0 });

    
    bool bHealth = false, 
         bAmmo = false, 
         bRecoil = false, 
         bSpeedhack = false,
         bSuperJump = false;

    mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);

    //hack loop
    while(true)
    {
        //key input
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            break;
        }

        if (bAmmo)
        {
            if (*(int*)(currentAmmoPtr) == 0)
                *(int*)(currentAmmoPtr) += 1;

            if (*(int*)(grenadeAmmoAddr) == 0)
                *(int*)(grenadeAmmoAddr) += 1;
        }
        
        if (bRecoil)
            mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
        else
            //write back original instructions
            mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);

        //continuous write/freeze
        if (localPlayerPtr)
        {   
            //godmode
            if (GetAsyncKeyState(VK_NUMPAD1) & 1)
            {
                bHealth = !bHealth;
                mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);
            }
        
            //inc ammo and grenades
            if (GetAsyncKeyState(VK_NUMPAD2) & 1)
            {
                bAmmo = !bAmmo;
                mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);
                if(bAmmo)
                {
                mem::Patch((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2);
                mem::Patch((BYTE*)(moduleBase + 0x63378), (BYTE*)"\xFF\x00", 2);
                }
                else
                {
                mem::Patch((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2);
                mem::Patch((BYTE*)(moduleBase + 0x63378), (BYTE*)"\xFF\x08", 2);
                }
            }
        
            //remove recoil
            if (GetAsyncKeyState(VK_NUMPAD3) & 1)
            {
                bRecoil = !bRecoil;
                mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);
                
            }
            //enable speedhack
            if (GetAsyncKeyState(VK_NUMPAD4) & 1)
            {
                bSpeedhack = !bSpeedhack;
                mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);
            }
            
            if (GetAsyncKeyState(VK_NUMPAD5) & 1)
            {
                bSuperJump = !bSuperJump;
                mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump);

            }
            
            if(GetAsyncKeyState(VK_NUMPAD7) & 1)
            {
                //vec3 self = mem::GetSelfCoords((uintptr_t)localPlayerPtr);
                //std::cout << "\nlocal coords: " << self.x << " " << self.y << " " << self.z << std::endl;  
                uintptr_t currentAmmoPtr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14 , 0x0 });
                std::cout << "ammo: " << *(int*)(currentAmmoPtr) << std::endl;
                std::cout << "grenade: " << *(int*)(grenadeAmmoAddr) << std::endl;
            }

            if (bHealth)
                *(int*)healthAddr = 1337;

            if(bSpeedhack)
            {
                if(GetAsyncKeyState(VK_CONTROL))
                    *(int*)(speedAddr) = 3;
            }

            if (bSuperJump)
            {
                if (GetAsyncKeyState(VK_SPACE) & 1)
                    *(float*)(superJumpAddr) = 4.f;
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

