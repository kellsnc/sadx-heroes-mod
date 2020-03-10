#include "stdafx.h"
#include "egg-fleet-objects.h"

ModelInfo * EF_CANNON1;
ModelInfo * EF_CANNON2;
ModelInfo * EF_BULLETS;
ModelInfo * EF_PROPPLR;
ModelInfo * EF_BGSHIPS;
ModelInfo * EF_PLTFRMS;
ModelInfo * EF_PIPLINE;
ModelInfo * EF_ENDRAIL;
ModelInfo * EF_OBJSHIP;
ModelInfo * EF_ANTENNA;
ModelInfo * EF_BIGSHIP;
ModelInfo * EF_DIRSGNS;
ModelInfo * EF_SHPBRK1;
ModelInfo * EF_BARRIER;
ModelInfo * EF_CANBRK1;
ModelInfo * EF_CANBRK2;
ModelInfo * EF_CANDECO;
ModelInfo * EF_EBIGFAN;
ModelInfo * EF_EHELICE;

void LoadExplosion(NJS_VECTOR* position) {
	ObjectMaster* temp = LoadObject(LoadObj_Data1, 3, (ObjectFuncPtr)0x4AC920);
	temp->Data1->Position = *position;
	temp->MainSub(temp);
	temp->Data1->Action = 1;
	temp->Data1->InvulnerableTime = 150;
	temp->Parent = LoadObject(LoadObj_Data1, 3, (ObjectFuncPtr)0x4AC920);
	temp->Parent->Data1->Position = { 9999999, 9999999, 9999999 };
}

void EFBullet_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 1) {
			njSetTexture(CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Position);
			njRotateXYZ(nullptr, a1->Data1->Rotation.y, a1->Parent->Data1->Rotation.y, 0);
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
			njDrawModel_SADX(a1->Data1->Object->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);

			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Scale);
			njRotateXYZ(nullptr, a1->Data1->Rotation.x, 0, a1->Data1->Rotation.z);
			njScale(0, 0.3f, 0.3f, 0.3f);
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(a1->Data1->Object->child->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void EFBullet(ObjectMaster* a1) {
	NJS_VECTOR temp;

	switch (a1->Data1->Action) {
	case 0:
		a1->DisplaySub = EFBullet_Display;
		a1->Data1->Object = EF_BULLETS->getmodel();
		a1->Data1->Action = 1;
		a1->Data1->Rotation.y = 0;
		Collision_Init(a1, (CollisionData*)0x223B3D8, 1, 4u);
		break;
	case 1:
		if (++a1->Data1->field_A > 105) {
			LoadExplosion(&a1->Data1->Position);
			DeleteObject_(a1);
			return;
		}
		else {
			temp = a1->Data1->Scale;
			temp.y = a1->Data1->Position.y + 100;
			temp = GetPathPosition(&temp, &a1->Data1->Scale, (float)a1->Data1->field_A / 100);

			a1->Data1->Position = GetPathPosition(&a1->Data1->Position, &temp, (float)a1->Data1->field_A / 800);
			a1->Data1->Rotation.y = -fPositionToRotation(&a1->Data1->Position, &temp).x;

			DrawShadow(a1->Data1, 4);

			AddToCollisionList(a1->Data1);
		}

		break;
	}

	a1->DisplaySub(a1);
}

void EFCannon_Display(ObjectMaster* a1) {
	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		njTranslate(0, 0, 40, 0);
		njDrawModel_SADX(a1->Data1->Object->child->basicdxmodel);
		njTranslate(0, 0, 20, 0);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, 0, 0);
		njDrawModel_SADX(a1->Data1->Object->child->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);

		if (a1->Data1->Scale.x && a1->Data1->InvulnerableTime && IsPlayerInsideSphere(&a1->Data1->Position, 400)) {
			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Scale);
			Rotation3* rot = &EntityData1Ptrs[a1->Data1->CharIndex]->Rotation;
			njRotateXYZ(0, rot->x, 0, rot->z);
			njScale(0, 0.3f, 0.3f, 0.3f);
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(EF_BULLETS->getmodel()->child->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void EFCannon_Main(ObjectMaster* a1) {
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 50, 0);
		
		uint8_t player = IsPlayerInsideSphere(&a1->Data1->Position, 200);
		if (player) {
			a1->Data1->CharIndex = player - 1;
			NJS_VECTOR* playerpos = &EntityData1Ptrs[player - 1]->Position;

			if (a1->Data1->Action == 0) {
				if (++a1->Data1->InvulnerableTime < 150) {
					// cannon face front
					if (a1->Data1->Rotation.x != 0) a1->Data1->Rotation.x -= 10;

					// rotate towards player
					a1->Data1->field_A += 1;

					if (a1->Data1->field_A == 100 || !a1->Data1->Scale.x) {
						a1->Data1->Scale = *playerpos;
						a1->Data1->field_A = 0;
					}
					else {
						a1->Data1->Scale = GetPathPosition(&a1->Data1->Scale, playerpos, (float)a1->Data1->field_A / 120);
					}

					a1->Data1->Scale.y = CharObj2Ptrs[player - 1]->_struct_a3.DistanceMax + 0.3;

					a1->Data1->Rotation.y = -fPositionToRotation(&a1->Data1->Position, &a1->Data1->Scale).y + 0x4000;
				}
				else {
					a1->Data1->InvulnerableTime = 0;
					a1->Data1->Action = 1;
				}
			}
		}
		else {
			a1->Data1->InvulnerableTime = 0;
		}

		if (a1->Data1->Action > 0) {
			switch (a1->Data1->Action) {
			case 1:
				if (a1->Data1->Rotation.x >= -5000) a1->Data1->Rotation.x -= 100;
				else {
					if (EntityData1Ptrs[player - 1]) {
						LoadChildObject(LoadObj_Data1, EFBullet, a1);
						a1->Child->Data1->Position = a1->Data1->Position;
						a1->Child->Data1->Position.y += 60;
						a1->Child->Data1->Rotation = EntityData1Ptrs[player - 1]->Rotation;
						a1->Child->Data1->Scale = a1->Data1->Scale;
						a1->Data1->Scale.x = 0;
						a1->Data1->Action = 2;
					}
					else {
						a1->Data1->Action = 0;
					}
				}
				break;
			case 2:
				if (a1->Child) RunObjectChildren(a1);
				else {
					if (a1->Data1->Rotation.x <= 0) a1->Data1->Rotation.x += 50;
					else {
						a1->Data1->Action = 0;
					}
				}

				break;
			}
		}

		
		a1->DisplaySub(a1);
	}
}

