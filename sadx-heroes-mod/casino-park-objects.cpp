#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "casino-park-objects.h"

extern NJS_MODEL_SADX * CASINOOBJLIST[9];
extern SH_ANIMTEXS CasinoParkAnimTexs[39];

void _cdecl FLIPPERS(ObjectMaster *a1);
void _cdecl CPDICE(ObjectMaster *a1);
void _cdecl CPBOBINAIR(ObjectMaster *a1);
void _cdecl CPSLOTS(ObjectMaster *a1);
void _cdecl CPSLOTL(ObjectMaster *a1);
void _cdecl CPDOOR(ObjectMaster *a1);

#pragma region Arrows
void CPARROWS_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.x == 1) DrawObjModel(a1, CP_ArrowRight.basicdxmodel, false);
	else DrawObjModel(a1, CP_ArrowLeft.basicdxmodel, false);
}

void CPARROWS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		CPARROWS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPARROWS(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = &CP_ArrowRight;
	else a1->Data1->Object = &CP_ArrowLeft;
	AddToCollision(a1, 0);

	a1->MainSub = &CPARROWS_Main;
	a1->DisplaySub = &CPARROWS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Giant Slots
extern uint8_t SlotState;
extern NJS_MODEL_SADX CP_SLOT;
extern NJS_MODEL_SADX CP_SLOT2;

void CPSLOT_Display(ObjectMaster *a1) {
	uint8_t type = (uint8_t)a1->Data1->Scale.y;

	if (a1->Data1->Scale.z != 0) {

		if (a1->Data1->Scale.z < 20) {
			if (CurrentChunk != a1->Data1->Scale.z) {
				return;
			}
		}
		else {
			if (IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.z) == false) {
				return;
			}
		}
	}

	auto entity = EntityData1Ptrs[0];
	if (entity->Position.z > 1050) return;

	if (type == 0 || type == 2) DrawObjModel(a1, &CP_SLOT, false);
	else DrawObjModel(a1, &CP_SLOT2, false);
}

