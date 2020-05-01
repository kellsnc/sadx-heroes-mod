#include "stdafx.h"

void SpecialStagesObjects_Init();
void __cdecl SSWaves(ObjectMaster *a1);

extern ModelInfo * SS_SSWAVES;

void SpecialStagesHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;

		LoadObject(LoadObj_Data1, 3, SSWaves);

		LoadLevelFile("SS01", "special-stages");
	}
	else {
		//Physics changes, makes character accelerate faster and allow ceiling-running
		if (a1->Data1->NextAction == 0) {
			a1->Data1->NextAction = 1;
			for (uint8_t id = 0; id < MaxPlayers; ++id) {
				if (CharObj2Ptrs[id]) {
					CharObj2Ptrs[id]->PhysicsData.MaxAccel = 20;
					CharObj2Ptrs[id]->PhysicsData.GroundAccel = 3;
					CharObj2Ptrs[id]->PhysicsData.GravityAdd = 0;
				}
			}
		}
	}
}

void SpecialStages_Unload() {
	SS_SSWAVES = FreeMDL(SS_SSWAVES);
}

void SpecialStages_Load() {
	SS_SSWAVES = LoadObjectModel(SS_SSWAVES, "SS_SSWAVES");
}

void SpecialStages_Init( const HelperFunctions &helperFunctions) {
	LevelObjects[HeroesLevelID_SpecialStages] = SpecialStagesHandler;
	SkyboxObjects[HeroesLevelID_SpecialStages] = HeroesSkybox_Main;
	DeathZoneList[HeroesLevelID_SpecialStages] = nullptr;

	SpecialStagesObjects_Init();
}

HeroesLevelData SpecialStagesData = { HeroesLevelID_SpecialStages, 0, 0, "special-stage", "SS", SpecialStages_Load, SpecialStages_Unload, SpecialStages_Init, { 200, 0, 0 } };