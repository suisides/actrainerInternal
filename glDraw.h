#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include "mem.h"

namespace rgb
{
	const GLubyte red[3] = { 255,0,0 };
	const GLubyte darkred[3] = { 128,0,0 };
	const GLubyte green[3] = { 0,255,0 };
	const GLubyte darkgreen[3] = { 0,128, 0 };
	const GLubyte blue[3] = { 0,0,255 };
	const GLubyte yellow[3] = { 255,255,0 };
	const GLubyte cyan[3] = { 0,255,255 };
	const GLubyte magenta[3] = { 128,0,128 };
	const GLubyte purple[3] = { 255,255,0 };
	const GLubyte gray[3] = { 55,55,55 };
	const GLubyte darkgray[3] = { 35,35,35 };
	const GLubyte lightgray[3] = { 192,192,192 };
	const GLubyte black[3] = { 0,0,0 };


}


namespace GL
{

	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height,float lineWidth, const GLubyte color[3]);
}