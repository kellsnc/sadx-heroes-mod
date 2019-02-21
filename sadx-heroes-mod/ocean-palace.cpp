#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "ocean-palace-paths.h"
#include "road-rock-deathzones.h"
#include "ocean-palace.h"

void OceanPalaceObjects_Init(const char *path);
void OceanPalaceObjects_OnFrame(EntityData1 * entity);
void SHSuns_Init(ObjectMaster * a1);

extern SOI_LIST ocean_palace_objects_common[];
extern SOI_LISTS ocean_palace_objects[];
extern SOI_LISTS road_rock_objects[];
extern DeathZone SeasideHillDeathZones[];

static int finsstate = 0;
static uint8_t rocktrigger = 0;
static uint8_t rockstate = 0;
static float rocktimer = 0;
static uint8_t rockstate2 = 0;
static float rocktimer2 = 0;
static uint8_t rockstate3 = 0;
static float rocktimer3 = 0;

//Sometimes sonic skips the running animation
void OP_AnimFix() {
	auto entity = EntityData1Ptrs[0];
	CharObj2 *co2 = GetCharObj2(0);
	if (!SuperSonicFlag) {
		entity->Action = 2;
		co2->AnimationThing.Index = 13;
	}
	else {
		entity->Action = 76;
		co2->AnimationThing.Index = 137;
	}
}

