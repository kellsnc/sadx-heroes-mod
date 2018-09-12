#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "hang-castle-objects.h"

void _cdecl HCWARP(ObjectMaster *a1);
void __cdecl HCDOOR(ObjectMaster *a1);
void __cdecl HCWALL(ObjectMaster *a1);
void __cdecl HCPLATFORM(ObjectMaster *a1);

extern SH_ANIMTEXS HangCastleAnimTexs[5];

#pragma region Torch/fire
void HCTORCH_Display(ObjectMaster *a1) {
	if (a1->Data1->Action != 0 && a1->Data1->Action != 3 && a1->Data1->Action != 4) DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);

	if (a1->Data1->Action < 5) {
		if (a1->Data1->Action == 0 || a1->Data1->Action == 3) {
			if (!IsPlayerInsideSphere(&a1->Data1->Position, 300)) return;
		}

		//replace will billboard sprite somehow
		if (!MissedFrames) {
			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Position);
			njRotateXYZ(nullptr, a1->Data1->Rotation.x, Camera_Data1->Rotation.y, a1->Data1->Rotation.z);
			njTranslate(0, 0, 33, 0);
			njScale(nullptr, a1->Data1->Scale.z, a1->Data1->Scale.z, a1->Data1->Scale.z);
			DrawQueueDepthBias = -9000.0f;
			if (a1->Data1->Action == 2 || a1->Data1->Action == 3) njDrawModel_SADX(&HC_FLAME_BLUE);
			else njDrawModel_SADX(&HC_FLAME);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void HCTORCH_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (a1->Data1->Action == 0 || a1->Data1->Action == 3) {
			if (IsPlayerInsideSphere(&a1->Data1->Position, 150)) {
				if (a1->Data1->Scale.z < 1) a1->Data1->Scale.z += 0.1f;
			}
			else {
				if (a1->Data1->Scale.z > 0) a1->Data1->Scale.z -= 0.05f;
			}
		}

		HCTORCH_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCTORCH(ObjectMaster *a1)
{
	a1->Data1->Action = a1->Data1->Scale.x;
	if (a1->Data1->Action == 0) a1->Data1->Position.y -= 20;
	else if (a1->Data1->Action == 3) a1->Data1->Position.y -= 30;
	else if (a1->Data1->Action == 4) {
		a1->Data1->Scale.z = 6;
		if (a1->Data1->Rotation.x != 0xFFFF8000) a1->Data1->Position.y -= 5;
		else a1->Data1->Position.y += 10;
	}
	else {
		a1->Data1->Object = &HC_TORCH;
		AddToCollision(a1, 0);
		a1->Data1->Scale.z = 2;
	}

	a1->MainSub = &HCTORCH_Main;
	a1->DisplaySub = &HCTORCH_Display;
	a1->DeleteSub = &deleteSub_Global;
}

#pragma endregion

#pragma region Flags
void HCFLAG_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.x == 1) DrawObjModel(a1, &HC_FLAGPOLEB, false);
	else DrawObjModel(a1, &HC_FLAGPOLEA, false);

	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		njRotateX(0, -a1->Data1->Rotation.x - 2000 + a1->Data1->Scale.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&HC_FLAG);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCFLAG_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 3500.0f)) {
		if (a1->Data1->Scale.z > 4000) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z == 0) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) a1->Data1->Scale.z += 10;
		if (a1->Data1->Scale.y == 1) a1->Data1->Scale.z -= 10;

		HCFLAG_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCFLAG(ObjectMaster *a1)
{
	if (a1->Data1->Position.x > 8000) {
		a1->Data1->Rotation.y += 0x8000;
		a1->Data1->Rotation.z += 0x8000;
	}

	a1->MainSub = &HCFLAG_Main;
	a1->DisplaySub = &HCFLAG_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Blade
void HCBLADE_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njRotateZ(0, a1->Data1->Scale.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&HC_BLADE);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCBLADE_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 3500.0f)) {

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

		HCBLADE_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCBLADE(ObjectMaster *a1)
{
	if (a1->Data1->Position.x > 8000) {
		a1->Data1->Rotation.x += 0x8000;
		a1->Data1->Rotation.y += 0x8000;
	}

	a1->Data1->Rotation.z -= 10000;

	a1->MainSub = &HCBLADE_Main;
	a1->DisplaySub = &HCBLADE_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Spooky tree
void HCTREE_Display(ObjectMaster *a1) {
	DrawObjModel(a1, HC_TREE.basicdxmodel, false);

	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njTranslate(0, 0, 40, 0);

		float offset = a1->Data1->Scale.z;

		njRotateZ(0, offset);
		njDrawModel_SADX(&HC_LEAVES2);
		njRotateZ(0, -offset);
		njRotateX(0, offset);
		njDrawModel_SADX(&HC_LEAVES1);
		njRotateX(0, -offset);

		njTranslate(0, 0, 15, 0);
		njRotateX(0, (offset / 0.9));
		njDrawModel_SADX(&HC_LEAVES3);
		njRotateX(0, -(offset / 0.9));
		njRotateZ(0, (offset / 0.9));
		njDrawModel_SADX(&HC_LEAVES4);
		njRotateZ(0, -(offset / 0.9));
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCTREE_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (a1->Data1->Scale.z > 900) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -900) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 90;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 90;
		}

		HCTREE_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCTREE(ObjectMaster *a1)
{
	a1->Data1->Object = &HC_TREE;
	AddToCollision(a1, 0);

	a1->MainSub = &HCTREE_Main;
	a1->DisplaySub = &HCTREE_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Spooky plant A
void HCPLANTA_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
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

void HCPLANTA_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (a1->Data1->Scale.z > 500) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -500) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 50;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 50;
		}

		HCPLANTA_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCPLANTA(ObjectMaster *a1)
{
	a1->MainSub = &HCPLANTA_Main;
	a1->DisplaySub = &HCPLANTA_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Spooky plant A
void HCPLANTB_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;

		float offset = a1->Data1->Scale.z;

		njRotateX(0, -offset);
		njDrawModel_SADX(&HC_PLANTB1);
		njRotateX(0, offset);

		njRotateZ(0, -offset);
		njDrawModel_SADX(&HC_PLANTB2);
		njRotateZ(0, offset);

		njRotateX(0, (offset / 0.9));
		njDrawModel_SADX(&HC_PLANTB3);
		njRotateX(0, -(offset / 0.9));

		njRotateZ(0, (offset / 0.9));
		njDrawModel_SADX(&HC_PLANTB4);
		njRotateZ(0, -(offset / 0.9));

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCPLANTB_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (a1->Data1->Scale.z > 400) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z < -400) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) {
			a1->Data1->Scale.z += 40;
		}
		if (a1->Data1->Scale.y == 1) {
			a1->Data1->Scale.z -= 40;
		}

		HCPLANTB_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl HCPLANTB(ObjectMaster *a1)
{
	a1->MainSub = &HCPLANTB_Main;
	a1->DisplaySub = &HCPLANTB_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

ObjectListEntry HangCastleObjectList_list[] = {
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x450370, "RING   " } /* "RING   " */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A4C30, "SPRING " } /* "SPRING " */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A4E50, "SPRINGB" } /* "SPRINGB" */,
{ 3, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A4450, "O AXPNL" } /* "O AXPNL" */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A4260, "O IRONB" } /* "O IRONB" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A3E50, "O FeBJG" } /* "O FeBJG" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A30E0, "O TOGE" } /* "O TOGE" */,
{ 3, 3, 1, 360000, 0, (ObjectFuncPtr)0x4A3420, "O EME P" } /* "O EME P" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x46B170, "O RELEASE" } /* "O RELEASE" */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4CBA80, "O SWITCH" } /* "O SWITCH" */,
{ 10, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A2B60, "CMN KUSA" } /* "CMN KUSA" */,
{ 14, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A26F0, "CMN_DRING" } /* "CMN_DRING" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A21C0, "O BALOON" } /* "O BALOON" */,
{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4D6F10, "O ITEMBOX" } /* "O ITEMBOX" */,
{ 14, 2, 1, 360000, 0, (ObjectFuncPtr)0x4CA530, "Rocket H" } /* "Rocket H" */,
{ 14, 2, 1, 360000, 0, (ObjectFuncPtr)0x4CA660, "Rocket HS" } /* "Rocket HS" */,
{ 14, 2, 1, 360000, 0, (ObjectFuncPtr)0x4CA7D0, "Rocket V" } /* "Rocket V" */,
{ 14, 2, 1, 360000, 0, (ObjectFuncPtr)0x4CA900, "Rocket VS" } /* "Rocket VS" */,
{ 2, 2, 1, 4000000, 0, (ObjectFuncPtr)0x4B8DC0, "O JPanel" } /* "O JPanel" */,
{ 15, 6, 1, 360000, 0, (ObjectFuncPtr)0x44F540, "O Save Point" } /* "O Save Point" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4D4850, "WALL   " } /* "WALL   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x79B2F0, "O_GRING" } /* "O_GRING" */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D4700, "C SPHERE" } /* "C SPHERE" */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D4770, "C CYLINDER" } /* "C CYLINDER" */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D47E0, "C CUBE" } /* "C CUBE" */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D4B70, "OTTOTTO" } /* "OTTOTTO" */,
{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A1AA0, "O TIKAL" } /* "O TIKAL" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4AD140, "E SARU  " } /* "E SARU  " */,
{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A1380, "E SAITO" } /* "E SAITO" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7AA960, "E AMEMB " } /* "E AMEMB " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x5001E0, "O JUMP   " } /* "O JUMP   " */,
{ 15, 3, 1, 160000, 0, (ObjectFuncPtr)0x4C07D0, "O ItemBoxAir" } /* "O ItemBoxAir" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0DF0, "SPINA A" } /* "SPINA A" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0F40, "SPINA B" } /* "SPINA B" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B1090, "SPINA C" } /* "SPINA C" */,
{ 2, 2, 0, 0, 0, (ObjectFuncPtr)0x7A4E50, "O SPRING B" } /* "O SPRING B" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x79F860, "O SPRING H" } /* "O SPRING H" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF190, "E UNI A" } /* "E UNI A" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF500, "E UNI B" } /* "E UNI B" */,
{ 2, 3, 5, 250000, 0, (ObjectFuncPtr)0x4AF860, "E UNI C" } /* "E UNI C" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4B3210, "E POLICE" } /* "E POLICE" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&HCWARP, "HC WARP" } /* "HC Warp" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCDOOR, "HC DOOR" } /* "HC Door" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCWALL, "HC WALL" } /* "HC Wall" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCPLATFORM, "HC PLAT" } /* "HC Moving Platform" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&ObjCannon, "CANNON" } /* "SH Cannon" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&HCTORCH, "HC TORCH" } /* "HC Torch or fire" */,
{ 2, 3, 1, 5800000, 0, (ObjectFuncPtr)&HCFLAG, "HC FLAG" } /* "HC Flag with pole" */,
{ 2, 3, 1, 5800000, 0, (ObjectFuncPtr)&HCBLADE, "HC BLADE" } /* "HC Blade thing that looks bad" */,
{ 2, 3, 1, 1560000, 0, (ObjectFuncPtr)&HCTREE, "HC TREE" } /* "Spooky tree" */,
{ 2, 3, 1, 1560000, 0, (ObjectFuncPtr)&HCPLANTA, "HC PLANTA" } /* "Spooky plant A" */,
{ 2, 3, 1, 1560000, 0, (ObjectFuncPtr)&HCPLANTB, "HC PLANTB" } /* "Spooky plant B" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)&SHCameraSwitch, "SH CAM SW" } /* "Camera flag switcher" */,
{ 2, 3, 1, 1560000, 0, (ObjectFuncPtr)&OBJCASE, "OBJ CASE" } /* "Heroes switch case" */,
};
ObjectList HangCastleObjectList = { arraylengthandptrT(HangCastleObjectList_list, int) };

static int hcflagtimer = 0;

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
	WriteData((PVMEntry**)0x90EB84, CommonObjectTextures);
	WriteData((ObjectList**)0x974BD8, &HangCastleObjectList); //974BDC 974BE0
}

void HangCastleObjects_OnFrame(EntityData1 * entity) {
	AnimateObjectsTextures(HCOBJLIST, LengthOfArray(HCOBJLIST), HangCastleAnimTexs, LengthOfArray(HangCastleAnimTexs));;

	HCFlags_Animate();

	if (CurrentChunk != 11) {
		CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
		CurrentLandTable->Col[0].Model->pos[1] = entity->Position.y;
		CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z + 8500;
	}
}