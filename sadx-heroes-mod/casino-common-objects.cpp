#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "casino-common-objects.h"

extern bool inSlot = false;

SOI_LIST2 Casino_Glass[] = {
	{ 0,{ -8820.0674f, 3360, -2320.059f },{ 0, 16384, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1500.0f },
	{ 0,{ -15350.516f, 1550.1f, -860.4109f },{ 0, 32768, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1500.0f },
	{ 0,{ -14480.477f, 2400, 2770.116f },{ 0, 32768, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1500.0f },
	{ 1,{ 11050.07f, -4464.9f, -14880 },{ 0, -16384, 0 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f }
};

ModelInfo * CP_FLIPPER;
ModelInfo * CP_MOVDICE;
ModelInfo * CP_SLDDOOR;
ModelInfo * CP_SLOTMCS;
ModelInfo * CP_CSNOBOB;

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
	if (a1->SETData.SETData->SETEntry->ObjectType == 42)  njDrawModel_SADX(CP_FLIPPER->getmodel()->basicdxmodel);
	else njDrawModel_SADX(CP_FLIPPER->getmodel()->child->basicdxmodel);
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
	NJS_MATERIAL * material = CP_FLIPPER->getmodel()->basicdxmodel->mats;
	NJS_MATERIAL * material2 = CP_FLIPPER->getmodel()->child->basicdxmodel->mats;
	if (CurrentLevel == 3) {
		material[0].attr_texId = 287;
		material[1].attr_texId = 293;
		material[2].attr_texId = 162;
		material[3].attr_texId = 163;
		material[4].attr_texId = 164;
		material2[0].attr_texId = 287;
		material2[1].attr_texId = 293;
		material2[2].attr_texId = 162;
		material2[3].attr_texId = 163;
		material2[4].attr_texId = 164;
	}
	else {
		material[0].attr_texId = 132;
		material[1].attr_texId = 138;
		material[2].attr_texId = 113;
		material[3].attr_texId = 114;
		material[4].attr_texId = 115;
		material2[0].attr_texId = 132;
		material2[1].attr_texId = 138;
		material2[2].attr_texId = 113;
		material2[3].attr_texId = 114;
		material2[4].attr_texId = 115;
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
			NJS_MATERIAL * material = CP_CSNOBOB->getmodel()->child->basicdxmodel->mats;
			if (CurrentLevel == 3) {
				material[0].attr_texId = 287;
				material[1].attr_texId = 293;
				material[2].attr_texId = 142;
			}
			else {
				material[0].attr_texId = 132;
				material[1].attr_texId = 138;
				material[2].attr_texId = 105;
			}

			EntityData1 *v1 = a2->Data1;

			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslateV(0, &v1->Position);
			if (v1->Rotation.x == 0) njRotateXYZ(nullptr, 8192, v1->Rotation.y, 0);
			else njRotateXYZ(nullptr, v1->Rotation.x, v1->Rotation.y, 0);
			njScale(nullptr, 1, 1, 1);
			DrawQueueDepthBias = 6000;
			njDrawModel_SADX(CP_CSNOBOB->getmodel()->child->basicdxmodel);
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
	NJS_MATERIAL * material = CP_MOVDICE->getmodel()->basicdxmodel->mats;
	if (CurrentLevel == 3) {
		material[0].attr_texId = 167;
		material[1].attr_texId = 315;
	}
	else {
		material[0].attr_texId = 118;
		material[1].attr_texId = 290;
	}

	a1->Data1->Object = CP_MOVDICE->getmodel();

	a1->Data1->Action = a1->Data1->Scale.x;
	a1->Data1->NextAction = 0;

	if (a1->Data1->Action == 0) {
		a1->Data1->Scale.x = 1;
		AddToCollision(a1, 0);
	}
	else {
		if (a1->Data1->Action == 2) {
			a1->Data1->Object = CP_MOVDICE->getmodel()->child;
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

#pragma region Glass
void CPGlass_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(Casino_Glass); ++i) {
			if (CurrentLevel == HeroesLevelID_CasinoPark && Casino_Glass[i].Model != 0) return;
			if (CurrentLevel == HeroesLevelID_BingoHighway && Casino_Glass[i].Model != 1) return;

			if (CheckModelDisplay(Casino_Glass[i])) {
				SOI_LIST2 item = Casino_Glass[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(&CP_Glass); break;
				case 1: njDrawModel_SADX(&BH_Glass); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void CPGlass(ObjectMaster *a1) {
	a1->DisplaySub = CPGlass_Display;
	a1->MainSub = CPGlass_Display;
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
	NJS_MATERIAL * material = CP_CSNOBOB->getmodel()->basicdxmodel->mats;
	if (CurrentLevel == 3) {
		material[0].attr_texId = 293;
		material[1].attr_texId = 300;
		material[2].attr_texId = 142;
	}
	else {
		material[0].attr_texId = 138;
		material[1].attr_texId = 145;
		material[2].attr_texId = 105;
	}

	a1->Data1->Object = CP_CSNOBOB->getmodel();
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
		njDrawModel_SADX(CP_SLOTMCS->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSLOTS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->basicdxmodel->mats;

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

			int v1 = material[7].attr_texId;
			int v2 = material[8].attr_texId;
			int v3 = material[9].attr_texId;

			v1++;
			v2++;
			v3++;

			if (v1 == slotmax2) v1 = slotmin2;
			if (v2 == slotmax2) v2 = slotmin2;
			if (v3 == slotmax2) v3 = slotmin2;

			inSlot = true;

			material[7].attr_texId = v1;
			material[8].attr_texId = v2;
			material[9].attr_texId = v3;

			SlotCamera(a1);
		}

		if (a1->Data1->Scale.y == 2) {
			a1->Data1->Scale.y = 1;

			inSlot = false;

			int v1 = material[8].attr_texId;
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
	NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->basicdxmodel->mats;
	if (CurrentLevel == HeroesLevelID_CasinoPark) {
		material[0].attr_texId = 287;
		material[1].attr_texId = 295;
		material[2].attr_texId = 141;
		material[3].attr_texId = 142;
		material[4].attr_texId = 192;
		material[5].attr_texId = 165;
		material[6].attr_texId = 141;
		material[7].attr_texId = 302;
		material[8].attr_texId = 301;
		material[9].attr_texId = 308;
		material[10].attr_texId = 344;
		material[11].attr_texId = 168;

		slotmin = 300;
		slotmin2 = 301;
		slotmax = 308;
		slotmax2 = 309;
	}
	else {
		material[0].attr_texId = 132;
		material[1].attr_texId = 140;
		material[2].attr_texId = 104;
		material[3].attr_texId = 111;
		material[4].attr_texId = 124;
		material[5].attr_texId = 116;
		material[6].attr_texId = 104;
		material[7].attr_texId = 147;
		material[8].attr_texId = 146;
		material[9].attr_texId = 152;
		material[10].attr_texId = 322;
		material[11].attr_texId = 119;

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

	material[7].attr_texId = v1;
	material[8].attr_texId = v2;
	material[9].attr_texId = v3;

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
		njDrawModel_SADX(CP_SLOTMCS->getmodel()->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSLOTL_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->child->basicdxmodel->mats;

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
				int v1 = material[17].attr_texId;
				int v2 = material[18].attr_texId;
				int v3 = material[19].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				material[17].attr_texId = v1;
				material[18].attr_texId = v2;
				material[19].attr_texId = v3;
			}

			if (a1->Data1->Scale.y >= 2) {
				int v1 = material[14].attr_texId;
				int v2 = material[15].attr_texId;
				int v3 = material[16].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				material[14].attr_texId = v1;
				material[15].attr_texId = v2;
				material[16].attr_texId = v3;
			}

			if (a1->Data1->Scale.y >= 1) {
				int v1 = material[11].attr_texId;
				int v2 = material[12].attr_texId;
				int v3 = material[13].attr_texId;

				if (anim % 10 == 0) {
					v1++;
					v2++;
					v3++;
				}

				if (v1 == slotmax2) v1 = slotmin2;
				if (v2 == slotmax2) v2 = slotmin2;
				if (v3 == slotmax2) v3 = slotmin2;

				material[11].attr_texId = v1;
				material[12].attr_texId = v2;
				material[13].attr_texId = v3;
			}

			SlotCamera(a1);
			inSlot = true;
		}

		if (a1->Data1->Action == 2) {
			a1->Data1->Action = 3;

			inSlot = false;

			int v1 = material[12].attr_texId;
			v1 -= slotmin;
			int v2 = material[15].attr_texId;
			v2 -= slotmin;
			int v3 = material[18].attr_texId;
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
				v1 = material[11].attr_texId;
				v2 = material[14].attr_texId;
				v3 = material[17].attr_texId;

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
					v1 = material[13].attr_texId;
					v2 = material[16].attr_texId;
					v3 = material[19].attr_texId;

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
	NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->child->basicdxmodel->mats;
	if (CurrentLevel == HeroesLevelID_CasinoPark) {
		material[0].attr_texId = 287;
		material[1].attr_texId = 295;
		material[2].attr_texId = 141;
		material[3].attr_texId = 142;
		material[4].attr_texId = 202;
		material[5].attr_texId = 165;
		material[6].attr_texId = 135;
		material[7].attr_texId = 140;
		material[8].attr_texId = 166;
		material[9].attr_texId = 344;
		material[10].attr_texId = 168;
		slotmin = 300;
		slotmin2 = 301;
		slotmax = 308;
		slotmax2 = 309;
	}
	else {
		material[0].attr_texId = 132;
		material[1].attr_texId = 140;
		material[2].attr_texId = 104;
		material[3].attr_texId = 111;
		material[4].attr_texId = 111;
		material[5].attr_texId = 116;
		material[6].attr_texId = 102;
		material[7].attr_texId = 103;
		material[8].attr_texId = 117;
		material[9].attr_texId = 322;
		material[10].attr_texId = 119;
		material[11].attr_texId = 119;
		material[12].attr_texId = 119;
		material[13].attr_texId = 119;
		material[14].attr_texId = 119;
		material[15].attr_texId = 119;
		material[16].attr_texId = 119;
		material[17].attr_texId = 119;
		material[18].attr_texId = 119;
		material[19].attr_texId = 119;
		slotmin = 145;
		slotmin2 = 146;
		slotmax = 152;
		slotmax2 = 153;
	}

	a1->Data1->Action = 0;

	material[13].attr_texId = 306;

	int rdm = rand() % 8;
	int v1 = slotmin2 + rdm - 1;
	int v2 = slotmin2 + rdm;
	int v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	material[17].attr_texId = v1;
	material[18].attr_texId = v2;
	material[19].attr_texId = v3;

	rdm = rand() % 8;
	v1 = slotmin2 + rdm - 1;
	v2 = slotmin2 + rdm;
	v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	material[14].attr_texId = v1;
	material[15].attr_texId = v2;
	material[16].attr_texId = v3;

	rdm = rand() % 8;
	v1 = slotmin2 + rdm - 1;
	v2 = slotmin2 + rdm;
	v3 = slotmin2 + rdm + 1;

	if (v1 == slotmin) v1 = slotmax;
	if (v3 == slotmax) v1 = slotmin2;

	material[11].attr_texId = v1;
	material[12].attr_texId = v2;
	material[13].attr_texId = v3;

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
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->basicdxmodel);
			njTranslate(0, a1->Data1->Scale.z * 2, 0, 0);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->child->child->basicdxmodel);
			njTranslate(0, -a1->Data1->Scale.z, -a1->Data1->Scale.z, 0);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->basicdxmodel);
			njTranslate(0, 0, a1->Data1->Scale.z * 2, 0);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->child->basicdxmodel);
		}
		else {
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->basicdxmodel);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->basicdxmodel);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->child->basicdxmodel);
			njDrawModel_SADX(CP_SLDDOOR->getmodel()->child->child->child->basicdxmodel);
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
	NJS_MATERIAL * material_left = CP_SLDDOOR->getmodel()->basicdxmodel->mats;
	NJS_MATERIAL * material_bottom = CP_SLDDOOR->getmodel()->child->basicdxmodel->mats;
	NJS_MATERIAL * material_top = CP_SLDDOOR->getmodel()->child->child->basicdxmodel->mats;
	NJS_MATERIAL * material_right = CP_SLDDOOR->getmodel()->child->child->child->basicdxmodel->mats;

	if (CurrentLevel == 3) {
		material_left[0].attr_texId = 53;
		material_left[1].attr_texId = 90;
		material_left[2].attr_texId = 125;
		material_left[3].attr_texId = 44;

		material_bottom[0].attr_texId = 53;
		material_bottom[1].attr_texId = 90;
		material_bottom[2].attr_texId = 101;
		material_bottom[3].attr_texId = 44;

		material_top[0].attr_texId = 53;
		material_top[1].attr_texId = 90;
		material_top[2].attr_texId = 93;
		material_top[3].attr_texId = 44;

		material_right[0].attr_texId = 53;
		material_right[1].attr_texId = 90;
		material_right[2].attr_texId = 125;
		material_right[3].attr_texId = 44;
	}
	else {
		material_left[0].attr_texId = 38;
		material_left[1].attr_texId = 75;
		material_left[2].attr_texId = 82;
		material_left[3].attr_texId = 26;

		material_bottom[0].attr_texId = 38;
		material_bottom[1].attr_texId = 75;
		material_bottom[2].attr_texId = 67;
		material_bottom[3].attr_texId = 26;

		material_top[0].attr_texId = 38;
		material_top[1].attr_texId = 75;
		material_top[2].attr_texId = 71;
		material_top[3].attr_texId = 26;

		material_right[0].attr_texId = 38;
		material_right[1].attr_texId = 75;
		material_right[2].attr_texId = 82;
		material_right[3].attr_texId = 26;
	}
	a1->Data1->Object = &CP_DOORCOL;
	AddToCollision(a1, 1);

	a1->MainSub = &CPDOOR_Main;
	a1->DisplaySub = &CPDOOR_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion