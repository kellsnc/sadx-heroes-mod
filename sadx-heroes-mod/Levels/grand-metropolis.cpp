#include "stdafx.h"
#include "grand-metropolis-paths.h"
#include "grand-metropolis-deathzones.h"
#include "grand-metropolis.h"

void GrandMetropolisSkybox(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DisplaySub = a1->MainSub;
		HeroesSkybox_Main(a1);
	}

	a1->Data1->Position = { 0, 12000, 5000 };
	DrawLensFlare(&a1->Data1->Position);
}

void GrandMetropolis_InitObjects() {
	GM_ADVERTS = LoadObjectModel(GM_ADVERTS, "GM_ADVERTS");
	GM_GPISTON = LoadObjectModel(GM_GPISTON, "GM_GPISTON");
	GM_GRFLUID = LoadObjectModel(GM_GRFLUID, "GM_GRFLUID");
	GM_GRPLANE = LoadObjectModel(GM_GRPLANE, "GM_GRPLANE");
	GM_MCLOUDS = LoadObjectModel(GM_MCLOUDS, "GM_MCLOUDS");
	GM_ZEPPLIN = LoadObjectModel(GM_ZEPPLIN, "GM_ZEPPLIN");

	LoadObject(LoadObj_Data1, 3, GMPistons);
	LoadObject(LoadObj_Data1, 3, GMCars);
	LoadObject(LoadObj_Data1, 3, GMSky);
	LoadObject(LoadObj_Data1, 3, GMAds);

	GrandMetropolis_UVSHIFT[0].List = GM_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].vertuv;
	GrandMetropolis_UVSHIFT[1].List = GM_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].vertuv;
	GrandMetropolis_UVSHIFT[0].Size = GM_MCLOUDS->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	GrandMetropolis_UVSHIFT[1].Size = GM_MCLOUDS->getmodel()->child->basicdxmodel->meshsets[0].nbMesh * 3;
}

void GrandMetropolis_Delete(ObjectMaster * a1) {
	GM_ADVERTS = FreeMDL(GM_ADVERTS);
	GM_FLYCARS = FreeMDL(GM_FLYCARS);
	GM_GPISTON = FreeMDL(GM_GPISTON);
	GM_GRFLUID = FreeMDL(GM_GRFLUID);
	GM_GRPLANE = FreeMDL(GM_GRPLANE);
	GM_MCLOUDS = FreeMDL(GM_MCLOUDS);
	GM_ZEPPLIN = FreeMDL(GM_ZEPPLIN);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
}

void GrandMetropolisHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_casino1);
		SoundManager_Delete2();

		if (IsLantern) 
			set_shader_flags_ptr(ShaderFlags_Blend, true);

		a1->Data1->Action = 1;
		a1->DeleteSub = GrandMetropolis_Delete;

		GM_FLYCARS = LoadObjectModel(GM_FLYCARS, "GM_FLYCARS");

		if (CurrentAct == 0) {
			CurrentLevelTexlist = &CASINO01_TEXLIST;
			CurrentLandAddress = (LandTable**)0x97DB28;

			GrandMetropolis_InitObjects();
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("GM", GrandMetropolisChunks, LengthOfArray(GrandMetropolisChunks), entity->Position);
			AnimateTextures(GrandMetropolisAnimTexs, LengthOfArray(GrandMetropolisAnimTexs));
			AnimateUV(GrandMetropolis_UVSHIFT, LengthOfArray(GrandMetropolis_UVSHIFT));
			AutoPathsMovs();

			break;
		}
	}
}

void GrandMetropolis_Init(const HelperFunctions &helperFunctions) {
	ReplacePVM("CASINO01", "grand-metropolis");
	ReplaceBIN("SET0900S", "grand-metropolis-set");
	ReplaceBIN("SET0900M", "grand-metropolis-set-tails");
	ReplaceBIN("SET0900K", "grand-metropolis-set");
	ReplaceBIN("CAM0900S", "heroes-cam");
	ReplaceBIN("PL_90B", "grand-metropolis-shaders");

	MusicList[MusicIDs_casino1].Name = "grand-metropolis";

	helperFunctions.RegisterPathList(GrandMetropolisPaths);

	LevelObjects[HeroesLevelID_GrandMetropolis] = GrandMetropolisHandler;
	SkyboxObjects[HeroesLevelID_GrandMetropolis] = GrandMetropolisSkybox;
	DeathZoneList[HeroesLevelID_GrandMetropolis][0] = GrandMetropolisDeathZones;

	GrandMetropolisObjects_Init();
}

HeroesLevelData GrandMetropolisData = { HeroesLevelID_GrandMetropolis, 0, 0, "grand-metropolis", "GM", nullptr, nullptr, GrandMetropolis_Init, { 0.5f, 70, 4125.8f } };