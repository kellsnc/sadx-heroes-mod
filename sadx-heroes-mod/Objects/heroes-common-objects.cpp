#include "stdafx.h"

bool EnableModels;

NJS_TEXNAME SHTexNames[40];
NJS_TEXLIST SHCommonTextures = { arrayptrandlength(SHTexNames) };
PVMEntry shobjpvm = { "heroes-common", &SHCommonTextures };

SH_ANIMTEXS CommonAnimTexs[]{
	{ 38, 2,{ 25, 25, 10 } }
};

SH_UVSHIFT Objects_UVSHIFT[]{
	{ nullptr,0,{ 0, 20 } },
	{ nullptr,0,{ 0, 20 } },
	{ nullptr,0,{ 0, 30 } },
};

ModelInfo * CO_BALLOON;
ModelInfo * CO_CMNREEL;
ModelInfo * CO_COMNFAN;
ModelInfo * CO_DSHHOOP;
ModelInfo * CO_DSHPANL;
ModelInfo * CO_GOALRNG;
ModelInfo * CO_LCHRAMP;
ModelInfo * CO_LCKCASE;
ModelInfo * CO_OCANNON;
ModelInfo * CO_WOODBOX;
ModelInfo * CO_OBJLASR;

CollisionData Reel_col{
	0, 0, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 1, 1, 1 }, 0, 0
};

CollisionData ObjBoxW_col{
	0, 2, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 13, 13, 13 }, 0, 0
};

CollisionData Cases_col{
	0, 2, 0x77, 0, 0x800400,{ 0, 10, 0 },{ 20, 17, 20 }, 0, 0
};

CollisionData CasesOpen_col{
	0, 2, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 20, 7, 20 }, 0, 0
};

void CPDashPanel(ObjectMaster *a1);

void SHCameraSwitch(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (camera_flags != (Uint32)a1->Data1->Scale.x) {
			if (IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.y) == 1) {
				if (a1->Data1->Scale.x == 7) SetCameraMode_(1);
				else camera_flags = a1->Data1->Scale.x;
			}
		}
	}
}

uint8_t Fans_IsSpecificPlayerInCylinder(EntityData1* entity, NJS_VECTOR* center, float radius, float height) {
	NJS_VECTOR* pos = &entity->Position;

	if ((powf(pos->x - center->x, 2) + pow(pos->z - center->z, 2)) <= pow(radius, 2) &&
		pos->y > center->y && pos->y < center->y + height * 40) {
		return true;
	}

	return 0;
}

uint8_t Fans_IsPlayerInCylinder(NJS_VECTOR* center, float radius, float height) {
	for (uint8_t p = 0; p < MaxPlayers; ++p) {
		EntityData1* entity = EntityData1Ptrs[p];
		if (entity && entity->field_A < 120) {
			if (Fans_IsSpecificPlayerInCylinder(entity, center, radius, height)) return p + 1;
		}
	}

	return 0;
}

void Fans_SetSpeedAndAnim(uint8_t player) {
	EntityData1* entity = EntityData1Ptrs[player];
	CharObj2* co2 = CharObj2Ptrs[player];

	co2->Speed.y += 0.1;
	entity->Status = 0;

	if (GetCharacterID(0) == Characters_Sonic && (co2->Upgrades & Upgrades_SuperSonic) == 0) {
		co2->AnimationThing.Index = 26;
	}
	else if (GetCharacterID(0) == Characters_Tails) {
		co2->AnimationThing.Index = 33;
	}
	else if (GetCharacterID(0) == Characters_Knuckles) {
		co2->AnimationThing.Index = 34;
	}
}

void Fans_HandlePlayer(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* entity = EntityData1Ptrs[data->CharIndex];
	CharObj2* co2 = CharObj2Ptrs[data->CharIndex];

	if (!entity) {
		DeleteObject_(obj);
		return;
	}

	if (entity->field_A < 120) entity->field_A = 130;

	if (Fans_IsSpecificPlayerInCylinder(entity, &data->Position, data->Scale.y, data->Scale.x)) {
		Fans_SetSpeedAndAnim(data->CharIndex);
	}
	else {
		if (entity->CollisionInfo->CollidingObject || entity->Status & Status_Ball) {
			DeleteObject_(obj);
			return;
		}

		data->Scale.z += 0.06f;
		CharObj2Ptrs[data->CharIndex]->Speed.y += data->Scale.z;

		entity->field_A = 131;

		if (entity->Status & Status_Ground) {
			DeleteObject_(obj);
			entity->field_A = 0;
		}
		else {
			if (data->Scale.z > 0) data->Scale.z = 0;
			data->Scale.z -= 0.07f;
			co2->Speed.y = data->Scale.z;

			if (data->Scale.z < -1.5f) {
				DeleteObject_(obj);
				entity->field_A = 0;
			}
		}
	}
}

