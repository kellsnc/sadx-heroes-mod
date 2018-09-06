#include "stdafx.h"
#include "mod.h"
#include "objects.h"
#include "IniFile.hpp"

#include "heroes-common-objects.h"

NJS_TEXNAME SHTexNames[40];
NJS_TEXLIST SHCommonTextures = { arrayptrandlength(SHTexNames) };
PVMEntry shobjpvm = { "heroescommon", &SHCommonTextures };

bool Animate = false;

#pragma region CameraModeSwitch
void __cdecl SHCameraSwitch_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 500.0f)) {
		if (camera_flags != (Uint32)a1->Data1->Scale.x) {
			if (IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.y)) {
				camera_flags = a1->Data1->Scale.x;
			}
		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl SHCameraSwitch(ObjectMaster *a1)
{
	a1->MainSub = &SHCameraSwitch_Main;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Fans
void ObjFan_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(a1->Data1->Object->basicdxmodel);
		njRotateXYZ(nullptr, 0, a1->Data1->Scale.z, 0);
		DrawModel(&SH_FanBlades);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjFan_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {

		a1->Data1->Scale.z += a1->Data1->Scale.y;

		int slot = IsPlayerInsideSphere(&a1->Data1->Position, 45.0f);
		if (slot > 0) {
			auto entity = EntityData1Ptrs[slot - 1];
			CharObj2 *co2 = GetCharObj2(slot - 1);
			if (co2 != NULL) {
				co2->Speed.y = a1->Data1->Scale.x;
				if (GetCharacterID(0) == Characters_Sonic && !SuperSonicFlag) {
					co2->AnimationThing.Index = 26;
					entity->Status = 0;
				}
				else if (GetCharacterID(0) == Characters_Tails) {
					co2->AnimationThing.Index = 33;
					entity->Status = 0;
				}
				else if (GetCharacterID(0) == Characters_Knuckles) {
					co2->AnimationThing.Index = 34;
					entity->Status = 0;
				}
			}
		}

		ObjFan_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjFan(ObjectMaster *a1)
{
	a1->Data1->Object = &SH_ObjFan;
	AddToCollision(a1, 0);

	a1->MainSub = &ObjFan_Main;
	a1->DisplaySub = &ObjFan_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Reel
CollisionData Reel_col{
	0, 0, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 1, 1, 1 }, 0, 0
};

void ObjReel_Display(ObjectMaster *a1)
{
	njSetTexture(&SHCommonTextures);
	njPushMatrix(0);
	DrawQueueDepthBias = -6000.0f;

	njTranslate(0, a1->Data1->Position.x, a1->Data1->Scale.x, a1->Data1->Position.z);
	njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
	njScale(nullptr, 1, 1, 1);
	DrawModel(&PP_REEL_TOP);

	njTranslate(0, 0, a1->Data1->Position.y - a1->Data1->Scale.x + 15, 0);
	njScale(nullptr, 1, 1, 1);
	DrawModel(&PP_REEL_RING);

	njTranslate(0, 0, a1->Data1->Scale.x - a1->Data1->Position.y - 15, 0);
	njScale(nullptr, 1, (a1->Data1->Scale.x - a1->Data1->Position.y) / 10.15f, 1);
	DrawModel(&PP_REEL);

	DrawQueueDepthBias = 0;
	njPopMatrix(1u);
}

void ObjReel_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		Animate = true;

		float max = a1->Data1->Scale.x;
		float min = a1->Data1->Scale.z;
		NJS_VECTOR * pos = &a1->Data1->Position;
		auto entity = EntityData1Ptrs[0];

		if (a1->Data1->Scale.y != 2 && IsPlayerInsideSphere(pos, 10)) a1->Data1->Scale.y = 1;

		if (a1->Data1->Scale.y == 1) {
			CharObj2 *co2 = GetCharObj2(0);

			if (GetCharacterID(0) == Characters_Sonic) co2->AnimationThing.Index = 47;
			if (GetCharacterID(0) == Characters_Tails) co2->AnimationThing.Index = 100;
			if (GetCharacterID(0) == Characters_Knuckles) co2->AnimationThing.Index = 110;
			if (GetCharacterID(0) == Characters_Amy) co2->AnimationThing.Index = 80;
			entity->Status = 0;

			//Getting off
			if (ControllerPointers[0]->PressedButtons & Buttons_A) {
				a1->Data1->Scale.y = 2;

				if (GetCharacterID(0) == Characters_Sonic) {
					entity->Action = 8;
					entity->Status = Status_Ball;
					co2->AnimationThing.Index = 14;
				}
				if (GetCharacterID(0) == Characters_Tails || GetCharacterID(0) == Characters_Knuckles) {
					entity->Action = 6;
					entity->Status = Status_Ball;
					co2->AnimationThing.Index = 14;
				}
				if (GetCharacterID(0) == Characters_Amy) {
					entity->Action = 5;
					co2->AnimationThing.Index = 14;
				}
				if (GetCharacterID(0) == Characters_Big) {
					entity->Action = 4;
					co2->AnimationThing.Index = 15;
				}
				if (GetCharacterID(0) == Characters_Gamma) {
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
				if (anim % 40) if (EnableSounds) PlaySound(53, 0, 0, 0);
			}
		}
		else AddToCollisionList(a1->Data1);

		if (a1->Data1->Scale.y > 1) {
			if (anim % 50 == 0) a1->Data1->Scale.y = 3;

			if (a1->Data1->Position.y > min) {
				a1->Data1->Position.y -= 1;
				if (anim % 40) if (EnableSounds) if (GetPlayerDistance(a1->Data1, 0) < 107600.0) QueueSound(53, a1->Data1, 1, 148, 0, 800.0, a1->Data1);
			}
		}

		ObjReel_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjReel(ObjectMaster *a1)
{
	a1->Data1->Object = &PP_REEL_TOP_COL;

	EntityData1 * v1 = a1->Data1;
	NJS_OBJECT * v3 = v1->Object;
	v3->evalflags = NJD_EVAL_UNIT_SCL | NJD_EVAL_BREAK | NJD_EVAL_SKIP | NJD_EVAL_HIDE; //ignore scale
	v3->scl[0] = 1.0;
	v3->scl[1] = 1.0;
	v3->scl[2] = 1.0;
	v3->ang[0] = v1->Rotation.x;
	v3->ang[1] = v1->Rotation.y;
	v3->ang[2] = v1->Rotation.z;
	v3->pos[0] = v1->Position.x;
	v3->pos[1] = a1->Data1->Scale.x;
	v3->pos[2] = v1->Position.z;
	DynamicCOL_Add((ColFlags)1, a1, v3);

	a1->Data1->Scale.z = a1->Data1->Position.y;
	a1->Data1->Scale.y = 0;
	Collision_Init(a1, &Reel_col, 1, 2u);

	a1->MainSub = &ObjReel_Main;
	a1->DisplaySub = &ObjReel_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Balloons
void ObjBalloon_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, a1->Data1->Scale.x, a1->Data1->Scale.y, a1->Data1->Scale.z);

		if (a1->Data1->Scale.y != 1) {
			float a = a1->Data1->Scale.y - 5;
			a = -a;
			SetMaterialAndSpriteColor_Float(a, 1.0f, 1.0f, 1.0f);
		}
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&SH_BALLOON);
		DrawQueueDepthBias = 0;
		SetMaterialAndSpriteColor_Float(1.0f, 1.0f, 1.0f, 1.0f);
		njPopMatrix(1u);
	}
}

void ObjBalloon_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		float item = a1->Data1->Action;

		if (a1->Data1->Scale.z != 4 && a1->Data1->NextAction != 2) {
			if (IsPlayerInsideSphere(&a1->Data1->Position, 50)) a1->Data1->NextAction = 1;

			if (a1->Data1->NextAction == 1) {
				a1->Data1->Scale.x += 0.1f;
				a1->Data1->Scale.y += 0.1f;
				a1->Data1->Scale.z += 0.1f;

				if (a1->Data1->Scale.x == 3.1f) {
					if (a1->Data1->Action == 12) {
						if (GetCharacterID(0) == Characters_Tails) {
							CharObj2 *co2 = GetCharObj2(0);
							co2->TailsFlightTime = 0.1f;
						}
					}
				}
			}
		}
		else if (a1->Data1->Scale.z == 4 && a1->Data1->NextAction == 1) {
			a1->Data1->NextAction == 2;
		}

		ObjBalloon_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjBalloon(ObjectMaster *a1)
{
	a1->Data1->Action = a1->Data1->Scale.x;
	a1->Data1->Scale.x = 3;
	a1->Data1->Scale.y = 3;
	a1->Data1->Scale.z = 3;

	a1->MainSub = &ObjBalloon_Main;
	a1->DisplaySub = &ObjBalloon_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region DashPanels
//extern void CPDashPanel(ObjectMaster *a1);

void __cdecl SHDashPanel(ObjectMaster *a1)
{
	if ((CurrentLevel == 3 || CurrentLevel == 4) && a1->Data1->Scale.z == 1) {
		//CPDashPanel(a1);
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
			njScale(nullptr, 1, 1, 1);
			DrawQueueDepthBias = -6000.0f;
			DrawModel(&SH_DASHPANEL);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}
#pragma endregion

#pragma region DashHoops
void __cdecl SHDashHoop(ObjectMaster *a1)
{
	EntityData1 *v1; // esi
	Angle v2; // eax
	Angle v3; // eax
	Angle v4; // eax

	v1 = a1->Data1;
	if (!MissedFrames && IsVisible(&v1->Position, 100.0))
	{
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &v1->Position);
		v2 = v1->Rotation.x;
		if (v2)
		{
			njRotateX(0, v2);
		}
		v3 = v1->Rotation.y;
		if (v3)
		{
			njRotateY(0, v3);
		}
		v4 = v1->Rotation.z;
		if (v4)
		{
			njRotateZ(0, v4);
		}
		if (CurrentLevel != 3 && CurrentLevel != 4) DrawModel(&SH_DASHHOOP);
		else DrawModel(&SH_DASHRING);
		njPopMatrix(1u);
	}
}
#pragma endregion

#pragma region Cannon
void DoBall(EntityData1 * entity) {
	CharObj2 *co2 = GetCharObj2(0);

	entity->Status = Status_Ball;
	if (GetCharacterID(0) == Characters_Sonic) {
		entity->Action = 5;
		co2->AnimationThing.Index = 15;
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
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&SH_CANNONBASE);

		njTranslate(0, 0, 5, 0);
		njRotateXYZ(0, a1->Data1->Rotation.x, 0, 0);
		DrawModel(&SH_CANNON);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjCannon_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {

		if (a1->Data1->Action == 0 && IsPlayerInsideSphere(&a1->Data1->Position, 40)) {
			a1->Data1->Action = 1;
			a1->Data1->Scale.z = 20;

			auto entity = EntityData1Ptrs[0];
			CharObj2 *co2 = GetCharObj2(0);

			co2->Speed.y += 2;
			DoBall(entity);
		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->Scale.z != 0) a1->Data1->Scale.z -= 1;
			else {
				auto entity = EntityData1Ptrs[0];

				NJS_VECTOR startPos = entity->Position;
				NJS_VECTOR targetPos = a1->Data1->Position;

				if (a1->Data1->NextAction < 21) a1->Data1->NextAction += 1;
				else {
					a1->Data1->NextAction = 0;
					PlaySound(82, 0, 0, 0);
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

				DoBall(entity);
			}
		}

		if (a1->Data1->Action == 2) {

			auto entity = EntityData1Ptrs[0];

			NJS_VECTOR startPos = entity->Position;
			NJS_VECTOR targetPos = a1->Data1->Position;

			if (a1->Data1->NextAction < 31) a1->Data1->NextAction += 1;
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 3;
				a1->Data1->Scale.z = 100;
				PlaySound(83, 0, 0, 0);
			}

			float timer = (float)a1->Data1->NextAction / 100;
			float y0 = startPos.y;
			float y1 = targetPos.y + 10;
			float dist = y1 - y0;
			entity->Position.x = targetPos.x;
			entity->Position.z = targetPos.z;
			entity->Position.y = (dist)* timer + y0;
			entity->Rotation.y = a1->Data1->Rotation.y;
			DoBall(entity);
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

			auto entity = EntityData1Ptrs[0];
			entity->Position = a1->Data1->Position;
			entity->Rotation.y = a1->Data1->Rotation.y;
			DoBall(entity);
		}

		if (a1->Data1->Action == 4) {
			auto entity = EntityData1Ptrs[0];
			if (a1->Data1->NextAction < 101) {

				a1->Data1->NextAction += 1;
			}
			else {
				a1->Data1->NextAction = 0;
				a1->Data1->Action = 5;
			}


			if (a1->Data1->NextAction == 1) {

				CharObj2 *co2 = GetCharObj2(0);
				entity->Rotation.y = a1->Data1->Rotation.y - 0x4000;
				co2->Speed.x = a1->Data1->Scale.x;
				co2->Speed.y = a1->Data1->Scale.y;
				PlaySound(84, 0, 0, 0);
			}

			DoBall(entity);
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
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjCannon(ObjectMaster *a1)
{
	a1->Data1->NextAction = 0;

	a1->MainSub = &ObjCannon_Main;
	a1->DisplaySub = &ObjCannon_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Launch Ramp
void __cdecl SHLaunchRamp(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		/*njRotateY(0, 0x8000);
		njRotateX(0, a1->Data1->Rotation.x * 2);*/
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&SH_LAUNCHRAMP);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}
#pragma endregion

#pragma region Switch cases
CollisionData Cases_col[]{
	{ 0, 2, 0x77, 0, 0x800400,{ 0, 10, 0 },{ 20, 17, 20 }, 0, 0 },
};
CollisionData CasesOpen_col[]{
	{ 0, 2, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 20, 7, 20 }, 0, 0 },
};

void OBJCASE_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1.5f, 1.5f, 1.5f);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&OBJ_CASEBOTTOM);

		if (a1->Data1->Action == 0) {
			njTranslate(0, 0, 9.5f, 0);
			DrawModel(&OBJ_CASEMIDDLE);
			njTranslate(0, 0, 7, 0);
			DrawModel(&OBJ_CASETOP);
		}
		else if (a1->Data1->Action == 1) {
			njTranslate(0, 0, a1->Data1->Scale.z, 0);
			DrawModel(&OBJ_CASEMIDDLE);
		}
		else if (a1->Data1->Action == 2) DrawModel(&OBJ_CASEMIDDLE);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void OBJCASE_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

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
				Collision_Init(a1, CasesOpen_col, 1, 6u);
			}

		}

		AddToCollisionList(a1->Data1);
		OBJCASE_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl OBJCASE(ObjectMaster *a1)
{
	if (a1->Data1->Scale.y == 1 || IsSwitchPressed(a1->Data1->Scale.x)) {
		a1->Data1->Action = 2;
		Collision_Init(a1, CasesOpen_col, 1, 6u);
	}
	else {
		Collision_Init(a1, Cases_col, 1, 3u);
	}

	a1->MainSub = &OBJCASE_Main;
	a1->DisplaySub = &OBJCASE_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Goal Ring
void Capsule_Display_r(ObjectMaster *a1) {
	EntityData1 *v1;
	v1 = a1->Data1;
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);

		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njTranslate(0, 0, 20, 0);
		njRotateXYZ(nullptr, 0, v1->Rotation.y, 0);

		njScale(nullptr, v1->Scale.x - (v1->Scale.x * 0.2f), v1->Scale.y - (v1->Scale.y * 0.2f), v1->Scale.z - (v1->Scale.z * 0.2f));
		njRotateY(nullptr, -(v1->Rotation.y * 2));

		DrawQueueDepthBias = -6000.0f;
		DrawModel(&SH_GOALRING_THING);

		njRotateY(nullptr, -v1->Rotation.y);
		DrawModel(&SH_GOALRING_STAR);

		njScale(nullptr, v1->Scale.x * 1.2f, v1->Scale.y *1.2f, v1->Scale.z *1.2f);
		DrawModel(&SH_GOALRING_RING);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void Capsule_Main_r(ObjectMaster *a1)
{
	EntityData1 *v1;
	v1 = a1->Data1;

	if (IsPlayerInsideSphere(&v1->Position, 2000)) {
		v1->Rotation.y += 200;

		if (IsPlayerInsideSphere(&v1->Position, 30) && a1->Data1->Action == 0) {
			LoadLevelResults();
			a1->Data1->Action = 1;
		}

		if (a1->Data1->Action == 1) {
			auto entity = EntityData1Ptrs[0];
			entity->Position = v1->Position;
			if (v1->Scale.x > 0.05f) {
				v1->Scale.x -= 0.05f;
				v1->Scale.y -= 0.05f;
				v1->Scale.z -= 0.05f;
			}
		}
		else {
			v1->Scale.x = 1;
			v1->Scale.y = 1;
			v1->Scale.z = 1;


			if (anim % 60 == 0) if (EnableSounds) if (GetPlayerDistance(a1->Data1, 0) < 1007600.0) PlaySound(85, 0, 0, 0);
		}

		Capsule_Display_r(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl Capsule_Load_r(ObjectMaster *a1) {
	a1->DisplaySub = Capsule_Display_r;
	a1->MainSub = Capsule_Main_r;
	a1->DeleteSub = deleteSub_Global;
}
#pragma endregion

#pragma region Bobsled
void ObjBob_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateY(0, a1->Data1->Rotation.y);
		if (a1->Data1->Rotation.x != -16384) njRotateX(0, a1->Data1->Rotation.x);
		else njRotateX(0, 0);
		njTranslate(0, 0, 0, -5);

		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&SH_BOBSLED);

		//Handles
		njTranslate(0, 0, 6.78f, -6.355f);
		int btn = HeldButtons2[0];
		if (btn & Buttons_Left) {
			njRotateZ(0, 0x1000);
			DrawModel(&SH_BOBHANDLES);
			njRotateZ(0, -0x1000);
		}
		else if (btn & Buttons_Right) {
			njRotateZ(0, -0x1000);
			DrawModel(&SH_BOBHANDLES);
			njRotateZ(0, 0x1000);
		}
		else {
			DrawModel(&SH_BOBHANDLES);
		}
		njTranslate(0, 0, -6.78f, 6.355f);

		//Star
		njTranslate(0, 0, 5.445f, -18.989f);
		njRotateZ(0, -a1->Data1->Scale.z);
		DrawModel(&SH_BOBSTAR);
		njRotateZ(0, a1->Data1->Scale.z);
		njTranslate(0, 0, -5.445f, 18.989f);

		//Wheels
		njTranslate(0, 0, 5.029f, 14.417f);
		njScale(nullptr, 1.33F, 1, 1);
		njRotateX(0, -a1->Data1->Scale.z);
		DrawModel(&SH_BOBWHEELS);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjBob_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		if (IsPlayerInsideSphere(&a1->Data1->Position, 20.0f) == true) {
			if (a1->Data1->Action == 0) {
				auto entity = EntityData1Ptrs[0];
				//GetBobSled(SH_BOBLEFT, SH_BOBRIGHT, a1->Data1->Scale.x);
				a1->Data1->Position = entity->Position;
				a1->Data1->Rotation.x = entity->Rotation.x;
				a1->Data1->Rotation.y = entity->Rotation.y + 0x4000;
				a1->Data1->Scale.z += 800;

				/*if (CurrentLoop == -1) {
					a1->Data1->Action = 4;
				}*/
			}

			if (a1->Data1->Action == 4) {
				PlaySound(42, 0, 0, 0);
				CharObj2 * co2 = GetCharObj2(0);
				co2->Speed.y = 2;
				co2->Speed.x = 1;
				deleteSub_Global(a1);
			}
		}

		ObjBob_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjBob(ObjectMaster *a1)
{
	/*a1->MainSub = &ObjBob_Main;
	a1->DisplaySub = &ObjBob_Display;*/
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Wooden Boxes
CollisionData ObjBoxW_col{
	0, 2, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 13, 13, 13 }, 0, 0
};

void ObjBoxW_Display(ObjectMaster *a1)
{
	if (!MissedFrames && a1->SETData.SETData->SETEntry->Properties.z != 1) {
		njSetTexture(&SHCommonTextures);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		if (a1->Data1->Action == 0) DrawModel(SH_BOXW.basicdxmodel);
		else if (a1->Data1->Action == 1) {
			njTranslate(0, 0, -(a1->Data1->Scale.y / 3), 0);

			njTranslate(0, 0, 0, 12 + a1->Data1->Scale.y / 90);
			DrawModel(&SH_BOXW_A); //bar
			njTranslate(0, 0, 20, 0);

			njTranslate(0, 0, -(a1->Data1->Scale.y), (a1->Data1->Scale.y / 3));
			njRotateY(0, 0x5000 + a1->Data1->Scale.y * 5);
			DrawModel(&SH_BOXW_B); //big one
			njRotateY(0, -(0x5000 + a1->Data1->Scale.y * 5));

			njTranslate(0, 0, -(a1->Data1->Scale.y * 2), -(a1->Data1->Scale.y / 5));
			njRotateY(0, -0x3000);
			DrawModel(&SH_BOXW_C); //right
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void ObjBoxW_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		if (a1->SETData.SETData->SETEntry->Properties.z != 1) {
			if (a1->Data1->Action == 0) {
				if (OhNoImDead(a1->Data1, (ObjectData2*)a1->Data2)) {
					PlaySound(86, 0, 0, 0);
					a1->Data1->Action = 1;
					DynamicCOL_Remove(a1, a1->Data1->Object);
				}
				else AddToCollisionList(a1->Data1);
			}

			if (a1->Data1->Action == 1) {
				if (a1->Data1->Scale.y < 100) a1->Data1->Scale.y++;
				else a1->SETData.SETData->SETEntry->Properties.z = 1;
			}
			ObjBoxW_Display(a1);
		}
		else {

		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl ObjBoxW(ObjectMaster *a1)
{
	a1->Data1->Object = &SH_BOXW;
	AllocateObjectData2(a1, a1->Data1);
	Collision_Init(a1, &ObjBoxW_col, 1, 2u);
	if (a1->SETData.SETData->SETEntry->Properties.z != 1) AddToCollision(a1, 0);
	else a1->Data1->Action = 2;

	a1->MainSub = &ObjBoxW_Main;
	a1->DisplaySub = &ObjBoxW_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions) {
	helperFunctions.ReplaceFile("system\\heroescommon.PVM", "system\\heroescommon.pvmx");
	helperFunctions.RegisterCommonObjectPVM(shobjpvm);

	const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
	bool temp = false;
	temp = config->getBool("Objects", "GoalRing", true);
	if (temp) WriteJump((void*)0x46B170, Capsule_Load_r);

	temp = config->getBool("Objects", "DashPanel", true);
	if (temp) WriteJump((void*)0x7A4360, SHDashPanel);

	temp = config->getBool("Objects", "DashHoop", true);
	if (temp)WriteJump((void*)0x7A2470, SHDashHoop);

	temp = config->getBool("Objects", "LaunchRamp", true);
	if (temp) WriteJump((void*)0x500020, SHLaunchRamp);
	delete config;
}

void CommonObjects_OnFrame() {
	AnimateObjectsTextures(COMMONOBJLIST, LengthOfArray(COMMONOBJLIST), CommonAnimTexs, LengthOfArray(CommonAnimTexs));
	AnimateUV(Objects_UVShift, LengthOfArray(Objects_UVShift));
	Animate = false;
}