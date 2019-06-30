#include "stdafx.h"
#include "mod.h"
#include "objects.h"

void _cdecl HeroesFlippers(ObjectMaster *a1);
void _cdecl CPDice(ObjectMaster *a1);
void _cdecl CPBobInAir(ObjectMaster *a1);
void _cdecl CPSlotS(ObjectMaster *a1);
void _cdecl CPSlotL(ObjectMaster *a1);
void _cdecl CPDoor(ObjectMaster *a1);
void CPRoulette(ObjectMaster *a1);

ModelInfo * BH_BIGCHIP;
ModelInfo * BH_BNGCHIP;
ModelInfo * BH_BNGCARD;
ModelInfo * BH_TBLSIGN;
NJS_MODEL_SADX * BINGONB[9];

static char BingoState[9];
static char BingoLines[8];
ObjectMaster * BingoHandlerPointer;

SOI_LIST2 Bingo_Signs[] = {
	{ 0,{ 8760.0611f, -3421.914f, -12177.89f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 0,{ 8600.0611f, -3421.914f, -12177.89f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 0,{ 8600.0537f, -3713.744f, -11886.06f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 0,{ 8760.0537f, -3713.744f, -11886.06f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 0,{ 21018.04f, -22066.37f, -23062.92f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 0,{ 21783.1f, -21077.97f, -22256.06f },{ 0, 16384, 10926 },{ 1, 1, 1 }, -6000.0f, 0, 1200.0f },
	{ 0,{ 21018.06f, -22350.63f, -23581.51f },{ -1366, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 0,{ 8510.0096f, -3550.143f, -13413.74f },{ 4096, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 0,{ 20818.27f, -16831.88f, -21620.41f },{ 512, 8960, 3072 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 0,{ 8500.0096f, -3520.143f, -13730.74f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1200.0f },
	{ 1,{ 8679.9996f, -909.469f, -6255.414f },{ -8192, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 13560, -7104.891f, -14880 },{ 0, 49152, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 14450.036f, -8498.55f, -16113.55f },{ -4096, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1600.0f },
	{ 1,{ 14444.189f, -7849.194f, -15373.72f },{ 0, -768, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 20600.01f, -18073.42f, -20805.03f },{ 0, -16384, -8192 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 20600.01f, -18073.42f, -20925.03f },{ 0, -16384, -8192 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 20362.45f, -16126.44f, -20794.18f },{ 0, -16384, -4096 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 20362.45f, -16126.44f, -20854.18f },{ 0, -16384, -4096 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 20027.23f, -17682.09f, -21072.92f },{ 2048, -26112, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 21358.12f, -18818.02f, -20865.01f },{ 0, -16384, -4096 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 20970.98f, -16611.92f, -21253.52f },{ -4352, -1536, 0 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 20253.53f, -17264.95f, -21761.31f },{ 2304, 21248, 0 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 20023.71f, -17625.09f, -21179.22f },{ 2048, -26112, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 18350, -14627.04f, -20824.03f },{ 0, 49152, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 18550, -14747.04f, -20824.03f },{ 0, 49152, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 18800, -14887.04f, -20824.03f },{ 0, 49152, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
	{ 1,{ 20857.82f, -16831.88f, -21669.85f },{ 512, 8960, 3072 },{ 1, 1, 1 }, -6000.0f, 0, 3000.0f },
	{ 1,{ 8410.005f, -4110.629f, -12906.28f },{ 0, 0, 0 },{ 1, 1, 1 }, -6000.0f, 0, 1000.0f },
	{ 1,{ 19090, -15025.04f, -20824.03f },{ 0, 49152, 0 },{ 1, 1, 1 }, -6000.0f, 0, 2000.0f },
};

void BHBingoCard_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(0, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njTranslate(0, 0, 40, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(BH_BNGCARD->getmodel()->basicdxmodel);

		njTranslate(0, 0, 0, -10);

		njTranslate(0, 45, 45, 0);
		njDrawModel_SADX(BINGONB[0]);
		njTranslate(0, -45, -45, 0);

		njTranslate(0, 45, 0, 0);
		njDrawModel_SADX(BINGONB[1]);
		njTranslate(0, -45, 0, 0);

		njDrawModel_SADX(BINGONB[2]);

		njTranslate(0, 0, 45, 0);
		njDrawModel_SADX(BINGONB[3]);
		njTranslate(0, 0, -45, 0);

		njTranslate(0, 45, -45, 0);
		njDrawModel_SADX(BINGONB[4]);
		njTranslate(0, -45, 45, 0);

		njTranslate(0, -45, 45, 0);
		njDrawModel_SADX(BINGONB[5]);
		njTranslate(0, 45, -45, 0);

		njTranslate(0, -45, 0, 0);
		njDrawModel_SADX(BINGONB[6]);
		njTranslate(0, 45, 0, 0);

		njTranslate(0, 0, -45, 0);
		njDrawModel_SADX(BINGONB[7]);
		njTranslate(0, 0, 45, 0);

		njTranslate(0, -45, -45, 0);
		njDrawModel_SADX(BINGONB[8]);
		njTranslate(0, 45, 45, 0);

		njTranslate(0, 0, 0, 10);
		njRotateZ(0, 0x4000);
		if (BingoLines[0]) {
			njTranslate(0, 45, 0, 0);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njTranslate(0, -45, 0, 0);
		}
		if (BingoLines[1]) njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
		if (BingoLines[2]) {
			njTranslate(0, -45, 0, 0);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njTranslate(0, 45, 0, 0);
		}
		njRotateZ(0, -0x4000);

		if (BingoLines[3]) {
			njTranslate(0, -45, 0, 0);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njTranslate(0, 45, 0, 0);
		}
		if (BingoLines[4]) njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
		if (BingoLines[5]) {
			njTranslate(0, 45, 0, 0);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njTranslate(0, -45, 0, 0);
		}

		if (BingoLines[6]) {
			njRotateZ(0, 0x2000);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njRotateZ(0, -0x2000);
		}
		if (BingoLines[7]) {
			njRotateZ(0, -0x2000);
			njDrawModel_SADX(BH_BNGCARD->getmodel()->child->basicdxmodel);
			njRotateZ(0, 0x2000);
		}

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void BHBingoCard_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (BingoState[0]) BINGONB[0]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[1]) BINGONB[1]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[2]) BINGONB[2]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[3]) BINGONB[3]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[4]) BINGONB[4]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[5]) BINGONB[5]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[6]) BINGONB[6]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[7]) BINGONB[7]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[8]) BINGONB[8]->mats[0].diffuse.color = 0xFF5B5B5B;

		a1->DisplaySub(a1);
	}
}

void BHBingoCard(ObjectMaster *a1)
{
	a1->MainSub = &BHBingoCard_Main;
	a1->DisplaySub = &BHBingoCard_Display;
}

void BHBingoHandler_Main(ObjectMaster *a1)
{
	uint8_t j = 0;
	if (!BingoLines[0] && BingoState[5] && BingoState[3] && BingoState[0]) {
		BingoLines[0] = 1;
		j += 1;
	}
	if (!BingoLines[1] && BingoState[6] && BingoState[2] && BingoState[1]) {
		BingoLines[1] = 1;
		j += 1;
	}
	if (!BingoLines[2] && BingoState[8] && BingoState[7] && BingoState[4]) {
		BingoLines[2] = 1;
		j += 1;
	}

	if (!BingoLines[3] && BingoState[5] && BingoState[6] && BingoState[8]) {
		BingoLines[3] = 1;
		j += 1;
	}
	if (!BingoLines[4] && BingoState[3] && BingoState[2] && BingoState[7]) {
		BingoLines[4] = 1;
		j += 1;
	}
	if (!BingoLines[5] && BingoState[0] && BingoState[1] && BingoState[4]) {
		BingoLines[5] = 1;
		j += 1;
	}

	if (!BingoLines[6] && BingoState[5] && BingoState[2] && BingoState[4]) {
		BingoLines[6] = 1;
		j += 1;
	}
	if (!BingoLines[7] && BingoState[8] && BingoState[2] && BingoState[0]) {
		BingoLines[7] = 1;
		j += 1;
	}

	if (j > 0) {
		//sound
		if (j < 2) AddRingsInterval_Delay5(30);
		if (j > 1) AddRingsInterval_Delay5(30);
		if (j > 2) AddRingsInterval_Delay5(30);
		if (j > 3) AddRingsInterval_Delay5(30);

		if (BingoLines[0] && BingoLines[1] && BingoLines[2] && BingoLines[3] && BingoLines[4] && BingoLines[5] && BingoLines[6] && BingoLines[7]) AddRingsInterval_Delay5(100);
	}
}

void BHBingoHandler(ObjectMaster *a1)
{
	if (BingoHandlerPointer) {
		deleteSub_Global(BingoHandlerPointer);
		BingoHandlerPointer = nullptr;
	}

	BingoHandlerPointer = a1;

	for (uint8_t i = 0; i < 9; ++i) {
		BingoState[i] = 0;
		if (i != 8) BingoLines[i] = 0;
	}

	BINGONB[0]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[1]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[2]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[3]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[4]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[5]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[6]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[7]->mats[0].diffuse.color = 0xFFFFFFFF;
	BINGONB[8]->mats[0].diffuse.color = 0xFFFFFFFF;

	a1->MainSub = &BHBingoHandler_Main;
}

void BHBingoNumber_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, a1->Data1->Scale.x, a1->Data1->Scale.y, 1);
		DrawQueueDepthBias = -6000.0f;

		uint8_t nb = a1->Data1->Scale.z;
		if (nb == 1) njDrawModel_SADX(BINGONB[0]);
		else if (nb == 2) njDrawModel_SADX(BINGONB[1]);
		else if (nb == 3) njDrawModel_SADX(BINGONB[2]);
		else if (nb == 4) njDrawModel_SADX(BINGONB[3]);
		else if (nb == 5) njDrawModel_SADX(BINGONB[4]);
		else if (nb == 6) njDrawModel_SADX(BINGONB[5]);
		else if (nb == 7) njDrawModel_SADX(BINGONB[6]);
		else if (nb == 8) njDrawModel_SADX(BINGONB[7]);
		else if (nb == 9) njDrawModel_SADX(BINGONB[8]);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void BHBingoNumber_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		for (uint8_t i = 1; i < 10; ++i) {
			if (i == a1->Data1->Scale.z) {
				if (!BingoState[i - 1]) {

					if (a1->Data1->Action != 2) {
						if (a1->Data1->Action == 0) {
							a1->Data1->Scale.x = 1;
							a1->Data1->Scale.y = 1;
							if (IsPlayerInsideSphere(&a1->Data1->Position, 30.0f)) {
								a1->Data1->NextAction = 30;
								a1->Data1->Action = 1;
								PlaySound(81, 0, 0, 0);
							}
						}

						if (a1->Data1->Action == 1) {
							if (a1->Data1->NextAction != 0) {
								--a1->Data1->NextAction;
								a1->Data1->Scale.x -= 0.03;
								a1->Data1->Scale.y -= 0.03;
							}
							else {
								a1->Data1->Scale.x = 0;
								a1->Data1->Scale.y = 0;
								a1->Data1->Action = 2;
								BingoState[i - 1] = 1;
							}
						}
					}
				}
				else {
					a1->Data1->Action = 2;
				}
			}
		}


		if (a1->Data1->Action != 2) BHBingoNumber_Display(a1);
	}
}

void BHBingoNumber(ObjectMaster *a1)
{
	a1->MainSub = &BHBingoNumber_Main;
	a1->DisplaySub = &BHBingoNumber_Display;
}

void BHGiantTokens_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.y != 0) if (CurrentChunk != a1->Data1->Scale.y) return;

	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateY(nullptr, a1->Data1->Rotation.y);
		if (a1->Data1->Scale.z == 2) njScale(nullptr, 0.5f, 0.5f, 0.5f);
		else if (a1->Data1->Scale.z == 3 || a1->Data1->Scale.z == 4) njScale(nullptr, 0.7f, 0.7f, 0.7f);
		DrawQueueDepthBias = -6000.0f;

		if (a1->Data1->Scale.x == 0) njDrawModel_SADX(BH_BIGCHIP->getmodel()->basicdxmodel);
		else if (a1->Data1->Scale.x == 1) njDrawModel_SADX(BH_BIGCHIP->getmodel()->child->child->child->child->basicdxmodel);
		else if (a1->Data1->Scale.x == 2) njDrawModel_SADX(BH_BIGCHIP->getmodel()->child->child->child->basicdxmodel);
		else if (a1->Data1->Scale.x == 3) njDrawModel_SADX(BH_BIGCHIP->getmodel()->child->basicdxmodel);
		else if (a1->Data1->Scale.x == 4) njDrawModel_SADX(BH_BIGCHIP->getmodel()->child->child->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void BHGiantTokens_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.x == 0) {
			if (a1->Data1->Scale.z == 0) a1->Data1->Rotation.y += 140;
			else a1->Data1->Rotation.y -= 140;
		}
		if (a1->Data1->Scale.x == 1) {
			if (a1->Data1->Scale.z == 0 || a1->Data1->Scale.z == 2) a1->Data1->Rotation.y += 130;
			else a1->Data1->Rotation.y -= 130;
		}
		if (a1->Data1->Scale.x == 2) {
			if (a1->Data1->Scale.z == 0 || a1->Data1->Scale.z == 3) a1->Data1->Rotation.y += 180;
			if (a1->Data1->Scale.z == 1 || a1->Data1->Scale.z == 4) a1->Data1->Rotation.y -= 180;
		}
		if (a1->Data1->Scale.x == 3) {
			if (a1->Data1->Scale.z == 0) a1->Data1->Rotation.y += 250;
			else a1->Data1->Rotation.y -= 250;
		}
		if (a1->Data1->Scale.x == 4) {
			if (a1->Data1->Scale.z == 0) a1->Data1->Rotation.y += 130;
			else a1->Data1->Rotation.y -= 130;
		}

		if (a1->Data1->Scale.x == 1) a1->Data1->Rotation.y += 20;
		else a1->Data1->Rotation.y -= 20;
		BHGiantTokens_Display(a1);
	}
}

