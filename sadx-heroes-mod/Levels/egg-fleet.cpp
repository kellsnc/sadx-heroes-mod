#include "stdafx.h"
#include "egg-fleet.h"
#include "egg-fleet-deathzones.h"
#include "egg-fleet-paths.h"

ModelInfo * EF_SKYMDLS;

MusicInfo EggFleetMusic = { "egg-fleet", 1 };
int eggfleetmusicid = 81;

void EggFleetObjects_Init();

void EggFleet_InitObjects() {
	EF_SKYMDLS = LoadObjectModel(EF_SKYMDLS, "EF_SKYMDLS");
	EF_CANNON1 = LoadObjectModel(EF_CANNON1, "EF_CANNON1");
	EF_BULLETS = LoadObjectModel(EF_BULLETS, "EF_BULLETS");
}

void EggFleet_Delete(ObjectMaster *a1) {
	EF_SKYMDLS = FreeMDL(EF_SKYMDLS);
	EF_CANNON1 = FreeMDL(EF_CANNON1);
	EF_BULLETS = FreeMDL(EF_BULLETS);

	LevelHandler_Delete(a1);
}

void EggFleetHandler(ObjectMaster *a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (a1->Data1->Action == 0) {
		a1->Data1->Action = 1;
		a1->DeleteSub = EggFleet_Delete;

		SetFog(&EggFleet_Fog);

		EggFleet_InitObjects();
		CurrentLevelTexlist = &SKYDECK01_TEXLIST;
		CurrentLandAddress = (LandTable**)0x97DAC8;

		PlayMusic((MusicIDs)eggfleetmusicid);

		entity->Position = { 500, 4230, 5320 };
		//entity->Position = { -9501.797, -4170.793, -38106.13 };
		entity->Position = { -4930,  340.3953,  -9121 };
	}
	else {

		//temp until the ship breaker object is ported
		NJS_VECTOR swap1 = { -5998.083, 3435.614, -13343.93 };
		if (IsPlayerInsideSphere_(&swap1, 20)) {
			SwapChunk("EF", 7);
		}
		
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
	ReplaceBIN("PL_60B", "egg-fleet-shaders");

	DefaultLight(HeroesLevelID_EggFleet);

	eggfleetmusicid =  helperFunctions.RegisterMusicFile(EggFleetMusic); //bgm
	helperFunctions.RegisterPathList(EggFleetPaths); //splines

	//Load the level handler
	SkyboxObjects[HeroesLevelID_EggFleet] = EggFleetSkybox;
	DeathZoneList[HeroesLevelID_EggFleet][0] = EggFleetDeathZones;

	EggFleetObjects_Init();
}