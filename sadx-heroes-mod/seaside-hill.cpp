#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "seaside-hill-paths.h"
#include "seaside-hill-deathzones.h"
#include "seaside-hill.h"

static int slowtimer = 0;
static uint8_t sh_trigger = 1;

void SHSuns_Init(ObjectMaster * a1);

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
				if (entity->Position.z > -801) { PlaySound(44, 0, 0, 0); sh_trigger = 0; }
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
		MovePlayerToStartPoint(entity);
		camerahax_b();
		PlaySound2(946, (void *)0xFFFFFFFF, 8, 0);
		PlaySound2(948, (void *)0xFFFFFFFF, 8, 0);
		InitializeSoundManager();
		PlayMusic(MusicIDs_EmeraldCoastAzureBlueWorld);
		SoundManager_Delete2();
		CurrentLevelTexlist = &BEACH01_TEXLIST;
		a1->Data1->Action = 1;
		a1->DeleteSub = LevelHandler_Delete;

		if (CurrentAct == 0) {
			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&seaside_hill_objects;
			matlist_waterfall[0].attr_texId = 87;
			LoadObject(LoadObj_Data1, 3, SHSuns_Init); //load the sun
		}
		else {
			matlist_waterfall[0].attr_texId = 83;
		}

		if (entity->Position.z > -6264) {
			switch (CurrentAct) {
			case 0: LoadLevelFile("SH", 16); break;
			case 2: LoadLevelFile("SG", 01); break;
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
		}
	}
}

void SeasideHill_Init(const char *path, const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplacePVM("BEACH01", "seaside-hill");
	ReplaceBIN("SET0100S", "seaside-hill-set");
	ReplaceBIN("SET0100M", "seaside-hill-set-tails");
	ReplaceBIN("CAM0100S", "heroes-cam");
	ReplaceDAT("EMERALD_COAST_BANK01", "HEROES_BANK");
	ReplaceADX("ecoast1", "seaside-hill");
	ReplaceBIN("PL_10B", "heroes-shaders");

	//HelperFunctions allows our mod to not override other mods' data that also use HelperFunctions
	helperFunctions.RegisterStartPosition(Characters_Sonic, SeasideHill_StartPositions[0]); //startpos
	helperFunctions.RegisterPathList(SeasideHillPaths); //splines
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 1, 0 }); //trial menu

	//Static stuff
	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_EmeraldCoast1[i].Maximum = -999999.0f;
		FogData_EmeraldCoast1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x102F8E8, SeasideHillDeathZones);

	//Do not draw ocean
	WriteData<5>((void*)0x005011B7, 0x90u);

	//Do not draw skybox
	WriteData<5>((void*)0x00408591, 0x90u);

	//Do not draw suns
	WriteData<5>((void*)0x004F6EC7, 0x90u);
	WriteData<5>((void*)0x004F6EE9, 0x90u);

	//Load the level handler
	WriteJump((void *)0x4F6D60, &SeasideHillHandler);

	SeasideHillObjects_Init(path);
}