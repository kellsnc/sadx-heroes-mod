#include "stdafx.h"
#include "egg-fleet.h"
#include "egg-fleet-deathzones.h"
#include "egg-fleet-paths.h"

ModelInfo* EF_SKYMDLS;

MusicInfo EggFleetMusic = { "egg-fleet", 1 };
int eggfleetmusicid = 81;

void EggFleetObjects_Init();

void EggFleet_InitObjects() {
	EF_SKYMDLS = LoadObjectModel(EF_SKYMDLS, "EF_SKYMDLS");
	EF_BGSHIPS = LoadObjectModel(EF_BGSHIPS, "EF_BGSHIPS");
	EF_CANNON1 = LoadObjectModel(EF_CANNON1, "EF_CANNON1");
	EF_CANNON2 = LoadObjectModel(EF_CANNON2, "EF_CANNON2");
	EF_BULLETS = LoadObjectModel(EF_BULLETS, "EF_BULLETS");
	EF_PROPPLR = LoadObjectModel(EF_PROPPLR, "EF_PROPPLR");
	EF_PLTFRMS = LoadObjectModel(EF_PLTFRMS, "EF_PLTFRMS");
	EF_PIPLINE = LoadObjectModel(EF_PIPLINE, "EF_PIPLINE");
	EF_ENDRAIL = LoadObjectModel(EF_ENDRAIL, "EF_ENDRAIL");
	EF_OBJSHIP = LoadObjectModel(EF_OBJSHIP, "EF_OBJSHIP");
	EF_ANTENNA = LoadObjectModel(EF_ANTENNA, "EF_ANTENNA");
	EF_BIGSHIP = LoadObjectModel(EF_BIGSHIP, "EF_BIGSHIP");
	EF_DIRSGNS = LoadObjectModel(EF_DIRSGNS, "EF_DIRSGNS");
	EF_SHPBRK1 = LoadObjectModel(EF_SHPBRK1, "EF_SHPBRK1");
	EF_BARRIER = LoadObjectModel(EF_BARRIER, "EF_BARRIER");
	EF_CANBRK1 = LoadObjectModel(EF_CANBRK1, "EF_CANBRK1");
	EF_CANBRK2 = LoadObjectModel(EF_CANBRK2, "EF_CANBRK2");
	EF_CANDECO = LoadObjectModel(EF_CANDECO, "EF_CANDECO");
	EF_EBIGFAN = LoadObjectModel(EF_EBIGFAN, "EF_EBIGFAN");
	EF_EHELICE = LoadObjectModel(EF_EHELICE, "EF_EHELICE");
	
	LoadObject((LoadObj)0, 3, EFRailends);
	LoadObject(LoadObj_Data1, 3, EFBgShips);

	PropellerModel = EF_PROPPLR->getmodel();
}

void EggFleet_Delete(ObjectMaster *a1) {
	EF_SKYMDLS = FreeMDL(EF_SKYMDLS);
	EF_BGSHIPS = FreeMDL(EF_BGSHIPS);
	EF_CANNON1 = FreeMDL(EF_CANNON1);
	EF_CANNON2 = FreeMDL(EF_CANNON2);
	EF_BULLETS = FreeMDL(EF_BULLETS);
	EF_PROPPLR = FreeMDL(EF_PROPPLR);
	EF_PLTFRMS = FreeMDL(EF_PLTFRMS);
	EF_PIPLINE = FreeMDL(EF_PIPLINE);
	EF_ENDRAIL = FreeMDL(EF_ENDRAIL);
	EF_OBJSHIP = FreeMDL(EF_OBJSHIP);
	EF_ANTENNA = FreeMDL(EF_ANTENNA);
	EF_BIGSHIP = FreeMDL(EF_BIGSHIP);
	EF_DIRSGNS = FreeMDL(EF_DIRSGNS);
	EF_SHPBRK1 = FreeMDL(EF_SHPBRK1);
	EF_BARRIER = FreeMDL(EF_BARRIER);
	EF_CANBRK1 = FreeMDL(EF_CANBRK1);
	EF_CANBRK2 = FreeMDL(EF_CANBRK2);
	EF_CANDECO = FreeMDL(EF_CANDECO);
	EF_EBIGFAN = FreeMDL(EF_EBIGFAN);
	EF_EHELICE = FreeMDL(EF_EHELICE);
	
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
		/*entity->Position = { -8169.233,  -4742.518,  -34860.18 };
		entity->Position = { -7004.627, 841.3749, -16349.14 };*/
		/*entity->Position = { -7964.454, 1097.875, -19403.42 };*/
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
	ReplaceBIN("PL_60B", "egg-fleet-shaders");

	DefaultLight(HeroesLevelID_EggFleet);

	eggfleetmusicid =  helperFunctions.RegisterMusicFile(EggFleetMusic); //bgm
	helperFunctions.RegisterPathList(EggFleetPaths); //splines

	//Load the level handler
	SkyboxObjects[HeroesLevelID_EggFleet] = EggFleetSkybox;
	DeathZoneList[HeroesLevelID_EggFleet][0] = EggFleetDeathZones;

	EggFleetObjects_Init();
}