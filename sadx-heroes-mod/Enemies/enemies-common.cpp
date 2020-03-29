#include "stdafx.h"

bool IsCommonEnmInitialized;

ModelInfo* EN_CMNMDLS;

const char* CommonEnmObjNames[]{
	"fire",
	"Z",
	"Zz",
	"Zzz",
	"Surprise",
	"Bubble",
	"HP1",
	"HP2",
	"HP3",
	"Zero",
	"One",
	"Two",
	"Three",
	"Four",
	"Five",
	"Six",
	"Seven",
	"Eight",
	"Nine"
};

void CommonEnm_Delete(ObjectMaster* obj) {
	//Delete files if the level is exited, or the player has no life left.
	if ((GameState == GameState_ExitLevel || (GameState == GameState_Death && Lives == 0))
		&& IsCommonEnmInitialized == true) {
		IsCommonEnmInitialized = false;
		EN_CMNMDLS = FreeMDL(EN_CMNMDLS);
	}
}

void Bomb_Main_r(ObjectMaster* obj);
Trampoline Bomb_Main_t(0x4AB120, 0x4AB127, Bomb_Main_r);
void Bomb_Main_r(ObjectMaster* obj) {
	if (obj->Data1->Action == 2 && !obj->Parent) {
		EntityData1* data = obj->Data1;
		ObjectData* od2 = (ObjectData*)obj->Data2;
		CollisionData* col;
		float v3;

		col = data->CollisionInfo->CollisionArray + 1;
		if (data->InvulnerableTime % 2)
		{
			SetMaterialAndSpriteColor_Float(data->Scale.y, 1.0, 0.0, 0.0);
		}
		else
		{
			SetMaterialAndSpriteColor_Float(data->Scale.y, 0.5, 0.5, 0.0);
		}
		if (data->Scale.x >= 25.0)
		{
			v3 = data->Scale.y - 0.050000001;
			data->Scale.y = v3;
			if (v3 < -1.0)
			{
				od2->ActionBackup = data->Action;
				data->Action = 3;
				data->InvulnerableTime = 0;
			}
			data->Rotation.y += 256;
			ClampGlobalColorThing_Thing();
		}
		else
		{
			data->Scale.x = data->Scale.x + 1.0;
			col->scale.x = col->scale.x + 1.0;
			data->Rotation.y += 256;
			ClampGlobalColorThing_Thing();
		}
	}
	else {
		ObjectFunc(original, Bomb_Main_t.Target());
		original(obj);
	}
}

void LoadExplosion(NJS_VECTOR* position) {
	ObjectMaster* temp = LoadObject(LoadObj_Data1, 3, Bomb_Init);
	temp->MainSub(temp);

	temp->Data1->Position = *position;
	temp->Data1->Action = 1;
	temp->Data1->InvulnerableTime = 150;
}

void Missile_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		if (data->Scale.x != 0) njScale(0, data->Scale.x, data->Scale.x, data->Scale.x);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(data->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void Missile_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (++data->InvulnerableTime > 200 || data->CollisionInfo->CollidingObject) {
		LoadExplosion(&data->Position);
		DeleteObject_(obj);
	}
	else {
		data->Position = UnitMatrix_GetPoint(&data->Position, &data->Rotation, 0, 0, -2);

		if (data->InvulnerableTime > 20) {
			AddToCollisionList(data);
		}
		
		obj->DisplaySub(obj);
	}
}

inline void CheckAndLoadCommonEnmFiles() {
	if (IsCommonEnmInitialized == false) {
		IsCommonEnmInitialized = true;
		
		EN_CMNMDLS = LoadEnemyModel("EN_CMNMDLS");
	}
}

void LoadMissile(NJS_VECTOR* pos, Rotation3* dir, float scale) {
	CheckAndLoadCommonEnmFiles();

	ObjectMaster* obj = LoadObject(LoadObj_Data1, 4, Missile_Main);
	obj->Data1->Position = *pos;
	obj->Data1->Rotation = *dir;
	obj->Data1->Scale.x = scale;
	obj->Data1->Object = EN_CMNMDLS->getmodel();
	obj->DisplaySub = Missile_Display;

	Collision_Init(obj, (CollisionData*)0x223B3D8, 1, 4u);
}