#include "stdafx.h"
#include "bingo-highway-paths.h"
#include "bingo-highway.h"

void CasinoSkybox(ObjectMaster *a1);

void BingoHighway_InitObjects() {
	CP_CSNOBOB = LoadObjectModel("CP_CSNOBOB");
	CP_FLIPPER = LoadObjectModel("CP_FLIPPER");
	CP_MOVDICE = LoadObjectModel("CP_MOVDICE");
	CP_RURETTO = LoadObjectModel("CP_RURETTO");
	CP_SLDDOOR = LoadObjectModel("CP_SLDDOOR");
	CP_SLOTMCS = LoadObjectModel("CP_SLOTMCS");
	CP_DSHPANL = LoadObjectModel("CP_DSHPANL");
	CP_SKYMDLS = LoadObjectModel("CP_SKYMDLS");
	BH_BIGCHIP = LoadObjectModel("BH_BIGCHIP");
	BH_BNGCHIP = LoadObjectModel("BH_BNGCHIP");
	BH_BNGCARD = LoadObjectModel("BH_BNGCARD");
	BH_TBLSIGN = LoadObjectModel("BH_TBLSIGN");

	BINGONB[0] = BH_BNGCHIP->getmodel()->basicdxmodel;
	BINGONB[1] = BH_BNGCHIP->getmodel()->child->basicdxmodel;
	BINGONB[2] = BH_BNGCHIP->getmodel()->child->child->basicdxmodel;
	BINGONB[3] = BH_BNGCHIP->getmodel()->child->child->child->basicdxmodel;
	BINGONB[4] = BH_BNGCHIP->getmodel()->child->child->child->child->basicdxmodel;
	BINGONB[5] = BH_BNGCHIP->getmodel()->child->child->child->child->child->basicdxmodel;
	BINGONB[6] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->basicdxmodel;
	BINGONB[7] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->child->basicdxmodel;
	BINGONB[8] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel;

	LoadObject(LoadObj_Data1, 3, CPGlass);
	LoadObject(LoadObj_Data1, 3, BHSigns);
}

void BingoHighway_Delete(ObjectMaster * a1) {
	FreeMDL(CP_CSNOBOB);
	FreeMDL(CP_FLIPPER);
	FreeMDL(CP_MOVDICE);
	FreeMDL(CP_RURETTO);
	FreeMDL(CP_SLDDOOR);
	FreeMDL(CP_SLOTMCS);
	FreeMDL(CP_DSHPANL);
	FreeMDL(BH_BIGCHIP);
	FreeMDL(BH_BNGCHIP);
	FreeMDL(BH_BNGCARD);
	FreeMDL(BH_TBLSIGN);
	FreeMDL(CP_SKYMDLS);

	CasinoCommon_Delete(a1);
	LevelHandler_Delete(a1);
}

void BingoHighwayHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_SpeedHighwaySpeedHighway);
		SoundManager_Delete2();

		BingoHighway_InitObjects();

		a1->Data1->Action = 1;
		a1->DeleteSub = BingoHighway_Delete;

		CurrentLevelTexlist = &HIGHWAY01_TEXLIST;
		CurrentLandAddress = (LandTable**)0x97DA88;
	}
	else {
		ChunkHandler("BH", BingoHighwayChunks, LengthOfArray(BingoHighwayChunks), entity->Position);
		AnimateTextures(BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
		CasinoCommon_OnFrame();
	}
}

void BingoHighway_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("HIGHWAY01", "bingo-highway");
	ReplaceBIN("SET0400S", "bingo-highway-set");
	ReplaceBIN("SET0400M", "bingo-highway-set");
	ReplaceBIN("CAM0400S", "heroes-cam");
	ReplaceBIN("PL_40B", "bingo-highway-shaders");

	MusicList[MusicIDs_highway1].Name = "bingo-highway";
	DefaultLight(HeroesLevelID_BingoHighway);

	helperFunctions.RegisterPathList(BingoHighwayPaths);

	DeathZoneList[HeroesLevelID_BingoHighway] = nullptr;

	LevelObjects[HeroesLevelID_BingoHighway] = BingoHighwayHandler;
	SkyboxObjects[HeroesLevelID_BingoHighway] = CasinoSkybox;

	BingoHighwayObjects_Init(path);
}