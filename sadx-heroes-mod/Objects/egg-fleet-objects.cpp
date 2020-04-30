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
ModelInfo * EF_SHPBRK2;
ModelInfo * EF_BARRIER;
ModelInfo * EF_CANBRK1;
ModelInfo * EF_CANBRK2;
ModelInfo * EF_CANDECO;
ModelInfo * EF_EBIGFAN;
ModelInfo * EF_EHELICE;

NJS_VECTOR Entity_GetPoint(NJS_VECTOR* orig, Rotation3* rot, float xpos, float ypos, float zpos) {
	NJS_VECTOR point;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, orig);
	njRotateXYZ(0, rot->x, rot->y, rot->z);
	njTranslate(0, xpos, ypos, zpos);
	njGetTranslation(_nj_current_matrix_ptr_, &point);
	njPopMatrix(1u);

	return point;
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
			data->Object->basicdxmodel->r = 1000;
			data->Scale.x = 0;
		}
		
		if (data->Scale.x == 2) {
			data->Action = EFPlatformAction_Reach;
		}
		else {
			data->Action = EFPlatformAction_Move;
		}

		if (data->Scale.z == 1) {
			data->CharID = data->Scale.z;
			data->Scale.z = 0;
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

		if (data->CharID && ObjectInRange(&EntityData1Ptrs[0]->Position, data->Position.x, data->Position.y, data->Position.z, 168100)) {
			switch (data->CharID) {
			case 1:
				if (!obj->Child) {
					LoadChildObject(LoadObj_Data1, Spring_Main, obj)->Data1->Scale.y = 2;
				}

				obj->Child->Data1->Position = Entity_GetPoint(&data->Position, &data->Rotation, 0, 0, -50);
				break;
			case 2:
				if (!obj->Child) {
					LoadChildObject(LoadObj_Data1, Spring_Main, obj)->Data1->Scale.y = 3;
				}

				obj->Child->Data1->Position = Entity_GetPoint(&data->Position, &data->Rotation, 0, 10, 0);
				break;
			}
			
			RunObjectChildren(obj);
		}
		else {
			if (obj->Child) DeleteChildObjects(obj);
		}
		
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
		obj->DeleteSub = DynCol_Delete;
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
			
			if (EggFleet_RailEnds[i].Chunk > 0 && CurrentChunk != EggFleet_RailEnds[i].Chunk) continue;

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
		obj->DeleteSub = DynCol_Delete;
		data->Object = EF_OBJSHIP->getmodel();
		data->Object->basicdxmodel->r = 200;
		data->Object->child->basicdxmodel->r = 150;
		data->Scale.y = -159;

		LoadChildObject(LoadObj_Data1, EFShipDoor_Collision, obj);
		obj->Child->Data1->Position = data->Position;
		obj->Child->Data1->Rotation = data->Rotation;
		obj->Child->Data1->Object = data->Object->child;
		obj->Child->DeleteSub = DynCol_Delete;
		
		if (IsSwitchPressed(data->Scale.x)) {
			data->Action = EFDoorAction_Opened;
			obj->Child->Data1->Position = Platform_GetPoint(&data->Position, &data->Rotation, data->Scale.y);
		}
		else {
			data->Action = EFDoorAction_CloseDoor;
		}

		return;
	case EFDoorAction_CloseDoor:
		if (IsPlayerInsideSphere_(&data->Position, 800) == 0) break;

		if (data->Scale.y == -159) {
			PlayCustomSound_EntityAndVolume(LevelSound_Egg_Door, obj, 1200, 3, false);
		}

		if (data->Scale.y < 0) {
			data->Scale.y += 3;
		}
		else {
			data->Action = EFDoorAction_Closed;
			data->Scale.y = 0;
		}

		break;
	case EFDoorAction_Closed:
		if (IsSwitchPressed(data->Scale.x)) {
			data->Action = EFDoorAction_OpenDoor;
			PlayCustomSound_EntityAndVolume(LevelSound_Egg_Door, obj, 900, 3, false);
		}

		break;
	case EFDoorAction_OpenDoor:
		if (data->Scale.y > -159) {
			data->Scale.y -= 3;
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

void EFBigShipControlRoom_Display(ObjectMaster* obj) {
	if (MissedFrames) return;

	EntityData1* data = obj->Data1; 

	njSetTexture(CurrentLevelTexlist);

	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	Direct3D_PerformLighting(1);
	njDrawModel_SADX(data->Object->basicdxmodel);
	Direct3D_PerformLighting(0);
	njPopMatrix(1);
}

void EFBigShipControlRoom_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		DynColRadiusAuto(obj, 0);

		if (obj->Parent->Data1->Action == EFConveyorAction_Destroy) {
			data->Object = EF_SHPBRK1->getmodel()->child;
			NJS_VECTOR temp = { data->Position.x, data->Position.y + 300, data->Position.z - 600 };
			LoadObjectBreaker(&temp, &data->Rotation, EF_SHPBRK2->getmodel(), nullptr);
			PlayCustomSound(LevelSound_Egg_Boom1);
			PlayCustomSound(LevelSound_Egg_Boom2);

			data->Action = 1;
			DynCol_Delete(obj);
		}
	}

	obj->DisplaySub(obj);
}

