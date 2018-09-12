#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "power-plant-paths.h"
#include "power-plant-deathzones.h"
#include "power-plant.h"

void PowerPlantObjects_Init(const char *path);
void PowerPlantObjects_OnFrame(EntityData1 * entity);
void PowerPlantObjects_Reset();

extern SOI_LISTS power_plant_objects[];

void PowerPlant_Delete(ObjectMaster * a1) {
	PowerPlantObjects_Reset();
	set_shader_flags(ShaderFlags_Blend, false);
	LevelHandler_Delete(a1);
}

void PowerPlantHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct != 0) {
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_Icecap);
			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			InitializeSoundManager();
			PlayMusic(MusicIDs_casino1);
			SoundManager_Delete2();

			a1->Data1->Action = 1;
			a1->DeleteSub = PowerPlant_Delete;

			set_shader_flags(ShaderFlags_Blend, true);

			if (CurrentAct == 0) {
				CurrentLevelTexlist = &ICECAP01_TEXLIST;
				CurrentLandAddress = (LandTable**)0x97DB08;

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&power_plant_objects;
			}
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("PP", PowerPlantChunks, LengthOfArray(PowerPlantChunks), entity->Position);
			AnimateTextures(PowerPlantAnimTexs, LengthOfArray(PowerPlantAnimTexs));
			PowerPlantObjects_OnFrame(entity);
			break;
		}
	}
}

void PowerPlant_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("ICECAP01", "power-plant");
	ReplaceBIN("SET0800S", "power-plant-set");
	ReplaceBIN("SET0800M", "power-plant-set-tails");
	ReplaceBIN("CAM0800S", "heroes-cam");
	ReplaceDAT("ICE_CAP_BANK01", "HEROES_BANK");
	ReplaceADX("icecap1", "power-plant");
	ReplaceBIN("PL_80B", "power-plant-shaders");

	helperFunctions.RegisterStartPosition(Characters_Sonic, PowerPlant_StartPositions[0]);
	helperFunctions.RegisterPathList(PowerPlantPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 9, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 9, 0 });

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_Icecap1[i].Maximum = -999999.0f;
		FogData_Icecap1[i].Toggle = false;
	}
	
	WriteData((DeathZone**)0xE2FE4C, PowerPlantDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF58, &PowerPlantHandler);

	PowerPlantObjects_Init(path);
}