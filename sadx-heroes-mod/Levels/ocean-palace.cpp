#include "stdafx.h"
#include "ocean-palace-paths.h"
#include "road-rock-deathzones.h"
#include "ocean-palace.h"

void OceanPalaceSkybox(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			HeroesSkybox_Main(a1);
		}
		else if (CurrentAct == 0) {
			DisableFog();
			SetHeroesLeveltex();
			njPushMatrix(0);
			NJS_VECTOR* pos = &EntityData1Ptrs[0]->Position;
			njTranslate(nullptr, pos->x, 1.8f, pos->z);
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
}

void OceanPalaceHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		
		LoadObject(LoadObj_Data1, 3, OPWaterfalls);

		if (CurrentAct == 0) {
			//Ocean Palace
			PlayCustomSound(LevelSound_Sea_Seagul);
			
			LoadObject(LoadObj_Data1, 3, OPFlowers);
			LoadObject(LoadObj_Data1, 3, OPFins_Main);
			LoadObject(LoadObj_Data1, 3, OPBoulders);
		}
		else {
			LoadLevelFile("RR01", "road-rock");
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

void OceanPalace_Unload() {
	OP_TURFINS = FreeMDL(OP_TURFINS);
	OP_POLFLAG = FreeMDL(OP_POLFLAG);
	OP_FLOWERS = FreeMDL(OP_FLOWERS);
	OP_BOULDER = FreeMDL(OP_BOULDER);
	OP_WATERFS = FreeMDL(OP_WATERFS);
}

void OceanPalace_Load() {
	OP_WATERFS = LoadObjectModel(OP_WATERFS, "OP_WATERFS");

	if (CurrentAct == 0) {
		OP_FLOWERS = LoadObjectModel(OP_FLOWERS, "OP_FLOWERS");
		OP_TURFINS = LoadObjectModel(OP_TURFINS, "OP_TURFINS");
		OP_BOULDER = LoadObjectModel(OP_BOULDER, "OP_BOULDER");
		OP_POLFLAG = LoadObjectModel(OP_POLFLAG, "OP_POLFLAG");
		OP_SKYMDLS = LoadObjectModel(OP_SKYMDLS, "OP_SKYMDLS");

		matlist_waterfall[0].attr_texId = 66;

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
		matlist_waterfall[0].attr_texId = 45;

		OceanPalace_UVShift[12].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].vertuv;
		OceanPalace_UVShift[13].List = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].vertuv;

		OceanPalace_UVShift[12].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[0].nbMesh * 3;
		OceanPalace_UVShift[13].Size = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel->meshsets[1].nbMesh * 3;
	}
}

void RoadRock_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_21B", "road-rock-shaders");

	MusicList[MusicIDs_wndyvly2].Name = "road-rock";

	DeathZoneList[HeroesLevelID_OceanPalace][1] = RoadRockDeathZones;
}

void OceanPalace_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_20B", "ocean-palace-shaders");

	MusicList[MusicIDs_wndyvly1].Name = "ocean-palace";

	helperFunctions.RegisterPathList(OceanPalacePaths);

	LevelObjects[HeroesLevelID_OceanPalace] = OceanPalaceHandler;
	SkyboxObjects[HeroesLevelID_OceanPalace] = OceanPalaceSkybox;
	DeathZoneList[HeroesLevelID_OceanPalace][0] = SeasideHillDeathZones;

	OceanPalaceObjects_Init();
}

HeroesLevelData OceanPalaceData = { HeroesLevelID_OceanPalace, 0, LengthOfArray(OceanPalaceChunks), "ocean-palace", "OP", OceanPalace_Load, OceanPalace_Unload, OceanPalace_Init, { -8771, 1303, -2819.688f } };
HeroesLevelData RoadRockData = { HeroesLevelID_OceanPalace, 1, 0, "road-rock", "RR", OceanPalace_Load, OceanPalace_Unload, RoadRock_Init, { 0, 3020, 4462 } };