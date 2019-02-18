#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "hang-castle-paths.h"
#include "hang-castle-deathzones.h"
#include "hang-castle.h"

void HangCastleObjects_Init(const char *path);
void HangCastleObjects_OnFrame(EntityData1 * entity);
void HCFlags_Reset();

extern SOI_LISTS hang_castle_objects[];

void HangCastle_Delete(ObjectMaster * a1) {
	HCFlags_Reset();
	set_shader_flags_ptr(ShaderFlags_Blend, false);
	set_blend_ptr(-1, -1);
	LevelHandler_Delete(a1);
}

void HangCastleHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
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

			set_shader_flags_ptr(ShaderFlags_Blend, true);

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&hang_castle_objects;

			if (entity->Position.z > -3111 && entity->Position.x < 8000) LoadLevelFile("HC", 01);
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
	SaveFile.LevelClear[(Characters_Tails * 43) + LevelIDs_LostWorld] = 1;
	SaveFile.LevelClear[(Characters_Knuckles * 43) + LevelIDs_LostWorld] = 1;

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_LostWorld1[i].Maximum = -999999.0f;
		FogData_LostWorld1[i].Toggle = false;
	}

	//Prevent skybox from loading
	WriteData<4>((void*)0x0090C20C, 0x00u);

	WriteData((DeathZone**)0x2032108, HangCastleDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF54, &HangCastleHandler);

	HangCastleObjects_Init(path);
}