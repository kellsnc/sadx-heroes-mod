#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "hang-castle-paths.h"
#include "hang-castle-deathzones.h"
#include "hang-castle.h"

ObjectFunc(LostWorld_SkyBox_Load, 0x5E1FC0);

void HangCastleObjects_Init(const char *path);
void HangCastleObjects_OnFrame(EntityData1 * entity);
void HCFlags_Reset();

extern SOI_LISTS hang_castle_objects[];

void HangCastle_Delete(ObjectMaster * a1) {
	HCFlags_Reset();
	LevelHandler_Delete(a1);
}

void HangCastleHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct != 0) {
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_LostWorld);
			LoadObject(LoadObj_Data1, 1, LostWorld_SkyBox_Load);

			LevelObjTexlists[0] = &OBJ_RUIN_TEXLIST;
			LoadPVM("OBJ_RUIN", &OBJ_RUIN_TEXLIST);
			LoadPVM("OBJ_RUIN2", &OBJ_RUIN2_TEXLIST);
			LoadPVM("E_SNAKE", &E_SNAKE_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			InitializeSoundManager();
			PlayMusic(MusicIDs_lstwrld1);
			SoundManager_Delete2();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

			a1->Data1->Action = 1;
			a1->DeleteSub = HangCastle_Delete;

			if (CurrentAct == 0) {
				CurrentLevelTexlist = &RUIN01_TEXLIST;
				CurrentLandAddress = (LandTable**)0x97DAE8;
				HCFlags_Reset();

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&hang_castle_objects;
			}
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("HC", HangCastleChunks, LengthOfArray(HangCastleChunks), entity->Position);
			AnimateTextures(HangCastleAnimTexs, LengthOfArray(HangCastleAnimTexs));
			HangCastleObjects_OnFrame(entity);

			if (chunkswapped) {
				if (CurrentChunk == 2 || CurrentChunk == 6 || CurrentChunk == 8 || CurrentChunk == 10 || CurrentChunk == 11) {
					CurrentSong = MusicIDs_lstwrld2;
					CurrentStageLight_Direction.y = 1;
					LastSong = MusicIDs_lstwrld2;
				}
				else {
					CurrentSong = MusicIDs_lstwrld1;
					CurrentStageLight_Direction.y = -1;
					LastSong = MusicIDs_lstwrld1;
				}
			}

			chunkswapped = false;

			break;
		}
	}
}

void HangCastle_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("RUIN01", "hang-castle");
	ReplaceBIN("SET0700S", "hang-castle-set");
	ReplaceBIN("CAM0700S", "heroes-cam");
	ReplaceDAT("LOST_WORLD_BANK01", "MYSTIC_BANK");
	ReplaceADX("lstwrld1", "hang-castle");
	ReplaceADX("lstwrld2", "hang-castle-alt");
	ReplaceBIN("PL_70B", "hang-castle-shaders");

	helperFunctions.RegisterStartPosition(Characters_Sonic, HangCastle_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Tails, HangCastle_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, HangCastle_StartPositions[0]);
	helperFunctions.RegisterPathList(HangCastlePaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 7, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 7, 0 });

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_LostWorld1[i].Maximum = -999999.0f;
		FogData_LostWorld1[i].Toggle = false;
	}

	//Prevent skybox from loading
	WriteData<4>((void*)0x0090C20C, 0x00u);

	WriteData((DeathZone**)0x2032108, HangCastleDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF54, &HangCastleHandler);

	HangCastleObjects_Init(path);
}