void ObjFan_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		njRotateXYZ(nullptr, 0, a1->Data1->Scale.z, 0);
		njDrawModel_SADX(a1->Data1->Object->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjFan(ObjectMaster *obj)
{
	if (ClipSetObject(obj) || !CO_COMNFAN) return;

	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DisplaySub = ObjFan_Display;
		obj->DeleteSub = DynCol_Delete;
		data->Object = CO_COMNFAN->getmodel();
		data->Action = 1;
	}
	else {
		if (CurrentLevel == HeroesLevelID_EggFleet && data->Scale.y > 0) {
			if (IsSwitchPressed(data->Scale.y) == false) {
				DynColRadius(obj, 350, 0);
				obj->DisplaySub(obj);
				return;
			}
		}

		data->CharIndex = Fans_IsPlayerInCylinder(&data->Position, 45.5f, data->Scale.x);
		if (data->CharIndex) {
			LoadChildObject(LoadObj_Data1, Fans_HandlePlayer, obj);
			obj->Child->Data1->CharIndex = data->CharIndex - 1;
			obj->Child->Data1->Position = data->Position;
			obj->Child->Data1->Scale.x = data->Scale.x;
			obj->Child->Data1->Scale.y = 45.5f;
		}

		data->Scale.z -= data->Scale.x * 80;

		DynColRadius(obj, 350, 0);
		obj->DisplaySub(obj);
		RunObjectChildren(obj);
	}
}

void ObjReel_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		DrawQueueDepthBias = -6000.0f;

		njTranslate(0, a1->Data1->Position.x, a1->Data1->Scale.x, a1->Data1->Position.z);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);

		njTranslate(0, 0, a1->Data1->Position.y - a1->Data1->Scale.x + 15, 0);
		njDrawModel_SADX(a1->Data1->Object->child->child->basicdxmodel);

		njTranslate(0, 0, a1->Data1->Scale.x - a1->Data1->Position.y - 15, 0);
		njScale(nullptr, 1, (a1->Data1->Scale.x - a1->Data1->Position.y) / 10.15f, 1);
		njDrawModel_SADX(a1->Data1->Object->child->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjReel_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (DynColRadius(a1, 30, 0) == 2) {
			NJS_OBJECT* obj = (NJS_OBJECT*)a1->Data1->LoopData;
			obj->pos[1] = a1->Data1->Scale.x;
		}

		float max = a1->Data1->Scale.x;
		float min = a1->Data1->Scale.z;
		NJS_VECTOR * pos = &a1->Data1->Position;

		if (a1->Data1->Scale.y != 9) {
			EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
			for (uint8_t slot = 0; slot < MaxPlayers; ++slot) {
				if (players[slot]) {
					int temp = IsPlayerInsideSphere(pos, 10);
					if (temp == slot + 1) a1->Data1->Scale.y = slot + 1;
				}
			}
		}
		
		if (a1->Data1->Scale.y > 0 && a1->Data1->Scale.y < 9) {
			uint8_t slot = a1->Data1->Scale.y - 1;
			EntityData1 *entity = EntityData1Ptrs[slot];
			if (!entity) a1->Data1->Scale.y = 9; //out
			else {
				CharObj2 *co2 = CharObj2Ptrs[slot];

				switch (GetCharacterID(slot)) {
				case Characters_Sonic:
					if ((co2->Upgrades & Upgrades_SuperSonic) == 0) co2->AnimationThing.Index = 47;
					else  co2->AnimationThing.Index = 141;
					break;
				case Characters_Tails: co2->AnimationThing.Index = 100; break;
				case Characters_Knuckles: co2->AnimationThing.Index = 110; break;
				case Characters_Amy: co2->AnimationThing.Index = 80; break;
				}
				
				entity->Status = 0;

				//Getting off
				if (ControllerPointers[slot]->PressedButtons & Buttons_A) {
					a1->Data1->Scale.y = 9;

					if (GetCharacterID(slot) == Characters_Sonic) {
						entity->Status = Status_Ball;
						
						if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
							entity->Action = 8;
							co2->AnimationThing.Index = 14;
						}
						else {
							co2->AnimationThing.Index = 145;
						}
					}
					else if (GetCharacterID(slot) == Characters_Tails || GetCharacterID(0) == Characters_Knuckles) {
						entity->Action = 6;
						entity->Status = Status_Ball;
						co2->AnimationThing.Index = 14;
					}
					else if (GetCharacterID(slot) == Characters_Amy) {
						entity->Action = 5;
						co2->AnimationThing.Index = 14;
					}
					else if (GetCharacterID(slot) == Characters_Big) {
						entity->Action = 4;
						co2->AnimationThing.Index = 15;
					}
					else if (GetCharacterID(slot) == Characters_Gamma) {
						entity->Action = 4;
						co2->AnimationThing.Index = 6;
					}

					co2->Speed.y = 2;
					co2->Speed.x = 2;
				}

				entity->Position = a1->Data1->Position;
				entity->Position.y -= 13;
				entity->Rotation.y = a1->Data1->Rotation.y - 0x4000;

				if (a1->Data1->Position.y < max - 15) {
					a1->Data1->Position.y += 1;
					if (anim % 40) PlayHeroesSound(CommonSound_Pulley);
				}
			}
		}
		else AddToCollisionList(a1->Data1);

		if (a1->Data1->Scale.y > 8) {
			if (anim % 50 == 0) a1->Data1->Scale.y = 10;

			if (a1->Data1->Position.y > min) {
				a1->Data1->Position.y -= 1;
				if (anim % 40) PlayHeroesSound_Entity(CommonSound_Pulley, a1, 300, 0);
			}
		}

		ObjReel_Display(a1);
	}
}

