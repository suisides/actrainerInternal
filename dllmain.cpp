// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "pch.h"
#include <iostream>
#include "mem.h"

uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);
uintptr_t* entityPtr = (uintptr_t*)(moduleBase + 0x110D90);
uintptr_t superJumpAddr = mem::FindDMAAddy((uintptr_t)localPlayerPtr, { 0x18 });
uintptr_t healthAddr = mem::FindDMAAddy((uintptr_t)localPlayerPtr, { 0xF8 });
uintptr_t speedAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x80 });
uintptr_t localPlayerYawAddr = mem::FindDMAAddy((uintptr_t)localPlayerPtr, { 0x40 });
uintptr_t localPlayerPitchAddr = mem::FindDMAAddy((uintptr_t)localPlayerPtr, { 0x44 });
uintptr_t localPlayerTeamAddr = mem::FindDMAAddy((uintptr_t)localPlayerPtr, { 0x32C });
uintptr_t playersNumAddr = mem::FindDMAAddy(moduleBase + 0x1170, { 0x42C });
uintptr_t directionAddr = mem::FindDMAAddy(moduleBase + 0x109b74, { 0x80 });
uintptr_t grenadeAmmoAddr = mem::FindDMAAddy(moduleBase + 0x109B74, { 0x158 });
uintptr_t currentAmmoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0});

//toggles
bool    bHealth = false,
        bAmmo = false,
        bRecoil = false,
        bSpeedhack = false,
        bSuperJump = false,
        bAimbot = false;

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers owglSwapBuffers;

vec3 self, currEnemy;

BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
    
    //key input

    //exit | unhook function ...
    /*if (GetAsyncKeyState(VK_INSERT) & 1)
    {
        break;
    }*/

    //godmode
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
        bHealth = !bHealth;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);
    }
    //inc ammo and grenades
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        bAmmo = !bAmmo;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);
        if (bAmmo)
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
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);

    }
    //enable speedhack
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
    {
        bSpeedhack = !bSpeedhack;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);
    }
    //super jump
    if (GetAsyncKeyState(VK_NUMPAD5) & 1)
    {
        bSuperJump = !bSuperJump;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);
    }
    //aimbot 
    if (GetAsyncKeyState(VK_NUMPAD6) & 1)
    {
        bAimbot = !bAimbot;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);
    }
    //debug
    if (GetAsyncKeyState(VK_NUMPAD7) & 1)
    {
        self = mem::GetSelfCoords((uintptr_t)localPlayerPtr);
        vec3 ent1coords = mem::GetEntCoords((uintptr_t)entityPtr, 0x0);
        std::cout << self.x << " , " << self.y << " , " << self.z << std::endl;
        std::cout << "local hp: " << std::dec <<*(int*)healthAddr << std::endl;
        std::cout << "local team: " << *(int*)localPlayerTeamAddr << std::endl;
        std::cout << "number of players: " << *(int*)playersNumAddr << std::endl;
        std::cout << "address of entity 0: " << std::hex << mem::FindDMAAddy((uintptr_t)entityPtr, { 0x0 }) << std::endl;
        std::cout << "coords of entity 0: " << std::dec << ent1coords.x << " , " << ent1coords.y << " , " << ent1coords.z << std::endl;
        std::cout << "address of entity 2: " << std::hex << mem::FindDMAAddy((uintptr_t)entityPtr, { 0x8 }) << std::endl;

       /* for (unsigned int i = 0; i < *(int*)playersNumAddr; i++)
        {
            uintptr_t entityPtr = mem::FindDMAAddy(entityPtr, { i * 4 });
            std::cout << "entity address:" << std::hex << entityPtr << std::endl;
            Sleep(100);
        }*/
    }



    //continuous write/freeze
    if (localPlayerPtr)
    {
        if (bHealth)
            *(int*)healthAddr = 1337;



        if (bAmmo)
        {
            currentAmmoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14 , 0x0 });
            if (*(int*)(currentAmmoAddr) == 0)
                *(int*)(currentAmmoAddr) += 1;

            if (*(int*)(grenadeAmmoAddr) == 0)
                *(int*)(grenadeAmmoAddr) += 1;
        }

        if (bRecoil)
            mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
        else
            mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10); //write back original instructions


        if (bSpeedhack)
        {
            if (GetAsyncKeyState(VK_CONTROL))
                *(int*)speedAddr = 3;
        }

        if (bSuperJump)
        {
            if (GetAsyncKeyState(VK_SPACE) & 1)
                *(float*)superJumpAddr = 4.f;
        }
        if (bAimbot)
        {
            if (GetAsyncKeyState(VK_XBUTTON1))
            {
                //skip if dead
                if (*(int*)healthAddr > 1)
                {
                    //get local position
                    self = mem::GetSelfCoords((uintptr_t)localPlayerPtr);
                    
                    unsigned int enemyIndex = 0;
                    if (*(int*)playersNumAddr > 0) 
                    {
                        
                        //get closest enemy
                        float distance = 99999999;
                        
                        
                        for (unsigned int i = 0; i < *(int*)playersNumAddr - 1; i++)
                        {
                            uintptr_t currEnemyTeamAddr = mem::FindDMAAddy((uintptr_t)entityPtr, { i * 4, 0x32C });
                            uintptr_t currEnemyHPAddr = mem::FindDMAAddy((uintptr_t)entityPtr, { i * 4, 0xF8 });
                            //skip if teammate
                            if (*(int*)currEnemyTeamAddr == *(int*)localPlayerTeamAddr)
                                continue;
                            //skip if dead
                            if (*(int*)currEnemyHPAddr < 1)
                                continue;

                            //get enemy coords
                            currEnemy = mem::GetEntCoords((uintptr_t)entityPtr, i);
                            float currDist = mem::GetDistance(self, currEnemy);
                            if (currDist < distance)
                            {
                                distance = currDist;
                                enemyIndex = i;
                            }
                        }
                    }
                    
                    vec3 angles = mem::GetAngle(self, mem::GetEntCoords((uintptr_t)entityPtr, enemyIndex));
                    
                    //set angles
                    
                    *(float*)localPlayerYawAddr = angles.x;
                    *(float*)localPlayerPitchAddr = angles.y;
                }
            }
        }
    }
    
    return owglSwapBuffers(hDc);//pointer to original function
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    std::cout << "Og for a fee, stay sippin' fam\n";
    Sleep(500);
    mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedhack, bSuperJump, bAimbot);

    // Hook
    owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
    owglSwapBuffers = (twglSwapBuffers)mem::TrampHook32((BYTE*)owglSwapBuffers, (BYTE*)hkwglSwapBuffers, 5); //address returned by TrampHook32(...)
    //
    

    //cleanup & eject
    
    //keep console open
    while(true)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) 
        {
            fclose(f);
            FreeConsole();
            break;
        }
        Sleep(5);
    }  
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
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

