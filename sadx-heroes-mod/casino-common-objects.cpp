#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "casino-common-objects.h"

extern bool inSlot = false;

NJS_MODEL_SADX * CASINOOBJLIST[] = {
	&CP_FLIPPERL,
	&CP_BOBINAIR,
	&CP_BOBIN,
	&CP_SLOTS,
	&CP_SLOTL,
	&CP_DOORBOTTOM,
	&CP_DOORLEFT,
	&CP_DOORRIGHT,
	&CP_DOORTOP
};

#pragma region FlipperL
ObjectFunc(FlipperL_Main, 0x5DC890);
ObjectFunc(FlipperL, 0x5DCB40);
ObjectFunc(FlipperR_Main, 0x5DC9E0);
ObjectFunc(FlipperR, 0x5DCB90);

void FLIPPERS_display(ObjectMaster *a1)
{
	EntityData1 *v1 = a1->Data1;
	njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
	njPushMatrix(0);
	njTranslateV(0, &v1->Position);
	njRotateXYZ(nullptr, 8192, v1->Rotation.y, 0);

	if (a1->SETData.SETData->SETEntry->ObjectType == 42) {
		njRotateXYZ(0, 0, -8000, 0);
		if (IsPlayerInsideSphere(&a1->Data1->Position, 300) && (ControllerPointers[0]->HeldButtons & Buttons_A || ControllerPointers[0]->HeldButtons & Buttons_L)) {
			njRotateXYZ(0, 0, 10000, 0);
		}
	}
	else {
		njRotateXYZ(0, 0, 8000, 0);
		if (IsPlayerInsideSphere(&a1->Data1->Position, 300) && (ControllerPointers[0]->HeldButtons & Buttons_A || ControllerPointers[0]->HeldButtons & Buttons_R)) {
			njRotateXYZ(0, 0, -10000, 0);
		}
	}

	njScale(nullptr, 1, 1, 1);
	DrawQueueDepthBias = 6000;
	if (a1->SETData.SETData->SETEntry->ObjectType == 42)  njDrawModel_SADX(&CP_FLIPPERL);
	else njDrawModel_SADX(&CP_FLIPPERR);
	DrawQueueDepthBias = 0;
	njPopMatrix(1u);
}