void ObjReel(ObjectMaster *a1)
{
	a1->Data1->Object = CO_CMNREEL->getmodel();

	a1->Data1->Scale.z = a1->Data1->Position.y;
	a1->Data1->Scale.y = 0;
	Collision_Init(a1, &Reel_col, 1, 2u);

	a1->MainSub = ObjReel_Main;
	a1->DisplaySub = ObjReel_Display;
	a1->DeleteSub = DynCol_Delete;
}

void WaterfallObject(ObjectMaster *a1) {
	if (a1->Data1->NextAction == 0 || a1->Data1->NextAction == CurrentChunk) {
		if (a1->Data1->Scale.x == 0 || IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.x) == 1) {
			if (DynColRadiusAuto(a1, 5)) {
				NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;
				if (col) col->pos[1] = a1->Data1->Position.y + 1;
			}

			if (!MissedFrames) {
				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, a1->Data1->Position.x, a1->Data1->Position.y, a1->Data1->Position.z);
				njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
				njScale(nullptr, a1->Data1->Scale.y, a1->Data1->Scale.y, a1->Data1->Scale.y);
				DrawQueueDepthBias = a1->Data1->InvulnerableTime;
				njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void ObjBalloon_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		Direct3D_SetZFunc(1u);

		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);

		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, a1->Data1->Scale.y, a1->Data1->Scale.y, a1->Data1->Scale.y);

		if (a1->Data1->Scale.y != 3) {
			a1->Data1->Scale.z -= 0.1f;
     		SetMaterialAndSpriteColor_Float(a1->Data1->Scale.z, 1.0f, 1.0f, 1.0f);
		}

		SetupWorldMatrix();
		Direct3D_SetChunkModelRenderState();
		DrawChunkModel(CO_BALLOON->getmodel()->chunkmodel);

		SetMaterialAndSpriteColor_Float(1.0f, 1.0f, 1.0f, 1.0f);
		Direct3D_UnsetChunkModelRenderState();
		njPopMatrix(1u);
		Direct3D_ResetZFunc();
	}
}

void ObjBalloon_Main(ObjectMaster *a1)
{
	if (a1->Data1->NextAction || !ClipSetObject(a1)) {
		float item = a1->Data1->Scale.x;

		if (!a1->Data1->NextAction) {
			a1->Data1->CharIndex = IsPlayerInsideSphere(&a1->Data1->Position, 50);

			if (a1->Data1->CharIndex) {
				a1->Data1->Scale.y += 0.2f;

				if (a1->Data1->Scale.y == 3.2f) {
					PlayHeroesSound(CommonSound_ItemBox);

					if (item == 12) {
						if (GetCharacterID(a1->Data1->CharIndex - 1) == Characters_Tails) {
							CharObj2 *co2 = CharObj2Ptrs[a1->Data1->CharIndex - 1];
							co2->TailsFlightTime = 0.1f;
						}
					}
				}
			}

			if (a1->Data1->Scale.y > 7.f) {
				a1->Data1->NextAction = 1;
			}
			else {
				ObjBalloon_Display(a1);
			}
		}
	}
}

