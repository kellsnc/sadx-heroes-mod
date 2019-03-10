#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "seaside-hill-paths.h"
#include "seaside-hill-deathzones.h"
#include "seaside-hill.h"

void SeasideHillObjects_Init(const char *path);
void SeasideHillObjects_OnFrame(EntityData1 * entity);
void SHSuns_Init(ObjectMaster * a1);
void SHFlowers(ObjectMaster *a1);
void SHWaterfalls(ObjectMaster *a1);

#pragma region Objects Data
extern float ruin;

extern ModelInfo * SH_WATERFS;
extern ModelInfo * SH_FLOWERS;
extern ModelInfo * SH_PLATFOR;
extern ModelInfo * SH_MORUINS;
extern ModelInfo * SH_POLFLAG;

SH_UVSHIFT SeasideHill_UVShift[]{
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
};
#pragma endregion

static int slowtimer = 0;
static uint8_t sh_trigger = 1;

void SeasideHill_OnFrame(EntityData1 * entity, CharObj2 * co2) {
	if (anim % 4 == 0) {
		if (CurrentChunk == 16) {
			if (entity->Position.z > -200) {
				co2->AnimationThing.Index = 13;
				entity->Action = 2;
				ruin = 0;
			}
				
			if (entity->Position.z > -1000)
				CurrentLandTable->Col[CurrentLandTable->COLCount - 1].Flags = 1;
			if (entity->Position.z > -1994 && entity->Position.z < -1974)
				CurrentLandTable->Col[CurrentLandTable->COLCount - 1].Flags = 0;
		}
	}

	if (EnableSounds == 1) {
		if (slowtimer > 0 && slowtimer < 301) slowtimer++;
		if (slowtimer == 300) { sh_trigger = 1; slowtimer = 1; }
		if (sh_trigger == 1) {
			auto entity = EntityData1Ptrs[0];
			if (entity != nullptr) {
				if (entity->Position.z < -2676 && entity->Position.z > -4364) { PlaySound(44, 0, 0, 0); sh_trigger = 0; }
				if (entity->Position.z < -7228 && entity->Position.z > -8029 && entity->Position.x > -90) { PlaySound(44, 0, 0, 0); sh_trigger = 0; }
				if (entity->Position.z < -13345 && entity->Position.z > -14023) { PlaySound(44, 0, 0, 0); sh_trigger = 0; }
				if (entity->Position.z < -14704 && entity->Position.z > -15648) { PlaySound(44, 0, 0, 0); sh_trigger = 0; }
			}
		}
	}
}

void SeasideHill_InitObjects() {
	SH_FLOWERS = LoadMDL("SH_FLOWERS");
	SH_PLATFOR = LoadMDL("SH_PLATFOR");
	SH_WATERFS = LoadMDL("SH_WATERFS");
	SH_MORUINS = LoadMDL("SH_MORUINS");
	SH_POLFLAG = LoadMDL("SH_POLFLAG");

	SeasideHill_UVShift[0].List = SH_WATERFS->getmodel()->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[1].List = SH_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[2].List = SH_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[3].List = SH_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].vertuv;
	SeasideHill_UVShift[0].Size = SH_WATERFS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[1].Size = SH_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[2].Size = SH_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	SeasideHill_UVShift[3].Size = SH_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;

	LoadObject(LoadObj_Data1, 3, SHSuns_Init)->Data1->Position = { 74, 11106, 425 }; //load the sun
	LoadObject(LoadObj_Data1, 3, SHFlowers);
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
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	SetFog(&SeasideHill_Fog);

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = SeasideHill_Delete;

		PlaySound(44, 0, 0, 0);

		SeasideHill_InitObjects();

		if (CurrentAct == 0) {
			//Seaside Hill
			InitializeSoundManager();
			PlayMusic(MusicIDs_EmeraldCoastAzureBlueWorld);
			SoundManager_Delete2();

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

			if (entity->Position.z > -6264) LoadLevelFile("SG", 01);
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
			SeasideHillObjects_OnFrame(entity);
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
	ReplaceADX("ecoast2", "sea-gate");
	ReplaceBIN("PL_11B", "sea-gate-shaders");

	helperFunctions.RegisterPathList(SeaGatePaths);

	WriteData((DeathZone**)0x102F8EC, SeaGateDeathZones);
}

void SeasideHill_Init(const char *path, const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplacePVM("BEACH01", "seaside-hill");
	ReplaceBIN("SET0100S", "seaside-hill-set");
	ReplaceBIN("SET0100M", "seaside-hill-set-tails");
	ReplaceBIN("CAM0100S", "heroes-cam");
	ReplaceDAT("EMERALD_COAST_BANK01", "HEROES_BANK");
	ReplaceADX("ecoast1", "seaside-hill");
	ReplaceBIN("PL_10B", "seaside-hill-shaders");

	helperFunctions.RegisterPathList(SeasideHillPaths); //splines

	WriteData((DeathZone**)0x102F8E8, SeasideHillDeathZones);

	//Do not draw suns
	WriteData<5>((void*)0x004F6EC7, 0x90u);
	WriteData<5>((void*)0x004F6EE9, 0x90u);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF3C, SeasideHillHandler);

	SeaGate_Init(path, helperFunctions);
	SeasideHillObjects_Init(path);
}