#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "seaside-hill-paths.h"
#include "seaside-hill-deathzones.h"
#include "seaside-hill.h"

ObjectFunc(EmeraldCoast_SkyBox_Load, 0x4F7230);

void SeasideHillObjects_Init(const char *path);
void SeasideHillObjects_OnFrame(EntityData1 * entity);
void SHSuns_Init(ObjectMaster * a1);

extern SOI_LISTS seaside_hill_objects[];
extern SOI_LISTS sea_gate_objects[];
extern float ruin;

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

void SeasideHillHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct == 2) {
			//if act 2, load back Emerald Coast
			LoadObject(LoadObj_Data1, 1, EmeraldCoast_SkyBox_Load);
			Obj_EmeraldCoast_InitOceanData();
			ObjectMaster * water = LoadObject(LoadObj_Data1, 6, (ObjectFuncPtr)0x4F7A00);
			water->Data1->Action = 3;
			LoadObject((LoadObj)(LoadObj_UnknownB | LoadObj_Data1), 1, (ObjectFuncPtr)0x4F75D0);
			LoadObject((LoadObj)(LoadObj_UnknownB | LoadObj_Data1), 1, (ObjectFuncPtr)0x4F7550);
			LoadObject((LoadObj)(LoadObj_UnknownB | LoadObj_Data1), 1, (ObjectFuncPtr)0x4F7640);
			PlayMusic(MusicIDs_EmeraldCoastBigFishin);

			LoadPVM("BEACH_SEA", &BEACH_SEA_TEXLIST);
			LoadPVM("BG_BEACH", &BG_BEACH_TEXLIST);
			LoadPVM("MECHA", &MECHA_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

			PlaySound(44, 0, 0, 0);
			InitializeSoundManager();
			PlayMusic(MusicIDs_EmeraldCoastAzureBlueWorld);
			SoundManager_Delete2();

			LoadObject(LoadObj_Data1, 3, SHSuns_Init); //load the sun

			a1->Data1->Action = 1;
			a1->DeleteSub = LevelHandler_Delete;

			if (CurrentAct == 0) {
				//Seaside Hill
				CurrentLevelTexlist = &BEACH01_TEXLIST;
				CurrentLandAddress = (LandTable**)0x97DA28;
				matlist_waterfall[0].attr_texId = 87;

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&seaside_hill_objects;

				if (entity->Position.z > -6264) LoadLevelFile("SH", 16);
			}
			else {
				//Sea Gate
				CurrentLevelTexlist = &BEACH02_TEXLIST;
				CurrentLandAddress = (LandTable**)0x97DA2C;
				matlist_waterfall[0].attr_texId = 83;

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&sea_gate_objects;

				if (entity->Position.z > -6264) LoadLevelFile("SG", 01);
			}
		}
	}
	else {
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

	helperFunctions.RegisterStartPosition(Characters_Sonic, SeasideHill_StartPositions[1]);
	helperFunctions.RegisterStartPosition(Characters_Amy, SeasideHill_StartPositions[1]);
	helperFunctions.RegisterStartPosition(Characters_Gamma, SeasideHill_StartPositions[1]);
	helperFunctions.RegisterPathList(SeaGatePaths);
	helperFunctions.RegisterTrialLevel(Characters_Amy, { 1, 1 });
	helperFunctions.RegisterTrialLevel(Characters_Gamma, { 1, 1 });
	SaveFile.LevelClear[(Characters_Amy * 43) + LevelIDs_EmeraldCoast] = 1;
	SaveFile.LevelClear[(Characters_Gamma * 43) + LevelIDs_EmeraldCoast] = 1;

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_EmeraldCoast2[i].Maximum = -999999.0f;
		FogData_EmeraldCoast2[i].Toggle = false;
	}

	WriteData((DeathZone**)0x102F8EC, SeaGateDeathZones);

	//Do not draw ocean
	WriteData<5>((void*)0x004F7A46, 0x09);
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

	//HelperFunctions allows our mod to not override other mods' data that also use HelperFunctions
	helperFunctions.RegisterStartPosition(Characters_Sonic, SeasideHill_StartPositions[0]); //startpos
	helperFunctions.RegisterStartPosition(Characters_Tails, SeasideHill_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, SeasideHill_StartPositions[0]);
	helperFunctions.RegisterPathList(SeasideHillPaths); //splines
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 1, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 1, 0 });
	SaveFile.LevelClear[(Characters_Tails * 43) + LevelIDs_EmeraldCoast] = 1;
	SaveFile.LevelClear[(Characters_Knuckles * 43) + LevelIDs_EmeraldCoast] = 1;

	//Static stuff
	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_EmeraldCoast1[i].Maximum = -999999.0f;
		FogData_EmeraldCoast1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x102F8E8, SeasideHillDeathZones);

	//Do not draw ocean
	WriteData<5>((void*)0x004F6A1A, 0x90u);

	//Do not draw skybox
	WriteData<4>((void*)0x0090C1F4, 0x00u);

	//Do not draw suns
	WriteData<5>((void*)0x004F6EC7, 0x90u);
	WriteData<5>((void*)0x004F6EE9, 0x90u);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF3C, SeasideHillHandler);

	SeaGate_Init(path, helperFunctions);
	SeasideHillObjects_Init(path);
}