void ObjBalloon(ObjectMaster *a1)
{
	a1->Data1->Scale.y = 3;
	a1->Data1->Scale.z = 1;

	a1->MainSub = &ObjBalloon_Main;
	a1->DisplaySub = &ObjBalloon_Display;
}

void SHDashPanel(ObjectMaster *a1)
{
	if ((CurrentLevel == HeroesLevelID_CasinoPark || CurrentLevel == HeroesLevelID_BingoHighway) && a1->Data1->Scale.z == 1) {
		CPDashPanel(a1);
	}
	else {
		if (!MissedFrames) {
			njSetTexture(&SHCommonTextures);
			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Position);
			EntityData1 *v1 = a1->Data1;
			int v2;
			int v3;
			v2 = v1->Rotation.z;
			if (v2)
			{
				njRotateZ(0, v2);
			}
			v3 = v1->Rotation.x;
			if (v3)
			{
				njRotateX(0, v3);
			}
			if (v1->Rotation.y != -32768)
			{
				njRotateY(0, (LOWORD(v1->Rotation.y) + -32768));
			}
			DrawQueueDepthBias = -6000.0f;
			njDrawModel_SADX(CO_DSHPANL->getmodel()->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void SHDashHoop(ObjectMaster *a1)
{
	EntityData1 *v1 = a1->Data1;
	Rotation3 Angle = v1->Rotation;

	if (!MissedFrames && IsVisible(&v1->Position, 100.0))
	{
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &v1->Position);
		if (Angle.x) njRotateX(0, Angle.x);
		if (Angle.y) njRotateY(0, Angle.y);
		if (Angle.z) njRotateZ(0, Angle.z);

		if (CurrentLevel != HeroesLevelID_CasinoPark && CurrentLevel != HeroesLevelID_BingoHighway) 
			njDrawModel_SADX(CO_DSHHOOP->getmodel()->basicdxmodel);
		else 
			njDrawModel_SADX(CO_DSHHOOP->getmodel()->child->basicdxmodel);

		njPopMatrix(1u);
	}
}

void DoBall(uint8_t id) {
	EntityData1 *entity = EntityData1Ptrs[id];
	CharObj2 *co2 = CharObj2Ptrs[id];

	entity->Status = Status_Ball;
	if (GetCharacterID(0) == Characters_Sonic) {
		if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
			entity->Action = 5;
			co2->AnimationThing.Index = 15;
		}
		else {
			co2->AnimationThing.Index = 145;
		}
	}
	else if (GetCharacterID(0) == Characters_Tails) {
		entity->Action = 4;
		co2->AnimationThing.Index = 15;
	}
	else if (GetCharacterID(0) == Characters_Knuckles) {
		entity->Action = 4;
		co2->AnimationThing.Index = 34;
	}
}

void ObjCannon_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CO_OCANNON->getmodel()->basicdxmodel);

		njTranslate(0, 0, 5, 0);
		njRotateXYZ(0, a1->Data1->Rotation.x, 0, 0);
		njDrawModel_SADX(CO_OCANNON->getmodel()->child->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjCannon_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action == 0) {
			a1->Data1->Status = IsPlayerInsideSphere(&a1->Data1->Position, 40);

			if (a1->Data1->Status) {
				a1->Data1->Action = 1;
				a1->Data1->Scale.z = 20;
				a1->Data1->Status -= 1;
				
				CharObj2 *co2 = CharObj2Ptrs[a1->Data1->Status];

				co2->Speed.y += 2;
				DoBall(a1->Data1->Status);
			}
		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->Scale.z != 0) a1->Data1->Scale.z -= 1;
			else {
				EntityData1 *entity = EntityData1Ptrs[a1->Data1->Status];

				NJS_VECTOR startPos = entity->Position;
				NJS_VECTOR targetPos = a1->Data1->Position;

				if (a1->Data1->NextAction < 21) a1->Data1->NextAction += 1;
				else {
					a1->Data1->NextAction = 0;
					PlayHeroesSound(CommonSound_CannonIn);
					a1->Data1->Action = 2;
				}
				float timer = (float)a1->Data1->NextAction / 100;

				float x0 = startPos.x;
				float x1 = targetPos.x;
				float y0 = startPos.y;
				float y1 = targetPos.y + 28;
				float z0 = startPos.z;
				float z1 = targetPos.z;

				float distx = x1 - x0;
				float disty = y1 - y0;
				float distz = z1 - z0;

				entity->Position.x = (distx)* timer + x0;
				entity->Position.y = (disty)* timer + y0;
				entity->Position.z = (distz)* timer + z0;

				DoBall(a1->Data1->Status);
			}
		}

		if (a1->Data1->Action == 2) {
			EntityData1 *entity = EntityData1Ptrs[a1->Data1->Status];

			NJS_VECTOR startPos = entity->Position;
			NJS_VECTOR targetPos = a1->Data1->Position;

			if (a1->Data1->NextAction < 31) a1->Data1->NextAction += 1;
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 3;
				a1->Data1->Scale.z = 100;
				PlayHeroesSound(CommonSound_CannonMov);
			}

			float timer = (float)a1->Data1->NextAction / 100;
			float y0 = startPos.y;
			float y1 = targetPos.y + 10;
			float dist = y1 - y0;
			entity->Position.x = targetPos.x;
			entity->Position.z = targetPos.z;
			entity->Position.y = (dist)* timer + y0;
			entity->Rotation.y = a1->Data1->Rotation.y;
			DoBall(a1->Data1->Status);
		}

		if (a1->Data1->Action == 3) {
			if (a1->Data1->Scale.z != 0) a1->Data1->Scale.z -= 1;

			if (a1->Data1->NextAction < 101) {
				a1->Data1->Rotation.x -= 100;
				a1->Data1->NextAction += 1;
			}
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 4;
			}

			EntityData1 *entity = EntityData1Ptrs[a1->Data1->Status];
			entity->Position = a1->Data1->Position;
			entity->Rotation.y = a1->Data1->Rotation.y;
			DoBall(a1->Data1->Status);
		}

		if (a1->Data1->Action == 4) {
			if (a1->Data1->NextAction < 101) {
				a1->Data1->NextAction += 1;
			}
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 5;
			}

			if (a1->Data1->NextAction == 1) {
				EntityData1 *entity = EntityData1Ptrs[a1->Data1->Status];
				CharObj2 *co2 = CharObj2Ptrs[a1->Data1->Status];
				entity->Rotation.y = a1->Data1->Rotation.y - 0x4000;
				if (CurrentLevel == HeroesLevelID_EggFleet) entity->Rotation.y += 0x8000;
				co2->Speed.x = a1->Data1->Scale.x;
				co2->Speed.y = a1->Data1->Scale.y;
				PlayHeroesSound(CommonSound_CannonLch);
			}

			DoBall(a1->Data1->Status);
		}
		
		
		if (a1->Data1->Action == 5) {
			if (a1->Data1->NextAction < 101) {
				a1->Data1->Rotation.x += 100;
				a1->Data1->NextAction += 1;
			}
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 0;
			}
		}

		ObjCannon_Display(a1);
	}
}

