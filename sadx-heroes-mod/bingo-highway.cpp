#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "bingo-highway-paths.h"
#include "bingo-highway.h"

ObjectFunc(SpeedHighway_SkyBox_Load, 0x610A70);

void BingoHighwayObjects_Init(const char *path);
void BingoHighwayObjects_OnFrame(EntityData1 * entity);
void CasinoCommon_OnFrame();
void CasinoCommon_Delete(ObjectMaster * a1);

extern SOI_LISTS bingo_highway_objects[];

void BingoHighwayHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct != 0) {
			//load back speed highway
			LoadObject(LoadObj_Data1, 1, SpeedHighway_SkyBox_Load);
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_SpeedHighway);

			LevelObjTexlists[0] = &OBJ_HIGHWAY_TEXLIST;
			LoadPVM("OBJ_HIGHWAY", &OBJ_HIGHWAY_TEXLIST);
			LoadPVM("OBJ_HIGHWAY2", &OBJ_HIGHWAY2_TEXLIST);
			LoadPVM("MILESRACE", &MILESRACE_TEXLIST);
			LoadPVM("NISEPAT", &NISEPAT_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			InitializeSoundManager();
			PlayMusic(MusicIDs_SpeedHighwaySpeedHighway);
			SoundManager_Delete2();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

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

	helperFunctions.RegisterStartPosition(Characters_Sonic, BingoHighway_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Tails, BingoHighway_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, BingoHighway_StartPositions[0]);
	helperFunctions.RegisterPathList(BingoHighwayPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 4, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 4, 0 });

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_SpeedHighway1[i].Maximum = -999999.0f;
		FogData_SpeedHighway1[i].Toggle = false;
	}

	WriteData<4>((void*)0x0090C200, 0x00u);

	WriteData((DeathZone**)0x26A5A94, BingoHighwayDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF48, &BingoHighwayHandler);

	BingoHighwayObjects_Init(path);
}