#include "stdafx.h"
#include "hang-castle-objects.h"

static int hcflagtimer = 0;

void _cdecl HCWarp(ObjectMaster *a1);
void HCDoor(ObjectMaster *a1);
void HCWall(ObjectMaster *a1);
void HCPlatform(ObjectMaster *a1);
void HCTorch(ObjectMaster *a1);

ModelInfo * HC_HCBLADE;
ModelInfo * HC_HPLANTA;
ModelInfo * HC_HPLANTB;
ModelInfo * HC_POLFLAG;
ModelInfo * HC_SPDSIGN;
ModelInfo * HC_SPKTREE;
ModelInfo * HC_SKYMDLS;
ModelInfo * HC_MCLOUDS;

void HCFlag_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.x == 1) DrawObjModel(a1, HC_POLFLAG->getmodel()->child->basicdxmodel, false);
	else DrawObjModel(a1, HC_POLFLAG->getmodel()->basicdxmodel, false);

	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njRotateX(0, -a1->Data1->Rotation.x - 2000 + a1->Data1->Scale.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&HC_FLAG);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCFlag_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.z > 4000) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z == 0) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) a1->Data1->Scale.z += 10;
		if (a1->Data1->Scale.y == 1) a1->Data1->Scale.z -= 10;

		HCFlag_Display(a1);
	}
}

void HCFlag(ObjectMaster *a1)
{
	if (a1->Data1->Position.x > 8000) {
		a1->Data1->Rotation.y += 0x8000;
		a1->Data1->Rotation.z += 0x8000;
	}

	a1->MainSub = &HCFlag_Main;
	a1->DisplaySub = &HCFlag_Display;
}

void HCBlade_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njRotateZ(0, a1->Data1->Scale.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(HC_HCBLADE->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCBlade_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.z > 20000) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < 0) a1->Data1->Scale.y = 0;

		float temp = abs(a1->Data1->Scale.z - 10000);
		temp = 10000 - temp;
		if (temp < 3000) temp = 3000;
		a1->Data1->Scale.x = temp / 70;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += a1->Data1->Scale.x;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= a1->Data1->Scale.x;
		}

		HCBlade_Display(a1);
	}
}

void HCBlade(ObjectMaster *a1)
{
	if (a1->Data1->Position.x > 8000) {
		a1->Data1->Rotation.x += 0x8000;
		a1->Data1->Rotation.y += 0x8000;
	}

	a1->Data1->Rotation.z -= 10000;

	a1->MainSub = &HCBlade_Main;
	a1->DisplaySub = &HCBlade_Display;
}

void HCTree_Display(ObjectMaster *a1) {
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);

	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njTranslate(0, 0, 40, 0);

		float offset = a1->Data1->Scale.z;

		njRotateZ(0, offset);
		njDrawModel_SADX(HC_SPKTREE->getmodel()->child->child->basicdxmodel);
		njRotateZ(0, -offset);
		njRotateX(0, offset);
		njDrawModel_SADX(HC_SPKTREE->getmodel()->child->basicdxmodel);
		njRotateX(0, -offset);

		njTranslate(0, 0, 15, 0);
		njRotateX(0, (offset / 0.9));
		njDrawModel_SADX(HC_SPKTREE->getmodel()->child->child->child->basicdxmodel);
		njRotateX(0, -(offset / 0.9));
		njRotateZ(0, (offset / 0.9));
		njDrawModel_SADX(HC_SPKTREE->getmodel()->child->child->child->child->basicdxmodel);
		njRotateZ(0, -(offset / 0.9));
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCTree_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 30, 0);

		if (a1->Data1->Scale.z > 900) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -900) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 90;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 90;
		}

		HCTree_Display(a1);
	}
}

void HCTree(ObjectMaster *a1)
{
	a1->Data1->Object = HC_SPKTREE->getmodel();

	a1->MainSub = &HCTree_Main;
	a1->DisplaySub = &HCTree_Display;
	a1->DeleteSub = DynCol_Delete;
}

void HCPlantA_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;

		float offset = a1->Data1->Scale.z;

		njRotateZ(0, offset);
		njDrawModel_SADX(&HC_PLANTA1);
		njRotateZ(0, -offset);

		njRotateZ(0, -offset);
		njDrawModel_SADX(&HC_PLANTA2);
		njRotateZ(0, offset);

		njRotateZ(0, (offset / 0.9));
		njDrawModel_SADX(&HC_PLANTA3);
		njRotateZ(0, -(offset / 0.9));

		njRotateX(0, (offset / 0.9));
		njDrawModel_SADX(&HC_PLANTA4);
		njRotateX(0, -(offset / 0.9));

		njRotateX(0, -(offset / 0.9));
		njDrawModel_SADX(&HC_PLANTA5);
		njRotateX(0, (offset / 0.9));

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCPlantA_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.z > 500) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -500) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 50;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 50;
		}

		HCPlantA_Display(a1);
	}
}

void HCPlantA(ObjectMaster *a1)
{
	a1->MainSub = &HCPlantA_Main;
	a1->DisplaySub = &HCPlantA_Display;
}

