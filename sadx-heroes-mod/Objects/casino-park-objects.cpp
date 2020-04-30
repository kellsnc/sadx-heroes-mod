#include "stdafx.h"

void _cdecl HeroesFlippers(ObjectMaster *a1);
void _cdecl CPDice(ObjectMaster *a1);
void _cdecl CPBobInAir(ObjectMaster *a1);
void _cdecl CPSlotS(ObjectMaster *a1);
void _cdecl CPSlotL(ObjectMaster *a1);
void _cdecl CPDoor(ObjectMaster *a1);
void CPRoulette(ObjectMaster *a1);

ModelInfo * CP_BIGDICE;
ModelInfo * CP_DIRSIGN;

extern ModelInfo * CP_SLOTMCS;

extern uint8_t SlotState;
extern NJS_MODEL_SADX CP_SLOT;
extern NJS_MODEL_SADX CP_SLOT2;

void CPARROWS(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = CP_DIRSIGN->getmodel()->child;
	else a1->Data1->Object = CP_DIRSIGN->getmodel();
	DynCol_Add(a1, 0);

	a1->MainSub = mainSub_Global;
	a1->DisplaySub = displaySub_Global;
	a1->DeleteSub = DynCol_Delete;
}

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

	EntityData1 *entity = EntityData1Ptrs[0];
	if (entity->Position.z > 1050) return;

	if (type == 0 || type == 2) DrawObjModel(a1, CP_SLOTMCS->getmodel()->child->child->basicdxmodel, false);
	else DrawObjModel(a1, CP_SLOTMCS->getmodel()->child->child->child->basicdxmodel, false);
}

void CPSLOT_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
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
}

void CPSLOT(ObjectMaster *a1)
{
	uint8_t type = (uint8_t)a1->Data1->Scale.y;
	if (type == 2 || type == 3) a1->Data1->Rotation.y = 0x8000;

	a1->MainSub = &CPSLOT_Main;
	a1->DisplaySub = &CPSLOT_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void CPGiantDice_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.y != 0) if (CurrentChunk != a1->Data1->Scale.y) return;
	
	if (!MissedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);

		if (a1->Data1->Scale.x == 1)
			njRotateY(nullptr, a1->Data1->Rotation.y);
		else
			njRotateY(nullptr, -a1->Data1->Rotation.y);

		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);

		if (a1->Data1->Scale.x == 0) njRotateY(nullptr, 0x3000);

		njDrawModel_SADX(CP_BIGDICE->getmodel()->child->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void CPGiantDice_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		a1->Data1->Rotation.y += 20;
		a1->DisplaySub(a1);
	}
}

void CPGiantDice(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = CP_BIGDICE->getmodel()->child;
	else a1->Data1->Object = CP_BIGDICE->getmodel();

	a1->MainSub = &CPGiantDice_Main;
	a1->DisplaySub = &CPGiantDice_Display;
	a1->DeleteSub = &DynCol_Delete;
}

PVMEntry CasinoParkObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "UNAGI", &UNAGI_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "WARA", &WARA_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

ObjectListEntry CasinoParkObjectList_list[] = {
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjReel, "Reel" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, SpringH_Load, "O SPRING H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Startshot, "StartShot" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, HeroesFlippers, "FlipperL" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Flipperlx, "FlipperLx" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, HeroesFlippers, "FlipperR" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Flipperrx, "FlipperRx" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, Bumper1, "Bumper1" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Bumper2, "Bumper2" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Slingl, "SlingL" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Slingr, "SlingR" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Stoperc, "StoperC" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Stoperl, "StoperL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Stoperr, "StoperR" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Roll, "Roll" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Jdail, "JdaiL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Jdair, "JdaiR" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Dtarget, "Dtarget" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, Dtarget2, "Dtarget2" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Rollupr, "RollUpR" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Rollupl, "RollUpL" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_Default, 0, 0, Rolldownl, "RollDownL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPARROWS, "CPARROWS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2360000, 0, CPDice, "CPDice" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 14015000, 0, CPSLOT, "CPSLOT" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPBobInAir, "CPBobInAir" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPSlotS, "CPSlotS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPSlotL, "CPSlotL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPDoor, "CPDoor" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 8400000, 0, CPGiantDice, "CPGDICE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPRoulette, "CPRoulette" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjCannon, "CANNON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO WOODBOX" },
};
ObjectList CasinoParkObjectList = { arraylengthandptrT(CasinoParkObjectList_list, int) };

void CasinoParkObjects_Init() {
	TexLists_Obj[HeroesLevelID_CasinoPark] = CasinoParkObjectTextures;
	ObjLists[HeroesLevelID_CasinoPark * 8] = &CasinoParkObjectList;
}