void ObjCannon(ObjectMaster *a1)
{
	a1->MainSub = &ObjCannon_Main;
	a1->DisplaySub = &ObjCannon_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void SHLaunchRamp(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CO_LCHRAMP->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void OBJCASE_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1.5f, 1.5f, 1.5f);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CO_LCKCASE->getmodel()->basicdxmodel);

		if (a1->Data1->Action == 0) {
			njTranslate(0, 0, 9.5f, 0);
			njDrawModel_SADX(CO_LCKCASE->getmodel()->child->basicdxmodel);
			njTranslate(0, 0, 7, 0);
			njDrawModel_SADX(CO_LCKCASE->getmodel()->child->child->basicdxmodel);
		}
		else if (a1->Data1->Action == 1) {
			njTranslate(0, 0, a1->Data1->Scale.z, 0);
			njDrawModel_SADX(CO_LCKCASE->getmodel()->child->basicdxmodel);
		}
		else if (a1->Data1->Action == 2) njDrawModel_SADX(CO_LCKCASE->getmodel()->child->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void OBJCASE_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action == 0) {
			if (IsSwitchPressed(a1->Data1->Scale.x)) {
				a1->Data1->Action = 1;
				a1->Data1->Scale.z = 9.5f;
			}
		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->Scale.z > 0) a1->Data1->Scale.z -= 0.2f;
			else {
				a1->Data1->Action = 2;
				a1->Data1->Scale.z = 20;
				Collision_Free(a1);
				Collision_Init(a1, &CasesOpen_col, 1, 6u);
			}
		}

		AddToCollisionList(a1->Data1);
		OBJCASE_Display(a1);
	}
}

