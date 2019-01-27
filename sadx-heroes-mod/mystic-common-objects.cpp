#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "mystic-common-objects.h"

ModelInfo * MM_MOVPLAT;
ModelInfo * MM_MYSDOOR;
ModelInfo * MM_MYSWALL;

#pragma region Warps
float hclight = 0;

CollisionData Warps_col[]{
	{ 0, 0, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 10, 10, 10 }, 0, 0 },
	{ 0, 0, 0x77, 0, 0x800400,{ 0, 10, 0 },{ 10, 10, 10 }, 0, 0 },
};

void HCWARP_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, 0, 1);
		njScale(nullptr, 1.5f, 1.5f, 1.5f);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&HC_SWITCH);
		if (a1->Data1->Action < 2) {
			njTranslate(0, 0, 8, 0);
			njRotateY(0, a1->Data1->Rotation.y);
			njDrawModel_SADX(&HC_SWITCH_BALL);
			njDrawModel_SADX(&HC_SWITCH_EFFECT);
		}
		else {
			njTranslate(0, 0, 5, 0);
			njDrawModel_SADX(&HC_SWITCH_BALL);
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCWARP_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		ObjectData2 *od2 = (ObjectData2*)a1->Data2;
		if (a1->Data1->Action == 0) {
			a1->Data1->Rotation.y += 100;

			od2->vector_a = a1->Data1->Position;
			od2->vector_a.y += 10;

			if (IsPlayerInsideSphere(&od2->vector_a, 15)) {
				if (EnableSounds) PlaySound(41, 0, 0, 0);
				auto entity = EntityData1Ptrs[0];
				od2->vector_a = entity->Position;
				a1->Data1->Action = 1;
			}
		}

		if (a1->Data1->Action == 1) {
			auto entity = EntityData1Ptrs[0];
			entity->Position = od2->vector_a;
			if (a1->Data1->Rotation.z = 2) od2->vector_a.y += 0.05f;
			else od2->vector_a.y -= 0.05f;

			entity->Status = 0;
			CharObj2 * co2 = GetCharObj2(0);
			if (GetCharacterID(0) == Characters_Sonic) {
				co2->AnimationThing.Index = 18;
			}
			else if (GetCharacterID(0) == Characters_Tails || GetCharacterID(0) == Characters_Knuckles) {
				co2->AnimationThing.Index = 19;
			}
			if (a1->Data1->NextAction < 100) { 
				a1->Data1->NextAction += 1; 
				set_specular_blend(0, 4);
				set_specular_blend(1, 4);
				set_specular_blend(2, 4);
				set_specular_blend(3, 4);
				set_specular_blend(7, 4);
				hclight = a1->Data1->NextAction;
				hclight /= 100;
				set_specular_blend_factor(hclight);
			}
			else {
				if (a1->Data1->Scale.x != 0) {

					if (a1->Data1->Rotation.z == 2) {
						entity->Position.x += a1->Data1->Scale.x;
						entity->Position.y += a1->Data1->Scale.y;
						entity->Position.z += a1->Data1->Scale.z;
					}
					else {
						entity->Position = a1->Data1->Scale;
					}

					if (EnableSounds) PlaySound(42, 0, 0, 0);
					if (a1->Data1->Rotation.z == 1) Camera_Data1->Position = entity->Position;
				}
				a1->Data1->Action = 2;
			}
		}

		if (a1->Data1->Action == 2) {
			if (hclight > 0) {
				hclight -= 0.05f;
				set_specular_blend_factor(hclight);
			}
			else {
				if (a1->Data1->Rotation.z == 2) a1->Data1->Action = 0;
				else a1->Data1->Action = 3;
				set_blend(-1, -1);
			}
		}

		AddToCollisionList(a1->Data1);
		HCWARP_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCWARP(ObjectMaster *a1)
{
	if (CurrentLevel == 10) {
		matlist_8D6060A4B230E713870[0].attr_texId = 154;
		matlist_8D6060A9096106CE9BF[0].attr_texId = 154;
		matlist_8D606C86190A15FDB23[0].attr_texId = 155;
	}
	else {
		matlist_8D6060A4B230E713870[0].attr_texId = 98;
		matlist_8D6060A9096106CE9BF[0].attr_texId = 98;
		matlist_8D606C86190A15FDB23[0].attr_texId = 99;
	}

	Collision_Init(a1, Warps_col, 2, 2u);
	if (a1->Data1->Rotation.y == 1) a1->Data1->Action = 2;
	AllocateObjectData2(a1, a1->Data1);

	a1->MainSub = &HCWARP_Main;
	a1->DisplaySub = &HCWARP_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Doors
extern NJS_OBJECT CP_DOORCOL;

void HCDOOR_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;

		njTranslate(0, -40, 0, 0);
		if (a1->Data1->Action == 0) {
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_LEFT);
		}
		else if (a1->Data1->Action == 1) {
			njRotateY(0, (a1->Data1->NextAction * 130));
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_LEFT);
			njRotateY(0, -(a1->Data1->NextAction * 130));
		}
		else if (a1->Data1->Action == 2) {
			njRotateY(0, 13000);
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_LEFT);
			njRotateY(0, -13000);
		}

		njTranslate(0, 80, 0, 0);
		if (a1->Data1->Action == 0) {
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->child->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_RIGHT);
		}
		else if (a1->Data1->Action == 1) {
			njRotateY(0, -(a1->Data1->NextAction * 130));
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->child->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_RIGHT);
			njRotateY(0, (a1->Data1->NextAction * 130));
		}
		else if (a1->Data1->Action == 2) {
			njRotateY(0, -13000);
			if (CurrentLevel == 10) njDrawModel_SADX(MM_MYSDOOR->getmodel()->child->basicdxmodel);
			else njDrawModel_SADX(&HC_DOOR_RIGHT);
			njRotateY(0, 13000);
		}

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCDOOR_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 3500.0f)) {
		if (a1->Data1->Action == 0) {
			if (a1->Data1->Scale.y == 0) {
				if (IsSwitchPressed(a1->Data1->Scale.x)) {
					a1->Data1->Action = 1;
					a1->Data1->Object->pos[1] = 0;
					PlaySound(44, 0, 0, 0);
				}
			}
			else {
				if (IsPlayerInsideSphere(&a1->Data1->Scale, 100)) {
					a1->Data1->Action = 1;
					a1->Data1->Object->pos[1] = 0;
					PlaySound(44, 0, 0, 0);
				}
			}

		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->NextAction < 100) a1->Data1->NextAction += 2;
			else {
				a1->Data1->Action = 2;
				PlaySound(45, 0, 0, 0);
			}
		}

		HCDOOR_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCDOOR(ObjectMaster *a1)
{
	a1->Data1->Object = &CP_DOORCOL;
	if (a1->Data1->Scale.z == 1) a1->Data1->Action = 2;
	else AddToCollision(a1, 1);

	a1->MainSub = &HCDOOR_Main;
	a1->DisplaySub = &HCDOOR_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Walls
CollisionData HCWALL_col{
	0, 0, 0x77, 0, 0x800400,{ 0, 40, 20 },{ 38, 40, 1 }, 0, 0
};

void HCWALL_Display(ObjectMaster *a1)
{
	if (!MissedFrames && a1->SETData.SETData->SETEntry->Properties.z != 1) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;

		if (a1->Data1->Action == 0) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_CLOSE2);
				else njDrawModel_SADX(&HC_WALL_CLOSE);
			}
		}
		else if (a1->Data1->Action == 1) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->child->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_OPEN2);
				else njDrawModel_SADX(&HC_WALL_OPEN);
			}
			njTranslate(0, 0, -(a1->Data1->Scale.y / 3), 0);

			njTranslate(nullptr, 0, 0, 12 + a1->Data1->Scale.y / 90);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
			njTranslate(nullptr, 0, 40, 0);

			njTranslate(nullptr, 0, -(a1->Data1->Scale.y), (a1->Data1->Scale.y / 3));
			njRotateY(0, 0x5000 + a1->Data1->Scale.y * 5);
			njScale(0, 0.8f, 0.8f, 0.8f);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
			njRotateY(0, -(0x5000 + a1->Data1->Scale.y * 5));

			njTranslate(nullptr, 0, -(a1->Data1->Scale.y * 2), -(a1->Data1->Scale.y / 5));
			njRotateY(0, -0x3000);
			njScale(0, 0.6f, 0.6f, 0.6f);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
		}
		else if (a1->Data1->Action == 2) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->child->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_OPEN2);
				else njDrawModel_SADX(&HC_WALL_OPEN);
			}
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCWALL_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 3500.0f)) {
		if (a1->Data1->Action == 0) {
			if (OhNoImDead(a1->Data1, (ObjectData2*)a1->Data2)) {
				PlaySound(86, 0, 0, 0);
				a1->Data1->Action = 1;
				a1->Data1->Object->pos[1] = 0;
			}
			else AddToCollisionList(a1->Data1);
		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->Scale.y < 100) a1->Data1->Scale.y++;
			else a1->Data1->Action = 2;

			if (a1->Data1->Scale.z != 0) SetSwitchPressedState(a1->Data1->Scale.z, 1);
		}
		HCWALL_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCWALL(ObjectMaster *a1)
{
	a1->Data1->Object = &CP_DOORCOL;
	if (a1->Data1->Scale.z == 0) {
		AllocateObjectData2(a1, a1->Data1);
		Collision_Init(a1, &HCWALL_col, 1, 5u);
		AddToCollision(a1, 1);
	}
	else {
		if (IsSwitchPressed(a1->Data1->Scale.z)) {
			a1->Data1->Action = 2;
		}
		else {
			AllocateObjectData2(a1, a1->Data1);
			Collision_Init(a1, &HCWALL_col, 1, 2u);
			AddToCollision(a1, 1);
		}
	}

	a1->MainSub = &HCWALL_Main;
	a1->DisplaySub = &HCWALL_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Moving Platform
void HCPLATFORM_Display(ObjectMaster *a1) {
	if (!DroppedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		if (CurrentLevel == 7 && a1->Data1->Position.x < 8000) njRotateX(0, 0x8000);
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCPLATFORM_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		uint8_t type = (uint8_t)a1->Data1->Action;

		if (type != 0) {
			if (anim % 80 == 0) if (EnableSounds) if (GetPlayerDistance(a1->Data1, 0) < 307600.0) PlaySound(46, 0, 0, 0);
			if (type == 1) {
				char timer = a1->Data1->NextAction;
				if (timer == 0) {
					float state = a1->Data1->Rotation.z;
					float min = a1->Data1->Scale.y;
					float max = a1->Data1->Scale.z;
					float pos = a1->Data1->Position.y;

					if (state == 0) {
						if (pos < max) a1->Data1->Position.y += 0.5f;
						else {
							a1->Data1->Position.y = max;
							a1->Data1->Rotation.z = 1;
							a1->Data1->NextAction = 10;
						}

					}
					else if (state == 1) {
						if (pos > min) a1->Data1->Position.y -= 0.5f;
						else {
							a1->Data1->Position.y = min;
							a1->Data1->Rotation.z = 0;
							a1->Data1->NextAction = 10;
						}
					}
				}
				else a1->Data1->NextAction--;
			}
			else if (type > 1) {
				ObjectData2 * od2 = (ObjectData2*)a1->Data2;
				NJS_VECTOR orig = od2->vector_a;
				NJS_VECTOR dest = od2->vector_b;

				od2->vector_c = a1->Data1->Position; //get the position before transform

													 //some bad things to get a movement speed
				float dist = (2 * (dest.x - orig.x) + 2 * (dest.y - orig.y) + 2 * (dest.z - orig.z)) / 2;
				od2->field_C4 += 1 / (abs(dist) * 2);

				//translate the position
				a1->Data1->Position.x = (dest.x - orig.x) * od2->field_C4 + orig.x;
				a1->Data1->Position.y = (dest.y - orig.y) * od2->field_C4 + orig.y;
				a1->Data1->Position.z = (dest.z - orig.z) * od2->field_C4 + orig.z;

				//if we've reached the destination, swap dest and orig
				if (od2->field_C4 > 1) {
					od2->field_C4 = 0;
					a1->Data1->Scale = od2->vector_b;
					od2->vector_b = od2->vector_a;
					od2->vector_a = a1->Data1->Scale;
				}

				//inacurate check to get if the character is on the platform
				auto entity = EntityData1Ptrs[0];

				if (entity->Position.x > a1->Data1->Position.x - 30
					&& entity->Position.x < a1->Data1->Position.x + 30
					&& entity->Position.z > a1->Data1->Position.z - 30
					&& entity->Position.z < a1->Data1->Position.z + 30
					&& entity->Position.y > a1->Data1->Position.y
					&& entity->Position.y < a1->Data1->Position.y + 30) {

					//use the old and new position to offset the character
					entity->Position.x += a1->Data1->Position.x - od2->vector_c.x;
					entity->Position.y += a1->Data1->Position.y - od2->vector_c.y;
					entity->Position.z += a1->Data1->Position.z - od2->vector_c.z;
				}
			}
		}

		//Dynamic collision position
		a1->Data1->Object->pos[0] = a1->Data1->Position.x;
		a1->Data1->Object->pos[1] = a1->Data1->Position.y;
		a1->Data1->Object->pos[2] = a1->Data1->Position.z;

		HCPLATFORM_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCPLATFORM(ObjectMaster *a1)
{
	a1->Data1->Object = &HC_PLATFORM;
	if (a1->Data1->Rotation.y == 1) a1->Data1->Object = MM_MOVPLAT->getmodel();

	if (CurrentLevel == 10) {
		matlist_8D606F400D46546E108[0].attr_texId = 24;
		matlist_8D606F400D46546E108[2].attr_texId = 101;
		matlist_8D606F400D46546E108[3].attr_texId = 32; //11
		matlist_8D606F400D46546E108[4].attr_texId = 24; //25
		matlist_8D606F400D46546E108[5].attr_texId = 24;
		matlist_8D606F400D46546E108[6].attr_texId = 101;
	}
	else {
		matlist_8D606F400D46546E108[0].attr_texId = 9;
		matlist_8D606F400D46546E108[2].attr_texId = 10;
		matlist_8D606F400D46546E108[3].attr_texId = 11;
		matlist_8D606F400D46546E108[4].attr_texId = 25;
		matlist_8D606F400D46546E108[5].attr_texId = 25;
		matlist_8D606F400D46546E108[6].attr_texId = 32;
	}

	if (a1->Data1->Scale.y == 0) a1->Data1->Action = 0;
	else if (a1->Data1->Scale.x == 0) a1->Data1->Action = 1;
	else {
		/* if all the parameters are set, the platform goes in all directions */

		a1->Data1->Action = 2;
		AllocateObjectData2(a1, a1->Data1);  //we need more object variables
		ObjectData2 * od2 = (ObjectData2*)a1->Data2;
		od2->vector_a = a1->Data1->Position; //orig
		od2->vector_b = a1->Data1->Scale; //dest
	}

	if (a1->Data1->Action > 0) AddToCollision(a1, 1); //moving collision
	else AddToCollision(a1, 0); //static collision

	a1->MainSub = &HCPLATFORM_Main;
	a1->DisplaySub = &HCPLATFORM_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion