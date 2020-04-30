#include "stdafx.h"
#include "power-plant-paths.h"
#include "power-plant-deathzones.h"
#include "power-plant.h"

void PowerPlantSkybox(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			HeroesSkybox_Main(a1);
		}
		else {
			DisableFog();
			SetHeroesLeveltex();
			njPushMatrix(0);
			NJS_VECTOR* pos = &EntityData1Ptrs[0]->Position;
			njTranslate(nullptr, pos->x, 1000, pos->z);
			DrawQueueDepthBias = -6000;
			njDrawModel_SADX(PP_SKYMDLS->getmodel()->basicdxmodel);
			njDrawModel_SADX(PP_SKYMDLS->getmodel()->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
			ToggleStageFog();
		}
	}
}

void PowerPlantHandler(ObjectMaster * a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_icecap1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;

		LoadObject(LoadObj_Data1, 3, PPSolarpnls);
		LoadObject(LoadObj_Data1, 3, PPCranes);
		LoadObject(LoadObj_Data1, 3, PPLights);
		LoadObject(LoadObj_Data1, 3, PPTankHandler);
		LoadObject(LoadObj_Data1, 3, PPTrucks);

		if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);
	}
	else {
		ChunkHandler("PP", PowerPlantChunks, LengthOfArray(PowerPlantChunks), entity->Position);
		AnimateTextures(PowerPlantAnimTexs, LengthOfArray(PowerPlantAnimTexs));
		AnimateUV(PowerPlant_UVSHIFT, LengthOfArray(PowerPlant_UVSHIFT));
		PPPathsHandler();
	}
}

void PowerPlant_Unload() {
	PP_ELEVATR = FreeMDL(PP_ELEVATR);
	PP_FLDPATH = FreeMDL(PP_FLDPATH);
	PP_LGTSIGN = FreeMDL(PP_LGTSIGN);
	PP_SKYMDLS = FreeMDL(PP_SKYMDLS);
	PP_MTRUCKS = FreeMDL(PP_MTRUCKS);
	PP_PLTFRMS = FreeMDL(PP_PLTFRMS);
	PP_PPCRANE = FreeMDL(PP_PPCRANE);
	PP_SOLARPN = FreeMDL(PP_SOLARPN);
	PP_TNKDOOR = FreeMDL(PP_TNKDOOR);
	PP_TNKSTEP = FreeMDL(PP_TNKSTEP);
	PP_ENERGYT = FreeMDL(PP_ENERGYT);
	PP_SHAFTST = FreeMDL(PP_SHAFTST);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}
}

void PowerPlant_Load() {
	PP_ELEVATR = LoadObjectModel(PP_ELEVATR, "PP_ELEVATR");
	PP_FLDPATH = LoadObjectModel(PP_FLDPATH, "PP_FLDPATH");
	PP_LGTSIGN = LoadObjectModel(PP_LGTSIGN, "PP_LGTSIGN");
	PP_SKYMDLS = LoadObjectModel(PP_SKYMDLS, "PP_SKYMDLS");
	PP_MTRUCKS = LoadObjectModel(PP_MTRUCKS, "PP_MTRUCKS");
	PP_PLTFRMS = LoadObjectModel(PP_PLTFRMS, "PP_PLTFRMS");
	PP_PPCRANE = LoadObjectModel(PP_PPCRANE, "PP_PPCRANE");
	PP_SOLARPN = LoadObjectModel(PP_SOLARPN, "PP_SOLARPN");
	PP_TNKDOOR = LoadObjectModel(PP_TNKDOOR, "PP_TNKDOOR");
	PP_TNKSTEP = LoadObjectModel(PP_TNKSTEP, "PP_TNKSTEP");
	PP_ENERGYT = LoadObjectModel(PP_ENERGYT, "PP_ENERGYT");
	PP_SHAFTST = LoadObjectModel(PP_SHAFTST, "PP_SHAFTST");

	PowerPlant_UVSHIFT[0].List = PP_SOLARPN->getmodel()->child->basicdxmodel->meshsets[2].vertuv;
	PowerPlant_UVSHIFT[1].List = PP_ELEVATR->getmodel()->basicdxmodel->meshsets[5].vertuv;
	PowerPlant_UVSHIFT[0].Size = PP_SOLARPN->getmodel()->child->basicdxmodel->meshsets[2].nbMesh * 3;
	PowerPlant_UVSHIFT[1].Size = PP_ELEVATR->getmodel()->basicdxmodel->meshsets[5].nbMesh * 3;
}

void PowerPlant_Init(const HelperFunctions &helperFunctions) {
	ReplaceBIN("PL_80B", "power-plant-shaders");

	MusicList[MusicIDs_icecap1].Name = "power-plant";

	helperFunctions.RegisterPathList(PowerPlantPaths);

	LevelObjects[HeroesLevelID_PowerPlant] = PowerPlantHandler;
	SkyboxObjects[HeroesLevelID_PowerPlant] = PowerPlantSkybox;
	DeathZoneList[HeroesLevelID_PowerPlant][0] = PowerPlantDeathZones;

	PowerPlantObjects_Init();
}

HeroesLevelData PowerPlantData = { HeroesLevelID_PowerPlant, 0, 11, "power-plant", "PP", PowerPlant_Load, PowerPlant_Unload, PowerPlant_Init, { 2000, 1564, 63 } };