void EFShipConveyor_Display(ObjectMaster* obj) {
	if (MissedFrames) return;
	
	EntityData1* data = obj->Data1;
	njSetTexture(CurrentLevelTexlist);

	if (IsPlayerInsideSphere_(&data->Position, 1500)) {
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njPopMatrix(1);
	}

	EFBigShipControlRoom_Display(obj);
}

void EFShipConveyor(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (ClipSetObject(obj)) return;

	if (data->Action == EFConveyorAction_Init) {
		obj->DisplaySub = EFShipConveyor_Display;
		obj->DeleteSub = DynCol_Delete;
		data->Object = EF_OBJSHIP->getmodel()->child->child;
		data->Object->basicdxmodel->r = 500;

		LoadChildObject(LoadObj_Data1, EFBigShipControlRoom_Main, obj);
		obj->Child->Data1->Object = EF_SHPBRK1->getmodel();
		obj->Child->Data1->Object->basicdxmodel->r = 2000;
		obj->Child->DisplaySub = EFBigShipControlRoom_Display;
		obj->Child->DeleteSub = DynCol_Delete;
		obj->Child->Data1->Position.z -= 3;
		data->Action = EFConveyorAction_Run;

		data->Index = 1;
	}
	else if (data->Action == EFConveyorAction_Run) {
		if (IsPlayerInsideSphere_(&data->Position, 1500) == 0) return;

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

				SetCameraMode_(1);

				if (CharObj2Ptrs[data->CharIndex]->SurfaceFlags & ColFlags_Solid) {
					player->Position.z += 3; //push the player back
				}

				if (data->CharIndex == 0 && player->Position.z < data->Position.z - 350) {
					if (data->Scale.x == 0) {
						player->Position = { -7422, 3714, -13470 };
						data->Scale = { -7900.307, 3418.483, -13492.21 };
					}
					else {
						player->Position = { -9497, -1835, -45280 };
						data->Scale = { -9497, -1260, -48034 };
						LoadLevelResults();
					}

					SetCameraMode_(0);
					SwapChunk(7);
					data->Action = EFConveyorAction_Destroy;
					DynCol_Delete(obj);
					return;
				}
			}
		}
		else {
			data->Index = GetCameraMode_();
		}

		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);
	}
	else if (data->Action == EFConveyorAction_Destroy) {
		EntityData1* player = EntityData1Ptrs[data->CharIndex];
		CharObj2Ptrs[0]->Speed = { 0, 0, 0 };
		player->Position = GetPathPosition(&player->Position, &data->Scale, 0.05f);
		player->Rotation.y = fPositionToRotation(&player->Position, &data->Scale).y;

		if (player->field_A > 100) {
			data->Action = EFConveyorAction_Destroyed;
			SetCameraMode_(data->Index);
		}
	}

	RunObjectChildren(obj);
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
		obj->DeleteSub = DynCol_Delete;
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
		obj->DeleteSub = DynCol_Delete;
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

		if (ChunkSwapped == true) {
			data->Position.z = 0;
		}
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
					if (CurrentChunk < 10 && IsPlayerInsideSphere_(&item.Position, 10000.0f)) {
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

void EFBigFans(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_EBIGFAN) return;

	EntityData1* data = obj->Data1;

	if (data->Scale.y != 1) {
		obj->MainSub = ObjFan;
		return;
	}

	if (data->Action == 0) {
		obj->DisplaySub = EFBigFans_Display;
		obj->DeleteSub = DynCol_Delete;
		data->Object = EF_EBIGFAN->getmodel();
		data->Object->basicdxmodel->r = 300;
		data->Action = 1;

		PlayCustomSound_Entity(LevelSound_Egg_Fan, obj, 500, true);

		ObjectMaster* child = LoadChildObject(LoadObj_Data1, mainSub_Dyncol, obj);
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

void EFMissilePods(ObjectMaster* obj) {
	obj->MainSub = mainSub_DyncolGlobal;
	obj->DeleteSub = DynCol_Delete;
	obj->DisplaySub = displaySub_Global;
	obj->Data1->Object = EF_CANDECO->getmodel();
	obj->Data1->Object->basicdxmodel->r = 50;
}

CollisionData EFHelice_Col[]{
	{ 0, 1, 0x77, 0, 0, {0, 0, 0}, { 100, 100, 0} },
	{ 0, 1, 0x77, 0, 0, {0, 0, 100}, { 100, 100, 0} },
};

void EFHelice_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njRotateZ(0, -data->Scale.z);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njDrawModel_SADX(data->Object->child->child->basicdxmodel);
		njPopMatrix(1);
	}
}

