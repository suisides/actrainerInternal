#include "stdafx.h"
#include "ESP.h"
#include "geom.h"
#include "GUI.h"

const int GUI_X = 50;
const int GUI_Y = 50;
const int GUI_WIDTH = 250;
const int GUI_HEIGHT = 350;
const int GUI_OUTLINE_WIDTH = 4;

const int GUI_FONT_HEIGHT = 15;
const int GUI_FONT_WIDTH = 9;
const int GUI_FONT_PADDING_X = 10;
const int GUI_FONT_PADDING_Y = 10;

const GLubyte* fontColor = rgb::black;


const char *keys[7]{"God Mode:", "Infinite Ammo:", "No Recoil:", "Speedhack:", "Super Jump:", "Aimbot:", "Rapid Fire:"};
bool keysStates[7]{ 0,0,0,0,0,0,0 };



void GUI::DrawGUI(GL::Font& font)
{
	//filled rectangle with outline
	GL::DrawFilledRect(GUI_X, GUI_Y, GUI_WIDTH, GUI_HEIGHT, rgb::lightgray);
	GL::DrawOutline(GUI_X - (GUI_OUTLINE_WIDTH / 2.0f), GUI_Y - (GUI_OUTLINE_WIDTH / 2.0f), GUI_WIDTH + (GUI_OUTLINE_WIDTH / 2.0f), GUI_HEIGHT + (GUI_OUTLINE_WIDTH / 2.0f), GUI_OUTLINE_WIDTH, rgb::black);

	//keys & features
	for (int i = 0; i < 7; i++)
	{
		float textX = GUI_X + (GUI_FONT_PADDING_X);
		float textY = GUI_Y + (i*GUI_FONT_PADDING_Y) + (i* GUI_FONT_HEIGHT);

		font.Print(textX, textY, fontColor, "%s", keys[i]);
		if (keysStates[i])
			font.Print(textX + (GUI_FONT_WIDTH * sizeof(keys[i])), textY, rgb::green, "Enabled");//sizeof(keys[i]) doesn't seem to work :/
		else
			font.Print(textX + (GUI_FONT_WIDTH * sizeof(keys[i])), textY, rgb::red, "Disabled");
	}


}

void GUI::UpdateKeys()
{



}