void EFCannon(ObjectMaster* a1) {
	a1->Data1->Rotation.x = 1000;
	a1->Data1->Object = EF_CANNON1->getmodel();

	a1->DisplaySub = EFCannon_Display;
	a1->MainSub = EFCannon_Main;
}

enum EFPlatforms_Actions {
	EFPlatformAction_Init,
	EFPlatformAction_Move,
	EFPlatformAction_Reach,
	EFPlatformAction_Still
};

NJS_VECTOR Platform_GetPoint(NJS_VECTOR* orig, Rotation3* rot, float dest) {
	NJS_VECTOR point;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, orig);
	if (rot) njRotateXYZ(0, rot->x, rot->y, rot->z);
	njTranslate(0, 0, dest, 0);
	njGetTranslation(_nj_current_matrix_ptr_, &point);
	njPopMatrix(1u);

	return point;
}

void ECPlatforms_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFPLatforms_Move(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	
	float dist = GetDistance(&data->Position, &data->Scale);
	dist = 20 - dist;
	data->field_A += 5 + dist;

	float pos = (float)data->field_A / 100;
	
	if (pos >= 60) {
		data->field_A = -2000;
		pos = -20;
	}
	else if (pos > 20) {
		pos = -pos + 40;
	}

	data->Position = Platform_GetPoint(&data->Scale, &data->Rotation, pos);

	DynCol_Update(data, 1);
}

void EFPlatforms_Reach(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	float dist = GetDistance(&data->Position, &data->Scale);
	dist = 60 - dist;
	data->field_A += 5 + dist;

	float pos = (float)data->field_A / 100;

	if (pos >= 60) {
		data->Action = EFPlatformAction_Still;
	}

	data->Position = Platform_GetPoint(&data->Scale, &data->Rotation, pos);

	DynCol_Update(data, 1);
}