void CPSLOT_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 4400.0f)) {
		uint8_t type = (uint8_t)a1->Data1->Scale.y;
		uint8_t state = (uint8_t)a1->Data1->Scale.x;

		if (type == 0) {
			if (state == 0) if (SlotState < 2) a1->Data1->Rotation.z += 600;
			if (state == 1) if (SlotState < 3) a1->Data1->Rotation.z += 600;
			if (state == 2) if (SlotState < 4) a1->Data1->Rotation.z += 600;
		}
		else if (type == 2) {
			if (state == 0) if (SlotState < 2) a1->Data1->Rotation.z -= 600;
			if (state == 1) if (SlotState < 3) a1->Data1->Rotation.z -= 600;
			if (state == 2) if (SlotState < 4) a1->Data1->Rotation.z -= 600;
		}
		else {
			if (state == 0) if (SlotState < 2) a1->Data1->Rotation.x += 600;
			if (state == 1) if (SlotState < 3) a1->Data1->Rotation.x += 600;
			if (state == 2) if (SlotState < 4) a1->Data1->Rotation.x += 600;
		}

		CPSLOT_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPSLOT(ObjectMaster *a1)
{
	uint8_t type = (uint8_t)a1->Data1->Scale.y;
	if (type == 2 || type == 3) a1->Data1->Rotation.y = 0x8000;
	a1->MainSub = &CPSLOT_Main;
	a1->DisplaySub = &CPSLOT_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Giant Dice
void CPGiantDice_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.y != 0) if (CurrentChunk != a1->Data1->Scale.y) return;
	if (a1->Data1->Scale.x == 1) DrawObjModel(a1, &CP_GIANTDICE2, false);
	else DrawObjModel(a1, &CP_GIANTDICE1, false);

	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		if (a1->Data1->Scale.x == 1) {
			njRotateY(nullptr, a1->Data1->Rotation.y);
		}
		else {
			njRotateY(nullptr, a1->Data1->Rotation.y + 0x3000);
		}

		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&CP_DICEREF);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPGiantDice_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 4400.0f)) {
		if (a1->Data1->Scale.x == 1) a1->Data1->Rotation.y += 20;
		else a1->Data1->Rotation.y -= 20;
		CPGiantDice_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPGiantDice(ObjectMaster *a1)
{
	a1->MainSub = &CPGiantDice_Main;
	a1->DisplaySub = &CPGiantDice_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Roulette
void CPRoulette_Display(ObjectMaster *a1) {
	DrawObjModel(a1, &CP_ROURETTE, false);
}

void CPRoulette_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (CurrentChunk == 5) a1->Data1->Rotation.y += 60;
		CPRoulette_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl CPRoulette(ObjectMaster *a1)
{
	a1->MainSub = &CPRoulette_Main;
	a1->DisplaySub = &CPRoulette_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

ObjectListEntry CasinoParkObjectList_list[] = {
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
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DCCE0, "StartShot" } /* "StartShot" */,
{ 6, 3, 1, 1360000, 0, &FLIPPERS, "FlipperL" } /* "FlipperL" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5DCBE0, "FlipperLx" } /* "FlipperLx" */,
{ 6, 3, 1, 1360000, 0, &FLIPPERS, "FlipperR" } /* "FlipperR" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5DCC00, "FlipperRx" } /* "FlipperRx" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)0x5DC000, "Bumper1" } /* "Bumper1" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DC040, "Bumper2" } /* "Bumper2" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5DBBF0, "SlingL" } /* "SlingL" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5DBC60, "SlingR" } /* "SlingR" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DB6A0, "StoperC" } /* "StoperC" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DB700, "StoperL" } /* "StoperL" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DB790, "StoperR" } /* "StoperR" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DB150, "Roll" } /* "Roll" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DA8A0, "JdaiL" } /* "JdaiL" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DA8B0, "JdaiR" } /* "JdaiR" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DA1C0, "Dtarget" } /* "Dtarget" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x5DA1E0, "Dtarget2" } /* "Dtarget2" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5D98A0, "RollUpR" } /* "RollUpR" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5D98F0, "RollUpL" } /* "RollUpL" */,
{ 6, 3, 0, 0, 0, (ObjectFuncPtr)0x5D9940, "RollDownL" } /* "RollDownL" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPARROWS, "CPARROWS" } /* "Giant Arrows" */,
{ 2, 3, 1, 1660000, 0, (ObjectFuncPtr)&CPDICE, "CPDICE" } /* "Moving Dice" */,
{ 2, 3, 1, 14360000, 0, (ObjectFuncPtr)&CPSLOT, "CPSLOT" } /* "Giant Slots" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPBOBINAIR, "CPBOBINAIR" } /* "Round bumpers" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPSLOTS, "CPSLOTS" } /* "Small slot machine" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPSLOTL, "CPSLOTL" } /* "Large slot machine" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPDOOR, "CPDOOR" } /* "Sliding X door" */,
{ 2, 3, 1, 14360000, 0, (ObjectFuncPtr)&CPGiantDice, "CPGDICE" } /* "Giant Dices" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPRoulette, "CPRoulette" } /* "Turning roulette" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&ObjCannon, "CANNON" } /* "SH Cannon" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)&SHCameraSwitch, "SH CAM SW" } /* "Camera flag switcher" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
};
ObjectList CasinoParkObjectList = { arraylengthandptrT(CasinoParkObjectList_list, int) };

void CasinoParkObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB74, CommonObjectTextures);
	WriteData((ObjectList**)0x974B58, &CasinoParkObjectList); //974B5C 974B60
}

void CasinoParkObjects_OnFrame(EntityData1 * entity) {
	AnimateObjectsTextures(CPOBJLIST, LengthOfArray(CPOBJLIST), CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));
	AnimateObjectsTextures(CASINOOBJLIST, LengthOfArray(CASINOOBJLIST), CasinoParkAnimTexs, LengthOfArray(CasinoParkAnimTexs));

	if (anim % 4 == 0) {
		CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
		CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z;
	}
}