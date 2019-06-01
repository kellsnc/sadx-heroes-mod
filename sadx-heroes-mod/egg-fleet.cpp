#include "stdafx.h"
#include "mod.h"
#include "levels.h"

#include "egg-fleet.h"

ModelInfo * EF_SKYMDLS;

void EggFleet_InitObjects() {
	EF_SKYMDLS = LoadMDL("EF_SKYMDLS");
}

void EggFleet_Delete(ObjectMaster *a1) {
	FreeMDL(EF_SKYMDLS);

	LevelHandler_Delete(a1);
}

void EggFleetHandler(ObjectMaster *a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = EggFleet_Delete;

		EggFleet_InitObjects();
		CurrentLevelTexlist = &SKYDECK01_TEXLIST;
		CurrentLandAddress = (LandTable**)0x97DAC8;

		PlayMusic(MusicIDs_SkyDeckSkydeckAGoGo);

		entity->Position = { 500, 4230, 5320 };
	}
	else {
		ChunkHandler("EF", EggFleetChunks, LengthOfArray(EggFleetChunks), entity->Position);
		AnimateTextures(EggFleetAnimTexs, LengthOfArray(EggFleetAnimTexs));
	}
}

void EggFleetSkybox(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DisplaySub = a1->MainSub;

		HeroesSkybox_Main(a1);

		return;
	}
	else if (a1->Data1->Action == 1) {
		DeleteObjectMaster(CurrentLevelObject);
		CurrentLevelObject = LoadObject(LoadObj_Data1, 1, EggFleetHandler);
		a1->Data1->Action = 2;

		return;
	}
	
	if (!MissedFrames) {
		DisableFog();
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(nullptr, &EntityData1Ptrs[0]->Position);
		njScale(nullptr, 0.1f, 0.1f, 0.1f);
		DrawQueueDepthBias = -6000;
		njDrawModel_SADX(EF_SKYMDLS->getmodel()->basicdxmodel);
		njDrawModel_SADX(EF_SKYMDLS->getmodel()->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
		ToggleStageFog();
	}
}

void EggFleet_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("SKYDECK01", "egg-fleet");
	ReplaceBIN("SET0600S", "egg-fleet-set");
	ReplaceBIN("SET0600M", "egg-fleet-set-tails");
	ReplaceBIN("CAM0600S", "egg-fleet-cam");
	ReplaceDAT("SKY_DECK_BANK01", "HEROES_BANK");
	ReplaceADX("skydeck1", "egg-fleet");
	ReplaceBIN("PL_60B", "egg-fleet-shaders");

	helperFunctions.RegisterPathList(EggFleetPaths); //splines

	SkyDeckDeathZones[0] = EggFleetDeathZones;

	//Load the level handler
	SkyboxObjects[HeroesLevelID_EggFleet] = EggFleetSkybox;
}