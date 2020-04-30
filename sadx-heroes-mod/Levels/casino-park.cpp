#include "stdafx.h"
#include "casino-park-paths.h"
#include "casino-park-deathzones.h"
#include "casino-park.h"

void CasinoSkybox(ObjectMaster *a1);

void CasinoParkHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->DeleteSub = CasinoCommon_Delete;
		a1->Data1->Action = 1;

		if (CurrentAct == 0) {
			LoadObject(LoadObj_Data1, 3, CPGlass);
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

void CasinoPark_Unload() {
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
}

void CasinoPark_Load() {
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
}

void CasinoPark_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_30B", "casino-park-shaders");

	helperFunctions.RegisterPathList(CasinoParkPaths);

	LevelObjects[HeroesLevelID_CasinoPark] = CasinoParkHandler;
	SkyboxObjects[HeroesLevelID_CasinoPark] = CasinoSkybox;
	DeathZoneList[HeroesLevelID_CasinoPark][0] = CasinoParkDeathZones;

	CasinoParkObjects_Init();
}

HeroesLevelData CasinoParkData = { HeroesLevelID_CasinoPark, 0, 11, "casino-park", "CP", CasinoPark_Load, CasinoPark_Unload, CasinoPark_Init, { -8000, 2188, 0 } };