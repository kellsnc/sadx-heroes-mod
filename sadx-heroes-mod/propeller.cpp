#include "stdafx.h"

NJS_OBJECT* PropellerModel = nullptr;

CollisionData Prop_col {
	0, 0, 0x77, 0, 0x800400,{ 0, -21, 0 },{ 1, 1, 1 }, 0, 0
};

enum PropellerActions {
	PropellerAction_Init,
	PropellerAction_CheckForPlayer,
	PropellerAction_Attached,
	PropellerAction_CheckUpAndDown,
	PropellerAction_Detach
};

NJS_VECTOR Propeller_GetHandlePoint(EntityData1* data, CharObj2* co2) {
	NJS_VECTOR point;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &data->Position);
	njRotateY(0, data->Rotation.y + 0x4000);
	njRotateZ(0, data->Rotation.z);
	njRotateX(0, data->Rotation.x);
	njTranslate(0, 0, -20 - co2->PhysicsData.CollisionSize + data->Scale.z, 0);
	njGetTranslation(_nj_current_matrix_ptr_, &point);
	njPopMatrix(1u);
	return point;
}

void Propeller_SetPlayerStatus(EntityData1* entity, CharObj2* co2) {
	switch (entity->CharID) {
	case Characters_Sonic:
		if ((co2->Upgrades & Upgrades_SuperSonic) == 0) co2->AnimationThing.Index = 80;
		else  co2->AnimationThing.Index = 141;
		break;
	case Characters_Tails: co2->AnimationThing.Index = 100; break;
	case Characters_Knuckles: co2->AnimationThing.Index = 110; break;
	case Characters_Amy: co2->AnimationThing.Index = 80; break;
	}

	co2->Speed = { 0, 0, 0 };
	entity->Status = 0;
}

void Propeller_SetPlayerFalling(EntityData1* entity, CharObj2* co2) {
	switch (entity->CharID) {
	case Characters_Sonic:
		if ((co2->Upgrades & Upgrades_SuperSonic) == 0) co2->AnimationThing.Index = 18;
		else  co2->AnimationThing.Index = 141;
		entity->Action = 8;
		break;
	case Characters_Amy: 
		co2->AnimationThing.Index = 18;
		entity->Action = 8; 
		break;
	case Characters_Gamma: 
		co2->AnimationThing.Index = 16;
		entity->Action = 4;
		break;
	default:
		co2->AnimationThing.Index = 19;
		entity->Action = 6;
		break;
	}
}

void Propeller_Delete(ObjectMaster* obj) {
	if (obj->Parent) {
		obj->Parent->Data1->Action = PropellerAction_CheckForPlayer;
	}
}

void Propeller_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(0, data->Rotation.x, -data->Rotation.y - 0x4000, data->Rotation.z);
		njTranslate(0, 0, -21 + data->Scale.z, 0);

		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njRotateY(0, data->Rotation.y + 0x4000);
		njRotateY(0, data->Scale.y);
		njDrawModel_SADX(data->Object->child->child->basicdxmodel);

		njPopMatrix(1u);
	}
}

void Propeller_Main(ObjectMaster* obj) {
	if (!ClipSetObject(obj)) {
		EntityData1* data = obj->Data1;
		EntityData1* player = EntityData1Ptrs[data->CharIndex];
		CharObj2* co2 = CharObj2Ptrs[data->CharIndex];
		LoopHead* path = (LoopHead*)data->LoopData;
		int point = data->InvulnerableTime;

		obj->DisplaySub(obj);

		PathHandler(data, path, 5);

		data->Scale.y += 0x500;

		if (point < path->Count - 2 && data->Action != PropellerAction_Detach) {
			Propeller_SetPlayerStatus(player, co2);
			player->Position = Propeller_GetHandlePoint(data, co2);
			player->Rotation = data->Rotation;

			switch (data->Action) {
			case PropellerAction_Init:
				if (data->Scale.z < 21) {
					data->Scale.z += 0.1f;
					data->Action = PropellerAction_CheckUpAndDown;
				}
				break;
			case PropellerAction_CheckUpAndDown:
				if (PressedButtons[data->CharIndex] & Buttons_A) {
					data->Action = PropellerAction_Detach;
					Propeller_SetPlayerFalling(player, co2);
					return;
				}

				if (HeldButtons2[data->CharIndex] & Buttons_Up) {
					data->Scale.z = 81;
				}
				else if (HeldButtons2[data->CharIndex] & Buttons_Down) {
					data->Scale.z = -41;
				}
				else {
					data->Scale.z = 21;
				}

				break;
			}
		}
		else {
			data->Scale.z += 3;

			if (point >= path->Count - 2) {
				Propeller_SetPlayerFalling(player, co2);
			}

			if (point == path->Count) {
				DeleteObject_(obj);
				return;
			}
		}
	}
}

