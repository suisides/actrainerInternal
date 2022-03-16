#include "stdafx.h"
#include "glDraw.h"


void GL::SetupOrtho()//setting up an orthographic projection matrix instead of game's 3d projeciton matrix
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();//push matrix onto the matrix stack
	GLint viewport[4];//x, y pos of viewport | width, height of the window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//blank viewmatrix
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);//defining frustrum
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);//everything drawn is on top
}

void GL::RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
	glColor3ub(color[0], color[1], color[2]);
	
	glBegin(GL_QUADS);	//glBegin() -> immediate mode
						//GL_QUADS -> filled rectangles mode
						
						
	glVertex2f(x, y);//top left,		will connect bottom left to top left,start top left -> go clockwise to bottom left
	glVertex2f(x + width, y);//top right
	glVertex2f(x + width, y + height);//bottom right
	glVertex2f(x, y + height);//bottom left
	glEnd();
}

void GL::DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x - 0.5f , y - 0.5f);//0.5f to make rectangles distinct
	glVertex2f(x + width + 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y  + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y - 0.5f);
	glEnd();
}