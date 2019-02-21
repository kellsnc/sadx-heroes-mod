#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "bingo-highway-paths.h"
#include "bingo-highway.h"

void BingoHighwayObjects_Init(const char *path);
void BingoHighwayObjects_OnFrame(EntityData1 * entity);
void CasinoCommon_OnFrame();
void CasinoCommon_Delete(ObjectMaster * a1);

extern SOI_LISTS bingo_highway_objects[];

void BingoHighwayHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_SpeedHighwaySpeedHighway);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = CasinoCommon_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &HIGHWAY01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA88;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&bingo_highway_objects;

			if (entity->Position.z > -3440 && entity->Position.y > 370) LoadLevelFile("BH", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("BH", BingoHighwayChunks, LengthOfArray(BingoHighwayChunks), entity->Position);
			AnimateTextures(BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
			BingoHighwayObjects_OnFrame(entity);
			CasinoCommon_OnFrame();
			break;
		}
	}
}

void BingoHighway_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("HIGHWAY01", "bingo-highway");
	ReplaceBIN("SET0400S", "bingo-highway-set");
	ReplaceBIN("SET0400M", "bingo-highway-set");
	ReplaceBIN("CAM0400S", "heroes-cam");
	ReplaceDAT("HIGHWAY_BANK01", "CASINO_BANK");
	ReplaceADX("highway1", "bingo-highway");
	ReplaceBIN("PL_40B", "bingo-highway-shaders");

	helperFunctions.RegisterPathList(BingoHighwayPaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_SpeedHighway1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x26A5A94, BingoHighwayDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF48, &BingoHighwayHandler);

	BingoHighwayObjects_Init(path);
}