void OBJCASE(ObjectMaster *a1)
{
	if (a1->Data1->Scale.y == 1 || IsSwitchPressed(a1->Data1->Scale.x)) {
		a1->Data1->Action = 2;
		Collision_Init(a1, &CasesOpen_col, 1, 6u);
	}
	else {
		Collision_Init(a1, &Cases_col, 1, 3u);
	}

	a1->MainSub = &OBJCASE_Main;
	a1->DisplaySub = &OBJCASE_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void Capsule_Display_r(ObjectMaster *a1) {
	if (!MissedFrames) {
		EntityData1 *v1 = a1->Data1;

		njSetTexture(&SHCommonTextures);

		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njTranslate(0, 0, 20, 0);
		njRotateXYZ(nullptr, 0, v1->Rotation.y, 0);

		njScale(nullptr, v1->Scale.x - (v1->Scale.x * 0.2f), v1->Scale.x - (v1->Scale.x * 0.2f), v1->Scale.x - (v1->Scale.x * 0.2f));
		njRotateY(nullptr, -(v1->Rotation.y * 2));

		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CO_GOALRNG->getmodel()->child->child->basicdxmodel);

		njRotateY(nullptr, -v1->Rotation.y);
		njDrawModel_SADX(CO_GOALRNG->getmodel()->child->basicdxmodel);

		njScale(nullptr, v1->Scale.x * 1.2f, v1->Scale.y *1.2f, v1->Scale.z *1.2f);
		njDrawModel_SADX(CO_GOALRNG->getmodel()->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void Capsule_Main_r(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		EntityData1 *v1 = a1->Data1;
		v1->Rotation.y += 200;

		if (IsPlayerInsideSphere(&v1->Position, 30) && a1->Data1->Action == 0) {
			LoadLevelResults();
			a1->Data1->Action = 1;
		}

		if (a1->Data1->Action == 1) {
			EntityData1 *entity = EntityData1Ptrs[0];
			entity->Position = v1->Position;
			if (v1->Scale.x > 0.05f) {
				v1->Scale.x -= 0.05f;
			}
		}
		else {
			v1->Scale.x = 1;

			if (anim % 60 == 0) PlayHeroesSound_Entity(CommonSound_GoalRing, a1, 500, 0);
		}

		Capsule_Display_r(a1);
	}
}

void Capsule_Load_r(ObjectMaster *a1) {
	a1->DisplaySub = Capsule_Display_r;
	a1->MainSub = Capsule_Main_r;
}

void ObjBox_Display(ObjectMaster* a1)
{
	if (!MissedFrames && a1->Data1->Action != 2 && IsPlayerInsideSphere(&a1->Data1->Position, a1->SETData.SETData->Distance / 1000)) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		if (a1->Data1->Action == 0) njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		else if (a1->Data1->Action == 1) {
			CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
			CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
			CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

			if (a1->Data1->Scale.x == 0) {
				CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel->mats[0].attr_texId = 10;
				CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 10;
				CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 10;
			}
			else if (a1->Data1->Scale.x == 1) {
				CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel->mats[0].attr_texId = 8;
				CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 8;
				CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 8;
			}
			else {
				CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel->mats[0].attrflags |= NJD_FLAG_USE_ALPHA;
				CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel->mats[0].attrflags |= NJD_FLAG_USE_ALPHA;
				CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel->mats[0].attrflags |= NJD_FLAG_USE_ALPHA;

				CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel->mats[0].attr_texId = 7;
				CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 7;
				CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel->mats[0].attr_texId = 7;
			}

			njTranslate(0, 0, -(a1->Data1->Scale.y / 3), 0);

			njTranslate(0, 0, 0, 12 + a1->Data1->Scale.y / 90);
			njDrawModel_SADX(CO_WOODBOX->getmodel()->child->child->child->child->basicdxmodel); //bar
			njTranslate(0, 0, 20, 0);

			njTranslate(0, 0, -(a1->Data1->Scale.y), (a1->Data1->Scale.y / 3));
			njRotateY(0, 0x5000 + a1->Data1->Scale.y * 5);
			njDrawModel_SADX(CO_WOODBOX->getmodel()->child->child->child->child->child->basicdxmodel); //big one
			njRotateY(0, -(0x5000 + a1->Data1->Scale.y * 5));

			njTranslate(0, 0, -(a1->Data1->Scale.y * 2), -(a1->Data1->Scale.y / 5));
			njRotateY(0, -0x3000);
			njDrawModel_SADX(CO_WOODBOX->getmodel()->child->child->child->child->child->child->basicdxmodel); //right
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjBoxW(ObjectMaster *a1)
{
	uint8_t type = a1->Data1->Scale.x;

	if (a1->Data1->NextAction == 0) {
		if (type == 0) a1->Data1->Object = CO_WOODBOX->getmodel();
		else if (type == 1) a1->Data1->Object = CO_WOODBOX->getmodel()->child;
		else if (type == 2) a1->Data1->Object = CO_WOODBOX->getmodel()->child->child;
		else if (type == 3) a1->Data1->Object = CO_WOODBOX->getmodel()->child->child->child;

		AllocateObjectData2(a1, a1->Data1);
		Collision_Init(a1, &ObjBoxW_col, 1, 2u);

		a1->DisplaySub = ObjBox_Display;
		a1->DeleteSub = DynCol_Delete;

		a1->Data1->NextAction = 1;
	}
	else {
		if (a1->Data1->Action == 0) {
			if (DynColRadius(a1, 100, 0)) {
				if (a1->Data1->Action == 0 && type < 3) {
					if (OhNoImDead(a1->Data1, (ObjectData2*)a1->Data2)) {
						if (type == 0 || type == 2) PlayHeroesSound(CommonSound_BoxBreak);
						else if (type == 1) PlayHeroesSound(CommonSound_BoxBreak);

						a1->Data1->Action = 1;

						DynamicCOL_Remove(a1, (NJS_OBJECT*)a1->Data1->LoopData);
						ObjectArray_Remove((NJS_OBJECT*)a1->Data1->LoopData);
						a1->Data1->LoopData = nullptr;

						return;
					}
					else {
						AddToCollisionList(a1->Data1);
					}
				}
			}
		}
		else {
			if (a1->Data1->Scale.y < 100) a1->Data1->Scale.y++;
			else a1->Data1->Action = 2;
		}

		a1->DisplaySub(a1);
	}
}

CollisionData Laser_col{
	0, CollisionShape_Cone, 0x77, 0xE2, 0x800400,{ 0, 0, 0 },{ 0, 4, 4 }, 0, 0
};

//	Draw the laser object
void ObjLaser_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		njSetTexture(&SHCommonTextures);

		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateXYZ(nullptr, data->Rotation.x, data->Rotation.y, data->Rotation.z);

		njTranslateX(data->Scale.x);
		njRotateZ(nullptr, 0x4000);
		njDrawModel_SADX(data->Object->basicdxmodel);
		njRotateZ(nullptr, 0xC000);

		njTranslateX(-(data->Scale.x * 2));
		njRotateZ(nullptr, 0xC000);
		njDrawModel_SADX(data->Object->basicdxmodel);
		
		if (anim % 2 == 0 && data->Action != 2) {
			njRotateZ(nullptr, 0x4000);
			njTranslateX(data->Scale.x);
			njRotateZ(nullptr, 0x4000);
			njScaleY(5 + (data->Scale.x / 10));
			njDrawModel_SADX(data->Object->child->basicdxmodel);
		}
		
		njPopMatrix(1u);
	}
}

// A laser that hurts the player
// Param x: width, y: switch id, z: on/off
void ObjLaser_Main(ObjectMaster* obj) {
	if (obj->Parent == nullptr && ClipSetObject(obj)) return;
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		data->Action = 1;
		data->Object = CO_OBJLASR->getmodel();
		obj->DisplaySub = ObjLaser_Display;

		Collision_Init(obj, &Laser_col, 1, 4);
		data->CollisionInfo->CollisionArray->scale.x = data->Scale.x * 2;
	}
	else if (data->Scale.y && data->Action == 1) {
		if (IsSwitchPressed((int)data->Scale.y)) {
			Collision_Free(obj);
			data->Action = 2;
		}
	}

	AddToCollisionList(data);
	obj->DisplaySub(obj);
}

// Load several laser objects
// Param x: width, y: count, z: switch id
void Laserdoor(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return;
	EntityData1* data = obj->Data1;

	while (data->Scale.y + 1) {

		EntityData1* child = LoadChildObject(LoadObj_Data1, ObjLaser_Main, obj)->Data1;
		child->Scale.x = data->Scale.x; //width
		child->Scale.y = data->Scale.z; //switch id
		child->Position = data->Position;
		child->Rotation = data->Rotation;

		data->Position.y += 20;
		--data->Scale.y;
	}

	RunObjectChildren(obj);
}

void CommonObjects_Sounds(int ID, void *a2, int a3, void *a4) {
	switch (ID) {
	case 738:
		PlayHeroesSound(CommonSound_DashPanel);
		break;
	default:
		PlaySound(ID, a2, a3, a4);
		break;
	}
}

void CasinoObjects_Sounds(int ID, void *a2, int a3, void *a4) {
	if (IsCurrentHeroesLevel() && CurrentLevel != LevelIDs_Casinopolis) {
		switch (ID) {
		case 245:
			PlayHeroesSound(LevelSound_Csn_Bumper1);
			break;
		case 246:
			PlayHeroesSound(LevelSound_Csn_Trigger);
			break;
		}
	}
	else {
		PlaySound(ID, a2, a3, a4);
	}
}

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	helperFunctions.RegisterCommonObjectPVM(shobjpvm);

	CO_BALLOON = LoadCommonModel("CO_BALLOON");
	CO_CMNREEL = LoadCommonModel("CO_CMNREEL");
	CO_COMNFAN = LoadCommonModel("CO_COMNFAN");
	CO_LCKCASE = LoadCommonModel("CO_LCKCASE");
	CO_OCANNON = LoadCommonModel("CO_OCANNON");
	CO_WOODBOX = LoadCommonModel("CO_WOODBOX");
	CO_OBJLASR = LoadCommonModel("CO_OBJLASR");

	if (config->getBool("3- Objects", "GoalRing", true)) {
		WriteJump((void*)0x46B170, Capsule_Load_r);
		CO_GOALRNG = LoadCommonModel("CO_GOALRNG");
		EnableModels = true;
	}

	if (config->getBool("3- Objects", "DashPanel", true)) {
		WriteJump((void*)0x7A4360, SHDashPanel);
		CO_DSHPANL = LoadCommonModel("CO_DSHPANL");
		Objects_UVSHIFT[0].List = CO_DSHPANL->getmodel()->basicdxmodel->meshsets[0].vertuv;
		Objects_UVSHIFT[0].Size = CO_DSHPANL->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
		EnableModels = true;
	}

	if (config->getBool("3- Objects", "DashHoop", true)) {
		WriteJump((void*)0x7A2470, SHDashHoop);
		CO_DSHHOOP = LoadCommonModel("CO_DSHHOOP");
		Objects_UVSHIFT[1].List = CO_DSHHOOP->getmodel()->basicdxmodel->meshsets[2].vertuv;
		Objects_UVSHIFT[1].Size = CO_DSHHOOP->getmodel()->basicdxmodel->meshsets[2].nbMesh * 3;
		EnableModels = true;
	}

	if (config->getBool("3- Objects", "LaunchRamp", true)) {
		WriteJump((void*)0x500020, SHLaunchRamp);
		CO_LCHRAMP = LoadCommonModel("CO_LCHRAMP");
		Objects_UVSHIFT[2].List = CO_LCHRAMP->getmodel()->basicdxmodel->meshsets[15].vertuv;
		Objects_UVSHIFT[2].Size = CO_LCHRAMP->getmodel()->basicdxmodel->meshsets[15].nbMesh * 3;
		EnableModels = true;
	}

	if (config->getBool("4- Sounds", "cmnsounds", true)) {
		WriteCall((void*)0x7A461D, CommonObjects_Sounds);
		WriteCall((void*)0x5002E0, CommonObjects_Sounds);
	}

	WriteCall((void*)0x5DC992, CasinoObjects_Sounds);
	WriteCall((void*)0x5DCAE4, CasinoObjects_Sounds);
	WriteCall((void*)0x5DBE3C, CasinoObjects_Sounds);
}

void CommonObjects_OnFrame() {
	AnimateTexlist(CommonAnimTexs, LengthOfArray(CommonAnimTexs), &SHCommonTextures);
	AnimateUV(Objects_UVSHIFT, LengthOfArray(Objects_UVSHIFT));
}