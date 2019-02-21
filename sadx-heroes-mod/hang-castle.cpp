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

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
	
	LevelHandler_Delete(a1);
}

void HangCastleHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_lstwrld1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = HangCastle_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &RUIN01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DAE8;
			HCFlags_Reset();

			if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);

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

	helperFunctions.RegisterPathList(HangCastlePaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_LostWorld1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x2032108, HangCastleDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF54, &HangCastleHandler);

	HangCastleObjects_Init(path);
}