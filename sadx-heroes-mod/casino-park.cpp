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
		if (CurrentAct != 0) {
			//load back twinkle park
			LoadObject(LoadObj_Data1, 1, TwinklePark_SkyBox_Load);
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_TwinklePark);

			LevelObjTexlists[0] = &OBJ_TWINKLE_TEXLIST;
			LoadPVM("OBJ_TWINKLE", &OBJ_TWINKLE_TEXLIST);
			LoadPVM("E_BUYON", &E_BUYON_TEXLIST);
			LoadPVM("BG_SHAREOBJ", &BG_SHAREOBJ_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

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

	helperFunctions.RegisterStartPosition(Characters_Sonic, CasinoPark_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Tails, CasinoPark_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, CasinoPark_StartPositions[0]);
	helperFunctions.RegisterPathList(CasinoParkPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 3, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 3, 0 });

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_TwinklePark1[i].Maximum = -999999.0f;
		FogData_TwinklePark1[i].Toggle = false;
	}

	WriteData<4>((void*)0x0090C1FC, 0x00u);

	WriteData((DeathZone**)0x26B3C58, CasinoParkDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF44, &CasinoParkHandler);

	CasinoParkObjects_Init(path);
}