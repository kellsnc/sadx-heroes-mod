#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "grand-metropolis-paths.h"
#include "grand-metropolis-deathzones.h"
#include "grand-metropolis.h"

void GrandMetropolisObjects_Init(const char *path);
void GrandMetropolisObjects_OnFrame(EntityData1 * entity);
void GrandMetropolisObjects_Reset();
void SHSuns_Init(ObjectMaster * a1);

extern SOI_LISTS grand_metropolis_objects[];

void GrandMetropolis_Delete(ObjectMaster * a1) {
	GrandMetropolisObjects_Reset();
	set_shader_flags(ShaderFlags_Blend, false);
	set_blend(-1, -1);
	LevelHandler_Delete(a1);
}

void GrandMetropolisHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		MovePlayerToStartPoint(entity);
		camerahax_b();

		InitializeSoundManager();
		PlayMusic(MusicIDs_casino1);
		SoundManager_Delete2();

		LevelDrawDistance.Maximum = -999999.0f;
		Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

		set_shader_flags(ShaderFlags_Blend, true);

		LoadObject(LoadObj_Data1, 3, SHSuns_Init); //load the sun

		a1->Data1->Action = 1;
		a1->DeleteSub = GrandMetropolis_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &CASINO01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DB28;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&grand_metropolis_objects;

			if (entity->Position.z > 3675) LoadLevelFile("GM", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("GM", GrandMetropolisChunks, LengthOfArray(GrandMetropolisChunks), entity->Position);
			AnimateTextures(GrandMetropolisAnimTexs, LengthOfArray(GrandMetropolisAnimTexs));
			GrandMetropolisObjects_OnFrame(entity);
			break;
		}
	}
}

void GrandMetropolis_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("CASINO01", "grand-metropolis");
	ReplaceBIN("SET0900S", "grand-metropolis-set");
	ReplaceBIN("SET0900M", "grand-metropolis-set-tails");
	ReplaceBIN("SET0900K", "grand-metropolis-set");
	ReplaceBIN("CAM0900S", "heroes-cam");
	ReplaceDAT("CASINOPOLIS_BANK01", "HEROES_BANK");
	ReplaceADX("casino1", "grand-metropolis");
	ReplaceBIN("PL_90B", "grand-metropolis-shaders");

	helperFunctions.RegisterStartPosition(Characters_Sonic, GrandMetropolis_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Tails, GrandMetropolis_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, GrandMetropolis_StartPositions[0]);
	helperFunctions.RegisterPathList(GrandMetropolisPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 9, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 9, 0 });

	WriteData((DeathZone**)0x1D7E374, GrandMetropolisDeathZones);

	//Load the level handler
	WriteJump((void *)0x5C0F40, &GrandMetropolisHandler);

	GrandMetropolisObjects_Init(path);
}