//A huge mess that was supposed to be temporary but then got lazy to rewrite
//Seriously though it's horrible
void GiantRockHandler() {
	if (IsPlayerInsideSphere(&OceanPalaceTriggers[1], 45.0f) == 1) OP_AnimFix();

	if (IsPlayerInsideSphere(&OceanPalaceTriggers[0], 45.0f) == 1) {
		rocktrigger = 1;
		OP_AnimFix();
		FreeCam = 0;
	}

	if (ocean_palace_objects_common[2].Position.z < -44954) rocktrigger = 2;
	if (ocean_palace_objects_common[2].Position.z < -46553) rocktrigger = 3;

	if (rocktrigger > 0) {
		if (IsPlayerInsideSphere(&ocean_palace_objects_common[2].Position, 120.0f) == 1 || IsPlayerInsideSphere(&ocean_palace_objects_common[3].Position, 120.0f) == 1 || IsPlayerInsideSphere(&ocean_palace_objects_common[4].Position, 120.0f) == 1) GameState = 7;

		ocean_palace_objects_common[2].Rotation[0] = ocean_palace_objects_common[2].Rotation[0] + 1500;
		if (rocktimer > 1) { rocktimer = 0; rockstate++; }

		if (rockstate < LengthOfArray(OP_ROCK1) - 1) {
			rocktimer = rocktimer + (OP_ROCKS[0].totaldist / OP_ROCK1[rockstate].Distance) / OP_ROCKS[0].totaldist * 8;
			if (OP_ROCK1[rockstate].Rotation[1] != 0) {
				ocean_palace_objects_common[2].Rotation[1] = OP_ROCK1[rockstate].Rotation[1];
			}
			ocean_palace_objects_common[2].Position.x = (OP_ROCK1[rockstate + 1].Position.x - OP_ROCK1[rockstate].Position.x) * rocktimer + OP_ROCK1[rockstate].Position.x;
			ocean_palace_objects_common[2].Position.y = (OP_ROCK1[rockstate + 1].Position.y - OP_ROCK1[rockstate].Position.y) * rocktimer + OP_ROCK1[rockstate].Position.y;
			ocean_palace_objects_common[2].Position.z = (OP_ROCK1[rockstate + 1].Position.z - OP_ROCK1[rockstate].Position.z) * rocktimer + OP_ROCK1[rockstate].Position.z;
		}
		if (rocktrigger > 1) {
			ocean_palace_objects_common[3].Rotation[0] = ocean_palace_objects_common[3].Rotation[0] + 1500;
			if (rocktimer2 > 1) { rocktimer2 = 0; rockstate2++; }
			if (rockstate2 < LengthOfArray(OP_ROCK2) - 1) {
				rocktimer2 = rocktimer2 + (OP_ROCKS[0].totaldist / OP_ROCK2[rockstate2].Distance) / OP_ROCKS[0].totaldist * 8;
				if (OP_ROCK2[rockstate2].Rotation[1] != 0) {
					ocean_palace_objects_common[3].Rotation[1] = OP_ROCK2[rockstate2].Rotation[1];
				}
				ocean_palace_objects_common[3].Position.x = (OP_ROCK2[rockstate2 + 1].Position.x - OP_ROCK2[rockstate2].Position.x) * rocktimer2 + OP_ROCK2[rockstate2].Position.x;
				ocean_palace_objects_common[3].Position.y = (OP_ROCK2[rockstate2 + 1].Position.y - OP_ROCK2[rockstate2].Position.y) * rocktimer2 + OP_ROCK2[rockstate2].Position.y;
				ocean_palace_objects_common[3].Position.z = (OP_ROCK2[rockstate2 + 1].Position.z - OP_ROCK2[rockstate2].Position.z) * rocktimer2 + OP_ROCK2[rockstate2].Position.z;
			}
			if (rocktrigger > 2) {
				ocean_palace_objects_common[4].Rotation[0] = ocean_palace_objects_common[4].Rotation[0] + 1500;
				if (rocktimer3 > 1) { rocktimer3 = 0; rockstate3++; }
				if (rockstate3 < LengthOfArray(OP_ROCK3) - 1) {
					rocktimer3 = rocktimer3 + (OP_ROCKS[0].totaldist / OP_ROCK3[rockstate3].Distance) / OP_ROCKS[0].totaldist * 8;
					if (OP_ROCK3[rockstate3].Rotation[1] != 0) {
						ocean_palace_objects_common[4].Rotation[1] = OP_ROCK3[rockstate3].Rotation[1];
					}
					ocean_palace_objects_common[4].Position.x = (OP_ROCK3[rockstate3 + 1].Position.x - OP_ROCK3[rockstate3].Position.x) * rocktimer3 + OP_ROCK3[rockstate3].Position.x;
					ocean_palace_objects_common[4].Position.y = (OP_ROCK3[rockstate3 + 1].Position.y - OP_ROCK3[rockstate3].Position.y) * rocktimer3 + OP_ROCK3[rockstate3].Position.y;
					ocean_palace_objects_common[4].Position.z = (OP_ROCK3[rockstate3 + 1].Position.z - OP_ROCK3[rockstate3].Position.z) * rocktimer3 + OP_ROCK3[rockstate3].Position.z;
				}
			}
		}
	}
}

//Got lazy to make turtle fins an object so hardcode everything
void FinsHandler() {
	if (CurrentChunk == 5) {
		finsstate++;
		if (finsstate > 300) finsstate = 0;
		for (Int i = 9; i < 41; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].Rotation[1] -= 100;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].Rotation[1] += 50.5f;
			}
		}
		for (Int i = 41; i < 73; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].Rotation[1] += 100;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].Rotation[1] -= 50.5f;
			}
		}
		for (Int i = 9; i < 73; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].Rotation[0] += 10;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].Rotation[0] -= 5;
			}
		}
	}
}

void OceanPalace_OnFrame(EntityData1 * entity, CharObj2 * co2) {
	if (CurrentLandTable) {
		if (anim % 4 == 0) {
			if (entity != nullptr) {

				CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
				CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z;
				CurrentLandTable->Col[1].Model->pos[0] = entity->Position.x;
				CurrentLandTable->Col[1].Model->pos[2] = entity->Position.z;

				if (entity->Position.x > -100) {
					entity->Position = { -8771, 1303, -2819.688f };
					entity->Rotation.y = 0xBFFF;
				}
			}
		}

		FinsHandler();
		GiantRockHandler();
	}
}

