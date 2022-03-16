// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "pch.h"
#include <iostream>
#include "mem.h"
#include "hook.h"
#include "glText.h"
#include "glDraw.h"
#include "ESP.h"
#include "assaultcube.h"
#include "geom.h"
#include "GUI.h"


ent* localPlayer = *(ent**)0x50F4F4;
int* numOfPlayers = (int*)(0x50f500);
entList* entlist = *(entList**)0x50F4F8;
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
float drawX = 300, drawY = 300, drawLX = 0, drawLY = 0;
bool bShowGUI = false;


//toggles
bool    bHealth = false,
        bAmmo = false,
        bRecoil = false,
        bSpeedHack = false,
        bSuperJump = false,
        bAimbot = false,
        bRapidFire = false;

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers owglSwapBuffers;
twglSwapBuffers wglSwapBuffersGateway;

GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

ESP esp;
GUI gui;
vec3 self, currEnemy;

void Draw()
{
    HDC currentHDC = wglGetCurrentDC();

    if(!glFont.bBuilt || currentHDC != glFont.hdc)
    {
        glFont.Build(FONT_HEIGHT);
    }

    GL::SetupOrtho();//DRAW HERE
    
    esp.Draw(glFont);
    if(bShowGUI)
    gui.DrawGUI(glFont);

    GL::RestoreGL();
}






BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
    //toggle GUI
    if(GetAsyncKeyState(VK_DELETE) & 1)
    {
        bShowGUI = !bShowGUI;
    }
    //godmode
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
        bHealth = !bHealth;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
    }
    //inc ammo and grenades
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        bAmmo = !bAmmo;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
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
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);

    }
    //speedhack
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
    {
        bSpeedHack = !bSpeedHack;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
    }
    //super jump
    if (GetAsyncKeyState(VK_NUMPAD5) & 1)
    {
        bSuperJump = !bSuperJump;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
    }
    //aimbot 
    if (GetAsyncKeyState(VK_NUMPAD6) & 1)
    {
        bAimbot = !bAimbot;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
    }
    //rapid fire
    if (GetAsyncKeyState(VK_NUMPAD7) & 1)
    {
        bRapidFire = !bRapidFire;
        mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);
    }



    //continuous write/freeze
    if (localPlayerPtr)
    {
        
        if (bHealth)
            localPlayer->health = 1337;

        if (bAmmo)
        {
            if (*(int*)(grenadeAmmoAddr) == 0)
                *(int*)(grenadeAmmoAddr) += 1;
            if (localPlayer->currentWeapon->magAmmo->ammo == 0)
                localPlayer->currentWeapon->magAmmo->ammo++;
        }

        if (bRecoil)
            mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
        else
            mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10); //write back original instructions


        if (bSpeedHack)
        {
            if (GetAsyncKeyState(VK_CONTROL))
                localPlayer->N00000041 = 3;
        }
        if (bSuperJump)
        {
            if (GetAsyncKeyState(VK_SPACE) & 1)
                *(float*)superJumpAddr = 4.0f;
        }
        if (bAimbot)
        {
            if (GetAsyncKeyState(VK_XBUTTON1))
            {
                //bool bAnglesSettable = true;      //one day...
                
                //skip if dead
                if (localPlayer->health > 1)
                {
                    unsigned int enemyIndex = 1;
                    if (*numOfPlayers > 0)
                    {
                        //get closest enemy
                        float distance = 99999999;

                        for (unsigned int i = 1; i < *numOfPlayers; i++)
                        {
                            if (!entlist)
                                continue;
                            if (!esp.IsValidEnt(entlist->ents[i]))
                                continue;
                            //skip if teammate
                            if (entlist->ents[i]->team == localPlayer->team)
                                continue;
                            //skip if dead
                            if (!esp.IsAlive(entlist->ents[i]))
                                continue;
                            //get enemy coords
                            currEnemy = entlist->ents[i]->head;
                            float currDist = localPlayer->head.Distance(entlist->ents[i]->head);
                            if (currDist < distance)
                            {
                                distance = currDist;
                                enemyIndex = i;
                            }
                        }
                    }
                    
                    vec3 angles = mem::GetAngle(localPlayer->head, entlist->ents[enemyIndex]->head);
                    //set angles
                    localPlayer->vViewAngle.x = angles.x;
                    localPlayer->vViewAngle.y = angles.y;
                }
            }
        }
        if (bRapidFire)
        {
            if (localPlayer->currentWeapon->gunwait->N000002D6 != 0)
                localPlayer->currentWeapon->gunwait->N000002D6 = 0;
        }
            
        
    }
    Draw();
    

    return wglSwapBuffersGateway(hDc);//pointer to original function
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    std::cout << "Og for a fee, stay sippin' fam\n";
    Sleep(500);
    mem::updateKeys(bHealth, bAmmo, bRecoil, bSpeedHack, bSuperJump, bAimbot, bRapidFire);

    Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);
    SwapBuffersHook.Enable();

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
    SwapBuffersHook.Disable();
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

