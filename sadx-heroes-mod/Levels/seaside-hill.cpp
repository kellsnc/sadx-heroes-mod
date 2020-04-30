#include "stdafx.h"
#include "seaside-hill-paths.h"
#include "seaside-hill-deathzones.h"
#include "seaside-hill.h"

static int slowtimer = 0;
static bool sh_trigger = true;

void SeasideHillSkybox(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
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
					PlayCustomSound(LevelSound_Sea_Seagul); 
					sh_trigger = false; 
				}
			}
		}
	}
}

void SeasideHill_Load() {
	SH_PLATFOR = LoadObjectModel(SH_PLATFOR, "SH_PLATFOR");
	SH_WATERFS = LoadObjectModel(SH_WATERFS, "SH_WATERFS");
	SH_MORUINS = LoadObjectModel(SH_MORUINS, "SH_MORUINS");
	
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

void SeasideHill_Unload() {
	SH_MORUINS = SH_MORUINS = FreeMDL(SH_MORUINS);
	SH_POLFLAG = SH_POLFLAG = FreeMDL(SH_POLFLAG);
	SH_FLOWERS = SH_FLOWERS = FreeMDL(SH_FLOWERS);
	SH_PLATFOR = SH_PLATFOR = FreeMDL(SH_PLATFOR);
	SH_WATERFS = SH_WATERFS = FreeMDL(SH_WATERFS);
}

void SeasideHillHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;

		SetFog(&SeasideHill_Fog);
		PlayCustomSound(LevelSound_Sea_Seagul);

		if (CurrentAct == 0) {
			SH_FLOWERS = LoadObjectModel(SH_FLOWERS, "SH_FLOWERS");
			SH_POLFLAG = LoadObjectModel(SH_POLFLAG, "SH_POLFLAG");
			LoadObject(LoadObj_Data1, 3, SHFlowers);

			matlist_waterfall[0].attr_texId = 87;
		}
		else {
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

void SeaGate_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_11B", "sea-gate-shaders");

	helperFunctions.RegisterPathList(SeaGatePaths);

	DeathZoneList[HeroesLevelID_SeasideHill][1] = nullptr;
}

void SeasideHill_Init(const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplaceBIN("PL_10B", "seaside-hill-shaders");
	
	helperFunctions.RegisterPathList(SeasideHillPaths); //splines

	//Load the level handler
	LevelObjects[HeroesLevelID_SeasideHill] = SeasideHillHandler;
	SkyboxObjects[HeroesLevelID_SeasideHill] = SeasideHillSkybox;
	DeathZoneList[HeroesLevelID_SeasideHill][0] = SeasideHillDeathZones;
	
	SeasideHillObjects_Init();
}

HeroesLevelData SeasideHillData = { HeroesLevelID_SeasideHill, 0, 16, "seaside-hill", "SH", SeasideHill_Load, SeasideHill_Unload, SeasideHill_Init, { 0, 6.800581f, 5.217285f } };
HeroesLevelData SeaGateData = { HeroesLevelID_SeasideHill, 1, LengthOfArray(SeaGateChunks), "sea-gate", "SG", SeasideHill_Load, SeasideHill_Unload, SeaGate_Init, { 6090, 30, 1000 } };