void OceanPalace_delete(ObjectMaster * a1) {
	rocktrigger = 0;
	rockstate = 0;
	rocktimer = 0;
	rockstate2 = 0;
	rocktimer2 = 0;
	rockstate3 = 0;
	rocktimer3 = 0;

	ocean_palace_objects_common[2].Position = { -8200.3408f, 2030, -39259.3f };
	ocean_palace_objects_common[3].Position = { -8274.9741f, 1363, -44980.36f };
	ocean_palace_objects_common[4].Position = { -6785.288f, 963, -46514.48f };

	ocean_palace_objects_common[2].Rotation[1] = 32768;
	ocean_palace_objects_common[3].Rotation[1] = 24735;
	ocean_palace_objects_common[4].Rotation[1] = 32768;

	LevelHandler_Delete(a1);
}

void OceanPalaceHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = OceanPalace_delete;

		if (CurrentAct == 0) {
			//Ocean Palace
			PlaySound(44, 0, 0, 0);
			InitializeSoundManager();
			PlayMusic(MusicIDs_WindyValleyWindyHill);
			SoundManager_Delete2();

			CurrentLevelTexlist = &WINDY01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA48;
			matlist_waterfall[0].attr_texId = 66;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&ocean_palace_objects;
			LoadObject(LoadObj_Data1, 3, SHSuns_Init); //load the sun

			if (entity->Position.z > -10637) LoadLevelFile("OP", 01);
		}
		else {
			//Road Rock
			InitializeSoundManager();
			PlayMusic(MusicIDs_WindyValleyTornado);
			SoundManager_Delete2();

			CurrentLevelTexlist = &WINDY02_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA4C;
			matlist_waterfall[0].attr_texId = 45;

			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&road_rock_objects;
			LoadLevelFile("RR", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("OP", OceanPalaceChunks, LengthOfArray(OceanPalaceChunks), entity->Position);
			AnimateTextures(OceanPalaceAnimTexs, LengthOfArray(OceanPalaceAnimTexs));
			OceanPalace_OnFrame(entity, co2);
			OceanPalaceObjects_OnFrame(entity);
			break;
		case 1:
			AnimateTextures(RoadRockAnimTexs, LengthOfArray(RoadRockAnimTexs));
			OceanPalaceObjects_OnFrame(entity);
			break;
		}
	}
}

void RoadRock_Init(const char *path, const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplacePVM("WINDY02", "road-rock");
	ReplaceBIN("SET0201B", "road-rock-set-big");
	ReplaceBIN("SET0201A", "road-rock-set-amy");
	ReplaceBIN("SET0201S", "road-rock-set-gamma");
	ReplaceBIN("CAM0201S", "heroes-cam");
	ReplaceDAT("WINDY_VALLEY_BANK01", "HEROES_BANK");
	ReplaceADX("wndyvly2", "road-rock");
	ReplaceBIN("PL_21B", "road-rock-shaders");

	for (uint8_t i = 0; i < 3; i++) {
		FogData_WindyValley2[i].Toggle = false;
	}

	WriteData((DeathZone**)0xBFD824, RoadRockDeathZones);
}

void OceanPalace_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("WINDY01", "ocean-palace");
	ReplaceBIN("SET0200S", "ocean-palace-set");
	ReplaceBIN("SET0200M", "ocean-palace-set-tails");
	ReplaceBIN("CAM0200S", "heroes-cam");
	ReplaceDAT("WINDY_VALLEY_BANK01", "HEROES_BANK");
	ReplaceADX("wndyvly1", "ocean-palace");
	ReplaceBIN("PL_20B", "ocean-palace-shaders");

	helperFunctions.RegisterPathList(OceanPalacePaths);
	
	for (uint8_t i = 0; i < 3; i++) {
		FogData_WindyValley1[i].Toggle = false;
	}

	WriteData((DeathZone**)0xBFD820, SeasideHillDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF40, OceanPalaceHandler);

	RoadRock_Init(path, helperFunctions);
	OceanPalaceObjects_Init(path);
}