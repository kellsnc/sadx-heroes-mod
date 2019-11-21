#include "stdafx.h"

void SpecialStagesObjects_Init();
void __cdecl SSWaves(ObjectMaster *a1);
DataArray(LandTable*, dword_97DBE8, 0x97DBE8, 193);

extern ModelInfo * SS_SSWAVES;

void SpecialStage_InitObjects() {
	SS_SSWAVES = LoadObjectModel("SS_SSWAVES");

	LoadObject(LoadObj_Data1, 3, SSWaves);
}

void SpecialStage_Delete(ObjectMaster * a1) {
	FreeMDL(SS_SSWAVES);

	LevelHandler_Delete(a1);
}

void SpecialStagesHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = SpecialStage_Delete;

		InitializeSoundManager();
		PlayMusic(MusicIDs_circuit);
		SoundManager_Delete2();

		CurrentLevelTexlist = (NJS_TEXLIST*)0xADF8A0;
		CurrentLandAddress = &dword_97DBE8[160];

		SpecialStage_InitObjects();

		LoadLevelFile("SS", CurrentAct + 1);
	}
	else {
		//Physics changes, makes character accelerate faster and allow ceiling-running
		if (a1->Data1->NextAction == 0) {
			a1->Data1->NextAction = 1;
			for (uint8_t id = 0; id < 8; ++id) {
				if (CharObj2Ptrs[id]) {
					CharObj2Ptrs[id]->PhysicsData.MaxAccel = 20;
					CharObj2Ptrs[id]->PhysicsData.GroundAccel = 3;
					CharObj2Ptrs[id]->PhysicsData.GravityAdd = 0;
				}
			}
		}
	}
}

void SpecialStages_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("MINI_CART01", "special-stages");
	ReplaceBIN("SETMCART00S", "special-stages");
	ReplaceBIN("PL_Z0B", "special-stages-shaders");

	MusicList[MusicIDs_circuit].Name = "special-stages";

	WriteJump((void*)0x4DAA80, SpecialStagesHandler);
	DeathZoneList[HeroesLevelID_SpecialStages] = nullptr;

	SpecialStagesObjects_Init();
}