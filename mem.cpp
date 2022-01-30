#include "stdafx.h"
#include "pch.h"
#include "mem.h"
#include <vector>
#define PI 3.14159265


void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}
void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}
uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

vec3 mem::GetSelfCoords(uintptr_t localPlayerPtr)
{
	vec3 coords;
	localPlayerPtr = mem::FindDMAAddy(localPlayerPtr, {0x0});
	coords.x = *(float*)(localPlayerPtr + 0x4);
	coords.y = *(float*)(localPlayerPtr + 0x8);
	coords.z = *(float*)(localPlayerPtr + 0xC);
	return coords;
}

vec3 mem::GetEntCoords(uintptr_t entityPtr, unsigned int index)
{
	entityPtr = FindDMAAddy(entityPtr, { index * 4 , 0x0 });
	vec3 coords;
	coords.x = *(float*)(entityPtr + 0x4);
	coords.y = *(float*)(entityPtr + 0x8);
	coords.z = *(float*)(entityPtr + 0xC);
	return coords;
}

vec3 mem::Subtract(vec3 src, vec3 dst)
{
	vec3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

float mem::Magnitude(vec3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float mem::GetDistance(vec3 src, vec3 dst)
{
	vec3 diff = Subtract(src, dst);
	return Magnitude(diff);
}

vec3 mem::GetAngle(vec3 src, vec3 dst)
{
	vec3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / GetDistance(src, dst)) * 180.0f / PI;
	angle.z = 0.0f;
	return angle;
}

vec3 mem::SetAngle(uintptr_t localPlayerPtr, vec3 angles)
{
	*(float*)(localPlayerPtr + 0x40) = angles.x;
	*(float*)(localPlayerPtr + 0x44) = angles.y;
	*(float*)(localPlayerPtr + 0x48) = angles.z;
}

unsigned short mem::GetMovementDirection(uintptr_t directionAddr)
{
	unsigned short directionVal = *(unsigned short*)directionAddr;
	return directionVal;
}

void mem::SetSelfCoords(uintptr_t localPlayerPtr, vec3 coords)
{	
	*(float*)(localPlayerPtr + 0x34) = coords.x;
	*(float*)(localPlayerPtr + 0x38) = coords.y;
	*(float*)(localPlayerPtr + 0x3C) = coords.z;
}

vec3 mem::IncrementBy(vec3 coords, float inc, int type)
{
	switch (type)
	{
	case 1://forward
		coords.x += inc;
		return coords;

	case 2://backward
		coords.x -= inc;
		return coords;

	case 3://right
		coords.y += inc;
		return coords;

	case 4://left
		coords.y -= inc;
		return coords;

	case 5://forwardleft
		coords.x += inc;
		coords.y -= inc;
		return coords;

	case 6://forwardright
		coords.x += inc;
		coords.y += inc;
		return coords;

	case 7://backwardleft
		coords.x -= inc;
		coords.y -= inc;
		return coords;

	case 8://backwardright
		coords.x -= inc;
		coords.y += inc;
		return coords;
	case 9://up
		coords.z += inc;
		return coords;
	case 10://down
		coords.z -= inc;
		return coords;

	}
}

void mem::Clear()
{
	std::system("cls");
}

void mem::updateKeys(bool health, bool ammo, bool recoil, bool speedhack, bool superjump, bool aimbot)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Clear();
	std::cout << "Numpad 1: Freeze Health at 1337: ";
	if (health)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << "Enabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Disabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}

	std::cout << "Numpad 2: Infinite Ammo and Grenades: ";
	if (ammo)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << "Enabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Disabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}

	std::cout << "Numpad 3: Disable Recoil: ";
	if (recoil)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << "Enabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Disabled!\n";
		SetConsoleTextAttribute(hConsole, 15);
	}

	std::cout << "Numpad 4: Speedhack(Ctrl): ";
	if (speedhack)
	{
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << "Enabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "Disabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}
	
	std::cout << "Numpad 5: Super Jump: ";
	if (superjump)
	{
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << "Enabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "Disabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}
	
	std::cout << "Numpad 6: Aimbot: ";
	if (aimbot)
	{
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << "Enabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}
	else
	{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "Disabled!\n";
					SetConsoleTextAttribute(hConsole, 15);
	}


	std::cout << "Insert: Exit\n";
}