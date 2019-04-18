#include "stdafx.h"
#include "mod.h"

#include "objects.h"

NJS_MATERIAL matlist_col[] = {
	{ { 0xFFFFFFFF },{ 0xFFFFFFFF }, 0, 0, NJD_DA_INV_SRC | NJD_SA_SRC }
};

NJS_MATERIAL matlist_waterfall[] = {
	{ { 0x64FFFFFF },{ 0x00FFFFFF }, 0, 87, NJD_FLAG_USE_ANISOTROPIC | NJD_FLAG_USE_ALPHA | NJD_FLAG_USE_TEXTURE | NJD_DA_ONE | NJD_SA_SRC }
};

#pragma region Killcount of entities
uint8_t killcount = 0;
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
#pragma endregion

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

void Objects_Init(const char *path, const HelperFunctions &helperFunctions) {
	if (EnableModels) CommonObjects_Init(path, helperFunctions);
	WriteJump((void*)0x004CD370, ObjectData2_SetStartPosition_r);
}