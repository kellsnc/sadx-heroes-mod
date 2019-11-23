#include "stdafx.h"

NJS_MATERIAL matlist_col[] = {
	{ { 0xFFFFFFFF },{ 0xFFFFFFFF }, 0, 0, NJD_DA_INV_SRC | NJD_SA_SRC }
};

NJS_MATERIAL matlist_waterfall[] = {
	{ { 0x64FFFFFF },{ 0x00FFFFFF }, 0, 87, NJD_FLAG_USE_ANISOTROPIC | NJD_FLAG_USE_ALPHA | NJD_FLAG_USE_TEXTURE | NJD_DA_ONE | NJD_SA_SRC }
};

#define SET_COUNT 4096
static SETObjData set_table[SET_COUNT]{};

uint8_t killcount = 0;

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void CommonObjects_OnFrame();

void __cdecl ObjectData2_SetStartPosition_r(EntityData1 *a1, ObjectData2 *a2)
{
	NJS_VECTOR *v2; // ecx

	v2 = &a2->vector_a;
	v2->x = a1->Position.x;
	v2->y = a1->Position.y;
	v2->z = a1->Position.z;

	if (a1->Scale.z == 2 && a1->Action == 4) {
		killcount += 1;
	}
}

void ElevatePlayer(uint8_t slot) {
	EntityData1 *entity = EntityData1Ptrs[slot];
	CharObj2 *co2 = CharObj2Ptrs[slot];
	co2->Speed.y = 2;

	if (GetCharacterID(slot) == Characters_Sonic && (co2->Upgrades & Upgrades_SuperSonic) == 0) {
		co2->AnimationThing.Index = 26;
		entity->Status = 0;
	}
	else if (GetCharacterID(slot) == Characters_Tails) {
		co2->AnimationThing.Index = 33;
		entity->Status = 0;
	}
	else if (GetCharacterID(slot) == Characters_Knuckles) {
		co2->AnimationThing.Index = 34;
		entity->Status = 0;
	}
}

void Objects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	CommonObjects_Init(path, helperFunctions, config);
	WriteJump((void*)0x004CD370, ObjectData2_SetStartPosition_r);

	WriteData((SETObjData**)0x0046B817, &set_table[0]);
	WriteData((SETObjData**)0x0046BAC8, &set_table[0]);
	WriteData((SETObjData**)0x0059230A, &set_table[0]);
	WriteData((SETObjData**)0x0046BD83, &set_table[0]);
	WriteData((short**)0x0046BE6E, &set_table[0].Flags);
	WriteData((SETObjData**)0x0046BE73, &set_table[0]);
	WriteData((short**)0x0046BE0C, &set_table[0].Flags);
	WriteData((short**)0x0046BF4D, &set_table[0].Flags);
	WriteData((ObjectMaster***)0x0059222D, &set_table[0].ObjInstance);
	WriteData((short**)0x0046C1DB, &set_table[SET_COUNT].Flags);

	WriteData((int*)0x0046BD64, SET_COUNT);
	WriteData((int*)0x0046BD6C, SET_COUNT);
	WriteData((int*)0x0046BF3D, SET_COUNT);
	WriteData((int*)0x0046BF44, SET_COUNT);
	WriteData((int*)0x0046C1E0, (SET_COUNT + 1));
}