void BHGiantTokens(ObjectMaster *a1)
{
	a1->MainSub = &BHGiantTokens_Main;
	a1->DisplaySub = &BHGiantTokens_Display;
}

void BHSigns(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			a1->Data1->Action = 1;
		}

		for (int i = 0; i < LengthOfArray(Bingo_Signs); ++i) {
			if (CheckModelDisplay2(Bingo_Signs[i])) {
				SOI_LIST2 item = Bingo_Signs[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(BH_TBLSIGN->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(BH_TBLSIGN->getmodel()->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

PVMEntry BingoHighwayObjectTextures[] = {
	{ "OBJ_HIGHWAY", &OBJ_HIGHWAY_TEXLIST },
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "UNAGI", &UNAGI_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "WARA", &WARA_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

ObjectListEntry BingoHighwayObjectList_list[] = {
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
	{ LoadObj_Data1, 3, 1, 1000000, 0, ObjReel, "Reel" },
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2360000, 0, CPDice, "CPDice" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPBobInAir, "CPBobInAir" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPSlotS, "CPSlotS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPSlotL, "CPSlotL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPDoor, "CPDoor" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjCannon, "CANNON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, BHBingoCard, "BING CARD" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, BHBingoHandler, "BINGHANDLE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, BHBingoNumber, "BINGO NB" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 14360000, 0, BHGiantTokens, "BHGIANTCHP" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, CPRoulette, "CPRoulette" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO WOODBOX" },
};
ObjectList BingoHighwayObjectList = { arraylengthandptrT(BingoHighwayObjectList_list, int) };

void BingoHighwayObjects_Init(const char *path) {
	TexLists_Obj[HeroesLevelID_BingoHighway] = BingoHighwayObjectTextures;
	ObjLists[HeroesLevelID_BingoHighway * 8] = &BingoHighwayObjectList;
}