void EFHelice(ObjectMaster* obj) {
	if (ClipSetObject(obj) || !EF_EHELICE) return;

	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DisplaySub = EFHelice_Display;
		data->Object = EF_EHELICE->getmodel();
		data->Action = 1;

		PlayCustomSound_EntityAndVolume(LevelSound_Egg_Engines, obj, 1500, 3, true);

		Collision_Init(obj, EFHelice_Col, 2, 5);
	}
	else {
		if (data->Scale.x) {
			if (CurrentChunk != data->Scale.x) return;
		}

		data->Scale.z += 1200;
		AddToCollisionList(data);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

CollisionData EFBarrier_Col {
	0, 3, 0x77, 0, 0x800400, {0, 6, 0}, { 15, 15, 0}
};

void ECBarrier(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;

	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		data->Action = 1;
		data->Object = EF_BARRIER->getmodel();
		data->Object->basicdxmodel->r = 50;
		obj->DisplaySub = displaySub_Global;
		obj->DeleteSub = DynCol_Delete;
		AllocateObjectData2(obj, obj->Data1);
		Collision_Init(obj, &EFBarrier_Col, 1, 2);
	}
	else {
		EntityData1* colent = GetCollidingEntityA(data);
		EntityData1* colent2 = GetCollidingEntityB(data);

		AddToCollisionList(data);
		DynColRadiusAuto(obj, 0);
		obj->DisplaySub(obj);

		if (colent2 || (colent && colent->Status & Status_Attack)) {
			DynCol_Delete(obj);
			obj->DisplaySub = nullptr;
			obj->SETData.SETData->Flags = 1;
			DeleteObject_(obj);

			LoadObjectBreaker(&data->Position, &data->Rotation, data->Object->child, nullptr);
		}
	}
}

