#include "stdafx.h"
#include "bingo-highway-paths.h"
#include "bingo-highway.h"

void CasinoSkybox(ObjectMaster *a1);

void BingoHighwayHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->DeleteSub = CasinoCommon_Delete;
		a1->Data1->Action = 1;
	}
	else {
		ChunkHandler("BH", BingoHighwayChunks, LengthOfArray(BingoHighwayChunks), entity->Position);
		AnimateTextures(BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
		CasinoCommon_OnFrame();
	}
}

void BingoHighway_Unload() {
	CP_CSNOBOB = FreeMDL(CP_CSNOBOB);
	CP_FLIPPER = FreeMDL(CP_FLIPPER);
	CP_MOVDICE = FreeMDL(CP_MOVDICE);
	CP_RURETTO = FreeMDL(CP_RURETTO);
	CP_SLDDOOR = FreeMDL(CP_SLDDOOR);
	CP_SLOTMCS = FreeMDL(CP_SLOTMCS);
	CP_DSHPANL = FreeMDL(CP_DSHPANL);
	BH_BIGCHIP = FreeMDL(BH_BIGCHIP);
	BH_BNGCHIP = FreeMDL(BH_BNGCHIP);
	BH_BNGCARD = FreeMDL(BH_BNGCARD);
	BH_TBLSIGN = FreeMDL(BH_TBLSIGN);
	CP_SKYMDLS = FreeMDL(CP_SKYMDLS);
}

void BingoHighway_Load() {
	CP_CSNOBOB = LoadObjectModel(CP_CSNOBOB, "CP_CSNOBOB");
	CP_FLIPPER = LoadObjectModel(CP_FLIPPER, "CP_FLIPPER");
	CP_MOVDICE = LoadObjectModel(CP_MOVDICE, "CP_MOVDICE");
	CP_RURETTO = LoadObjectModel(CP_RURETTO, "CP_RURETTO");
	CP_SLDDOOR = LoadObjectModel(CP_SLDDOOR, "CP_SLDDOOR");
	CP_SLOTMCS = LoadObjectModel(CP_SLOTMCS, "CP_SLOTMCS");
	CP_DSHPANL = LoadObjectModel(CP_DSHPANL, "CP_DSHPANL");
	CP_SKYMDLS = LoadObjectModel(CP_SKYMDLS, "CP_SKYMDLS");
	BH_BIGCHIP = LoadObjectModel(BH_BIGCHIP, "BH_BIGCHIP");
	BH_BNGCHIP = LoadObjectModel(BH_BNGCHIP, "BH_BNGCHIP");
	BH_BNGCARD = LoadObjectModel(BH_BNGCARD, "BH_BNGCARD");
	BH_TBLSIGN = LoadObjectModel(BH_TBLSIGN, "BH_TBLSIGN");

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

void BingoHighway_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_40B", "bingo-highway-shaders");

	helperFunctions.RegisterPathList(BingoHighwayPaths);

	LevelObjects[HeroesLevelID_BingoHighway] = BingoHighwayHandler;
	SkyboxObjects[HeroesLevelID_BingoHighway] = CasinoSkybox;

	BingoHighwayObjects_Init();
}

HeroesLevelData BingoHighwayData = { HeroesLevelID_BingoHighway, 0, 12, "bingo-highway", "BH", BingoHighway_Load, BingoHighway_Unload, BingoHighway_Init, { 7999, 2277, 472 } };