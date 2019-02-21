#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "casino-park-paths.h"
#include "casino-park-deathzones.h"
#include "casino-park.h"

ObjectFunc(TwinklePark_SkyBox_Load, 0x61D570);

void CasinoParkObjects_Init(const char *path);
void CasinoParkObjects_OnFrame(EntityData1 * entity);
void CasinoCommon_OnFrame();
void CasinoCommon_Delete(ObjectMaster * a1);

extern SOI_LISTS casino_park_objects[];

void CasinoParkHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_TwinkleParkTwinklePark);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = CasinoCommon_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &TWINKLE01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA68;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&casino_park_objects;

			if (entity->Position.z > -1697 && entity->Position.x > -8696) LoadLevelFile("CP", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("CP", CasinoParkChunks, LengthOfArray(CasinoParkChunks), entity->Position);
			AnimateTextures(CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));
			CasinoParkObjects_OnFrame(entity);
			CasinoCommon_OnFrame();
			break;
		}
	}
}

void CasinoPark_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("TWINKLE01", "casino-park");
	ReplaceBIN("SET0300S", "casino-park-set");
	ReplaceBIN("CAM0300S", "heroes-cam");
	ReplaceDAT("TWINKLE_PARK_BANK01", "CASINO_BANK");
	ReplaceADX("twnklpk1", "casino-park");
	ReplaceBIN("PL_30B", "casino-park-shaders");

	helperFunctions.RegisterPathList(CasinoParkPaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_TwinklePark1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x26B3C58, CasinoParkDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF44, &CasinoParkHandler);

	CasinoParkObjects_Init(path);
}