void EFPlatforms(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;

	EntityData1* data = obj->Data1;

	switch (data->Action) {
	case EFPlatformAction_Init:
		data->Object = EF_PLTFRMS->getmodel();
		data->Object->basicdxmodel->r = 80;

		if (data->Scale.x == 1) {
			data->Object = data->Object->child;
			data->Object->basicdxmodel->r = 100;
			data->Scale.x = 0;
		}
		
		if (data->Scale.x == 2) {
			data->Action = EFPlatformAction_Reach;
		}
		else {
			data->Action = EFPlatformAction_Move;
		}

		obj->DisplaySub = ECPlatforms_Display;
		obj->DeleteSub = DynCol_Delete;

		if (data->Scale.y == 1) {
			data->Action = EFPlatformAction_Still;
			data->Scale.y = 0;
		}
		else {
			data->Scale = data->Position;
		}
		
		if (data->Action == EFPlatformAction_Move){
			data->field_A = (short)(rand() % 40 * 100);
			data->Position = Platform_GetPoint(&data->Scale, &data->Rotation, (float)data->field_A / 100);
		}

		return;
	case EFPlatformAction_Move:
		EFPLatforms_Move(obj);
		break;
	case EFPlatformAction_Reach:
		EFPlatforms_Reach(obj);
		break;
	}

	DynColRadiusAuto(obj, 1);
	obj->DisplaySub(obj);
}

void EFPipeline_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFPipeLine_Collision(ObjectMaster* obj) {
	DynColRadiusAuto(obj, 0);
}

void EFPipeline(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_PIPLINE) return;

	EntityData1* data = obj->Data1;
	
	if (data->Action == 0) {
		obj->DisplaySub = EFPipeline_Display;
		data->Object = EF_PIPLINE->getmodel();
		data->Object->basicdxmodel->r = 300;
		data->Object->child->basicdxmodel->r = 300;
		data->Action = 1;
		
		ObjectMaster* child = LoadChildObject(LoadObj_Data1, EFPipeLine_Collision, obj);
		child->Data1->Position = data->Position;
		child->Data1->Rotation = data->Rotation;
		child->Data1->Object = data->Object->child;
		child->DeleteSub = DynCol_Delete;
	} 
	else {
		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

void EFRailends(ObjectMaster* obj) {
	if (obj->field_30 != 1) {
		obj->DisplaySub = obj->MainSub;
		obj->field_30 = 1;
	}

	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(EggFleet_RailEnds); ++i) {
			SOI_LIST item = EggFleet_RailEnds[i];
			
			if (IsPlayerInsideSphere_(&item.Position, 5000.0f)) {
				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);

				if (item.DrawDistance == 1) {
					EF_ENDRAIL->getmodel()->basicdxmodel->mats->attr_texId = 30;
				}
				else if (item.DrawDistance == 2) {
					EF_ENDRAIL->getmodel()->basicdxmodel->mats->attr_texId = 34;
				}

				njDrawModel_SADX(EF_ENDRAIL->getmodel()->child->basicdxmodel);
				njDrawModel_SADX(EF_ENDRAIL->getmodel()->basicdxmodel);

				njPopMatrix(1u);
			}
		}
	}
}

enum EFDoorActions {
	EFDoorAction_Init,
	EFDoorAction_CloseDoor,
	EFDoorAction_Closed,
	EFDoorAction_OpenDoor,
	EFDoorAction_Opened
};

void EFShipDoor_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njTranslate(0, 0, data->Scale.y, 0);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFShipDoor_Collision(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* parent = obj->Parent->Data1;
	
	switch (parent->Action) {
	case EFDoorAction_CloseDoor:
	case EFDoorAction_OpenDoor:
		data->Position = Platform_GetPoint(&parent->Position, &parent->Rotation, parent->Scale.y);
		DynCol_Update(obj->Data1, 1);
		break;
	}

	DynColRadiusAuto(obj, 1);
}

