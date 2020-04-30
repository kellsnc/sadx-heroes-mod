#include "stdafx.h"
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
	MM_SKELFAN = LoadObjectModel(MM_SKELFAN, "MM_SKELFAN");
	MM_SPHERE1 = LoadObjectModel(MM_SPHERE1, "MM_SPHERE1");
	MM_SPHERE2 = LoadObjectModel(MM_SPHERE2, "MM_SPHERE2");
	MM_MYSTCAR = LoadObjectModel(MM_MYSTCAR, "MM_MYSTCAR");
	MM_MOVPLAT = LoadObjectModel(MM_MOVPLAT, "MM_MOVPLAT");
	MM_MYSDOOR = LoadObjectModel(MM_MYSDOOR, "MM_MYSDOOR");
	MM_MYSWALL = LoadObjectModel(MM_MYSWALL, "MM_MYSWALL");
	MM_TORCHES = LoadObjectModel(MM_TORCHES, "MM_TORCHES");
	HC_SPKWARP = LoadObjectModel(HC_SPKWARP, "HC_SPKWARP");
	HC_HFLAMES = LoadObjectModel(HC_HFLAMES, "HC_HFLAMES");
	
	MMMODELLIST[0] = MM_SPHERE1->getmodel()->basicdxmodel;
	MMMODELLIST[1] = MM_SPHERE2->getmodel()->basicdxmodel;
	MMMODELLIST[2] = HC_HFLAMES->getmodel()->basicdxmodel;
	MMMODELLIST[3] = HC_HFLAMES->getmodel()->child->basicdxmodel;
	MysticMansion_UVShift[0].List = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[1].List = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[0].Size = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	MysticMansion_UVShift[1].Size = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
}

void MysticMansion_Delete(ObjectMaster * a1) {
	MM_SPHERE1 = FreeMDL(MM_SPHERE1);
	MM_SPHERE2 = FreeMDL(MM_SPHERE2);
	MM_SKELFAN = FreeMDL(MM_SKELFAN);
	MM_MYSTCAR = FreeMDL(MM_MYSTCAR);
	MM_MOVPLAT = FreeMDL(MM_MOVPLAT);
	MM_MYSDOOR = FreeMDL(MM_MYSDOOR);
	MM_MYSWALL = FreeMDL(MM_MYSWALL);
	MM_TORCHES = FreeMDL(MM_TORCHES);
	HC_SPKWARP = FreeMDL(HC_SPKWARP);
	HC_HFLAMES = FreeMDL(HC_HFLAMES);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}

	LevelHandler_Delete(a1);
}

void MysticMansionHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

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
			
			LoadPVM("FINALEGG1", (TexList*)0x1B98518);
				
			MysticMansion_InitObjects();

			if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("MM", MysticMansionChunks, LengthOfArray(MysticMansionChunks), entity->Position);
			AnimateTextures(MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateUV(MysticMansion_UVShift, LengthOfArray(MysticMansion_UVShift));

			if (CurrentChunk > 1 && !FreeCam) {
				FreeCam = 1;
				SetCameraMode_(1);
			}

			WriteData((char*)0x47B39F, (char)0x02);
			
			if (CurrentChunk == 1 && EntityData1Ptrs[0]->Position.z < -1240 && EntityData1Ptrs[0]->Position.y < 198) SetCameraMode_(1);
			else SetCameraMode_(FreeCam);

			++a1->Data1->InvulnerableTime;
			if (musicid && a1->Data1->InvulnerableTime == 960) {
				MysticMansion_MusicHandler();
				a1->Data1->InvulnerableTime = 0;
			}
			
			break;
		}
	}
}

void MysticMansion_Init(const HelperFunctions &helperFunctions) {
	ReplacePVM("FINALEGG1", "mystic-mansion");
	ReplaceBIN("SET1000S", "mystic-mansion-set");
	ReplaceBIN("SET1000M", "mystic-mansion-set-tails");
	ReplaceBIN("SET1000K", "mystic-mansion-set-knux");
	ReplaceBIN("CAM1000S", "mystic-mansion-cam");
	ReplaceBIN("PL_A0B", "mystic-mansion-shaders");

	MusicList[MusicIDs_finaleg1].Name = "mystic-mansion";
	DefaultLight(HeroesLevelID_MysticMansion);

	if (helperFunctions.Version >= 9 && IsNoMysticMusicEnabled() == true) {
		musicid = helperFunctions.RegisterMusicFile(MysticMansionMusics[0]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[1]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[2]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[3]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[4]);
		helperFunctions.RegisterMusicFile(MysticMansionMusics[5]);
	}

	helperFunctions.RegisterPathList(MysticMansionPaths);

	LevelObjects[HeroesLevelID_MysticMansion] = MysticMansionHandler;
	SkyboxObjects[HeroesLevelID_MysticMansion] = HeroesSkybox_Main;
	DeathZoneList[HeroesLevelID_MysticMansion][0] = MysticMansionDeathZones;

	MysticMansionObjects_Init();
}