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

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
	
	LevelHandler_Delete(a1);
}

void PowerPlantHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_icecap1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = PowerPlant_Delete;

		if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &ICECAP01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DB08;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&power_plant_objects;

			if (entity->Position.z > -2081) LoadLevelFile("PP", 01);
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

	helperFunctions.RegisterPathList(PowerPlantPaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_Icecap1[i].Toggle = false;
	}

	WriteData((DeathZone**)0xE2FE4C, PowerPlantDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF58, &PowerPlantHandler);

	PowerPlantObjects_Init(path);
}