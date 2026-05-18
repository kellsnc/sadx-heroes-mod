#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "levels.h"
#include "objects.h"
#include "characters.h"
#include "enemies.h"

bool IsFlyerInitialized;

ModelInfo*     FlyerMdl;
AnimationFile* FlyerAnm;
AnimData       FlyerAnimData;

std::vector<AnimationFile*> FlyerPaths;

NJS_TEXNAME FLYER_TEXNAMES[4];
NJS_TEXLIST FLYER_TEXLIST = { arrayptrandlength(FLYER_TEXNAMES) };

CollisionData FlyerCollisionData {
	0, CollisionShape_Sphere, 0x77, 0x21, 0x2400, { 0, 0, 0}, 10, 10, 0
};

uint8_t FlyerTriggerID = 0;
uint8_t Count = 0;

void Flyer_DeleteFiles() {
	if (IsFlyerInitialized == true) {
		IsFlyerInitialized = false;
		njReleaseTexture(&FLYER_TEXLIST);
		FreeMDL(FlyerMdl);
		FreeANM(FlyerAnm);
		for (auto& anim : FlyerPaths)
		{
			FreeANM(anim);
		}
		FlyerPaths.clear();
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

NJS_VECTOR Flyer_GetPoint(NJS_VECTOR* orig, NJS_VECTOR* start, NJS_VECTOR* end, Angle y, float state) {
	NJS_VECTOR point;
	NJS_VECTOR start2;
	NJS_VECTOR end2;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(nullptr, orig);
	njRotateY(nullptr, y);
	njCalcPoint(0, start, &start2);
	njPopMatrix(1u);

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(nullptr, orig);
	njRotateY(nullptr, y);
	njCalcPoint(0, end, &end2);
	njPopMatrix(1u);

	return GetPathPosition(&start2, &end2, state);
}

void Flyer_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	ObjectData* data2 = (ObjectData*)obj->Data2;
	
	if (data->Action == 0) {
		if (ClipSetObject(obj)) return;
		
		if (FlyerTriggerID == obj->Data1->Scale.z) {
			data->Action = 1;
			obj->DisplaySub = Flyer_Display;
			PlayHeroesSound_EntityAndVolume(LevelSound_Egg_Engines, obj, 300, 3, true);
		}
	}
	else {
		if (OhNoImDead(data, (ObjectData2*)data2)) {
			LoadObjectBreaker(&data->Position, &data->Rotation, FlyerMdl->getmodel()->child, &FLYER_TEXLIST);
			PlayHeroesSound_Pos(CommonSound_Explosion, &data->Position, 300, 5, false);

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

		data->Rotation = *rot;

		if (data2->field_0) {
			data->Position = Flyer_GetPoint(&data2->StartPosition, start, end, data2->field_0, data->Scale.y);
			data->Rotation.y += data2->field_0;
		}
		else {
			data->Position = GetPathPosition(start, end, data->Scale.y);
			njAddVector(&data->Position, &data2->StartPosition);
		}
		
		//PlayHeroesAnimation(obj, 0, &FlyerAnimData, 0, 0);
		AddToCollisionList(data);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

void Flyer_LoadFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\en_flyer.arcx"));

	LoadPVM("Flyer", &FLYER_TEXLIST);
	FlyerMdl = arc.GetModel("EN_FLYER.sa1mdl");
	FlyerAnm = arc.GetAnimation("EN_FLYER.saanim");
	FlyerAnimData.Animation = new NJS_ACTION;
	FlyerAnimData.Animation->object = FlyerMdl->getmodel();
	FlyerAnimData.Animation->motion = FlyerAnm->getmotion();
	FlyerAnimData.NextAnim = 0;
	FlyerAnimData.AnimationSpeed = 0.5f;

	int i = 0;
	while (1)
	{
		std::string num = std::to_string(i);
		if (i < 10) num = "0" + num;
		AnimationFile* anim = arc.GetAnimation(std::string("PATH_CMN_0" + num + ".saanim"));
		if (!anim) break;
		FlyerPaths.push_back(anim);
		++i;
	}

	i = 0;
	while (1)
	{
		std::string num = std::to_string(i);
		if (i < 10) num = "0" + num;
		AnimationFile* anim = arc.GetAnimation(std::string("PATH_STG_0" + num + ".saanim"));
		if (!anim) break;
		FlyerPaths.push_back(anim);
		++i;
	}
}

void Flyer_Init(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	//	Load the files only once
	if (IsFlyerInitialized == false) {
		IsFlyerInitialized = true;
		Flyer_LoadFiles();
	}

	if (data->Scale.x > FlyerPaths.size()) {
		UpdateSetDataAndDelete(obj);
		return;
	}

	//	Init the enemy handler (used for Gamma's missile, Sonic's attacks)
	AllocateObjectData2(obj, data);
	ObjectData2_SetStartPosition(data, (ObjectData2*)obj->Data2);

	//	Init the collision (with the "hurt player if not attacking" and "target" flags)
	Collision_Init(obj, &FlyerCollisionData, 1, 2);

	//	Get the path
	data->LoopData = (Loop*)FlyerPaths[(int)data->Scale.x]->getmotion();

	if (data->Rotation.y) {
		ObjectData* data2 = (ObjectData*)obj->Data2;
		data2->field_0 = data->Rotation.y;
	}

	obj->MainSub = Flyer_Main;
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