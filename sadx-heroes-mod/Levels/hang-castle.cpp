#include "stdafx.h"
#include "hang-castle-paths.h"
#include "hang-castle-deathzones.h"
#include "hang-castle.h"

void HangCastleSkybox(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 0) {
			a1->Data1->Action = 1;
			a1->DisplaySub = a1->MainSub;
			HeroesSkybox_Main(a1);
		}
		
		if (!CurrentChunk) return;

		DisableFog();
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		NJS_VECTOR * pos = &EntityData1Ptrs[0]->Position;
		
		switch (CurrentChunk) {
		case 1: case 3: case 4: case 5: case 7: case 9: case 12:
			a1->Data1->Object = HC_SKYMDLS->getmodel(); break;
			njTranslate(nullptr, pos->x, 4000, pos->z + 8500);
		case 2: case 6: case 8: case 10:
			a1->Data1->Object = HC_SKYMDLS->getmodel()->child; break;
			njTranslate(nullptr, pos->x + 8500, 4000, pos->z);
		case 11:
			njTranslate(nullptr, 4000, 4000, 0);
			a1->Data1->Object = HC_SKYMDLS->getmodel()->child->child; break;
		}

		DrawQueueDepthBias = -6000;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);

		for (int i = 0; i < LengthOfArray(HangCastle_Clouds); ++i) {
			SOI_LIST item = HangCastle_Clouds[i];

			if (CurrentChunk == item.Chunk) {
				njPushMatrix(0);
				njTranslate(nullptr, 4000, 4000, 0);
				njTranslateV(0, &item.Position);
				if (item.Position.x > 8000) njRotateZ(0, 0x8000);

				switch (item.Chunk) {
				case 1: a1->Data1->Object = HC_MCLOUDS->getmodel(); break;
				case 2: case 3: a1->Data1->Object = HC_MCLOUDS->getmodel()->child; break;
				case 4: a1->Data1->Object = HC_MCLOUDS->getmodel()->child->child; break;
				case 5: case 6: a1->Data1->Object = HC_MCLOUDS->getmodel()->child->child->child; break;
				case 7: case 8: a1->Data1->Object = HC_MCLOUDS->getmodel()->child->child->child->child; break;
				case 9: case 10: a1->Data1->Object = HC_MCLOUDS->getmodel()->child->child->child->child->child; break;
				}

				DrawModel_Queue(a1->Data1->Object->basicdxmodel, QueuedModelFlagsB_EnableZWrite);
				njPopMatrix(1u);
			}
		}

		ToggleStageFog();
	}
}

void HangCastle_InitObjects() {
	HC_SPKWARP = LoadObjectModel(HC_SPKWARP, "HC_SPKWARP");
	HC_HFLAMES = LoadObjectModel(HC_HFLAMES, "HC_HFLAMES");
	HC_HCBLADE = LoadObjectModel(HC_HCBLADE, "HC_HCBLADE");
	HC_HPLANTA = LoadObjectModel(HC_HPLANTA, "HC_HPLANTA");
	HC_HPLANTB = LoadObjectModel(HC_HPLANTB, "HC_HPLANTB");
	HC_POLFLAG = LoadObjectModel(HC_POLFLAG, "HC_POLFLAG");
	HC_SPDSIGN = LoadObjectModel(HC_SPDSIGN, "HC_SPDSIGN");
	HC_SKYMDLS = LoadObjectModel(HC_SKYMDLS, "HC_SKYMDLS");
	HC_MCLOUDS = LoadObjectModel(HC_MCLOUDS, "HC_MCLOUDS");

	HCMODELLIST[0] = HC_HFLAMES->getmodel()->basicdxmodel;
	HCMODELLIST[1] = HC_HFLAMES->getmodel()->child->basicdxmodel;

	LoadObject(LoadObj_Data1, 3, HCSpiders);
}

