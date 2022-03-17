#pragma once
#include "stdafx.h"
#include "ESP.h"
#include "geom.h"
#include "assaultcube.h"
#include "glDraw.h"
#include "glText.h"



class GUI
{
public:
	const char* keys[8]{ "Num 1. God Mode:", "Num 2. Infinite Ammo:", "Num 3. No Recoil:", "Num 4. Speedhack:", "Num 5. Super Jump:", "Num 6. Aimbot:", "Num 7. Rapid Fire:", "Num 8. ESP"};
	bool bKeysStates[9]{ 0,0,0,0,0,0,0,0,0 };
						//0-showmenu 1-godmode, 2-inf ammo, 3-recoil, 4-speedhack, 5-superjump, 6-aimbot, 7-rapid fire, 8-esp
	void DrawGUI(GL::Font& font);
	int GetLongestString(const char* strarray[]);
};
