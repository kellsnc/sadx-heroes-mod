#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "hang-castle-paths.h"
#include "hang-castle-deathzones.h"
#include "hang-castle.h"

void HangCastle_InitObjects() {
	HC_SPKWARP = LoadMDL("HC_SPKWARP");
	HC_HFLAMES = LoadMDL("HC_HFLAMES");
	HC_HCBLADE = LoadMDL("HC_HCBLADE");
	HC_HPLANTA = LoadMDL("HC_HPLANTA");
	HC_HPLANTB = LoadMDL("HC_HPLANTB");
	HC_POLFLAG = LoadMDL("HC_POLFLAG");
	HC_SPDSIGN = LoadMDL("HC_SPDSIGN");
	HC_SPKDOOR = LoadMDL("HC_SPKDOOR");
	HC_SPKTREE = LoadMDL("HC_SPKTREE");

	HCMODELLIST[0] = HC_HFLAMES->getmodel()->basicdxmodel;
	HCMODELLIST[1] = HC_HFLAMES->getmodel()->child->basicdxmodel;

	LoadObject(LoadObj_Data1, 3, HCSpiders);
}

void HangCastle_Delete(ObjectMaster * a1) {
	HCFlags_Reset();

	FreeMDL(HC_HCBLADE);
	FreeMDL(HC_HFLAMES);
	FreeMDL(HC_HPLANTA);
	FreeMDL(HC_HPLANTB);
	FreeMDL(HC_POLFLAG);
	FreeMDL(HC_SPDSIGN);
	FreeMDL(HC_SPKDOOR);
	FreeMDL(HC_SPKTREE);
	FreeMDL(HC_SPKWARP);

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

			HangCastle_InitObjects();
			HCFlags_Reset();

			if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);

			if (entity->Position.z > -3111 && entity->Position.x < 8000) LoadLevelFile("HC", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("HC", HangCastleChunks, LengthOfArray(HangCastleChunks), entity->Position);
			AnimateTextures(HangCastleAnimTexs, LengthOfArray(HangCastleAnimTexs));
			AnimateObjectsTextures(HCMODELLIST, 2, HangCastleAnimTexs, LengthOfArray(HangCastleAnimTexs));

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

			if (CurrentChunk != 11) {
				CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
				CurrentLandTable->Col[0].Model->pos[1] = entity->Position.y;
				CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z + 8500;
			}

			HCFlags_Animate();

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