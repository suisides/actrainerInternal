#include "stdafx.h"
#include "ESP.h"
#include "geom.h"
#include "GUI.h"
#include <iostream>
#include <Windows.h>


GUI gui1;
const int GUI_FONT_HEIGHT = 15;
const int GUI_FONT_WIDTH = 9;
const int GUI_FONT_PADDING_X = 10;
const int GUI_FONT_PADDING_Y = 10;

const int GUI_X = 30;
const int GUI_Y = 25;
const int GUI_WIDTH = GUI_FONT_PADDING_X + (GUI_FONT_WIDTH * gui1.GetLongestString(gui1.keys)) + (strlen("Disabled") * GUI_FONT_WIDTH) + GUI_FONT_PADDING_X;
const int GUI_HEIGHT = GUI_FONT_PADDING_Y + (GUI_FONT_HEIGHT * std::size(gui1.keys)) + (GUI_FONT_PADDING_Y * std::size(gui1.keys)) + GUI_FONT_PADDING_Y;
const int GUI_OUTLINE_WIDTH = 4;

const GLubyte* fontColor = rgb::black;

int GUI::GetLongestString(const char* strarray[])
{
	int length = 0;
	for (int i = 0; i < sizeof(strarray); i++)
	{
		if (strlen(strarray[i]) > length)
			length = strlen(strarray[i]);
	}
	return length;
}

void GUI::DrawGUI(GL::Font& font)
{
	//filled rectangle with outline
	GL::DrawFilledRect(GUI_X, GUI_Y, GUI_WIDTH, GUI_HEIGHT, rgb::lightgray);
	GL::DrawOutline(GUI_X - (GUI_OUTLINE_WIDTH / 2.0f), GUI_Y - (GUI_OUTLINE_WIDTH / 2.0f), GUI_WIDTH + (GUI_OUTLINE_WIDTH / 2.0f), GUI_HEIGHT + (GUI_OUTLINE_WIDTH / 2.0f), GUI_OUTLINE_WIDTH, rgb::black);

	//keys & features
	for (int i = 0; i < std::size(gui1.keys); i++)
	{
		float textX = GUI_X + (GUI_FONT_PADDING_X);
		float textY = GUI_Y + ((i+1)*GUI_FONT_PADDING_Y) + ((i+1)* GUI_FONT_HEIGHT);

		font.Print(textX, textY, fontColor, "%s", keys[i]);
		if (bKeysStates[i+1])
			font.Print(textX + (GUI_FONT_WIDTH * GetLongestString(keys)), textY, rgb::darkgreen, "Enabled");
		else
			font.Print(textX + (GUI_FONT_WIDTH * GetLongestString(keys)), textY, rgb::darkred, "Disabled");
	}
}
