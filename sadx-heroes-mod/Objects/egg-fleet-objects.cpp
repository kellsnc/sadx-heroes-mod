#include "stdafx.h"

ModelInfo * EF_CANNON1;
ModelInfo * EF_BULLETS;

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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, &ObjReel, "OBJREEL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, EFCannon, "EFCannon" }
};
ObjectList EggFleetObjectList = { arraylengthandptrT(EggFleetObjectList_list, int) };

void EggFleetObjects_Init() {
	TexLists_Obj[HeroesLevelID_EggFleet] = EggFleetObjectTextures;
	ObjLists[HeroesLevelID_EggFleet * 8] = &EggFleetObjectList;
}