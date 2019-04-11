#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "bingo-highway-paths.h"
#include "bingo-highway.h"

void BingoHighway_InitObjects() {
	CP_CSNOBOB = LoadMDL("CP_CSNOBOB");
	CP_FLIPPER = LoadMDL("CP_FLIPPER");
	CP_MOVDICE = LoadMDL("CP_MOVDICE");
	CP_RURETTO = LoadMDL("CP_RURETTO");
	CP_SLDDOOR = LoadMDL("CP_SLDDOOR");
	CP_SLOTMCS = LoadMDL("CP_SLOTMCS");
	CP_DSHPANL = LoadMDL("CP_DSHPANL");
	BH_BIGCHIP = LoadMDL("BH_BIGCHIP");
	BH_BNGCHIP = LoadMDL("BH_BNGCHIP");
	BH_BNGCARD = LoadMDL("BH_BNGCARD");
	BH_TBLSIGN = LoadMDL("BH_TBLSIGN");

	BINGONB[0] = BH_BNGCHIP->getmodel()->basicdxmodel;
	BINGONB[1] = BH_BNGCHIP->getmodel()->child->basicdxmodel;
	BINGONB[2] = BH_BNGCHIP->getmodel()->child->child->basicdxmodel;
	BINGONB[3] = BH_BNGCHIP->getmodel()->child->child->child->basicdxmodel;
	BINGONB[4] = BH_BNGCHIP->getmodel()->child->child->child->child->basicdxmodel;
	BINGONB[5] = BH_BNGCHIP->getmodel()->child->child->child->child->child->basicdxmodel;
	BINGONB[6] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->basicdxmodel;
	BINGONB[7] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->child->basicdxmodel;
	BINGONB[8] = BH_BNGCHIP->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel;

	BINGOOBJLIST[0] = CP_FLIPPER->getmodel()->basicdxmodel;
	BINGOOBJLIST[1] = CP_FLIPPER->getmodel()->child->basicdxmodel;
	BINGOOBJLIST[2] = CP_CSNOBOB->getmodel()->basicdxmodel;
	BINGOOBJLIST[3] = CP_CSNOBOB->getmodel()->child->basicdxmodel;
	BINGOOBJLIST[4] = CP_SLOTMCS->getmodel()->basicdxmodel;
	BINGOOBJLIST[5] = CP_SLOTMCS->getmodel()->child->basicdxmodel;
	BINGOOBJLIST[6] = CP_SLDDOOR->getmodel()->basicdxmodel;
	BINGOOBJLIST[7] = CP_SLDDOOR->getmodel()->child->basicdxmodel;
	BINGOOBJLIST[8] = CP_SLDDOOR->getmodel()->child->child->basicdxmodel;
	BINGOOBJLIST[9] = CP_SLDDOOR->getmodel()->child->child->child->basicdxmodel;
	BINGOOBJLIST[10] = BH_BNGCARD->getmodel()->basicdxmodel;

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

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &HIGHWAY01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA88;

			if (entity->Position.z > -3440 && entity->Position.y > 370) LoadLevelFile("BH", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("BH", BingoHighwayChunks, LengthOfArray(BingoHighwayChunks), entity->Position);
			AnimateObjectsTextures(BINGOOBJLIST, LengthOfArray(BINGOOBJLIST), BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
			AnimateTextures(BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
			CasinoCommon_OnFrame();

			if (anim % 4 == 0) {
				CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
				CurrentLandTable->Col[0].Model->pos[1] = entity->Position.y;
				CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z;
			}

			break;
		}
	}
}

void BingoHighway_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("HIGHWAY01", "bingo-highway");
	ReplaceBIN("SET0400S", "bingo-highway-set");
	ReplaceBIN("SET0400M", "bingo-highway-set");
	ReplaceBIN("CAM0400S", "heroes-cam");
	ReplaceDAT("HIGHWAY_BANK01", "CASINO_BANK");
	ReplaceADX("highway1", "bingo-highway");
	ReplaceBIN("PL_40B", "bingo-highway-shaders");

	helperFunctions.RegisterPathList(BingoHighwayPaths);

	DeathZoneList[HeroesLevelID_BingoHighway] = nullptr;

	LevelObjects[HeroesLevelID_BingoHighway] = BingoHighwayHandler;

	BingoHighwayObjects_Init(path);
}