void EFShipDoor(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;

	EntityData1* data = obj->Data1;
	
	switch (data->Action) {
	case EFDoorAction_Init:
		obj->DisplaySub = EFShipDoor_Display;
		data->Object = EF_OBJSHIP->getmodel();
		data->Object->basicdxmodel->r = 200;
		data->Object->child->basicdxmodel->r = 150;
		data->Scale.y = -159;

		LoadChildObject(LoadObj_Data1, EFShipDoor_Collision, obj);
		obj->Child->Data1->Position = data->Position;
		obj->Child->Data1->Rotation = data->Rotation;
		obj->Child->Data1->Object = data->Object->child;
		obj->Child->DeleteSub = DynCol_Delete;
		
		if (obj->SETData.SETData->SETEntry->Properties.x == 1) {
			data->Action = EFDoorAction_Opened;
			obj->Child->Data1->Position = Platform_GetPoint(&data->Position, &data->Rotation, data->Scale.y);
		}
		else {
			data->Action = EFDoorAction_CloseDoor;
		}

		return;
	case EFDoorAction_CloseDoor:
		if (IsPlayerInsideSphere_(&data->Position, 800) == 0) break;

		if (data->Scale.y < 0) {
			data->Scale.y += 3;
		}
		else {
			data->Action = EFDoorAction_Closed;
			data->Scale.y = 0;
		}

		break;
	case EFDoorAction_Closed:
		if (PressedButtons[0] & Buttons_C) {
			data->Action = EFDoorAction_OpenDoor;
			obj->SETData.SETData->SETEntry->Properties.x = 1;
		}

		break;
	case EFDoorAction_OpenDoor:
		if (data->Scale.y > -159) {
			data->Scale.y -= 1;
		}
		else {
			data->Action = EFDoorAction_Opened;
			data->Scale.y = -158.5;
		}

		break;
	}

	DynColRadiusAuto(obj, 0);
	obj->DisplaySub(obj);
	RunObjectChildren(obj);
}

enum EFConveyorActions {
	EFConveyorAction_Init,
	EFConveyorAction_Run,
	EFConveyorAction_Destroy,
	EFConveyorAction_Destroyed
};

void EFShipConveyor_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFShipConveyor(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;

	EntityData1* data = obj->Data1;

	switch (data->Action) {
	case EFConveyorAction_Init:
		obj->DisplaySub = EFShipConveyor_Display;
		data->Object = EF_OBJSHIP->getmodel()->child->child;
		data->Object->basicdxmodel->r = 500;

		if (obj->SETData.SETData->SETEntry->Properties.y == 1) {
			data->Action = EFConveyorAction_Destroyed;
			obj->Child->Data1->Position = Platform_GetPoint(&data->Position, &data->Rotation, data->Scale.y);
		}
		else {
			data->Action = EFConveyorAction_Run;
		}

		return;
	case EFConveyorAction_Run:
		data->CharIndex = IsPlayerInsideSphere_(&data->Position, 400);

		if (data->CharIndex) {
			data->CharIndex -= 1;
			EntityData1* player = EntityData1Ptrs[data->CharIndex];

			//check boundaries without a collision because it's always facing -z
			if (player->Position.z < data->Position.z - 30 &&
				player->Position.y > data->Position.y - 2 &&
				player->Position.y < data->Position.y + 20 &&
				player->Position.x > data->Position.x - 37 &&
				player->Position.x < data->Position.x + 37) {
				
				if (CharObj2Ptrs[data->CharIndex]->SurfaceFlags & ColFlags_Solid) {
					player->Position.z += 3; //push the player back
				}
				
				if (player->Position.z < data->Position.z - 350) {
					if (data->Scale.x == 0) {
						player->Position = { -7422, 3714, -13470 };
						data->Scale = { -7899.938, 3433.604, -13462.01 };
					}
					else {
						player->Position = { -9497, -1835, -45280 };
						data->Scale = { -9497, -1260, -48034 };
					}

					SwapChunk("EF", 7);
					data->Action = EFConveyorAction_Destroy;
					return;
				}
			}
		}

		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);

		break;
	case EFConveyorAction_Destroy:
		EntityData1* player = EntityData1Ptrs[data->CharIndex];
		TransformSpline(player, player->Position, data->Scale, (float)++data->field_A / 100);

		break;
	}
}

void EFAntenna_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;
		
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		
		if (data->Scale.x == 1) {
			njDrawModel_SADX(data->Object->basicdxmodel);
			njRotateY(nullptr, data->Scale.y);
			njDrawModel_SADX(data->Object->child->basicdxmodel);
		}
		else {
			njRotateY(nullptr, data->Scale.y);
			njDrawModel_SADX(data->Object->basicdxmodel);
		}

		njPopMatrix(1);
	}
}