void HCPlantB_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;

		float offset = a1->Data1->Scale.z;

		njRotateX(0, -offset);
		njDrawModel_SADX(HC_HPLANTB->getmodel()->basicdxmodel);
		njRotateX(0, offset);

		njRotateZ(0, -offset);
		njDrawModel_SADX(HC_HPLANTB->getmodel()->child->basicdxmodel);
		njRotateZ(0, offset);

		njRotateX(0, (offset / 0.9));
		njDrawModel_SADX(HC_HPLANTB->getmodel()->child->child->basicdxmodel);
		njRotateX(0, -(offset / 0.9));

		njRotateZ(0, (offset / 0.9));
		njDrawModel_SADX(HC_HPLANTB->getmodel()->child->child->child->basicdxmodel);
		njRotateZ(0, -(offset / 0.9));

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCPlantB_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.z > 400) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -400) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 40;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 40;
		}

		HCPlantB_Display(a1);
	}
}

void HCPlantB(ObjectMaster *a1)
{
	a1->MainSub = &HCPlantB_Main;
	a1->DisplaySub = &HCPlantB_Display;
}

void HCSpiders(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			a1->Data1->Action = 1;
		}

		for (int i = 0; i < LengthOfArray(HangCastle_Spiders); ++i) {
			if (CheckModelDisplay2(HangCastle_Spiders[i])) {
				SOI_LIST2 item = HangCastle_Spiders[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(HC_SPDSIGN->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(HC_SPDSIGN->getmodel()->child->basicdxmodel); break;
				case 2: njDrawModel_SADX(HC_SPDSIGN->getmodel()->child->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(HC_SPDSIGN->getmodel()->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

PVMEntry HangCastleObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "UNAGI", &UNAGI_TEXLIST },
	{ "PEN", &PEN_TEXLIST },
	{ "LION", &LION_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "GORI", &GORI_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

ObjectListEntry HangCastleObjectList_list[] = {
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, HCWarp, "HC WARP" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCDoor, "HC DOOR" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCWall, "HC WALL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCPlatform, "HC PLAT" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjCannon, "CANNON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, HCTorch, "HC TORCH" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 5800000, 0, HCFlag, "HC FLAG" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 5800000, 0, HCBlade, "HC BLADE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, HCTree, "HC TREE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, HCPlantA, "HC PLANTA" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, HCPlantB, "HC PLANTB" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, OBJCASE, "OBJ CASE" },
};
ObjectList HangCastleObjectList = { arraylengthandptrT(HangCastleObjectList_list, int) };

void HCFlags_Reset() {
	hcflagtimer = 0;
	HC_FLAG.points[1].z = 0.05f * 25;
	HC_FLAG.points[3].z = 0.05f * 25;
	HC_FLAG.points[4].z = 0.05f * 25;
	HC_FLAG.points[5].z = 0.05f * 25;
	HC_FLAG.points[0].z = 0.1f * 25;
	HC_FLAG.points[2].z = 0.1f * 25;
	HC_FLAG.points[8].z = 0.1f * 25;
	HC_FLAG.points[9].z = 0.1f * 25;
	HC_FLAG.points[6].z = 0.02f * 25;
	HC_FLAG.points[7].z = 0.02f * 25;
	HC_FLAG.points[10].z = -0.1f * 25;
	HC_FLAG.points[11].z = -0.1f * 25;
	HC_FLAG.points[14].z = -0.1f * 25;
	HC_FLAG.points[15].z = -0.1f * 25;
	HC_FLAG.points[12].z = -0.02f * 25;
	HC_FLAG.points[13].z = -0.02f * 25;
}

void HCFlags_Animate() {
	hcflagtimer++;
	if (hcflagtimer > 100) hcflagtimer = 1;
	if (hcflagtimer < 51) {
		HC_FLAG.points[1].z -= 0.05f;
		HC_FLAG.points[3].z -= 0.05f;
		HC_FLAG.points[4].z -= 0.05f;
		HC_FLAG.points[5].z -= 0.05f;
		HC_FLAG.points[0].z -= 0.1f;
		HC_FLAG.points[2].z -= 0.1f;
		HC_FLAG.points[8].z -= 0.1f;
		HC_FLAG.points[9].z -= 0.1f;
		HC_FLAG.points[6].z -= 0.02f;
		HC_FLAG.points[7].z -= 0.02f;
		HC_FLAG.points[10].z += 0.1f;
		HC_FLAG.points[11].z += 0.1f;
		HC_FLAG.points[14].z += 0.1f;
		HC_FLAG.points[15].z += 0.1f;
		HC_FLAG.points[12].z += 0.02f;
		HC_FLAG.points[13].z += 0.02f;
	}
	else {
		HC_FLAG.points[1].z += 0.05f;
		HC_FLAG.points[3].z += 0.05f;
		HC_FLAG.points[4].z += 0.05f;
		HC_FLAG.points[5].z += 0.05f;
		HC_FLAG.points[0].z += 0.1f;
		HC_FLAG.points[2].z += 0.1f;
		HC_FLAG.points[8].z += 0.1f;
		HC_FLAG.points[9].z += 0.1f;
		HC_FLAG.points[6].z += 0.02f;
		HC_FLAG.points[7].z += 0.02f;
		HC_FLAG.points[10].z -= 0.1f;
		HC_FLAG.points[11].z -= 0.1f;
		HC_FLAG.points[14].z -= 0.1f;
		HC_FLAG.points[15].z -= 0.1f;
		HC_FLAG.points[12].z -= 0.02f;
		HC_FLAG.points[13].z -= 0.02f;
	}
}

void HangCastleObjects_Init(const char *path) {
	TexLists_Obj[HeroesLevelID_HangCastle] = HangCastleObjectTextures;
	ObjLists[HeroesLevelID_HangCastle * 8] = &HangCastleObjectList;
}