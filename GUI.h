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
	const char* keys[7]{ "God Mode:", "Infinite Ammo:", "No Recoil:", "Speedhack:", "Super Jump:", "Aimbot:", "Rapid Fire:" };
	bool bKeysStates[7]{ 0,0,0,0,0,0,0 };

	void DrawGUI(GL::Font& font);
	int GetLongestString(const char* strarray[]);
};