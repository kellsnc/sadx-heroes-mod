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
	set_blend(-1, -1);
	LevelHandler_Delete(a1);
}

void PowerPlantHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct != 0) {
			//if act > 0, load back ice cap
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_Icecap);

			LevelObjTexlists[0] = &OBJ_ICECAP_TEXLIST;
			LoadPVM("OBJ_ICECAP", &OBJ_ICECAP_TEXLIST);
			LoadPVM("OBJ_ICECAP2", &OBJ_ICECAP2_TEXLIST);
			LoadPVM("E102TIME", &E102TIME_TEXLIST);
			LoadPVM("E_SNOWMAN", &E_SNOWMAN_TEXLIST);
			LoadPVM("MILESRACE", &MILESRACE_TEXLIST);
			LoadPVM("E_SNAKE", &E_SNAKE_TEXLIST);
			LoadPVM("KAOS_EME", &KAOS_EME_TEXLIST);
			LoadPVM("PIRANIA", &PIRANIA_TEXLIST);
			LoadPVM("NEW_BB", &NEW_BB_TEXLIST);
			LoadPVM("BOARD_SCORE", &BOARD_SCORE_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

			InitializeSoundManager();
			PlayMusic(MusicIDs_icecap1);
			SoundManager_Delete2();

			a1->Data1->Action = 1;
			a1->DeleteSub = PowerPlant_Delete;

			set_shader_flags(ShaderFlags_Blend, true);

			if (CurrentAct == 0) {
				CurrentLevelTexlist = &ICECAP01_TEXLIST;
				CurrentLandAddress = (LandTable**)0x97DB08;

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&power_plant_objects;

				if (entity->Position.z > -2081) LoadLevelFile("PP", 01);
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
	helperFunctions.RegisterStartPosition(Characters_Tails, PowerPlant_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, PowerPlant_StartPositions[0]);
	helperFunctions.RegisterPathList(PowerPlantPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 9, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 9, 0 });
	SaveFile.LevelClear[(Characters_Tails * 43) + LevelIDs_IceCap] = 1;
	SaveFile.LevelClear[(Characters_Knuckles * 43) + LevelIDs_IceCap] = 1;

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_Icecap1[i].Maximum = -999999.0f;
		FogData_Icecap1[i].Toggle = false;
	}
	
	WriteData<4>((void*)0x0090C210, 0x00u);

	WriteData((DeathZone**)0xE2FE4C, PowerPlantDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF58, &PowerPlantHandler);

	PowerPlantObjects_Init(path);
}