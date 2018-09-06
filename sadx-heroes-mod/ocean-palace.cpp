#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "ocean-palace-paths.h"
#include "ocean-palace.h"

static int finsstate = 0;
static uint8_t rocktrigger = 0;
static uint8_t rockstate = 0;
static float rocktimer = 0;
static uint8_t rockstate2 = 0;
static float rocktimer2 = 0;
static uint8_t rockstate3 = 0;
static float rocktimer3 = 0;

void SHSuns_Init(ObjectMaster * a1);

void InitSomeAnim() {
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

void GiantRockHandler() {
	if (IsPlayerInsideSphere(&OceanPalaceTriggers[1], 45.0f) == true) InitSomeAnim();

	if (IsPlayerInsideSphere(&OceanPalaceTriggers[0], 45.0f) == true) {
		rocktrigger = 1;
		InitSomeAnim();
		FreeCam = 0;
	}

	if (ocean_palace_objects_common[2].soipos.z < -44954) rocktrigger = 2;
	if (ocean_palace_objects_common[2].soipos.z < -46553) rocktrigger = 3;

	if (rocktrigger > 0) {
		if (IsPlayerInsideSphere(&ocean_palace_objects_common[2].soipos, 120.0f) == true || IsPlayerInsideSphere(&ocean_palace_objects_common[3].soipos, 120.0f) == true || IsPlayerInsideSphere(&ocean_palace_objects_common[4].soipos, 120.0f) == true) GameState = 7;

		ocean_palace_objects_common[2].soirot[0] = ocean_palace_objects_common[2].soirot[0] + 1500;
		if (rocktimer > 1) { rocktimer = 0; rockstate++; }

		if (rockstate < LengthOfArray(OP_ROCK1) - 1) {
			rocktimer = rocktimer + (OP_ROCKS[0].totaldist / OP_ROCK1[rockstate].pathdist) / OP_ROCKS[0].totaldist * 8;
			if (OP_ROCK1[rockstate].pathrot[1] != 0) {
				ocean_palace_objects_common[2].soirot[1] = OP_ROCK1[rockstate].pathrot[1];
			}
			ocean_palace_objects_common[2].soipos.x = (OP_ROCK1[rockstate + 1].pathpos.x - OP_ROCK1[rockstate].pathpos.x) * rocktimer + OP_ROCK1[rockstate].pathpos.x;
			ocean_palace_objects_common[2].soipos.y = (OP_ROCK1[rockstate + 1].pathpos.y - OP_ROCK1[rockstate].pathpos.y) * rocktimer + OP_ROCK1[rockstate].pathpos.y;
			ocean_palace_objects_common[2].soipos.z = (OP_ROCK1[rockstate + 1].pathpos.z - OP_ROCK1[rockstate].pathpos.z) * rocktimer + OP_ROCK1[rockstate].pathpos.z;
		}
		if (rocktrigger > 1) {
			ocean_palace_objects_common[3].soirot[0] = ocean_palace_objects_common[3].soirot[0] + 1500;
			if (rocktimer2 > 1) { rocktimer2 = 0; rockstate2++; }
			if (rockstate2 < LengthOfArray(OP_ROCK2) - 1) {
				rocktimer2 = rocktimer2 + (OP_ROCKS[0].totaldist / OP_ROCK2[rockstate2].pathdist) / OP_ROCKS[0].totaldist * 8;
				if (OP_ROCK2[rockstate2].pathrot[1] != 0) {
					ocean_palace_objects_common[3].soirot[1] = OP_ROCK2[rockstate2].pathrot[1];
				}
				ocean_palace_objects_common[3].soipos.x = (OP_ROCK2[rockstate2 + 1].pathpos.x - OP_ROCK2[rockstate2].pathpos.x) * rocktimer2 + OP_ROCK2[rockstate2].pathpos.x;
				ocean_palace_objects_common[3].soipos.y = (OP_ROCK2[rockstate2 + 1].pathpos.y - OP_ROCK2[rockstate2].pathpos.y) * rocktimer2 + OP_ROCK2[rockstate2].pathpos.y;
				ocean_palace_objects_common[3].soipos.z = (OP_ROCK2[rockstate2 + 1].pathpos.z - OP_ROCK2[rockstate2].pathpos.z) * rocktimer2 + OP_ROCK2[rockstate2].pathpos.z;
			}
			if (rocktrigger > 2) {
				ocean_palace_objects_common[4].soirot[0] = ocean_palace_objects_common[4].soirot[0] + 1500;
				if (rocktimer3 > 1) { rocktimer3 = 0; rockstate3++; }
				if (rockstate3 < LengthOfArray(OP_ROCK3) - 1) {
					rocktimer3 = rocktimer3 + (OP_ROCKS[0].totaldist / OP_ROCK3[rockstate3].pathdist) / OP_ROCKS[0].totaldist * 8;
					if (OP_ROCK3[rockstate3].pathrot[1] != 0) {
						ocean_palace_objects_common[4].soirot[1] = OP_ROCK3[rockstate3].pathrot[1];
					}
					ocean_palace_objects_common[4].soipos.x = (OP_ROCK3[rockstate3 + 1].pathpos.x - OP_ROCK3[rockstate3].pathpos.x) * rocktimer3 + OP_ROCK3[rockstate3].pathpos.x;
					ocean_palace_objects_common[4].soipos.y = (OP_ROCK3[rockstate3 + 1].pathpos.y - OP_ROCK3[rockstate3].pathpos.y) * rocktimer3 + OP_ROCK3[rockstate3].pathpos.y;
					ocean_palace_objects_common[4].soipos.z = (OP_ROCK3[rockstate3 + 1].pathpos.z - OP_ROCK3[rockstate3].pathpos.z) * rocktimer3 + OP_ROCK3[rockstate3].pathpos.z;
				}
			}
		}
	}
}

void FinsHandler() {
	if (CurrentChunk == 5) {
		finsstate++;
		if (finsstate > 300) finsstate = 0;
		for (Int i = 9; i < 41; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].soirot[1] -= 100;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].soirot[1] += 50.5f;
			}
		}
		for (Int i = 41; i < 73; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].soirot[1] += 100;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].soirot[1] -= 50.5f;
			}
		}
		for (Int i = 9; i < 73; ++i) {
			if (finsstate <= 100) {
				ocean_palace_objects_common[i].soirot[0] += 10;
			}
			if (finsstate > 100) {
				ocean_palace_objects_common[i].soirot[0] -= 5;
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

	ocean_palace_objects_common[2].soipos = { -8200.3408f, 2030, -39259.3f };
	ocean_palace_objects_common[3].soipos = { -8274.9741f, 1363, -44980.36f };
	ocean_palace_objects_common[4].soipos = { -6785.288f, 963, -46514.48f };

	ocean_palace_objects_common[2].soirot[1] = 32768;
	ocean_palace_objects_common[3].soirot[1] = 24735;
	ocean_palace_objects_common[4].soirot[1] = 32768;

	LevelHandler_Delete(a1);
}

void OceanPalaceHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		MovePlayerToStartPoint(entity);
		camerahax_b();
		PlaySound2(946, (void *)0xFFFFFFFF, 8, 0);
		PlaySound2(948, (void *)0xFFFFFFFF, 8, 0);
		InitializeSoundManager();
		PlayMusic(MusicIDs_WindyValleyWindyHill);
		SoundManager_Delete2();
		CurrentLevelTexlist = &WINDY01_TEXLIST;
		a1->Data1->Action = 1;
		a1->DeleteSub = OceanPalace_delete;

		if (CurrentAct == 0) {
			ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
			modelhandler->Data1->LoopData = (Loop*)&ocean_palace_objects;
			matlist_waterfall[0].attr_texId = 66;
			LoadObject(LoadObj_Data1, 3, SHSuns_Init); //load the sun
		}
		else {
			matlist_waterfall[0].attr_texId = 45;
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
		}
	}
}

