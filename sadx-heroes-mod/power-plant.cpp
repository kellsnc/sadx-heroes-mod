#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "power-plant-paths.h"
#include "power-plant-deathzones.h"
#include "power-plant.h"

void PowerPlantSkybox(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 0) {
			a1->Data1->Action = 1;
			a1->DisplaySub = a1->MainSub;
			HeroesSkybox_Main(a1);
		}
		
		DisableFog();
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		NJS_VECTOR * pos = &EntityData1Ptrs[0]->Position;
		njTranslate(nullptr, pos->x, 1000, pos->z);
		DrawQueueDepthBias = -6000;
		njDrawModel_SADX(PP_SKYMDLS->getmodel()->basicdxmodel);
		njDrawModel_SADX(PP_SKYMDLS->getmodel()->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
		ToggleStageFog();
	}
}

void PowerPlant_InitObjects() {
	PP_ELEVATR = LoadMDL("PP_ELEVATR");
	PP_FLDPATH = LoadMDL("PP_FLDPATH");
	PP_LGTSIGN = LoadMDL("PP_LGTSIGN");
	PP_SKYMDLS = LoadMDL("PP_SKYMDLS");
	PP_MTRUCKS = LoadMDL("PP_MTRUCKS");
	PP_PLTFRMS = LoadMDL("PP_PLTFRMS");
	PP_PPCRANE = LoadMDL("PP_PPCRANE");
	PP_SOLARPN = LoadMDL("PP_SOLARPN");
	PP_TNKDOOR = LoadMDL("PP_TNKDOOR");
	PP_TNKSTEP = LoadMDL("PP_TNKSTEP");

	PowerPlant_UVSHIFT[0].List = PP_SOLARPN->getmodel()->child->basicdxmodel->meshsets[2].vertuv;
	PowerPlant_UVSHIFT[1].List = PP_ELEVATR->getmodel()->basicdxmodel->meshsets[5].vertuv;
	PowerPlant_UVSHIFT[0].Size = PP_SOLARPN->getmodel()->child->basicdxmodel->meshsets[2].nbMesh * 3;
	PowerPlant_UVSHIFT[1].Size = PP_ELEVATR->getmodel()->basicdxmodel->meshsets[5].nbMesh * 3;

	PPOBJLIST[0] = PP_FLDPATH->getmodel()->basicdxmodel;
	PPOBJLIST[1] = PP_FLDPATH->getmodel()->child->basicdxmodel;
	PPOBJLIST[2] = PP_FLDPATH->getmodel()->child->child->basicdxmodel;
	PPOBJLIST[3] = PP_PLTFRMS->getmodel()->basicdxmodel;
	PPOBJLIST[4] = PP_PLTFRMS->getmodel()->child->basicdxmodel;
	PPOBJLIST[5] = PP_SOLARPN->getmodel()->basicdxmodel;

	LoadObject(LoadObj_Data1, 3, PPSolarpnls);
	LoadObject(LoadObj_Data1, 3, PPCranes);
	LoadObject(LoadObj_Data1, 3, PPLights);
	LoadObject(LoadObj_Data1, 3, PPTankHandler);
	LoadObject(LoadObj_Data1, 3, PPTrucks);
}

void PowerPlant_Delete(ObjectMaster * a1) {
	FreeMDL(PP_ELEVATR);
	FreeMDL(PP_FLDPATH);
	FreeMDL(PP_LGTSIGN);
	FreeMDL(PP_SKYMDLS);
	FreeMDL(PP_MTRUCKS);
	FreeMDL(PP_PLTFRMS);
	FreeMDL(PP_PPCRANE);
	FreeMDL(PP_SOLARPN);
	FreeMDL(PP_TNKDOOR);
	FreeMDL(PP_TNKSTEP);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
	
	LevelHandler_Delete(a1);
}

void PowerPlantHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_icecap1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = PowerPlant_Delete;

		PowerPlant_InitObjects();

		if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);

		CurrentLevelTexlist = &ICECAP01_TEXLIST;
		CurrentLandAddress = (LandTable**)0x97DB08;
	}
	else {
		ChunkHandler("PP", PowerPlantChunks, LengthOfArray(PowerPlantChunks), entity->Position);
		AnimateTextures(PowerPlantAnimTexs, LengthOfArray(PowerPlantAnimTexs));
		AnimateObjectsTextures(PPOBJLIST, LengthOfArray(PPOBJLIST), PowerPlantAnimTexs, LengthOfArray(PowerPlantAnimTexs));
		AnimateUV(PowerPlant_UVSHIFT, LengthOfArray(PowerPlant_UVSHIFT));
		PPPathsHandler();
	}
}

void PowerPlant_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("ICECAP01", "power-plant");
	ReplaceBIN("SET0800S", "power-plant-set");
	ReplaceBIN("SET0800M", "power-plant-set-tails");
	ReplaceBIN("CAM0800S", "heroes-cam");
	ReplaceDAT("ICE_CAP_BANK01", "HEROES_BANK");
	ReplaceADX("icecap1", "power-plant");
	ReplaceBIN("PL_80B", "power-plant-shaders");

	helperFunctions.RegisterPathList(PowerPlantPaths);

	IceCapDeathZones[0] = PowerPlantDeathZones;

	LevelObjects[HeroesLevelID_PowerPlant] = PowerPlantHandler;
	SkyboxObjects[HeroesLevelID_PowerPlant] = PowerPlantSkybox;

	PowerPlantObjects_Init(path);
}