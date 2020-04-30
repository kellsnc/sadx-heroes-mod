#include "stdafx.h"
#include "casino-park-paths.h"
#include "casino-park-deathzones.h"
#include "casino-park.h"

void CasinoSkybox(ObjectMaster *a1);

void CasinoPark_InitObjects() {
	CP_BIGDICE = LoadObjectModel(CP_BIGDICE, "CP_BIGDICE");
	CP_CSNOBOB = LoadObjectModel(CP_CSNOBOB, "CP_CSNOBOB");
	CP_DIRSIGN = LoadObjectModel(CP_DIRSIGN, "CP_DIRSIGN");
	CP_FLIPPER = LoadObjectModel(CP_FLIPPER, "CP_FLIPPER");
	CP_MOVDICE = LoadObjectModel(CP_MOVDICE, "CP_MOVDICE");
	CP_RURETTO = LoadObjectModel(CP_RURETTO, "CP_RURETTO");
	CP_SLDDOOR = LoadObjectModel(CP_SLDDOOR, "CP_SLDDOOR");
	CP_SLOTMCS = LoadObjectModel(CP_SLOTMCS, "CP_SLOTMCS");
	CP_DSHPANL = LoadObjectModel(CP_DSHPANL, "CP_DSHPANL");
	CP_SKYMDLS = LoadObjectModel(CP_SKYMDLS, "CP_SKYMDLS");

	LoadObject(LoadObj_Data1, 3, CPGlass);
}

void CasinoPark_Delete(ObjectMaster * a1) {
	CP_BIGDICE = FreeMDL(CP_BIGDICE);
	CP_CSNOBOB = FreeMDL(CP_CSNOBOB);
	CP_DIRSIGN = FreeMDL(CP_DIRSIGN);
	CP_FLIPPER = FreeMDL(CP_FLIPPER);
	CP_MOVDICE = FreeMDL(CP_MOVDICE);
	CP_RURETTO = FreeMDL(CP_RURETTO);
	CP_SLDDOOR = FreeMDL(CP_SLDDOOR);
	CP_SLOTMCS = FreeMDL(CP_SLOTMCS);
	CP_DSHPANL = FreeMDL(CP_DSHPANL);
	CP_SKYMDLS = FreeMDL(CP_SKYMDLS);

	CasinoCommon_Delete(a1);
}

void CasinoParkHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_TwinkleParkTwinklePark);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = CasinoPark_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &TWINKLE01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA68;

			CasinoPark_InitObjects();
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("CP", CasinoParkChunks, LengthOfArray(CasinoParkChunks), entity->Position);
			AnimateTextures(CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));
			CasinoCommon_OnFrame();

			break;
		}
	}
}

void CasinoPark_Init(const HelperFunctions &helperFunctions) {
	ReplacePVM("TWINKLE01", "casino-park");
	ReplaceBIN("SET0300S", "casino-park-set");
	ReplaceBIN("CAM0300S", "heroes-cam");
	ReplaceBIN("PL_30B", "casino-park-shaders");

	MusicList[MusicIDs_twnklpk1].Name = "casino-park";

	helperFunctions.RegisterPathList(CasinoParkPaths);

	LevelObjects[HeroesLevelID_CasinoPark] = CasinoParkHandler;
	SkyboxObjects[HeroesLevelID_CasinoPark] = CasinoSkybox;
	DeathZoneList[HeroesLevelID_CasinoPark][0] = CasinoParkDeathZones;

	CasinoParkObjects_Init();
}

HeroesLevelData CasinoParkData = { HeroesLevelID_CasinoPark, 0, 11, "casino-park", "CP", nullptr, nullptr, CasinoPark_Init, { -8000, 2188, 0 } };