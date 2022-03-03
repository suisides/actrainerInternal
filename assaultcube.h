#pragma once
#include <Windows.h>
#include "geom.h"

enum gameModes
{
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKILL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	TEAMKEEPTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	BOtlSS,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

class ent;

class weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t ID; //0x0004
	class ent* owner; //0x0008
	class N000002E8* guninfo; //0x000C
	class ammo_t* ammo; //0x0010
	class MagAmmo_t* magAmmo; //0x0014
	class gunwait_t* gunwait; //0x0018
	int32_t shots; //0x001C
	int32_t reloading; //0x0020
	char pad_0024[41]; //0x0024
};

class ent
{
public:
	DWORD vTable; //0x0
	vec3 head; //0x0004
	char pad_0010[36]; //0x0010
	vec3 pos; //0x0034
	vec3 vViewAngle; //0x0040
	char pad_004C[37]; //0x004C
	uint8_t bScoping; //0x0071
	char pad_0072[14]; //0x0072
	int8_t N00000041; //0x0080
	int8_t N000002EC; //0x0081
	int8_t N000002EF; //0x0082
	int8_t N000002ED; //0x0083
	char pad_0084[116]; //0x0084
	int32_t health; //0x00F8
	int32_t armor; //0x00FC
	char pad_0100[292]; //0x0100
	uint8_t bAttacking; //0x0224
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	uint8_t team; //0x032C
	char pad_032D[11]; //0x032D
	uint8_t state; //0x0338
	char pad_0339[59]; //0x0339
	class weapon* currentWeapon; //0x0374
};

class entList
{
public:
	ent* ents[31];
};

class mapEnt
{
public:
	int16_t x; //0x0000
	int16_t y; //0x0002
	int16_t z; //0x0004
	int16_t attr1; //0x0006
	uint8_t type; //0x0008
	uint8_t attr2; //0x0009
	uint8_t attr3; //0x000A
	uint8_t attr4; //0x000B
	uint8_t bSpawned; //0x000C
	int32_t lastmillis; //0x000D
	char pad_0011[3]; //0x0011
}; //Size: 0x0014

class MapEntVectPointer
{
public:
	class MapEntVec* MapEntP; //0x0000
}; //Size: 0x0004

class MapEntVec
{
public:
	class mapEnt EntArray[264]; //0x0000
}; //Size: 0x14A0

class flagEnt
{
public:
	int32_t team; //0x0000
	class mapEnt* mapent; //0x0004
	int32_t number; //0x0008
	class ent* player; //0x000C
	vec3 pos; //0x0010
	int32_t state; //0x001C
	int32_t unknown2; //0x0020
}; //Size: 0x0024

class flagArray
{
public:
	class flagEnt flags[2]; //0x0000
}; //Size: 0x0048

class mapheader
{
public:
	char head[4]; //0x0000
	int32_t version; //0x0004
	int32_t headersize; //0x0008
	int32_t sfactor; //0x000C
	int32_t numEnts; //0x0010
	char mapTitle[128]; //0x0014
	char pad_0094[904]; //0x0094
	int32_t cgzsize; //0x041C
	char pad_0420[8]; //0x0420
	int32_t spawn; //0x0428
	int32_t spawn2; //0x042C
	int32_t spawns; //0x0430
	int32_t flag; //0x0434
	int32_t flags; //0x0438
	int32_t flagents; //0x043C
	int32_t flagents2; //0x0440
	char pad_0444[80]; //0x0444
}; //Size: 0x0494

class sqrWORLD
{
public:
	uint8_t type; //0x0000
	int8_t floor; //0x0001
	uint8_t ceil; //0x0002
	uint8_t wtex; //0x0003
	uint8_t ftex; //0x0004
	uint8_t ctex; //0x0005
	uint8_t r; //0x0006
	uint8_t g; //0x0007
	uint8_t b; //0x0008
	uint8_t vdelta; //0x0009
	int8_t defer; //0x000A
	int8_t occluded; //0x000B
	uint8_t utex; //0x000C
	uint8_t tag; //0x000D
	uint8_t reserved1; //0x000E
	uint8_t reserved2; //0x000F
}; //Size: 0x0010

class world
{
public:
	int32_t sfactor; //0x0000
	int32_t ssize; //0x0004
	int32_t cubicsize; //0x0008
	int32_t mipsize; //0x000C
	char N00000224[4]; //0x0010
	char N00000225[4]; //0x0014
	char pad_0018[52]; //0x0018
}; //Size: 0x004C

class worldPtr
{
public:
	class sqrWORLD* N00000259; //0x0000
}; //Size: 0x0004

class ammo_t
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004

class N000002E8
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004

class MagAmmo_t
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004

class gunwait_t
{
public:
	int32_t N000002D6; //0x0000
}; //Size: 0x0004

class ptrToLocalPlayer
{
public:
	class ent* localPlayerPtr; //0x0000
}; //Size: 0x0004