void EFAntenna(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_ANTENNA) return;
	
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DisplaySub = EFAntenna_Display;
		data->Object = EF_ANTENNA->getmodel();
		data->Object->basicdxmodel->r = 50;

		if (data->Scale.x == 1) {
			data->Object = data->Object->child;
			data->Object->basicdxmodel->r = 50;
		}

		data->Action = 1;
	}
	else {
		data->Scale.y += 200;
		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);
	}
}

void EFRailSign_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);

		njDrawModel_SADX(data->Object->basicdxmodel);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njDrawModel_SADX(data->Object->child->child->basicdxmodel);
		njDrawModel_SADX(data->Object->child->child->child->basicdxmodel);

		njPopMatrix(1);
	}
}

void EFRailSign(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_DIRSGNS) return;

	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DisplaySub = EFRailSign_Display;
		data->Object = EF_DIRSGNS->getmodel()->child;
		data->Object->basicdxmodel->r = 50;

		if (data->Scale.x == 1) {
			data->Object = data->Object->sibling;
			data->Object->basicdxmodel->r = 50;
		}

		data->Action = 1;
	}
	else {
		if (anim % 10 == 0) {
			data->Unknown++;
			if (data->Unknown == 3) data->Unknown = 0;
		}

		NJS_OBJECT* test = data->Object->child;
		int i = 0;
		while (i < 3) {
			
			if (i == data->Unknown) {
				test->basicdxmodel->mats->attr_texId = 33;
				test->basicdxmodel->mats->attrflags |= NJD_FLAG_IGNORE_LIGHT;
			}
			else {
				test->basicdxmodel->mats->attr_texId = 32;
				test->basicdxmodel->mats->attrflags &= ~NJD_FLAG_IGNORE_LIGHT;
			}

			++i;

			if (test->child) {
				test = test->child;
			}
		}

		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);
	}
}

enum EFShipTypes {
	EFShipType_LargeShip,
	EFShipType_FarMoveTopR,
	EFShipType_FarMoveSideR,
	EFShipType_FarMoveTopL,
	EFShipType_FarMoveSideL,
	EFShipType_FarMoveBig
};

void EFBgShips(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (data->Action != 1) {
		obj->DisplaySub = obj->MainSub;
		data->Object = EF_BGSHIPS->getmodel();
		data->Action = 1;
	}

	if (GameState != 16) {
		data->Position.z += 0.5f;
	}

	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(EggFleet_Ships); ++i) {
			SOI_LIST item = EggFleet_Ships[i];

			if ((item.Chunk == 0 || item.Chunk == CurrentChunk)) {
				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njTranslate(nullptr, 0, 0, data->Position.z);

				if (item.DrawDistance != EFShipType_LargeShip) {
					DisableFog();
				}

				switch ((int)item.DrawDistance) {
				case EFShipType_LargeShip:
					if (IsPlayerInsideSphere_(&item.Position, 10000.0f)) {
						DrawObject(EF_BIGSHIP->getmodel());
					}
					break;
				/*case EFShipType_FarMoveTopR:
					njDrawModel_SADX(data->Object->basicdxmodel);
					break;
				case EFShipType_FarMoveSideR:
					njDrawModel_SADX(data->Object->child->basicdxmodel);
					break;
				case EFShipType_FarMoveTopL:
					njDrawModel_SADX(data->Object->child->child->basicdxmodel);
					break;
				case EFShipType_FarMoveSideL:
					njDrawModel_SADX(data->Object->child->child->child->basicdxmodel);
					break;
				case EFShipType_FarMoveBig:
					njDrawModel_SADX(data->Object->child->child->child->child->basicdxmodel);
					break;*/
				}

				ToggleStageFog();
				njPopMatrix(1u);
			}
		}
	}
}

