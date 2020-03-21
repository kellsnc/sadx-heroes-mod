#include "stdafx.h"

bool IsFlyerInitialized;

ModelInfo*		FlyerMdl;
AnimationFile*	FlyerAnm;
AnimData		FlyerAnimData;

AnimationFile*	CommonPaths[11];
AnimationFile*	EggFleetPaths[36];

AnimationFile** CurrentLevelPath;

NJS_TEXNAME FLYER_TEXNAMES[4];
NJS_TEXLIST FLYER_TEXLIST = { arrayptrandlength(FLYER_TEXNAMES) };

CollisionData FlyerCollisionData {
	0, CollisionShape_Sphere, 0x77, 0x21, 0x2400, { 0, 0, 0}, {10, 10, 0}
};

uint8_t FlyerTriggerID = 0;
uint8_t Count = 0;

void Flyer_DeleteFiles() {
	if (IsFlyerInitialized == true) {
		IsFlyerInitialized = false;
		njReleaseTexture(&FLYER_TEXLIST);
		FreeMDL(FlyerMdl);
		FreeANM(FlyerAnm);
		FreeANMFiles(arrayptrandlength(CommonPaths));

		if (EggFleetPaths[0]) FreeANMFiles(arrayptrandlength(EggFleetPaths));
	}
}

void Flyer_Display(ObjectMaster* obj) {
	if (MissedFrames && obj->Data1->Action) return;

	EntityData1* data = obj->Data1;

	Direct3D_PerformLighting(6);
	njSetTexture(&FLYER_TEXLIST);
	njPushMatrix(0);

	njTranslateV(0, &data->Position);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);

	njAction(FlyerAnimData.Animation, data->Scale.x);

	njPopMatrix(1);
	Direct3D_PerformLighting(0);

	DrawShadow(data, 2);
}

void Flyer_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	ObjectData* data2 = (ObjectData*)obj->Data2;

	if (data->Action == 0) {
		if (ClipSetObject(obj)) return;
		
		if (FlyerTriggerID == obj->Data1->Scale.z || 0) {
			data->Action = 1;
		}
	}
	else {
		if (OhNoImDead(data, (ObjectData2*)data2)) {
			LoadObjectBreaker(&data->Position, &data->Rotation, FlyerMdl->getmodel()->child, &FLYER_TEXLIST);
			Score += 400;
			UpdateSetDataAndDelete(obj);
			
			return;
		}

		NJS_MOTION* motion = (NJS_MOTION*)data->LoopData;

		NJS_MDATA2* mdata2 = (NJS_MDATA2*)motion->mdata;
		NJS_MKEY_F* pos = (NJS_MKEY_F*)mdata2[1].p[0];
		NJS_MKEY_A* ang = (NJS_MKEY_A*)mdata2[1].p[1];

		data->Scale.y += 0.1f;
		if (data->Scale.y >= 1) {
			data->Scale.y = 0;
			data->field_A += 1;

			NJS_MKEY_F id = pos[data->field_A];

			if (id.keyframe == motion->nbFrame - 1) {
				UpdateSetDataAndDelete(obj);
				return;
			}
		}

		NJS_VECTOR* start = (NJS_VECTOR*)pos[data->field_A].key;
		NJS_VECTOR* end = (NJS_VECTOR*)pos[data->field_A + 1].key;
		Rotation3* rot = (Rotation3*)ang[data->field_A].key;

		data->Position = GetPathPosition(start, end, data->Scale.y);
		data->Rotation = *rot;
		
		njAddVector(&data->Position, &data2->StartPosition);
		//EntityData1Ptrs[0]->Position = data->Position;

		PlayHeroesAnimation(obj, 0, &FlyerAnimData, 0, 0);
		AddToCollisionList(data);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

void Flyer_LoadLevelPath(AnimationFile** animfile, uint8_t count, const char* level) {
	PrintDebug("[SHM] Load Flyers paths for %s \n", level);
	CurrentLevelPath = animfile;

	for (uint8_t i = 0; i < count; ++i) {
		std::string fullPath = "system\\";
		std::string num = std::to_string(i);
		if (i < 10) num = "0" + num;
		fullPath = fullPath + "enemies\\paths\\" + level + "\\PATH_STG_0" + num + ".saanim";

		animfile[i] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));
		Count += 1;
	}
}

void Flyer_LoadCommonPaths() {
	PrintDebug("[SHM] Load Flyers common paths \n");
	Count = LengthOfArray(CommonPaths);

	for (uint8_t i = 0; i < Count; ++i) {
		std::string fullPath = "system\\";
		std::string num = std::to_string(i);
		if (i < 10) num = "0" + num;
		fullPath = fullPath + "enemies\\paths\\PATH_CMN_0" + num + ".saanim";

		CommonPaths[i] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));
	}
}

inline void Flyer_LoadFiles() {
	LoadPVM("Flyer", &FLYER_TEXLIST);
	FlyerMdl = LoadEnemyModel("EN_FLYER");
	FlyerAnm = LoadEnemyAnim("EN_FLYER");
	FlyerAnimData.Animation = new NJS_ACTION;
	FlyerAnimData.Animation->object = FlyerMdl->getmodel();
	FlyerAnimData.Animation->motion = FlyerAnm->getmotion();
	FlyerAnimData.NextAnim = 0;
	FlyerAnimData.AnimationSpeed = 0.5f;

	Flyer_LoadCommonPaths();
	if (CurrentLevel == HeroesLevelID_EggFleet) Flyer_LoadLevelPath(arrayptrandlength(EggFleetPaths), "eggfleet");
}

void Flyer_Init(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	//	Load the files only once
	if (IsFlyerInitialized == false) {
		IsFlyerInitialized = true;
		Flyer_LoadFiles();
	}

	if (data->Scale.x > Count) {
		UpdateSetDataAndDelete(obj);
		return;
	}

	//	Init the enemy handler (used for Gamma's missile, Sonic's attacks)
	AllocateObjectData2(obj, data);
	ObjectData2_SetStartPosition(data, (ObjectData2*)obj->Data2);

	//	Init the collision (with the "hurt player if not attacking" and "target" flags)
	Collision_Init(obj, &FlyerCollisionData, 1, 2);

	//	Get the correct path
	int path = (int)data->Scale.x;
	
	if (path < LengthOfArray(CommonPaths)) {
		data->LoopData = (Loop*)CommonPaths[path]->getmotion();
	}
	else {
		data->LoopData = (Loop*)CurrentLevelPath[path - LengthOfArray(CommonPaths)]->getmotion();
		data->NextAction = 1;
	}

	data->Scale.x = 0;

	//	Object functions
	obj->MainSub = Flyer_Main;
	obj->DisplaySub = Flyer_Display;
}

void Flyer_Trigger(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;
	
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		FlyerTriggerID = 0;
		data->Scale.y *= 100;
		data->Action = 1;
	}

	if (IsPlayerInsideSphere_(&data->Position, data->Scale.y) == 1) {
		FlyerTriggerID = obj->Data1->Scale.x;
	}
}