#include "stdafx.h"
#include "seaside-hill-paths.h"
#include "seaside-hill-deathzones.h"
#include "seaside-hill.h"

static int slowtimer = 0;
static bool sh_trigger = true;

void SeasideHillSkybox(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DisplaySub = a1->MainSub;
		HeroesSkybox_Main(a1);
	}

	a1->Data1->Position = { 74, 11106, 425 };
	DrawLensFlare(&a1->Data1->Position);
}

void SeasideHill_OnFrame(EntityData1 * entity, CharObj2 * co2) {
	if (anim % 4 == 0) {
		if (CurrentChunk == 16) {
			if (entity->Position.z > -200) {
				SetCameraMode_(0);
				co2->AnimationThing.Index = 13;
				entity->Action = 2;
				ruin = 0;
			}

			if (entity->Position.z > -1000)
				CurrentLandTable->Col[CurrentLandTable->COLCount - 1].Flags = 1;
			if (entity->Position.z > -1994 && entity->Position.z < -1974) {
				CurrentLandTable->Col[CurrentLandTable->COLCount - 1].Flags = 0;
				SetCameraMode_(1);
			}
		}

		if (CurrentChunk == 8 || CurrentChunk == 9 || CurrentChunk == 10) {
			LevelFogData.Toggle = false;
		}
		else {
			LevelFogData.Toggle = true;
		}
	}

	if (envsounds == 1) {
		if (++slowtimer == 400) { sh_trigger = true; slowtimer = 0; }
		if (sh_trigger) {
			EntityData1 *entity = EntityData1Ptrs[0];
			if (entity != nullptr) {
				if ((entity->Position.z < -2676 && entity->Position.z > -4364)
					|| (entity->Position.z < -7228 && entity->Position.z > -8029 && entity->Position.x > -90)
					|| (entity->Position.z < -13345 && entity->Position.z > -14023) || (entity->Position.z < -14704 && entity->Position.z > -15648)) {
					PlayHeroesSound(LevelSound_Sea_Seagul); 
					sh_trigger = false; 
				}
			}
		}
	}
}

void SeasideHill_InitObjects() {
	SH_PLATFOR = LoadObjectModel("SH_PLATFOR");
	SH_WATERFS = LoadObjectModel("SH_WATERFS");
	SH_MORUINS = LoadObjectModel("SH_MORUINS");
	
	SeasideHill_UVShift[0].List = SH_WATERFS->getmodel()->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[1].List = SH_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[2].List = SH_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[3].List = SH_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].vertuv;

	SeasideHill_UVShift[0].Size = SH_WATERFS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[1].Size = SH_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[2].Size = SH_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[3].Size = SH_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;

	LoadObject(LoadObj_Data1, 3, SHWaterfalls);
}

void SeasideHill_Delete(ObjectMaster * a1) {
	FreeMDL(SH_MORUINS);
	FreeMDL(SH_POLFLAG);
	FreeMDL(SH_FLOWERS);
	FreeMDL(SH_PLATFOR);
	FreeMDL(SH_WATERFS);

	LevelHandler_Delete(a1);
}

void SeasideHillHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = SeasideHill_Delete;

		SetFog(&SeasideHill_Fog);
		SeasideHill_InitObjects();
		PlayHeroesSound(LevelSound_Sea_Seagul);

		if (CurrentAct == 0) {
			//Seaside Hill
			InitializeSoundManager();
			PlayMusic(MusicIDs_EmeraldCoastAzureBlueWorld);
			SoundManager_Delete2();

			SH_FLOWERS = LoadObjectModel("SH_FLOWERS");
			SH_POLFLAG = LoadObjectModel("SH_POLFLAG");
			LoadObject(LoadObj_Data1, 3, SHFlowers);

			CurrentLevelTexlist = &BEACH01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA28;
			matlist_waterfall[0].attr_texId = 87;

			if (entity->Position.z > -6264) LoadLevelFile("SH", 16);
		}
		else {
			//Sea Gate
			InitializeSoundManager();
			PlayMusic(MusicIDs_EmeraldCoastWindyAndRipply);
			SoundManager_Delete2();
			
			CurrentLevelTexlist = &BEACH02_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA2C;
			matlist_waterfall[0].attr_texId = 83;
		}
	}
	else {
		AnimateUV(SeasideHill_UVShift, LengthOfArray(SeasideHill_UVShift));

		switch (CurrentAct) {
		case 0:
			ChunkHandler("SH", SeasideHillChunks, LengthOfArray(SeasideHillChunks), entity->Position);
			AnimateTextures(SeasideHillAnimTexs, LengthOfArray(SeasideHillAnimTexs));
			SeasideHill_OnFrame(entity, co2);
			SeasideHillObjects_OnFrame(entity);
			break;
		case 1:
			ChunkHandler("SG", SeaGateChunks, LengthOfArray(SeaGateChunks), entity->Position);
			AnimateTextures(SeaGateAnimTexs, LengthOfArray(SeaGateAnimTexs));
			break;
		}	
	}
}

void SeaGate_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("BEACH02", "sea-gate");
	ReplaceBIN("SET0101S", "sea-gate-set-gamma");
	ReplaceBIN("SET0101A", "sea-gate-set-amy");
	ReplaceBIN("CAM0101S", "heroes-cam");
	ReplaceDAT("EMERALD_COAST_BANK01", "HEROES_BANK");
	ReplaceBIN("PL_11B", "sea-gate-shaders");

	MusicList[MusicIDs_ecoast2].Name = "sea-gate";

	helperFunctions.RegisterPathList(SeaGatePaths);

	EmeraldCoastDeathZones[1] = nullptr;
}

void SeasideHill_Init(const char *path, const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplacePVM("BEACH01", "seaside-hill");
	ReplaceBIN("SET0100S", "seaside-hill-set");
	ReplaceBIN("SET0100M", "seaside-hill-set-tails");
	ReplaceBIN("CAM0100S", "heroes-cam");
	ReplaceDAT("EMERALD_COAST_BANK01", "HEROES_BANK");
	ReplaceBIN("PL_10B", "seaside-hill-shaders");

	MusicList[MusicIDs_ecoast1].Name = "seaside-hill";
	
	helperFunctions.RegisterPathList(SeasideHillPaths); //splines

	EmeraldCoastDeathZones[0] = SeasideHillDeathZones;

	//Load the level handler
	LevelObjects[HeroesLevelID_SeasideHill] = SeasideHillHandler;
	SkyboxObjects[HeroesLevelID_SeasideHill] = SeasideHillSkybox;
	
	SeaGate_Init(path, helperFunctions);
	SeasideHillObjects_Init(path);
}