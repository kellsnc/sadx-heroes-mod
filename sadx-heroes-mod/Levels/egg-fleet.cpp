#include "stdafx.h"
#include "egg-fleet.h"
#include "egg-fleet-paths.h"

ModelInfo* EF_SKYMDLS;

void EggFleetSkybox(ObjectMaster* obj) {
	if (!MissedFrames) {
		if (obj->Data1->Action == 0) {
			obj->DisplaySub = obj->MainSub;
			HeroesSkybox_Main(obj);
		}
		else {
			DisableFog();
			SetHeroesLeveltex();
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
}

void EggFleetHandler(ObjectMaster *obj) {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 * co2 = CharObj2Ptrs[0];

	if (obj->Data1->Action == 0) {
		obj->Data1->Action = 1;

		// fix a weird lighting issue
		Obj_SkyDeck(obj); 
		DeleteChildObjects(obj);

		SetFog(&EggFleet_Fog);

		LoadObject((LoadObj)0, 3, EFRailends);
		LoadObject(LoadObj_Data1, 3, EFBgShips);

		if (RestartLevel.Position.x == 0) {
			entity->Position = { 500, 4230, 5320 };
		}

		SetCameraMode_(1);
	}
	else {
		ChunkHandler("EF", EggFleetChunks, LengthOfArray(EggFleetChunks), entity->Position);
		AnimateTextures(EggFleetAnimTexs, LengthOfArray(EggFleetAnimTexs));
		AnimateUV(EggFleet_UVShift, LengthOfArray(EggFleet_UVShift));

		// kill the player if he falls for too long
		if (entity->Status & Status_Ground || co2->AnimationThing.Index < 4 || co2->AnimationThing.Index > 19) {
			obj->Data1->InvulnerableTime = 0;
		}
		else if (co2->Speed.y < 4 && entity->field_A < 100) {
			if (++obj->Data1->InvulnerableTime == 800) {
				KillPlayer(0);
			}
		}
	}
}

void EggFleet_Unload() {
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
	EF_SHPBRK2 = FreeMDL(EF_SHPBRK2);
	EF_BARRIER = FreeMDL(EF_BARRIER);
	EF_CANBRK1 = FreeMDL(EF_CANBRK1);
	EF_CANBRK2 = FreeMDL(EF_CANBRK2);
	EF_CANDECO = FreeMDL(EF_CANDECO);
	EF_EBIGFAN = FreeMDL(EF_EBIGFAN);
	EF_EHELICE = FreeMDL(EF_EHELICE);
}

void EggFleet_Load() {
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
	EF_SHPBRK2 = LoadObjectModel(EF_SHPBRK1, "EF_SHPBRK2");
	EF_BARRIER = LoadObjectModel(EF_BARRIER, "EF_BARRIER");
	EF_CANBRK1 = LoadObjectModel(EF_CANBRK1, "EF_CANBRK1");
	EF_CANBRK2 = LoadObjectModel(EF_CANBRK2, "EF_CANBRK2");
	EF_CANDECO = LoadObjectModel(EF_CANDECO, "EF_CANDECO");
	EF_EBIGFAN = LoadObjectModel(EF_EBIGFAN, "EF_EBIGFAN");
	EF_EHELICE = LoadObjectModel(EF_EHELICE, "EF_EHELICE");

	AddUVList(EggFleet_UVShift, 0, EF_OBJSHIP->getmodel()->child->child->child->basicdxmodel, 0);
	AddUVList(EggFleet_UVShift, 1, EF_PIPLINE->getmodel()->child->basicdxmodel, 0);

	PropellerModel = EF_PROPPLR->getmodel();
}

void EggFleet_Init(const HelperFunctions &helperFunctions) {
	helperFunctions.RegisterPathList(EggFleetPaths);

	//Load the level handler
	SkyboxObjects[HeroesLevelID_EggFleet] = EggFleetSkybox;
	LevelObjects[HeroesLevelID_EggFleet] = EggFleetHandler;
	DeathZoneList[HeroesLevelID_EggFleet][0] = nullptr;

	EggFleetObjects_Init();
}

HeroesLevelData EggFleetData = { HeroesLevelID_EggFleet, 0, 11, "egg-fleet", "EF", EggFleet_Load, EggFleet_Unload, EggFleet_Init, { 500, 4230, 5320 } };