inline void Propeller_Init(ObjectMaster* obj, char player) {
	ObjectMaster* child = LoadChildObject(LoadObj_Data1, Propeller_Main, obj);
	child->Data1->CharIndex = player;
	child->Data1->LoopData = obj->Data1->LoopData;
	child->Data1->Position = obj->Data1->Position;
	child->Data1->Object = obj->Data1->Object;
	child->DisplaySub = Propeller_Display;
	child->DeleteSub = Propeller_Delete;
	PlayHeroesSound_EntityAndVolume(CommonSound_Propeller, child, 500, 5, true);
}

void PropellerPath_Display(ObjectMaster* obj) {
	if (!MissedFrames && IsPlayerInsideSphere(&obj->Data1->Position, 3000)) {
		EntityData1* data = obj->Data1;

		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslate(0, data->Position.x, data->Object->pos[1], data->Position.z);
		njRotateXYZ(0, data->Rotation.x, -data->Rotation.y - 0x4000, data->Rotation.z);

		njDrawModel_SADX(data->Object->basicdxmodel);

		njPopMatrix(1);

		if (data->Action == PropellerAction_CheckForPlayer) {
			Propeller_Display(obj);
		}
	}
}

void PropellerPath_DynCol(ObjectMaster* obj) {
	if (IsPlayerInsideSphere_(&obj->Data1->Position, 50)) {
		if (!obj->field_30) {
			EntityData1* data = obj->Data1;
			NJS_OBJECT* colobject;
			
			colobject = ObjectArray_GetFreeObject();

			colobject->scl[0] = 1.0;
			colobject->scl[1] = 1.0;
			colobject->scl[2] = 1.0;

			colobject->pos[0] = data->Position.x;
			colobject->pos[1] = obj->Data1->Object->pos[1];
			colobject->pos[2] = data->Position.z;
			colobject->ang[0] = data->Rotation.x;
			colobject->ang[2] = data->Rotation.z;

			colobject->basicdxmodel = obj->Data1->Object->basicdxmodel;
			obj->field_30 = (int)colobject;

			DynamicCOL_Add(ColFlags_Solid, obj, colobject);
		}
	}
	else if (obj->field_30){
		DynamicCOL_Remove(obj, (NJS_OBJECT*)obj->field_30);
		ObjectArray_Remove((NJS_OBJECT*)obj->field_30);
		obj->field_30 = 0;
	}
}

void PropellerPath(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	LoopHead* path = (LoopHead*)data->LoopData;

	char player = 0;

	switch (data->Action) {
	case PropellerAction_Init:
		if (GameState == 15) {
			obj->DisplaySub = PropellerPath_Display;
			
			data->Action = PropellerAction_CheckForPlayer;
			data->Position = path->LoopList->Position;
			data->Rotation.y = fPositionToRotation(&path->LoopList->Position, &path->LoopList[1].Position).y;
			data->Object = PropellerModel;

			Collision_Init(obj, &Prop_col, 1, 2u);
		}

		break;
	case PropellerAction_CheckForPlayer:
		player = IsPlayerInsideSphere_(&obj->Data1->Position, 3000);

		if (player) {
			player -= 1;

			if (IsSpecificPlayerInSphere(&obj->Data1->Position, 300, player)) {
				if (!obj->field_30) {
					data->Object->pos[1] = GetGroundYPosition(data->Position.x, data->Position.y, data->Position.z, &data->Rotation);
				}

				PropellerPath_DynCol(obj);
				AddToCollisionList(data);
			}
			else {
				data->Object->pos[1] = obj->Data1->Position.y - 40;
			}
			
			NJS_VECTOR pos = { data->Position.x, data->Position.y - 21, data->Position.z };

			if (IsSpecificPlayerInSphere(&pos, 10, player)) {
				Propeller_Init(obj, player);
				data->Action = PropellerAction_Attached;
			}

			obj->DisplaySub(obj);
		}

		break;
	case PropellerAction_Attached:
		RunObjectChildren(obj);
		obj->DisplaySub(obj);

		break;
	}
}