void EFBigFans_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njRotateY(0, data->Scale.z);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njRotateY(0, -data->Scale.z);
		njDrawModel_SADX(data->Object->child->child->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFBigFans_Collision(ObjectMaster* obj) {
	DynColRadiusAuto(obj, 0);
}

void EFBigFans(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_EBIGFAN) return;

	EntityData1* data = obj->Data1;

	if (data->Scale.y != 1) {
		obj->MainSub = ObjFan;
		return;
	}

	if (data->Action == 0) {
		Collision_Init(obj, BigFans_col, 2, 1);
		obj->DisplaySub = EFBigFans_Display;
		data->Object = EF_EBIGFAN->getmodel();
		data->Object->basicdxmodel->r = 300;
		data->Action = 1;

		ObjectMaster* child = LoadChildObject(LoadObj_Data1, EFBigFans_Collision, obj);
		child->Data1->Position = data->Position;
		child->Data1->Rotation = data->Rotation;
		child->Data1->Object = data->Object->child->child;
		child->Data1->Object->basicdxmodel->r = 300;
		child->DeleteSub = DynCol_Delete;
	}
	else {
		NJS_VECTOR temp = { data->Position.x, data->Position.y - 34, data->Position.z };
		data->CharIndex = Fans_IsPlayerInCylinder(&temp, 130, data->Scale.x);
		if (data->CharIndex) {
			LoadChildObject(LoadObj_Data1, Fans_HandlePlayer, obj);
			obj->Child->Data1->CharIndex = data->CharIndex - 1;
			obj->Child->Data1->Position = temp;
			obj->Child->Data1->Scale.x = data->Scale.x;
			obj->Child->Data1->Scale.y = 130;
		}

		data->Scale.z -= data->Scale.x * 80;

		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

PVMEntry EggFleetObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ "GOMA", &GOMA_TEXLIST },
	{ "PEN", &PEN_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "WARA", &WARA_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "LION", &LION_TEXLIST },
	{ "GORI", &GORI_TEXLIST },
	{ "SUKA", &SUKA_TEXLIST },
	{ "RAKO", &RAKO_TEXLIST },
	{ 0 }
};

ObjectListEntry EggFleetObjectList_list[] = {
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Ring_Main, "RING   " },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, Spring_Main, "SPRING " },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, SpringB_Main, "SPRINGB" },
	{ LoadObj_Data2 | LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, DashPanel_Main, "O AXPNL" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SwingSpikeBall_Load, "O IRONB" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, FallingSpikeBall_Load, "O FeBJG" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Spikes_Main, "O TOGE" },
	{ LoadObj_Data2 | LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, EmeraldPiece_Load, "O EME P" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Capsule_Load, "O RELEASE" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Switch_Main, "O SWITCH" },
	{ LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, Weed_Main, "CMN KUSA" },
	{ LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, DashHoop_Main, "CMN_DRING" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Balloon_Main, "O BALOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, ItemBox_Main, "O ITEMBOX" },
	{ LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Common, DistObj_UseDist, 360000, 0, RocketH_Main, "Rocket H" },
	{ LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Common, DistObj_UseDist, 360000, 0, RocketHS_Main, "Rocket HS" },
	{ LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Common, DistObj_UseDist, 360000, 0, RocketV_Main, "Rocket V" },
	{ LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Common, DistObj_UseDist, 360000, 0, RocketVS_Main, "Rocket VS" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 4000000, 0, JumpPanel_Load, "O JPanel" },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_6, DistObj_UseDist, 360000, 0, CheckPoint_Main, "O Save Point" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Wall_Main, "WALL   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, RingGroup_Main, "O_GRING" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, CSphere, "C SPHERE" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, ColCylinder_Main, "C CYLINDER" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, ColCube_Main, "C CUBE" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, Ottotto_Main, "OTTOTTO" },
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 360000, 0, TikalHint_Load, "O TIKAL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Kiki_Load, "E SARU  " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, RhinoTank_Main, "E SAITO" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Sweep_Load, "E AMEMB" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OJump, "O JUMP   " },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, ItemBoxAir_Main, "O ItemBoxAir" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerA_Main, "SPINA A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerB_Main, "SPINA B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerC_Main, "SPINA C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, SpringH_Load, "O SPRING H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO_WOODBOX" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjCannon, "CANNON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, OBJCASE, "OBJ CASE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjReel, "OBJREEL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 4060000, 0, EFBigFans, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, EFCannon, "EFCannon" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFPlatforms, "EFPlatforms" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFPipeline, "EFPipeline" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFShipDoor, "EFShipDoor" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, EFShipConveyor, "EFShipConveyor" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, EFAntenna, "EFAntenna" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, EFRailSign, "EFRailSign" },
};
ObjectList EggFleetObjectList = { arraylengthandptrT(EggFleetObjectList_list, int) };

void EggFleetObjects_Init() {
	TexLists_Obj[HeroesLevelID_EggFleet] = EggFleetObjectTextures;
	ObjLists[HeroesLevelID_EggFleet * 8] = &EggFleetObjectList;
}