void EFBullet_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		if (data->Action == 1) {
			njSetTexture(CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslateV(0, &data->Position);
			njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
			if (data->Scale.x == 1) njScale(0, 0.5f, 0.5f, 0.5f);
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(data->Object->basicdxmodel);
			njDrawModel_SADX(data->Object->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);

			if (data->Unknown) {
				njPushMatrix(0);
				njTranslateV(0, &data->Scale);

				Rotation3 rot;
				GetGroundYPosition(data->Scale.x, data->Scale.y + 10, data->Scale.z, &rot);
				njRotateXYZ(nullptr, rot.x, 0, rot.z);

				njScale(0, 0.3f, 0.3f, 0.3f);
				DrawQueueDepthBias = -6000.0f;
				njDrawModel_SADX(data->Object->child->child->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void EFBullet(ObjectMaster* obj) {
	NJS_VECTOR temp;
	EntityData1* data = obj->Data1;

	switch (data->Action) {
	case 0:
		obj->DisplaySub = EFBullet_Display;
		data->Object = EF_BULLETS->getmodel();
		data->Action = 1;

		PlayCustomSound_Entity(CommonSound_Shoot2, obj, 500, false);
		
		Collision_Init(obj, (CollisionData*)0x223B3D8, 1, 4u);
		break;
	case 1:
		if (++data->field_A > 105 || GetCollidingEntityA(data)) {
			LoadExplosion(&data->Position);
			DeleteObject_(obj);
			return;
		}
		else {
			if (data->Scale.x == 1 || data->Scale.x == 2) {
				data->Position = GetPathPosition(&data->Position, &Entity_GetPoint(&data->Position, &data->Rotation, 0, 0, 50), (float)data->field_A / 800);
			}
			else {
				temp = data->Scale;
				temp.y = data->Position.y + 100;
				temp = GetPathPosition(&temp, &data->Scale, (float)data->field_A / 100);

				data->Position = GetPathPosition(&data->Position, &temp, (float)data->field_A / 800);
				data->Rotation.x = fPositionToRotation(&data->Position, &temp).x;
			}
		}

		break;
	}

	DrawShadow(data, 4);
	AddToCollisionList(data);
	obj->DisplaySub(obj);
}

enum EFCannonActions {
	EFCannonAction_Init,
	EFCannonAction_Seek,
	EFCannonAction_Shoot,
	EFCannonAction_Bullet,
};

CollisionData EFCannon1_Col[]{
	{ 0, 0, 0x77, 0, 0x800400, {0, 50, 0}, {25, 25, 0} },
	{ 0, 2, 0x77, 0, 0, {0, 0, 0}, {25, 50, 0} },
	{ 0, 2, 0x77, 0, 0, {0, 25, 0}, {25, 50, 0} }
};

CollisionData EFCannon2_Col {
	0, 3, 0x77, 0, 0x800400, {0, 0, 0}, {25, 25, 25}
};

void EFCannon2_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njTranslate(0, 0, 10, 0);
		njDrawModel_SADX(data->Object->child->basicdxmodel);
		njPopMatrix(1u);
	}
}

void EFCannon1_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (!MissedFrames) {
		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateX(0, data->Rotation.x);
		njRotateY(0, data->Rotation.y);
		
		if (!data->Unknown) {
			njDrawModel_SADX(data->Object->basicdxmodel);
		}

		njTranslate(0, 0, 40, 0);

		njDrawModel_SADX(data->Object->child->basicdxmodel);

		njTranslate(0, 0, 20, 0);

		njRotateX(nullptr, data->field_A);
		njDrawModel_SADX(data->Object->child->child->basicdxmodel);
		njPopMatrix(1u);

		if (data->Unknown == 0 && data->InvulnerableTime && IsPlayerInsideSphere(&data->Position, 400)) {
			njPushMatrix(0);
			njTranslateV(0, &data->Scale);
			Rotation3* rot = &EntityData1Ptrs[data->CharIndex]->Rotation;
			njRotateXYZ(0, rot->x, 0, rot->z);
			njScale(0, 0.3f, 0.3f, 0.3f);
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(EF_BULLETS->getmodel()->child->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

bool EFCannon_Explode(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (CheckObjectDamage(data)) {
		if (++obj->Data1->NextAction < 3) {
			PlayCustomSound_Pos(CommonSound_Explosion, &data->Position, 300, 2, false);
			return false;
		}

		ObjectMaster* temp = LoadObject(LoadObj_Data1, 5, ObjectBreaker);
		temp->Data1->Position = data->Position;
		temp->Data1->Rotation = data->Rotation;

		if (obj->DisplaySub == EFCannon1_Display) {
			temp->Data1->Object = EF_CANBRK1->getmodel()->child;
		}
		else {
			temp->Data1->Object = EF_CANBRK2->getmodel()->child;
		}

		PlayCustomSound_Pos(CommonSound_Explosion, &data->Position, 300, 5, false);

		DynCol_Delete(obj);
		obj->DisplaySub = nullptr;
		obj->SETData.SETData->Flags = 1;
		DeleteObject_(obj);

		return true;
	}

	return false;
}

void EFCannon2_Fire(EntityData1* data, float xpos) {
	ObjectMaster* bullet = LoadObject(LoadObj_Data1, 3, EFBullet);
	bullet->Data1->Position = Entity_GetPoint(&data->Position, &data->Rotation, xpos, 10, 80);
	bullet->Data1->Rotation = data->Rotation;
	bullet->Data1->Scale.x = 1;
}

void EFCannon1_Fire(EntityData1* data) {
	ObjectMaster* bullet = LoadObject(LoadObj_Data1, 3, EFBullet);
	bullet->Data1->Position = Entity_GetPoint(&data->Position, &data->Rotation, 0, 60, 10);
	bullet->Data1->Rotation = data->Rotation;
	bullet->Data1->Scale.x = 2;
}

void EFCanon_SetHeight(EntityData1* data) {
	float y = GetGroundYPosition(data->Position.x, data->Position.y + 20, data->Position.z, &data->Rotation);
	if (y > -10000) {
		data->Position.y = y;
	}
}

void EFCannon2_Main(ObjectMaster* obj) {
	if (ClipSetObject(obj) || EFCannon_Explode(obj)) return;

	EntityData1* data = obj->Data1;

	if (data->CharID == 1) EFCanon_SetHeight(data);

	switch (data->Action) {
	case EFConveyorAction_Run:
		//Detect if the player is in front of the cannon
		if (IsPlayerInsideSphere_(&Entity_GetPoint(&data->Position, &data->Rotation, 0, 10, 150), 150)) {
			EFCannon2_Fire(data, -10);
			EFCannon2_Fire(data, 10);
			data->Action = EFCannonAction_Bullet;
		}

		break;
	case EFCannonAction_Bullet:
		if (++data->InvulnerableTime > 160) {
			data->Action = EFConveyorAction_Run;
			data->InvulnerableTime = 0;
		}
	}

	obj->DisplaySub(obj);
	AddToCollisionList(data);
	DynColRadius(obj, 50, 0);
	RunObjectChildren(obj);
}

void EFCannon1_Main(ObjectMaster* obj) {
	if (ClipSetObject(obj) || EFCannon_Explode(obj)) return;

	EntityData1* data = obj->Data1;
	uint8_t player = 0;

	if (data->CharID == 1) EFCanon_SetHeight(data);
	
	if (data->Action >= EFCannonAction_Seek) {
		if (data->Unknown == 1 || data->Unknown == 2) {
			player = IsPlayerInsideSphere_(&Entity_GetPoint(&data->Position, &data->Rotation, 0, 10, 120), 150);
		}
		else {
			player = IsPlayerInsideSphere_(&data->Position, 300);
		}

		if (player) {
			data->CharIndex = player - 1;
			EntityData1* playerdata = EntityData1Ptrs[player - 1];
			NJS_VECTOR* playerpos = &playerdata->Position;

			if (data->Action == EFCannonAction_Seek) {
				if (++data->InvulnerableTime < 80) {
					// cannon face front
					if (data->field_A != 0) data->field_A -= 10;

					// rotate towards player
					data->field_A += 1;

					if (data->field_A == 100 || !data->Scale.x) {
						data->Scale = *playerpos;
						data->field_A = 0;
					}
					else {
						data->Scale = GetPathPosition(&data->Scale, playerpos, (float)data->field_A / 120);
					}

					data->Scale.y = CharObj2Ptrs[player - 1]->_struct_a3.DistanceMax + 0.3;

					if (data->Unknown == 0) {
						data->Rotation.y = -fPositionToRotation(&data->Position, &data->Scale).y + 0x4000;
					}
				}
				else {
					data->InvulnerableTime = 0;
					data->Action = EFCannonAction_Shoot;

					if (data->Unknown == 1) {
						data->Scale = Entity_GetPoint(&data->Position, &data->Rotation, 0, 50, 130);
						Rotation3 temp = { 0, 0, 0 };
						data->Scale.y = GetGroundYPosition(data->Scale.x, data->Scale.y, data->Scale.z, &temp);
					}
					else if (data->Unknown == 2) {
						EFCannon1_Fire(data);
					}
				}
			}
		}
		else {
			data->InvulnerableTime = 0;
		}
	}

	switch (data->Action) {
	case EFCannonAction_Shoot:
		if (data->Unknown == 2) {
			if (++data->InvulnerableTime > 120) {
				data->InvulnerableTime = 0;
				data->Action = EFCannonAction_Seek;
			}
			
			break;
		}

		if (data->field_A >= -5000) data->field_A -= 400;
		else {
			if (EntityData1Ptrs[player - 1]) {
				ObjectMaster* bullet = LoadObject(LoadObj_Data1, 3, EFBullet);
				bullet->Data1->Position = Entity_GetPoint(&data->Position, &data->Rotation, 0, 60, 0);
				bullet->Data1->Rotation.y = -fPositionToRotation(&data->Position, &data->Scale).y + 0x4000;
				bullet->Data1->Scale = data->Scale;
				if (data->Unknown == 0) bullet->Data1->Unknown = 1;
				data->Scale.x = 0;
				data->Action = EFCannonAction_Bullet;
			}
			else {
				data->Action = EFCannonAction_Seek;
			}
		}
		break;
	case EFCannonAction_Bullet:
		if (++data->InvulnerableTime < 60) break;
		else {
			if (data->field_A <= 0) data->field_A += 50;
			else {
				data->InvulnerableTime = 0;
				data->Action = EFCannonAction_Seek;
			}
		}
	}

	obj->DisplaySub(obj);
	AddToCollisionList(data);
	RunObjectChildren(obj);
}

void EFCannon_Collision(ObjectMaster* obj) {
	if (DynColRadiusAuto(obj, 1)) {
		EntityData1* data = obj->Data1;
		EntityData1* parent = obj->Parent->Data1;

		data->Position.x = parent->Position.x;
		data->Position.z = parent->Position.z;

		data->Rotation = parent->Rotation;

		DynCol_Update(data, 4);
	}
}

void EFCannon(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;

	EntityData1* data = obj->Data1;
	
	if (data->Action == EFCannonAction_Init) {
		data->Action = EFCannonAction_Seek;
		AllocateObjectData2(obj, obj->Data1);

		if (data->Scale.x < 3) {
			data->Object = EF_CANNON1->getmodel();
			obj->DisplaySub = EFCannon1_Display;
			obj->MainSub = EFCannon1_Main;
			
			if (data->Scale.x >= 1) {
				data->Position.y -= 45;
				Collision_Init(obj, EFCannon1_Col, 1, 3);
			}
			else {
				Collision_Init(obj, EFCannon1_Col, 3, 3);
			}

			ObjectMaster* child = LoadSubDynCol(obj, data->Object->child->child, 1, 100);
			if (child) {
				child->MainSub = EFCannon_Collision;
				child->Data1->Position.y += 60;
			}
		}
		else {
			data->Object = EF_CANNON2->getmodel();
			data->Object->basicdxmodel->r = 50;
			obj->DisplaySub = EFCannon2_Display;
			obj->MainSub = EFCannon2_Main;

			Collision_Init(obj, &EFCannon2_Col, 1, 3);

			ObjectMaster* child = LoadSubDynCol(obj, data->Object->child, 1, 100);
			if (child) {
				child->MainSub = EFCannon_Collision;
				child->Data1->Position.y += 10;
			}
		}

		data->Unknown = data->Scale.x;
		data->CharID = data->Scale.z;
		data->Scale.x = 0;
		data->Scale.z = 0;
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjReel, "OBJREEL" }, //45
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 4060000, 0, EFBigFans, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, EFCannon, "EFCannon" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFPlatforms, "EFPlatforms" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFPipeline, "EFPipeline" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, EFShipDoor, "EFShipDoor" }, //50
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 5060000, 0, EFShipConveyor, "EFShipConveyor" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, EFAntenna, "EFAntenna" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, EFRailSign, "EFRailSign" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFMissilePods, "EFMissilePods" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, EFHelice, "EFHelice" }, //55
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3060000, 0, ECBarrier, "ECBarrier" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, e2000_Init, "E2000" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 10000000, 0, Flyer_Init, "Flyer" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, Flyer_Trigger, "Flyer Trigger" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, Laserdoor, "Laserdoor" } //60
};
ObjectList EggFleetObjectList = { arraylengthandptrT(EggFleetObjectList_list, int) };

void EggFleetObjects_Init() {
	TexLists_Obj[HeroesLevelID_EggFleet] = EggFleetObjectTextures;
	ObjLists[HeroesLevelID_EggFleet * 8] = &EggFleetObjectList;
}