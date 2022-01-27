#pragma once
#include "stdafx.h"
#include <windows.h>
#include <vector>


struct vec3
{
	float x, y, z;
};

namespace mem
{

	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	
	vec3 GetSelfCoords(uintptr_t localPlayerPtr);
	vec3 GetEntCoords(uintptr_t entityPtr, unsigned int index);

	vec3 Subtract(vec3 src, vec3 dst);
	float Magnitude(vec3 vec);
	float GetDistance(vec3 src, vec3 dst);

	vec3 GetAngle(vec3 enemyCoord, vec3 selfCoord);

	unsigned short GetMovementDirection(uintptr_t directionAddr);
	void SetSelfCoords(uintptr_t localPlayerPtr, vec3 coords);
	vec3 IncrementBy(vec3 coords, float inc, int type);

	void Clear();
	void updateKeys(bool health, bool ammo, bool recoil, bool speedhack, bool superjump);

}