void HangCastle_Delete(ObjectMaster * a1) {
	HCFlags_Reset();

	HC_HCBLADE = FreeMDL(HC_HCBLADE);
	HC_HFLAMES = FreeMDL(HC_HFLAMES);
	HC_HPLANTA = FreeMDL(HC_HPLANTA);
	HC_HPLANTB = FreeMDL(HC_HPLANTB);
	HC_POLFLAG = FreeMDL(HC_POLFLAG);
	HC_SPDSIGN = FreeMDL(HC_SPDSIGN);
	HC_SPKDOOR = FreeMDL(HC_SPKDOOR);
	HC_SPKTREE = FreeMDL(HC_SPKTREE);
	HC_SPKWARP = FreeMDL(HC_SPKWARP);
	HC_SKYMDLS = FreeMDL(HC_SKYMDLS);
	HC_MCLOUDS = FreeMDL(HC_MCLOUDS);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}

	CasinoLightRotation_Z = 0;
	
	LevelHandler_Delete(a1);
}

void HangCastleHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_lstwrld1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = HangCastle_Delete;

		HangCastle_InitObjects();
		HCFlags_Reset();

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &RUIN01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DAE8;

			HC_SPKDOOR = LoadObjectModel(HC_SPKDOOR, "HC_SPKDOOR");
			HC_SPKTREE = LoadObjectModel(HC_SPKTREE, "HC_SPKTREE");

			HangCastle_UVShift[0].List = HC_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].vertuv;
			HangCastle_UVShift[1].List = HC_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
			HangCastle_UVShift[2].List = HC_MCLOUDS->getmodel()->child->child->basicdxmodel->meshsets[0].vertuv;
			HangCastle_UVShift[3].List = HC_MCLOUDS->getmodel()->child->child->child->basicdxmodel->meshsets[0].vertuv;
			HangCastle_UVShift[4].List = HC_MCLOUDS->getmodel()->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
			HangCastle_UVShift[5].List = HC_MCLOUDS->getmodel()->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;

			HangCastle_UVShift[0].Size = HC_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
			HangCastle_UVShift[1].Size = HC_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;
			HangCastle_UVShift[2].Size = HC_MCLOUDS->getmodel()->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
			HangCastle_UVShift[3].Size = HC_MCLOUDS->getmodel()->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
			HangCastle_UVShift[4].Size = HC_MCLOUDS->getmodel()->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
			HangCastle_UVShift[5].Size = HC_MCLOUDS->getmodel()->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;

			if (IsLantern) 
				set_shader_flags_ptr(ShaderFlags_Blend, true);
		}
	}
	else {
		HCFlags_Animate();

		switch (CurrentAct) {
		case 0:
			ChunkHandler("HC", HangCastleChunks, LengthOfArray(HangCastleChunks), entity->Position);
			AnimateTextures(HangCastleAnimTexs, LengthOfArray(HangCastleAnimTexs));
			AnimateUV(HangCastle_UVShift, LengthOfArray(HangCastle_UVShift));

			if (ChunkSwapped) {
				if (CurrentChunk == 2 || CurrentChunk == 6 || CurrentChunk == 8 || CurrentChunk == 10 || CurrentChunk == 11) {
					CurrentSong = MusicIDs_lstwrld2;
					CasinoLightRotation_Z = 0x8000;
					LastSong = MusicIDs_lstwrld2;
				}
				else {
					CurrentSong = MusicIDs_lstwrld1;
					CasinoLightRotation_Z = 0;
					LastSong = MusicIDs_lstwrld1;
				}
			}

			break;
		}
	}
}

void HangCastle_Init(const HelperFunctions &helperFunctions) {
	ReplacePVM("RUIN01", "hang-castle");
	ReplaceBIN("SET0700S", "hang-castle-set");
	ReplaceBIN("CAM0700S", "heroes-cam");
	ReplaceBIN("PL_70B", "hang-castle-shaders");

	MusicList[MusicIDs_lstwrld1].Name = "hang-castle";
	MusicList[MusicIDs_lstwrld2].Name = "hang-castle-alt";
	DefaultLight(HeroesLevelID_HangCastle);

	helperFunctions.RegisterPathList(HangCastlePaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_LostWorld1[i].Toggle = false;
	}

	LevelObjects[HeroesLevelID_HangCastle] = HangCastleHandler;
	SkyboxObjects[HeroesLevelID_HangCastle] = HangCastleSkybox;
	DeathZoneList[HeroesLevelID_HangCastle][0] = HangCastleDeathZones;

	HangCastleObjects_Init();
}