void FLIPPERS_main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1400.0f)) {
		CharObj2 *co2 = GetCharObj2(0);
		if (co2->SurfaceFlags == 0x81 && IsPlayerInsideSphere(&a1->Data1->Position, 300)) {
			if (a1->SETData.SETData->SETEntry->ObjectType == 42)  FlipperL_Main(a1);
			else FlipperR_Main(a1);
		}
		else {
			FLIPPERS_display(a1);
		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void _cdecl FLIPPERS(ObjectMaster *a1) {
	if (CurrentLevel == 3) {
		matlist_8D5FAF219211B9279F3[0].attr_texId = 287;
		matlist_8D5FAF219211B9279F3[1].attr_texId = 293;
		matlist_8D5FAF219211B9279F3[2].attr_texId = 162;
		matlist_8D5FAF219211B9279F3[3].attr_texId = 163;
		matlist_8D5FAF219211B9279F3[4].attr_texId = 164;
	}
	else {
		matlist_8D5FAF219211B9279F3[0].attr_texId = 132;
		matlist_8D5FAF219211B9279F3[1].attr_texId = 138;
		matlist_8D5FAF219211B9279F3[2].attr_texId = 113;
		matlist_8D5FAF219211B9279F3[3].attr_texId = 114;
		matlist_8D5FAF219211B9279F3[4].attr_texId = 115;
	}

	if (a1->SETData.SETData->SETEntry->ObjectType == 42) FlipperL(a1);
	else FlipperR(a1);
	a1->MainSub = FLIPPERS_main;
	a1->DisplaySub = FLIPPERS_display;
	a1->DeleteSub = deleteSub_Global;
}

static  void __cdecl sub_5DC0D0_h(ObjectMaster *a1);
static Trampoline sub_5DC0D0_t(0x5DC0D0, 0x5DC158, sub_5DC0D0_h);
static  void __cdecl sub_5DC0D0_h(ObjectMaster *a1) {
	if (CurrentLevel != 9) {
		FLIPPERS_display(a1);
	}
	else {
		auto original = reinterpret_cast<decltype(sub_5DC0D0_h)*>(sub_5DC0D0_t.Target());
		original(a1);
	}
}
#pragma endregion

#pragma region Pintable Bumper
void __cdecl sub_5DBCD0_h(ObjectMaster *a2);
static Trampoline sub_5DBCD0_t(0x5DBCD0, 0x5DBD61, sub_5DBCD0_h);
void __cdecl sub_5DBCD0_h(ObjectMaster *a2)
{
	if (IsPlayerInsideSphere(&a2->Data1->Position, 1400.0f)) {
		if (CurrentLevel != 9) {
			if (CurrentLevel == 3) {
				matlist_8D5FA670FFE17C7A926[0].attr_texId = 287;
				matlist_8D5FA670FFE17C7A926[1].attr_texId = 293;
				matlist_8D5FA670FFE17C7A926[2].attr_texId = 142;
			}
			else {
				matlist_8D5FA670FFE17C7A926[0].attr_texId = 132;
				matlist_8D5FA670FFE17C7A926[1].attr_texId = 138;
				matlist_8D5FA670FFE17C7A926[2].attr_texId = 105;
			}

			EntityData1 *v1 = a2->Data1;

			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslateV(0, &v1->Position);
			if (v1->Rotation.x == 0) njRotateXYZ(nullptr, 8192, v1->Rotation.y, 0);
			else njRotateXYZ(nullptr, v1->Rotation.x, v1->Rotation.y, 0);
			njScale(nullptr, 1, 1, 1);
			DrawQueueDepthBias = 6000;
			njDrawModel_SADX(&CP_BOBIN);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
		else {
			auto original = reinterpret_cast<decltype(sub_5DC0D0_h)*>(sub_5DC0D0_t.Target());
			original(a2);
		}
	}
	else {
		deleteSub_Global(a2);
	}
}
#pragma endregion

#pragma region Dice
void CPDICE_Display(ObjectMaster *a1) {
	if (!DroppedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, a1->Data1->Scale.x, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPDICE_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		uint8_t type = (uint8_t)a1->Data1->Action;

		if (type > 9) {
			a1->Data1->Scale.x = 1;
			if (IsSwitchPressed(type)) {
				a1->Data1->Action = 1;
			}
		}

		if (type == 1) {
			char timer = a1->Data1->NextAction;
			if (timer == 0) {
				float state = a1->Data1->Rotation.z;
				float min = a1->Data1->Scale.y;
				float max = a1->Data1->Scale.z;
				float pos = a1->Data1->Position.y;

				if (state == 0) {
					if (pos < max) a1->Data1->Position.y += 3;
					else {
						a1->Data1->Position.y = max;
						a1->Data1->Rotation.z = 1;
						a1->Data1->NextAction = 60;
					}

				}
				else if (state == 1) {
					if (pos > min) a1->Data1->Position.y -= 3;
					else {
						a1->Data1->Position.y = min;
						a1->Data1->Rotation.z = 0;
						a1->Data1->NextAction = 60;
					}
				}
				if (a1->Data1->NextAction == 0) {
					float mid = (max - min) / 2;
					float currentmid = max - a1->Data1->Position.y;

					if (currentmid < mid) currentmid = currentmid / mid;
					else if (currentmid >= mid) {
						currentmid -= (currentmid - mid) * 2;
						currentmid = currentmid / mid;
					}
					a1->Data1->Scale.x = 1 + currentmid;
				}
			}
			else a1->Data1->NextAction--;
		}
		else if (type == 2) {
			a1->Data1->Rotation.y += 100;
			a1->Data1->Object->ang[1] = a1->Data1->Rotation.y;
		}

		a1->Data1->Object->scl[1] = a1->Data1->Scale.x;
		a1->Data1->Object->pos[1] = a1->Data1->Position.y;

		CPDICE_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPDICE(ObjectMaster *a1)
{
	if (CurrentLevel == 3) {
		matlist_8D5F8A8F0231A138580[0].attr_texId = 167;
		matlist_8D5F8A8F0231A138580[1].attr_texId = 315;
	}
	else {
		matlist_8D5F8A8F0231A138580[0].attr_texId = 118;
		matlist_8D5F8A8F0231A138580[1].attr_texId = 290;
	}

	a1->Data1->Object = &CP_DICEobj;

	a1->Data1->Action = a1->Data1->Scale.x;
	a1->Data1->NextAction = 0;

	if (a1->Data1->Action == 0) {
		a1->Data1->Scale.x = 1;
		AddToCollision(a1, 0);
	}
	else {
		if (a1->Data1->Action == 2) {
			a1->Data1->Object = &CP_DICE2obj;
			AddToCollision(a1, 1);
			a1->Data1->Scale.x = 1;
		}
		else {
			AddToCollision(a1, 1);
			a1->Data1->Scale.x = 0;
			if (a1->Data1->Rotation.x == 0) a1->Data1->Position.y == a1->Data1->Scale.y;
			else a1->Data1->Position.y == a1->Data1->Scale.z;
		}
	}

	a1->MainSub = &CPDICE_Main;
	a1->DisplaySub = &CPDICE_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Round bumpers
CollisionData Bumpers_col{
	0, 0, 0x77, 0, 0x800400,{ 0, 0, 0 },{ 1, 1, 1 }, 0, 0
};

void CPBOBINAIR_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		a1->Data1->Rotation.y += 10;

		if (IsPlayerInsideSphere(&a1->Data1->Position, 12)) {

			auto entity = EntityData1Ptrs[0];
			CharObj2 *co2 = GetCharObj2(0);

			if ((entity->Position.y > a1->Data1->Position.y + 12)
				|| (entity->Position.y < a1->Data1->Position.y - 12)) return;

			entity->Position = a1->Data1->Position;
			entity->Position.y += 20;
			co2->Speed.y = 4;

			if (GetCharacterID(0) == Characters_Sonic || GetCharacterID(0) == Characters_Tails) {
				entity->Status = Status_Ball;
				entity->Action = 8;
				co2->AnimationThing.Index = 14;
			}

			PlaySound(46, 0, 0, 0);
		}
		else AddToCollisionList(a1->Data1);

		displaySub_Global(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPBOBINAIR(ObjectMaster *a1)
{
	if (CurrentLevel == 3) {
		matlist_8D5F9635B407EB41FC7[0].attr_texId = 293;
		matlist_8D5F9635B407EB41FC7[1].attr_texId = 300;
		matlist_8D5F9635B407EB41FC7[2].attr_texId = 142;
	}
	else {
		matlist_8D5F9635B407EB41FC7[0].attr_texId = 138;
		matlist_8D5F9635B407EB41FC7[1].attr_texId = 145;
		matlist_8D5F9635B407EB41FC7[2].attr_texId = 105;
	}

	a1->Data1->Object = &CP_BOBINAIRobj;
	Collision_Init(a1, &Bumpers_col, 1, 2u);

	a1->MainSub = &CPBOBINAIR_Main;
	a1->DisplaySub = &displaySub_Global;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Alt Dashpanel
void CPDashPanel(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (CurrentLevel == 3) matlist_8D5FB297B8FAC6D4AD9[0].attr_texId = 144;
		else matlist_8D5FB297B8FAC6D4AD9[0].attr_texId = 107;

		njSetTexture(CurrentLevelTexlist);
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
		njDrawModel_SADX(&CP_DASHPANEL);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}
#pragma endregion

#pragma region Small Slot
int slotmin;
int slotmin2;
int slotmax;
int slotmax2;

void SlotCamera(ObjectMaster *a1) {
	auto entity = EntityData1Ptrs[0];
	Camera_Data1->Position.x = a1->Data1->Position.x;
	Camera_Data1->Position.y = a1->Data1->Position.y + 40;
	Camera_Data1->Position.z = a1->Data1->Position.z;
	if (a1->Data1->Rotation.y == 0) Camera_Data1->Position.z = a1->Data1->Position.z + 200;
	if (a1->Data1->Rotation.y == 0x4000) Camera_Data1->Position.z = a1->Data1->Position.x + 200;
	if (a1->Data1->Rotation.y == 0x8000) Camera_Data1->Position.z = a1->Data1->Position.z - 200;
	if (a1->Data1->Rotation.y == 0xC000 || a1->Data1->Rotation.y == 0xBFFF) Camera_Data1->Position.z = a1->Data1->Position.x - 200;
}

void TakeRings(char nb) {
	if (Rings < nb) Rings = 0;
	else Rings -= nb;
}

void CPSLOTS_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 8192, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&CP_SLOTS);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSLOTS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (a1->Data1->Scale.y == 0 && IsPlayerInsideSphere(&a1->Data1->Position, 20)) {
			int rdmtime = rand() % 350;
			if (rdmtime < 220) rdmtime = 220;
			a1->Data1->Scale.y = rdmtime;

			PlaySound(50, 0, 0, 0);
		}

		if (a1->Data1->Scale.y > 2) {
			a1->Data1->Scale.y--;
			auto entity = EntityData1Ptrs[0];
			entity->Position = a1->Data1->Position;

			int v1 = CP_SLOTS.mats[7].attr_texId;
			int v2 = CP_SLOTS.mats[8].attr_texId;
			int v3 = CP_SLOTS.mats[9].attr_texId;

			v1++;
			v2++;
			v3++;

			if (v1 == slotmax2) v1 = slotmin2;
			if (v2 == slotmax2) v2 = slotmin2;
			if (v3 == slotmax2) v3 = slotmin2;

			inSlot = true;

			CP_SLOTS.mats[7].attr_texId = v1;
			CP_SLOTS.mats[8].attr_texId = v2;
			CP_SLOTS.mats[9].attr_texId = v3;

			SlotCamera(a1);
		}

		if (a1->Data1->Scale.y == 2) {
			a1->Data1->Scale.y = 1;

			inSlot = false;

			int v1 = CP_SLOTS.mats[8].attr_texId;
			v1 -= slotmin;

			if (v1 != 6 && v1 != 8) PlaySound(51, 0, 0, 0);
			else PlaySound(52, 0, 0, 0);

			switch (v1) {
			case 1: AddRingsInterval_Delay5(50);
			case 2: AddRingsInterval_Delay5(30);
			case 3: AddRingsInterval_Delay5(20);
			case 4: AddRingsInterval_Delay5(10);
			case 5: AddRingsInterval_Delay5(20);
			case 6: TakeRings(35);
			case 7: AddRingsInterval_Delay5(30);
			case 8: TakeRings(10);
			}
		}

		if (a1->Data1->Scale.y == 1) if (anim % 320 == 0) a1->Data1->Scale.y = 0;

		CPSLOTS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPSLOTS(ObjectMaster *a1)
{
	if (CurrentLevel == 3) {
		matlist_8D5FBC4BC16C0F1F012[0].attr_texId = 287;
		matlist_8D5FBC4BC16C0F1F012[1].attr_texId = 295;
		matlist_8D5FBC4BC16C0F1F012[2].attr_texId = 141;
		matlist_8D5FBC4BC16C0F1F012[3].attr_texId = 142;
		matlist_8D5FBC4BC16C0F1F012[4].attr_texId = 192;
		matlist_8D5FBC4BC16C0F1F012[5].attr_texId = 165;
		matlist_8D5FBC4BC16C0F1F012[6].attr_texId = 141;
		matlist_8D5FBC4BC16C0F1F012[7].attr_texId = 302;
		matlist_8D5FBC4BC16C0F1F012[8].attr_texId = 301;
		matlist_8D5FBC4BC16C0F1F012[9].attr_texId = 308;
		matlist_8D5FBC4BC16C0F1F012[10].attr_texId = 344;
		matlist_8D5FBC4BC16C0F1F012[11].attr_texId = 168;

		slotmin = 300;
		slotmin2 = 301;
		slotmax = 308;
		slotmax2 = 309;
	}
	else {
		matlist_8D5FBC4BC16C0F1F012[0].attr_texId = 132;
		matlist_8D5FBC4BC16C0F1F012[1].attr_texId = 140;
		matlist_8D5FBC4BC16C0F1F012[2].attr_texId = 104;
		matlist_8D5FBC4BC16C0F1F012[3].attr_texId = 111;
		matlist_8D5FBC4BC16C0F1F012[4].attr_texId = 124;
		matlist_8D5FBC4BC16C0F1F012[5].attr_texId = 116;
		matlist_8D5FBC4BC16C0F1F012[6].attr_texId = 104;
		matlist_8D5FBC4BC16C0F1F012[7].attr_texId = 147;
		matlist_8D5FBC4BC16C0F1F012[8].attr_texId = 146;
		matlist_8D5FBC4BC16C0F1F012[9].attr_texId = 152;
		matlist_8D5FBC4BC16C0F1F012[10].attr_texId = 322;
		matlist_8D5FBC4BC16C0F1F012[11].attr_texId = 119;

		slotmin = 145;
		slotmin2 = 146;
		slotmax = 152;
		slotmax2 = 153;
	}

	int rdm = rand() % 8;
	int v1 = slotmin2 + rdm - 1;
	int v2 = slotmin2 + rdm;
	int v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	CP_SLOTS.mats[7].attr_texId = v1;
	CP_SLOTS.mats[8].attr_texId = v2;
	CP_SLOTS.mats[9].attr_texId = v3;

	a1->MainSub = &CPSLOTS_Main;
	a1->DisplaySub = &CPSLOTS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Large Slot
void CPSLOTL_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 8192, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&CP_SLOTL);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSLOTL_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (a1->Data1->Action == 0 && IsPlayerInsideSphere(&a1->Data1->Position, 20)) {
			a1->Data1->Action = 1;
			a1->Data1->Scale.y = 3;
			PlaySound(50, 0, 0, 0);
		}

		if (a1->Data1->Action == 1) {
			auto entity = EntityData1Ptrs[0];
			entity->Position = a1->Data1->Position;

			if (ControllerPointers[0]->PressedButtons & Buttons_A) {
				PlaySound(41, 0, 0, 0);
				if (a1->Data1->Scale.y == 3) a1->Data1->Scale.y = 2;
				else if (a1->Data1->Scale.y == 2) a1->Data1->Scale.y = 1;
				else if (a1->Data1->Scale.y == 1) {
					a1->Data1->Scale.y = 0;
					a1->Data1->Action = 2;
				}
			}

			if (a1->Data1->Scale.y == 3) {
				int v1 = CP_SLOTL.mats[17].attr_texId;
				int v2 = CP_SLOTL.mats[18].attr_texId;
				int v3 = CP_SLOTL.mats[19].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				CP_SLOTL.mats[17].attr_texId = v1;
				CP_SLOTL.mats[18].attr_texId = v2;
				CP_SLOTL.mats[19].attr_texId = v3;
			}

			if (a1->Data1->Scale.y >= 2) {
				int v1 = CP_SLOTL.mats[14].attr_texId;
				int v2 = CP_SLOTL.mats[15].attr_texId;
				int v3 = CP_SLOTL.mats[16].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				CP_SLOTL.mats[14].attr_texId = v1;
				CP_SLOTL.mats[15].attr_texId = v2;
				CP_SLOTL.mats[16].attr_texId = v3;
			}

			if (a1->Data1->Scale.y >= 1) {
				int v1 = CP_SLOTL.mats[11].attr_texId;
				int v2 = CP_SLOTL.mats[12].attr_texId;
				int v3 = CP_SLOTL.mats[13].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				CP_SLOTL.mats[11].attr_texId = v1;
				CP_SLOTL.mats[12].attr_texId = v2;
				CP_SLOTL.mats[13].attr_texId = v3;
			}

			SlotCamera(a1);
			inSlot = true;
		}

		if (a1->Data1->Action == 2) {
			a1->Data1->Action = 3;

			inSlot = false;

			int v1 = CP_SLOTL.mats[12].attr_texId;
			v1 -= slotmin;
			int v2 = CP_SLOTL.mats[15].attr_texId;
			v2 -= slotmin;
			int v3 = CP_SLOTL.mats[18].attr_texId;
			v3 -= slotmin;

			if (v1 == v2 && v2 == v3) {
				if (v1 != 6 && v1 != 8) PlaySound(51, 0, 0, 0);
				else PlaySound(52, 0, 0, 0);

				switch (v1) {
				case 1: AddRingsInterval_Delay5(250);
				case 2: AddRingsInterval_Delay5(230);
				case 3: AddRingsInterval_Delay5(220);
				case 4: AddRingsInterval_Delay5(210);
				case 5: AddRingsInterval_Delay5(220);
				case 6: TakeRings(135);
				case 7: AddRingsInterval_Delay5(230);
				case 8: TakeRings(110);
				}
			}
			else {
				v1 = CP_SLOTL.mats[11].attr_texId;
				v2 = CP_SLOTL.mats[14].attr_texId;
				v3 = CP_SLOTL.mats[17].attr_texId;

				if (v1 == v2 && v2 == v3) {
					if (v1 != 6 && v1 != 8) PlaySound(51, 0, 0, 0);
					else PlaySound(52, 0, 0, 0);

					switch (v1) {
					case 1: AddRingsInterval_Delay5(150);
					case 2: AddRingsInterval_Delay5(130);
					case 3: AddRingsInterval_Delay5(120);
					case 4: AddRingsInterval_Delay5(110);
					case 5: AddRingsInterval_Delay5(120);
					case 6: TakeRings(135);
					case 7: AddRingsInterval_Delay5(130);
					case 8: TakeRings(110);
					}
				}
				else {
					v1 = CP_SLOTL.mats[13].attr_texId;
					v2 = CP_SLOTL.mats[16].attr_texId;
					v3 = CP_SLOTL.mats[19].attr_texId;

					if (v1 == v2 && v2 == v3) {
						if (v1 != 6 && v1 != 8) PlaySound(51, 0, 0, 0);
						else PlaySound(52, 0, 0, 0);
						switch (v1) {
						case 1: AddRingsInterval_Delay5(150);
						case 2: AddRingsInterval_Delay5(130);
						case 3: AddRingsInterval_Delay5(120);
						case 4: AddRingsInterval_Delay5(110);
						case 5: AddRingsInterval_Delay5(120);
						case 6: TakeRings(135);
						case 7: AddRingsInterval_Delay5(130);
						case 8: TakeRings(110);
						}
					}
				}
			}


		}

		if (a1->Data1->Action == 3) if (anim % 320 == 0) a1->Data1->Action = 0;

		CPSLOTL_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPSLOTL(ObjectMaster *a1)
{
	if (CurrentLevel == 3) {
		matlist_8D5FBD241A52B7CBECB[0].attr_texId = 287;
		matlist_8D5FBD241A52B7CBECB[1].attr_texId = 295;
		matlist_8D5FBD241A52B7CBECB[2].attr_texId = 141;
		matlist_8D5FBD241A52B7CBECB[3].attr_texId = 142;
		matlist_8D5FBD241A52B7CBECB[4].attr_texId = 202;
		matlist_8D5FBD241A52B7CBECB[5].attr_texId = 165;
		matlist_8D5FBD241A52B7CBECB[6].attr_texId = 135;
		matlist_8D5FBD241A52B7CBECB[7].attr_texId = 140;
		matlist_8D5FBD241A52B7CBECB[8].attr_texId = 166;
		matlist_8D5FBD241A52B7CBECB[9].attr_texId = 344;
		matlist_8D5FBD241A52B7CBECB[10].attr_texId = 168;

		slotmin = 300;
		slotmin2 = 301;
		slotmax = 308;
		slotmax2 = 309;
	}
	else {
		matlist_8D5FBD241A52B7CBECB[0].attr_texId = 132;
		matlist_8D5FBD241A52B7CBECB[1].attr_texId = 140;
		matlist_8D5FBD241A52B7CBECB[2].attr_texId = 104;
		matlist_8D5FBD241A52B7CBECB[3].attr_texId = 111;
		matlist_8D5FBD241A52B7CBECB[4].attr_texId = 111;
		matlist_8D5FBD241A52B7CBECB[5].attr_texId = 116;
		matlist_8D5FBD241A52B7CBECB[6].attr_texId = 102;
		matlist_8D5FBD241A52B7CBECB[7].attr_texId = 103;
		matlist_8D5FBD241A52B7CBECB[8].attr_texId = 117;
		matlist_8D5FBD241A52B7CBECB[9].attr_texId = 322;
		matlist_8D5FBD241A52B7CBECB[10].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[11].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[12].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[13].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[14].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[15].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[16].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[17].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[18].attr_texId = 119;
		matlist_8D5FBD241A52B7CBECB[19].attr_texId = 119;
		slotmin = 145;
		slotmin2 = 146;
		slotmax = 152;
		slotmax2 = 153;
	}

	a1->Data1->Action = 0;

	CP_SLOTL.mats[13].attr_texId = 306;

	int rdm = rand() % 8;
	int v1 = slotmin2 + rdm - 1;
	int v2 = slotmin2 + rdm;
	int v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	CP_SLOTL.mats[17].attr_texId = v1;
	CP_SLOTL.mats[18].attr_texId = v2;
	CP_SLOTL.mats[19].attr_texId = v3;

	rdm = rand() % 8;
	v1 = slotmin2 + rdm - 1;
	v2 = slotmin2 + rdm;
	v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	CP_SLOTL.mats[14].attr_texId = v1;
	CP_SLOTL.mats[15].attr_texId = v2;
	CP_SLOTL.mats[16].attr_texId = v3;

	rdm = rand() % 8;
	v1 = slotmin2 + rdm - 1;
	v2 = slotmin2 + rdm;
	v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	CP_SLOTL.mats[11].attr_texId = v1;
	CP_SLOTL.mats[12].attr_texId = v2;
	CP_SLOTL.mats[13].attr_texId = v3;

	a1->MainSub = &CPSLOTL_Main;
	a1->DisplaySub = &CPSLOTL_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Door
void CPDOOR_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;

		if (a1->Data1->Scale.z != 0) {
			njTranslate(0, -a1->Data1->Scale.z, 0, 0);
			njDrawModel_SADX(&CP_DOORLEFT);
			njTranslate(0, a1->Data1->Scale.z * 2, 0, 0);
			njDrawModel_SADX(&CP_DOORRIGHT);
			njTranslate(0, -a1->Data1->Scale.z, -a1->Data1->Scale.z, 0);
			njDrawModel_SADX(&CP_DOORBOTTOM);
			njTranslate(0, 0, a1->Data1->Scale.z * 2, 0);
			njDrawModel_SADX(&CP_DOORTOP);
		}
		else {
			njDrawModel_SADX(&CP_DOORLEFT);
			njDrawModel_SADX(&CP_DOORRIGHT);
			njDrawModel_SADX(&CP_DOORTOP);
			njDrawModel_SADX(&CP_DOORBOTTOM);
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPDOOR_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (IsSwitchPressed(a1->Data1->Scale.x)) {

			if (a1->Data1->Action == 0) PlaySound(42, 0, 0, 0);

			a1->Data1->Action = 1;
			a1->Data1->Object->pos[1] = a1->Data1->Position.y + 200;


			if (a1->Data1->Scale.z != 60) a1->Data1->Scale.z += 2;
		}
		else {
			if (a1->Data1->Action == 1) PlaySound(42, 0, 0, 0);

			a1->Data1->Action = 0;
			a1->Data1->Object->pos[1] = a1->Data1->Position.y;

			if (a1->Data1->Scale.z != 0) a1->Data1->Scale.z -= 3;
		}

		CPDOOR_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPDOOR(ObjectMaster *a1)
{
	if (CurrentLevel == 3) {
		matlist_8D5FBE26E26587094F6[0].attr_texId = 53;
		matlist_8D5FBE26E26587094F6[1].attr_texId = 90;
		matlist_8D5FBE26E26587094F6[2].attr_texId = 125;
		matlist_8D5FBE26E26587094F6[3].attr_texId = 44;

		matlist_8D5FBE2AE120AFD83F3[0].attr_texId = 53;
		matlist_8D5FBE2AE120AFD83F3[1].attr_texId = 90;
		matlist_8D5FBE2AE120AFD83F3[2].attr_texId = 101;
		matlist_8D5FBE2AE120AFD83F3[3].attr_texId = 44;

		matlist_8D5FBE3073DF16C7F7C[0].attr_texId = 53;
		matlist_8D5FBE3073DF16C7F7C[1].attr_texId = 90;
		matlist_8D5FBE3073DF16C7F7C[2].attr_texId = 93;
		matlist_8D5FBE3073DF16C7F7C[3].attr_texId = 44;

		matlist_8D5FBE354DDE7DFD26E[0].attr_texId = 53;
		matlist_8D5FBE354DDE7DFD26E[1].attr_texId = 90;
		matlist_8D5FBE354DDE7DFD26E[2].attr_texId = 125;
		matlist_8D5FBE354DDE7DFD26E[3].attr_texId = 44;
	}
	else {
		matlist_8D5FBE26E26587094F6[0].attr_texId = 38;
		matlist_8D5FBE26E26587094F6[1].attr_texId = 75;
		matlist_8D5FBE26E26587094F6[2].attr_texId = 82;
		matlist_8D5FBE26E26587094F6[3].attr_texId = 26;

		matlist_8D5FBE2AE120AFD83F3[0].attr_texId = 38;
		matlist_8D5FBE2AE120AFD83F3[1].attr_texId = 75;
		matlist_8D5FBE2AE120AFD83F3[2].attr_texId = 67;
		matlist_8D5FBE2AE120AFD83F3[3].attr_texId = 26;

		matlist_8D5FBE3073DF16C7F7C[0].attr_texId = 38;
		matlist_8D5FBE3073DF16C7F7C[1].attr_texId = 75;
		matlist_8D5FBE3073DF16C7F7C[2].attr_texId = 71;
		matlist_8D5FBE3073DF16C7F7C[3].attr_texId = 26;

		matlist_8D5FBE354DDE7DFD26E[0].attr_texId = 38;
		matlist_8D5FBE354DDE7DFD26E[1].attr_texId = 75;
		matlist_8D5FBE354DDE7DFD26E[2].attr_texId = 82;
		matlist_8D5FBE354DDE7DFD26E[3].attr_texId = 26;
	}
	a1->Data1->Object = &CP_DOORCOL;
	AddToCollision(a1, 1);

	a1->MainSub = &CPDOOR_Main;
	a1->DisplaySub = &CPDOOR_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion