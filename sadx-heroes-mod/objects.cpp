#include "stdafx.h"
#include "mod.h"

#include "objects.h"

PVMEntry CommonObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "kajiki", &kajiki_TEXLIST },

	//animals
	{ "PEN", &PEN_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "RAKO", &RAKO_TEXLIST },
	{ "mecha", &mecha_TEXLIST },
	{ "sea_bass", &sea_bass_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "utsubo", &utsubo_TEXLIST },
	{ "GOMA", &GOMA_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "GORI", &GORI_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "tai", &tai_TEXLIST },
	{ "WARA", &WARA_TEXLIST },

	{ "hammer", &HAMMER_TEXLIST },
	{ "same", &SAME_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ NULL, (TexList *)0x10A3130 },
	{ NULL, (TexList *)0x10C786C },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST},
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

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
	auto entity = EntityData1Ptrs[slot];
	CharObj2 *co2 = GetCharObj2(slot);
	co2->Speed.y = 2;

	if (GetCharacterID(slot) == Characters_Sonic && !SuperSonicFlag) {
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