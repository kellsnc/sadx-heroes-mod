#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "casino-park-paths.h"
#include "casino-park-deathzones.h"
#include "casino-park.h"

void CasinoPark_InitObjects() {
	CP_BIGDICE = LoadMDL("CP_BIGDICE");
	CP_CSNOBOB = LoadMDL("CP_CSNOBOB");
	CP_DIRSIGN = LoadMDL("CP_DIRSIGN");
	CP_FLIPPER = LoadMDL("CP_FLIPPER");
	CP_MOVDICE = LoadMDL("CP_MOVDICE");
	CP_RURETTO = LoadMDL("CP_RURETTO");
	CP_SLDDOOR = LoadMDL("CP_SLDDOOR");
	CP_SLOTMCS = LoadMDL("CP_SLOTMCS");
	CP_DSHPANL = LoadMDL("CP_DSHPANL");

	CASINOOBJLIST[0] = CP_FLIPPER->getmodel()->basicdxmodel;
	CASINOOBJLIST[1] = CP_FLIPPER->getmodel()->child->basicdxmodel;
	CASINOOBJLIST[2] = CP_CSNOBOB->getmodel()->basicdxmodel;
	CASINOOBJLIST[3] = CP_CSNOBOB->getmodel()->child->basicdxmodel;
	CASINOOBJLIST[4] = CP_SLOTMCS->getmodel()->basicdxmodel;
	CASINOOBJLIST[5] = CP_SLOTMCS->getmodel()->child->basicdxmodel;
	CASINOOBJLIST[6] = CP_SLDDOOR->getmodel()->basicdxmodel;
	CASINOOBJLIST[7] = CP_SLDDOOR->getmodel()->child->basicdxmodel;
	CASINOOBJLIST[8] = CP_SLDDOOR->getmodel()->child->child->basicdxmodel;
	CASINOOBJLIST[9] = CP_SLDDOOR->getmodel()->child->child->child->basicdxmodel;
	CASINOOBJLIST[10] = CP_DIRSIGN->getmodel()->basicdxmodel;
	CASINOOBJLIST[11] = CP_DIRSIGN->getmodel()->child->basicdxmodel;

	LoadObject(LoadObj_Data1, 3, CPGlass);
}

void CasinoPark_Delete(ObjectMaster * a1) {
	FreeMDL(CP_BIGDICE);
	FreeMDL(CP_CSNOBOB);
	FreeMDL(CP_DIRSIGN);
	FreeMDL(CP_FLIPPER);
	FreeMDL(CP_MOVDICE);
	FreeMDL(CP_RURETTO);
	FreeMDL(CP_SLDDOOR);
	FreeMDL(CP_SLOTMCS);
	FreeMDL(CP_DSHPANL);

	CasinoCommon_Delete(a1);
	LevelHandler_Delete(a1);
}

void CasinoParkHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		CasinoPark_InitObjects();

		InitializeSoundManager();
		PlayMusic(MusicIDs_TwinkleParkTwinklePark);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = CasinoPark_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &TWINKLE01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DA68;

			if (entity->Position.z > -1697 && entity->Position.x > -8696) LoadLevelFile("CP", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("CP", CasinoParkChunks, LengthOfArray(CasinoParkChunks), entity->Position);
			AnimateTextures(CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));
			AnimateObjectsTextures(CASINOOBJLIST, LengthOfArray(CASINOOBJLIST), CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));
			CasinoCommon_OnFrame();

			if (anim % 4 == 0) {
				CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
				CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z;
			}

			break;
		}
	}
}

void CasinoPark_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("TWINKLE01", "casino-park");
	ReplaceBIN("SET0300S", "casino-park-set");
	ReplaceBIN("CAM0300S", "heroes-cam");
	ReplaceDAT("TWINKLE_PARK_BANK01", "CASINO_BANK");
	ReplaceADX("twnklpk1", "casino-park");
	ReplaceBIN("PL_30B", "casino-park-shaders");

	helperFunctions.RegisterPathList(CasinoParkPaths);

	WriteData((DeathZone**)0x26B3C58, CasinoParkDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF44, &CasinoParkHandler);

	CasinoParkObjects_Init(path);
}