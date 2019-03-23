#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "mystic-mansion-paths.h"
#include "mystic-mansion-deathzones.h"
#include "mystic-mansion.h"

int musicid;

void MysticMansion_MusicHandler() {
	uint8_t nb;

	switch (CurrentChunk) {
	case 1:
	case 2:
	case 3: nb = 2; break;
	case 4: nb = 4; break;
	case 5: 
	case 6:
	case 7: nb = 2; break;
	case 8: 
	case 9: nb = 5; break;
	case 10:
	case 11: nb = 3; break;
	case 12: 
	case 13: nb = 5; break;
	case 14: 
	case 15:
	case 16:
	case 17: 
	case 18: nb = 3; break;
	}

	if (CurrentChunk == 16 && EntityData1Ptrs[0]->Position.x > 28500) nb = 2;

	CurrentSong = musicid + nb - 1;
}

void MysticMansion_InitObjects() {
	MM_SKELFAN = LoadMDL("MM_SKELFAN");
	MM_SPHERE1 = LoadMDL("MM_SPHERE1");
	MM_SPHERE2 = LoadMDL("MM_SPHERE2");
	MM_MYSTCAR = LoadMDL("MM_MYSTCAR");
	MM_MOVPLAT = LoadMDL("MM_MOVPLAT");
	MM_MYSDOOR = LoadMDL("MM_MYSDOOR");
	MM_MYSWALL = LoadMDL("MM_MYSWALL");
	MM_TORCHES = LoadMDL("MM_TORCHES");
	HC_SPKWARP = LoadMDL("HC_SPKWARP");
	HC_HFLAMES = LoadMDL("HC_HFLAMES");
	
	MMMODELLIST[0] = MM_SPHERE1->getmodel()->basicdxmodel;
	MMMODELLIST[1] = MM_SPHERE1->getmodel()->basicdxmodel;
	MMMODELLIST[2] = HC_HFLAMES->getmodel()->basicdxmodel;
	MMMODELLIST[3] = HC_HFLAMES->getmodel()->child->basicdxmodel;
	MysticMansion_UVShift[0].List = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[1].List = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[0].Size = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	MysticMansion_UVShift[1].Size = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
}

void MysticMansion_Delete(ObjectMaster * a1) {
	FreeMDL(MM_SPHERE1);
	FreeMDL(MM_SPHERE2);
	FreeMDL(MM_SKELFAN);
	FreeMDL(MM_MYSTCAR);
	FreeMDL(MM_MOVPLAT);
	FreeMDL(MM_MYSDOOR);
	FreeMDL(MM_MYSWALL);
	FreeMDL(MM_TORCHES);
	FreeMDL(HC_SPKWARP);
	FreeMDL(HC_HFLAMES);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}

	LevelHandler_Delete(a1);
}

void MysticMansionHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();

		if (musicid) PlayMusic((MusicIDs)musicid);
		else PlayMusic(MusicIDs_finaleg1);

		SoundManager_Delete2();

		flamecount = 0;
		for (uint16_t i = 0; i < SETTable_Count; ++i) {
			if (CurrentSetFile[i].ObjectType == 53) flamecount += 1;
		}

		a1->Data1->Action = 1;
		a1->DeleteSub = MysticMansion_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = (TexList*)0x1B98518;
			CurrentLandAddress = (LandTable**)0x97DB48;
				
			MysticMansion_InitObjects();

			if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("MM", MysticMansionChunks, LengthOfArray(MysticMansionChunks), entity->Position);
			AnimateTextures(MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateObjectsTextures(MMMODELLIST, 4, MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateUV(MysticMansion_UVShift, LengthOfArray(MysticMansion_UVShift));
			
			if (CurrentChunk > 1 && !FreeCam) {
				FreeCam = 1;
				SetCameraMode_(1);
			}

			if (CurrentChunk == 1 && EntityData1Ptrs[0]->Position.z < -1240 && EntityData1Ptrs[0]->Position.y < 198) SetCameraMode_(1);
			else SetCameraMode_(FreeCam);

			++a1->Data1->InvulnerableTime;
			if (musicid && a1->Data1->InvulnerableTime == 960) {
				MysticMansion_MusicHandler();
				a1->Data1->InvulnerableTime = 0;
			}

			chunkswapped = false;
			
			break;
		}
	}
}

void MysticMansion_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("FINALEGG1", "mystic-mansion");
	ReplaceBIN("SET1000S", "mystic-mansion-set");
	ReplaceBIN("SET1000M", "mystic-mansion-set-tails");
	ReplaceBIN("SET1000K", "mystic-mansion-set-knux");
	ReplaceBIN("CAM1000S", "mystic-mansion-cam");
	ReplaceDAT("FINAL_EGG_BANK01", "MYSTIC_BANK");
	ReplaceADX("finaleg1", "mystic-mansion");
	ReplaceBIN("PL_A0B", "mystic-mansion-shaders");

	if (helperFunctions.Version >= 9 && !NoMysticMusic) {
		MoveADX("mystic-mansion-a");
		MoveADX("mystic-mansion-b");
		MoveADX("mystic-mansion-c");
		MoveADX("mystic-mansion-d");
		MoveADX("mystic-mansion-e");
		MoveADX("mystic-mansion-f");

		musicid = helperFunctions.RegisterMusicFile(MysticMansionMusics[0]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[1]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[2]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[3]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[4]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[5]);
	}

	helperFunctions.RegisterPathList(MysticMansionPaths);
	
	WriteData((DeathZone**)0x1A49218, MysticMansionDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF60, &MysticMansionHandler);

	MysticMansionObjects_Init();
}