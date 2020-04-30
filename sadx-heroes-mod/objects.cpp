#include "stdafx.h"

NJS_MATERIAL matlist_col[] = {
	{ { 0xFFFFFFFF },{ 0xFFFFFFFF }, 0, 0, NJD_DA_INV_SRC | NJD_SA_SRC }
};

NJS_MATERIAL matlist_waterfall[] = {
	{ { 0x64FFFFFF },{ 0x00FFFFFF }, 0, 87, NJD_FLAG_USE_ANISOTROPIC | NJD_FLAG_USE_ALPHA | NJD_FLAG_USE_TEXTURE | NJD_DA_ONE | NJD_SA_SRC }
};

#define SET_COUNT 4096
static SETObjData set_table[SET_COUNT]{};

uint8_t killcount = 0;

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void CommonObjects_OnFrame();

void __cdecl ObjectData2_SetStartPosition_r(EntityData1 *a1, ObjectData2 *a2)
{
	NJS_VECTOR *v2; // ecx

	v2 = &a2->vector_a;
	v2->x = a1->Position.x;
	v2->y = a1->Position.y;
	v2->z = a1->Position.z;

	if (a1->Scale.z == 2 && a1->Action == 4) {
		killcount += 1;
	}
}

void ElevatePlayer(uint8_t slot) {
	EntityData1 *entity = EntityData1Ptrs[slot];
	CharObj2 *co2 = CharObj2Ptrs[slot];
	co2->Speed.y = 2;

	if (GetCharacterID(slot) == Characters_Sonic && (co2->Upgrades & Upgrades_SuperSonic) == 0) {
		co2->AnimationThing.Index = 26;
		entity->Status = 0;
	}
	else if (GetCharacterID(slot) == Characters_Tails) {
		co2->AnimationThing.Index = 33;
		entity->Status = 0;
	}
	else if (GetCharacterID(slot) == Characters_Knuckles) {
		co2->AnimationThing.Index = 34;
		entity->Status = 0;
	}
}

CollisionData ObjBrkr_Col{
	0, 0, 0, 0, 0x2400, {0, 5, 0}, {10, 10, 10}
};

NJS_VECTOR Breaker_GetPoint(NJS_VECTOR* orig, Rotation3* rot, float dest) {
	NJS_VECTOR point;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, orig);
	njRotateXYZ(0, rot->x, rot->y, rot->z);
	njTranslate(0, 0, 0, dest);
	njGetTranslation(_nj_current_matrix_ptr_, &point);
	njPopMatrix(1u);

	return point;
}

void ObjectBreaker_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		if (data->LoopData) {
			njSetTexture((NJS_TEXLIST*)data->LoopData);
		}
		else {
			SetHeroesLeveltex();
		}

		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njRotateX(0, data->InvulnerableTime * 100);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njPopMatrix(1);
	}
}

void ObjectBreaker_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	ObjectData* od2 = (ObjectData*)obj->Data2;

	if (data->Action == 0) {
		data->Action = 1;

		data->Position.y += 10;
		data->Rotation.x = (rand() * 0.000030517578 * 360.0 * 65536.0 * 0.002777777777777778);
		data->Rotation.y = (rand() * 0.000030517578 * 360.0 * 65536.0 * 0.002777777777777778);
		data->Rotation.z = (rand() * 0.000030517578 * 360.0 * 65536.0 * 0.002777777777777778);

		data->Scale = Breaker_GetPoint(&data->Position, &data->Rotation, 30);
	}

	++data->InvulnerableTime;
	data->field_A += 2;
	float timer = (float)data->InvulnerableTime / 100;
	timer *= 1 + data->field_A / 100;

	data->Position = GetPathPosition(&data->Position, &data->Scale, timer);

	if (data->InvulnerableTime > 20)
		DeleteObject_(obj);

	AddToCollisionList(data);
	obj->DisplaySub(obj);
}

void ObjectBreaker(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		NJS_OBJECT* child = data->Object;
		while (1) {
			if (child) {
				if (child->basicdxmodel) {
					LoadChildObject(LoadObj_Data1, ObjectBreaker_Main, obj);
					obj->Child->Data1->Position.x = data->Position.x + child->pos[0];
					obj->Child->Data1->Position.y = data->Position.y + child->pos[1];
					obj->Child->Data1->Position.z = data->Position.z + child->pos[2];
					obj->Child->Data1->Rotation = fPositionToRotation(&data->Position, &obj->Child->Data1->Position);
					obj->Child->Data1->Object = child;
					obj->Child->DisplaySub = ObjectBreaker_Display;

					Collision_Init(obj->Child, &ObjBrkr_Col, 1, 4u);

					if (data->LoopData) {
						obj->Child->Data1->LoopData = data->LoopData;
					}
				}
				
				if (child->child) child = child->child;
				else if (child->sibling) child = child->sibling;
				else child = nullptr;
			}
			else {
				break;
			}
		}

		//3D Sound

		data->Action = 1;
	}
	else {
		if (obj->Child) {
			RunObjectChildren(obj);
		}
		else {
			DeleteObject_(obj);
		}
	}
}

void LoadModelBroken(NJS_VECTOR* pos, Rotation3* dir, NJS_OBJECT* model, NJS_TEXLIST* tex) {
	ObjectMaster* obj = LoadObject(LoadObj_Data1, 4, ObjectBreaker_Main);
	obj->Data1->Position = *pos;
	obj->Data1->Rotation = *dir;
	obj->Data1->Object = model;
	obj->DisplaySub = ObjectBreaker_Display;

	Collision_Init(obj, &ObjBrkr_Col, 1, 4u);

	if (tex) {
		obj->Data1->LoopData = (Loop*)tex;
	}
}

void LoadObjectBreaker(NJS_VECTOR* pos, Rotation3* rot, NJS_OBJECT* obj, NJS_TEXLIST* tex) {
	ObjectMaster* temp = LoadObject(LoadObj_Data1, 5, ObjectBreaker);
	temp->Data1->Position = *pos;
	temp->Data1->Rotation = *rot;
	temp->Data1->Object = obj;
	temp->Data1->LoopData = (Loop*)tex;
}

void Objects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	CommonObjects_Init(path, helperFunctions, config);
	WriteJump((void*)0x004CD370, ObjectData2_SetStartPosition_r);

	WriteData((SETObjData**)0x0046B817, &set_table[0]);
	WriteData((SETObjData**)0x0046BAC8, &set_table[0]);
	WriteData((SETObjData**)0x0059230A, &set_table[0]);
	WriteData((SETObjData**)0x0046BD83, &set_table[0]);
	WriteData((short**)0x0046BE6E, &set_table[0].Flags);
	WriteData((SETObjData**)0x0046BE73, &set_table[0]);
	WriteData((short**)0x0046BE0C, &set_table[0].Flags);
	WriteData((short**)0x0046BF4D, &set_table[0].Flags);
	WriteData((ObjectMaster***)0x0059222D, &set_table[0].ObjInstance);
	WriteData((short**)0x0046C1DB, &set_table[SET_COUNT].Flags);

	WriteData((int*)0x0046BD64, SET_COUNT);
	WriteData((int*)0x0046BD6C, SET_COUNT);
	WriteData((int*)0x0046BF3D, SET_COUNT);
	WriteData((int*)0x0046BF44, SET_COUNT);
	WriteData((int*)0x0046C1E0, (SET_COUNT + 1));
}