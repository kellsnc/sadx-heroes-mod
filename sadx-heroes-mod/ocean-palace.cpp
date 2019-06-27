#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "ocean-palace-paths.h"
#include "road-rock-deathzones.h"
#include "ocean-palace.h"

void OceanPalaceSkybox(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DisplaySub = a1->MainSub;
		HeroesSkybox_Main(a1);
	}
	
	if (CurrentAct == 0 && !MissedFrames) {
		DisableFog();
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		NJS_VECTOR * pos = &EntityData1Ptrs[0]->Position;
		njTranslate(nullptr, pos->x, 0, pos->z);
		DrawQueueDepthBias = -6000;
		njDrawModel_SADX(OP_SKYMDLS->getmodel()->basicdxmodel);
		njDrawModel_SADX(OP_SKYMDLS->getmodel()->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
		ToggleStageFog();

		switch (CurrentChunk) {
		case 0:
		case 1:
		case 2:
		case 3:
			a1->Data1->Position = { -7000, 3000, -900 };
			break;
		case 4:
		case 5:
			a1->Data1->Position = { -5696, 3000, -24900 };
			break;
		case 6:
			a1->Data1->Position = { -7200, 4000, -38900 };
			break;
		case 7:
		case 8:
			a1->Data1->Position = { -7000, 4500, -42900 };
			break;
		}

		DrawLensFlare(&a1->Data1->Position);
	}
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
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = OceanPalace_delete;

		OP_WATERFS = LoadMDL("OP_WATERFS");
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
			LoadObject(LoadObj_Data1, 3, OPFins_Main);
			LoadObject(LoadObj_Data1, 3, OPBoulders);

			OP_FLOWERS = LoadMDL("OP_FLOWERS");
			OP_TURFINS = LoadMDL("OP_TURFINS");
			OP_BOULDER = LoadMDL("OP_BOULDER");
			OP_POLFLAG = LoadMDL("OP_POLFLAG");
			OP_SKYMDLS = LoadMDL("OP_SKYMDLS");

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
		}
		else {
			//Road Rock
			InitializeSoundManager();
			PlayMusic(MusicIDs_WindyValleyTornado);
			SoundManager_Delete2();

			CurrentLevelTexlist = &WINDY02_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA4C;
			matlist_waterfall[0].attr_texId = 45;

			OceanPalace_UVShift[12].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
			OceanPalace_UVShift[13].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].vertuv;

			OceanPalace_UVShift[12].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
			OceanPalace_UVShift[13].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].nbMesh * 3;

			LoadLevelFile("RR", 01);
		}
	}
	else {
		AnimateUV(OceanPalace_UVShift, LengthOfArray(OceanPalace_UVShift));

		switch (CurrentAct) {
		case 0:
			ChunkHandler("OP", OceanPalaceChunks, LengthOfArray(OceanPalaceChunks), entity->Position);
			AnimateTextures(OceanPalaceAnimTexs, LengthOfArray(OceanPalaceAnimTexs));
			break;
		case 1:
			AnimateTextures(RoadRockAnimTexs, LengthOfArray(RoadRockAnimTexs));
			break;
		}
	}
}

void RoadRock_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("WINDY02", "road-rock");
	ReplaceBIN("SET0201B", "road-rock-set-big");
	ReplaceBIN("SET0201A", "road-rock-set-amy");
	ReplaceBIN("SET0201S", "road-rock-set-gamma");
	ReplaceBIN("CAM0201S", "heroes-cam");
	ReplaceDAT("WINDY_VALLEY_BANK01", "HEROES_BANK");
	ReplaceADX("wndyvly2", "road-rock");
	ReplaceBIN("PL_21B", "road-rock-shaders");

	WindyValleyDeathZones[1] = RoadRockDeathZones;
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

	WindyValleyDeathZones[0] = SeasideHillDeathZones;

	LevelObjects[HeroesLevelID_OceanPalace] = OceanPalaceHandler;
	SkyboxObjects[HeroesLevelID_OceanPalace] = OceanPalaceSkybox;

	RoadRock_Init(path, helperFunctions);
	OceanPalaceObjects_Init(path);
}