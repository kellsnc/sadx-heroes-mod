#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "casino-common-objects.h"

extern bool inSlot = false;

ModelInfo * CP_FLIPPER;
ModelInfo * CP_MOVDICE;
ModelInfo * CP_SLDDOOR;
ModelInfo * CP_SLOTMCS;
ModelInfo * CP_CSNOBOB;
ModelInfo * CP_DSHPANL;
ModelInfo * CP_RURETTO;

ObjectFunc(FlipperL_Main, 0x5DC890);
ObjectFunc(FlipperL, 0x5DCB40);
ObjectFunc(FlipperR_Main, 0x5DC9E0);
ObjectFunc(FlipperR, 0x5DCB90);

int slotmin;
int slotmin2;
int slotmax;
int slotmax2;

void HeroesFlippers_display(ObjectMaster *a1)
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

	DrawQueueDepthBias = 6000;
	if (a1->SETData.SETData->SETEntry->ObjectType == 42)  njDrawModel_SADX(CP_FLIPPER->getmodel()->basicdxmodel);
	else njDrawModel_SADX(CP_FLIPPER->getmodel()->child->basicdxmodel);
	DrawQueueDepthBias = 0;
	njPopMatrix(1u);
}

void HeroesFlippers_main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		CharObj2 *co2 = CharObj2Ptrs[0];
		if (co2->SurfaceFlags == 0x81 && IsPlayerInsideSphere(&a1->Data1->Position, 300)) {
			if (a1->SETData.SETData->SETEntry->ObjectType == 42)  FlipperL_Main(a1);
			else FlipperR_Main(a1);
		}
		else {
			a1->DisplaySub(a1);
		}
	}
}

void HeroesFlippers(ObjectMaster *a1) {
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

	a1->MainSub = HeroesFlippers_main;
	a1->DisplaySub = HeroesFlippers_display;
	a1->DeleteSub = deleteSub_Global;
}

//Hook sadx flippers display
static  void sub_5DC0D0_h(ObjectMaster *a1);
static Trampoline sub_5DC0D0_t(0x5DC0D0, 0x5DC158, sub_5DC0D0_h);
static  void sub_5DC0D0_h(ObjectMaster *a1) {
	if (CurrentLevel != 9) {
		HeroesFlippers_display(a1);
	}
	else {
		auto original = reinterpret_cast<decltype(sub_5DC0D0_h)*>(sub_5DC0D0_t.Target());
		original(a1);
	}
}

//Hook sadx bumpers display
void sub_5DBCD0_h(ObjectMaster *a2);
static Trampoline sub_5DBCD0_t(0x5DBCD0, 0x5DBD61, sub_5DBCD0_h);
void sub_5DBCD0_h(ObjectMaster *a2)
{
	if (!ClipSetObject(a2)) {
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
}

void CPDice_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
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

void CPDice_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action > 9) {
			a1->Data1->Scale.x = 1;
			if (IsSwitchPressed(a1->Data1->Action)) {
				a1->Data1->Action = 1;
			}
			return;
		}

		NJS_OBJECT * col;
		char timer = a1->Data1->NextAction;

		switch (a1->Data1->Action) {
		case 0:
			DynColRadius(a1, 100, 0);
			break;
		case 1:
			DynColRadius(a1, 100, 1);

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

			col = (NJS_OBJECT*)a1->Data1->LoopData;
			if (col) {
				col->scl[1] = a1->Data1->Scale.x;
				col->pos[1] = a1->Data1->Position.y;
			}
			
			break;
		case 2:
			DynColRadius(a1, 100, 1);
			a1->Data1->Rotation.y += 100;

			col = (NJS_OBJECT*)a1->Data1->LoopData;
			if (col) col->ang[1] = a1->Data1->Rotation.y;
			break;
		}
		
		a1->DisplaySub(a1);
	}
}

void CPDice(ObjectMaster *a1)
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
	}
	else {
		if (a1->Data1->Action == 2) {
			a1->Data1->Object = CP_MOVDICE->getmodel()->child;
			a1->Data1->Scale.x = 1;
		}
		else {
			a1->Data1->Scale.x = 0;
			if (a1->Data1->Rotation.x == 0) a1->Data1->Position.y == a1->Data1->Scale.y;
			else a1->Data1->Position.y == a1->Data1->Scale.z;
		}
	}

	a1->MainSub = &CPDice_Main;
	a1->DisplaySub = &CPDice_Display;
	a1->DeleteSub = &deleteSub_Global;
}

void CPGlass(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			a1->Data1->Action = 1;
		}

		for (int i = 0; i < LengthOfArray(Casino_Glass); ++i) {
			if (CurrentLevel == HeroesLevelID_CasinoPark && Casino_Glass[i].Model != 0) continue;
			if (CurrentLevel == HeroesLevelID_BingoHighway && Casino_Glass[i].Model != 1) continue;

			if (CheckModelDisplay2(Casino_Glass[i])) {
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

void CPBobInAir_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		a1->Data1->Rotation.y += 10;
		a1->Data1->CharIndex = IsPlayerInsideSphere(&a1->Data1->Position, 12);

		if (a1->Data1->CharIndex) {
			EntityData1 *entity = EntityData1Ptrs[a1->Data1->CharIndex - 1];
			CharObj2 *co2 = CharObj2Ptrs[a1->Data1->CharIndex - 1];

			if ((entity->Position.y > a1->Data1->Position.y + 12)
				|| (entity->Position.y < a1->Data1->Position.y - 12)) return;

			entity->Position = a1->Data1->Position;
			entity->Position.y += 20;
			co2->Speed.y = 4;

			if (GetCharacterID(a1->Data1->CharIndex) == Characters_Sonic || GetCharacterID(a1->Data1->CharIndex) == Characters_Tails) {
				entity->Status = Status_Ball;
				if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
					entity->Action = 8;
					co2->AnimationThing.Index = 14;
				}
				else {
					co2->AnimationThing.Index = 145;
				}
			}

			PlaySound(46, 0, 0, 0);
		}
		else AddToCollisionList(a1->Data1);

		a1->DisplaySub(a1);
	}
}