void OceanPalace_Init(const char *path, const HelperFunctions &helperFunctions) {
	//Initiliazing files
	ReplacePVM("WINDY01", "ocean-palace");
	ReplaceBIN("SET0200S", "ocean-palace-set");
	ReplaceBIN("SET0200M", "ocean-palace-set-tails");
	ReplaceBIN("CAM0200S", "heroes-cam");
	ReplaceDAT("WINDY_VALLEY_BANK01", "HEROES_BANK");
	ReplaceADX("wndyvly1", "ocean-palace");
	ReplaceBIN("PL_20B", "heroes-shaders");

	//HelperFunctions allows our mod to not override other mods' data that also use HelperFunctions
	helperFunctions.RegisterStartPosition(Characters_Sonic, OceanPalace_StartPositions[0]); //startpos
	helperFunctions.RegisterPathList(OceanPalacePaths); //splines
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 2, 0 }); //trial menu

	//Static stuff
	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_WindyValley1[i].Maximum = -999999.0f;
		FogData_WindyValley1[i].Toggle = false;
	}

	WriteData((DeathZone**)0xBFD820, SeasideHillDeathZones);

	//Do not draw skybox
	WriteData<3>((void*)0x004DD71A, 0x90u);

	//Load the level handler
	WriteJump((void *)0x4DDB30, &OceanPalaceHandler);

	OceanPalaceObjects_Init(path);
}