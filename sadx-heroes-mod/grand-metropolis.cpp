#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "grand-metropolis-paths.h"
#include "grand-metropolis-deathzones.h"
#include "grand-metropolis.h"

void GrandMetropolis_InitObjects() {
	GM_ADVERTS = LoadMDL("GM_ADVERTS");
	GM_FLYCARS = LoadMDL("GM_FLYCARS");
	GM_GPISTON = LoadMDL("GM_GPISTON");
	GM_GRFLUID = LoadMDL("GM_GRFLUID");
	GM_GRPLANE = LoadMDL("GM_GRPLANE");
	GM_MCLOUDS = LoadMDL("GM_MCLOUDS");
	GM_ZEPPLIN = LoadMDL("GM_ZEPPLIN");

	LoadObject(LoadObj_Data1, 3, SHSuns_Init)->Data1->Position = { 0, 12000, 5000 };;
	LoadObject(LoadObj_Data1, 3, GMPistons);
	LoadObject(LoadObj_Data1, 3, GMCars);
	LoadObject(LoadObj_Data1, 3, GMSky);
	LoadObject(LoadObj_Data1, 3, GMAds);

	GrandMetropolis_UVSHIFT[0].List = GM_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].vertuv;
	GrandMetropolis_UVSHIFT[1].List = GM_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
	GrandMetropolis_UVSHIFT[0].Size = GM_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	GrandMetropolis_UVSHIFT[1].Size = GM_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;

	GMOBJLIST[0] = GM_GRFLUID->getmodel()->basicdxmodel;
	GMOBJLIST[1] = GM_GRFLUID->getmodel()->child->basicdxmodel;
	GMOBJLIST[2] = &s03_door_energy;
}

void GrandMetropolis_Delete(ObjectMaster * a1) {
	FreeMDL(GM_ADVERTS);
	FreeMDL(GM_FLYCARS);
	FreeMDL(GM_GPISTON);
	FreeMDL(GM_GRFLUID);
	FreeMDL(GM_GRPLANE);
	FreeMDL(GM_MCLOUDS);
	FreeMDL(GM_ZEPPLIN);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
	
	LevelHandler_Delete(a1);
}

void GrandMetropolisHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_casino1);
		SoundManager_Delete2();

		if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);

		a1->Data1->Action = 1;
		a1->DeleteSub = GrandMetropolis_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &CASINO01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DB28;

			GrandMetropolis_InitObjects();

			if (entity->Position.z > 3675) LoadLevelFile("GM", 01);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("GM", GrandMetropolisChunks, LengthOfArray(GrandMetropolisChunks), entity->Position);
			AnimateTextures(GrandMetropolisAnimTexs, LengthOfArray(GrandMetropolisAnimTexs));
			AnimateObjectsTextures(GMOBJLIST, LengthOfArray(GMOBJLIST), GrandMetropolisAnimTexs, LengthOfArray(GrandMetropolisAnimTexs));
			AnimateUV(GrandMetropolis_UVSHIFT, LengthOfArray(GrandMetropolis_UVSHIFT));
			AutoPathsMovs();

			if (anim % 4 == 0) {
				if (entity->Position.z > 3675) {
					CharObj2 * co2 = CharObj2Ptrs[0];
					co2->AnimationThing.Index = 13;
					entity->Action = 2;
				}
			}

			break;
		}
	}
}

void GrandMetropolis_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("CASINO01", "grand-metropolis");
	ReplaceBIN("SET0900S", "grand-metropolis-set");
	ReplaceBIN("SET0900M", "grand-metropolis-set-tails");
	ReplaceBIN("SET0900K", "grand-metropolis-set");
	ReplaceBIN("CAM0900S", "heroes-cam");
	ReplaceDAT("CASINOPOLIS_BANK01", "HEROES_BANK");
	ReplaceADX("casino1", "grand-metropolis");
	ReplaceBIN("PL_90B", "grand-metropolis-shaders");

	helperFunctions.RegisterPathList(GrandMetropolisPaths);

	WriteData((DeathZone**)0x1D7E374, GrandMetropolisDeathZones);

	WriteJump((void *)0x5C0F40, &GrandMetropolisHandler);

	GrandMetropolisObjects_Init(path);
}