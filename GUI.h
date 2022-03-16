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
	void DrawGUI(GL::Font& font);
	void UpdateKeys();
};