void CPBobInAir(ObjectMaster *a1)
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

	a1->MainSub = CPBobInAir_Main;
	a1->DisplaySub = displaySub_Global;
	a1->DeleteSub = deleteSub_Global;
}

void CPDashPanel(ObjectMaster *a1) {
	if (!MissedFrames) {
		NJS_MATERIAL * mat = CP_DSHPANL->getmodel()->basicdxmodel->mats;
		if (CurrentLevel == 3) mat[0].attr_texId = 144;
		else mat[0].attr_texId = 107;

		njSetTexture(CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);

		Rotation3 rot = a1->Data1->Rotation;
		if (rot.z) njRotateZ(0, rot.z);
		if (rot.x) njRotateX(0, rot.x);
		if (rot.y != -32768) njRotateY(0, (LOWORD(rot.y) + -32768));

		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CP_DSHPANL->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void SlotCamera(ObjectMaster *a1) {
	EntityData1 *entity = EntityData1Ptrs[0];
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

void CPSlotS_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 8192, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CP_SLOTMCS->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSlotS_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->basicdxmodel->mats;

		if (a1->Data1->Scale.y == 0 && IsPlayerInsideSphere(&a1->Data1->Position, 20)) {
			int rdmtime = rand() % 350;
			if (rdmtime < 220) rdmtime = 220;
			a1->Data1->Scale.y = rdmtime;

			PlaySound(50, 0, 0, 0);
		}

		if (a1->Data1->Scale.y > 2) {
			a1->Data1->Scale.y--;
			EntityData1 *entity = EntityData1Ptrs[0];
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

		CPSlotS_Display(a1);
	}
}

void CPSlotS(ObjectMaster *a1)
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

	a1->MainSub = CPSlotS_Main;
	a1->DisplaySub = CPSlotS_Display;
	a1->DeleteSub = deleteSub_Global;
}

void CPSlotL_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 8192, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(CP_SLOTMCS->getmodel()->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPSlotL_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		NJS_MATERIAL * material = CP_SLOTMCS->getmodel()->child->basicdxmodel->mats;

		if (a1->Data1->Action == 0 && IsPlayerInsideSphere(&a1->Data1->Position, 20)) {
			a1->Data1->Action = 1;
			a1->Data1->Scale.y = 3;
			PlaySound(50, 0, 0, 0);
		}

		if (a1->Data1->Action == 1) {
			EntityData1 *entity = EntityData1Ptrs[0];
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

		CPSlotL_Display(a1);
	}
}

void CPSlotL(ObjectMaster *a1)
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

	a1->MainSub = &CPSlotL_Main;
	a1->DisplaySub = &CPSlotL_Display;
	a1->DeleteSub = &deleteSub_Global;
}

void CPDoor_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
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

void CPDoor_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (IsSwitchPressed(a1->Data1->Scale.x)) {
			if (a1->Data1->Action == 0) PlaySound(42, 0, 0, 0);
			a1->Data1->Action = 1;
			
			deleteSub_Global(a1);

			if (a1->Data1->Scale.z != 60) a1->Data1->Scale.z += 2;
		}
		else {
			if (a1->Data1->Action == 1) PlaySound(42, 0, 0, 0);
			a1->Data1->Action = 0;

			DynColRadius(a1, 200, 0);

			if (a1->Data1->Scale.z != 0) a1->Data1->Scale.z -= 3;
		}

		CPDoor_Display(a1);
	}
}

void CPDoor(ObjectMaster *a1)
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

	a1->MainSub = &CPDoor_Main;
	a1->DisplaySub = &CPDoor_Display;
	a1->DeleteSub = &deleteSub_Global;
}

void CPRoulette_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		if (CurrentLevel == HeroesLevelID_BingoHighway) njRotateY(0, a1->Data1->Scale.z);
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPRoulette_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (CurrentLevel == HeroesLevelID_CasinoPark) a1->Data1->Rotation.y += 60;

		if (CurrentLevel == HeroesLevelID_BingoHighway) {
			DynColRadius(a1, 200, 0);
			a1->Data1->Scale.z += 60;
		}

		a1->DisplaySub(a1);
	}
}

void CPRoulette(ObjectMaster *a1)
{
	if (CurrentLevel == HeroesLevelID_CasinoPark) a1->Data1->Object = CP_RURETTO->getmodel();
	else a1->Data1->Object = &BH_ROURETTE;

	a1->MainSub = &CPRoulette_Main;
	a1->DisplaySub = &CPRoulette_Display;
	a1->DeleteSub = &deleteSub_Global;
}