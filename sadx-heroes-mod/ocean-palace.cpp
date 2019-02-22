#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "ocean-palace-paths.h"
#include "road-rock-deathzones.h"
#include "ocean-palace.h"

void OceanPalaceObjects_Init(const char *path);
void SHSuns_Init(ObjectMaster * a1);

void OPFins(ObjectMaster *a1);
void OPFlowers(ObjectMaster *a1);
void OPWaterfalls(ObjectMaster *a1);
void OPBoulders(ObjectMaster *a1);

extern DeathZone SeasideHillDeathZones[];

#pragma region Objects Data
SH_UVSHIFT OceanPalace_UVShift[]{
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
};

extern ModelInfo * OP_WATERFS;
extern ModelInfo * OP_FLOWERS;
extern ModelInfo * OP_TURFINS;
extern ModelInfo * OP_BOULDER;
extern ModelInfo * OP_POLFLAG;
#pragma endregion

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
	}
}

void OceanPalace_InitObjects() {
	OP_FLOWERS = LoadMDL("OP_FLOWERS");
	OP_TURFINS = LoadMDL("OP_TURFINS");
	OP_WATERFS = LoadMDL("OP_WATERFS");
	OP_BOULDER = LoadMDL("OP_BOULDER");
	OP_POLFLAG = LoadMDL("OP_POLFLAG");

	OceanPalace_UVShift[0].List = OP_WATERFS->getmodel()->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[1].List = OP_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[2].List = OP_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[3].List = OP_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[4].List = OP_WATERFS->getmodel()->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[5].List = OP_WATERFS->getmodel()->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[6].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[7].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[8].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[9].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[10].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[11].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[12].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
	OceanPalace_UVShift[12].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].vertuv;

	OceanPalace_UVShift[0].Size = OP_WATERFS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[1].Size = OP_WATERFS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[2].Size = OP_WATERFS->getmodel()->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[3].Size = OP_WATERFS->getmodel()->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[4].Size = OP_WATERFS->getmodel()->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[5].Size = OP_WATERFS->getmodel()->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[6].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[7].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[8].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[9].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[10].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[11].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[12].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
	OceanPalace_UVShift[12].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].nbMesh * 3;
}

void OceanPalace_delete(ObjectMaster * a1) {
	FreeMDL(OP_TURFINS);
	FreeMDL(OP_POLFLAG);
	FreeMDL(OP_FLOWERS);
	FreeMDL(OP_BOULDER);
	FreeMDL(OP_WATERFS);

	LevelHandler_Delete(a1);
}

void OceanPalaceHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = OceanPalace_delete;

		OceanPalace_InitObjects();

		LoadObject(LoadObj_Data1, 3, OPWaterfalls);

		if (CurrentAct == 0) {
			//Ocean Palace
			PlaySound(44, 0, 0, 0);
			InitializeSoundManager();
			PlayMusic(MusicIDs_WindyValleyWindyHill);
			SoundManager_Delete2();

			CurrentLevelTexlist = &WINDY01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA48;
			matlist_waterfall[0].attr_texId = 66;

			LoadObject(LoadObj_Data1, 3, OPFlowers);
			LoadObject(LoadObj_Data1, 3, OPFins);
			LoadObject(LoadObj_Data1, 3, OPBoulders);
			LoadObject(LoadObj_Data1, 3, SHSuns_Init)->Data1->Position = { 74, 11106, 425 }; //load the sun

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

			LoadLevelFile("RR", 01);
		}
	}
	else {
		AnimateUV(OceanPalace_UVShift, LengthOfArray(OceanPalace_UVShift));

		switch (CurrentAct) {
		case 0:
			ChunkHandler("OP", OceanPalaceChunks, LengthOfArray(OceanPalaceChunks), entity->Position);
			AnimateTextures(OceanPalaceAnimTexs, LengthOfArray(OceanPalaceAnimTexs));
			OceanPalace_OnFrame(entity, co2);
			break;
		case 1:
			AnimateTextures(RoadRockAnimTexs, LengthOfArray(RoadRockAnimTexs));
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