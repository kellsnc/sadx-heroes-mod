#include "stdafx.h"
#include "mod.h"
#include "objects.h"

void _cdecl FLIPPERS(ObjectMaster *a1);
void _cdecl CPDICE(ObjectMaster *a1);
void _cdecl CPBOBINAIR(ObjectMaster *a1);
void _cdecl CPSLOTS(ObjectMaster *a1);
void _cdecl CPSLOTL(ObjectMaster *a1);
void _cdecl CPDOOR(ObjectMaster *a1);
void __cdecl CPRoulette(ObjectMaster *a1);

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

#pragma region Bingo Card
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
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (BingoState[0]) BINGONB[0]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[1]) BINGONB[1]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[2]) BINGONB[2]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[3]) BINGONB[3]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[4]) BINGONB[4]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[5]) BINGONB[5]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[6]) BINGONB[6]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[7]) BINGONB[7]->mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[8]) BINGONB[8]->mats[0].diffuse.color = 0xFF5B5B5B;

		BHBingoCard_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl BHBingoCard(ObjectMaster *a1)
{
	a1->MainSub = &BHBingoCard_Main;
	a1->DisplaySub = &BHBingoCard_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region BingoHandler
void __cdecl BHBingoHandler_Main(ObjectMaster *a1)
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

void __cdecl BHBingoHandler(ObjectMaster *a1)
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
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Bingo Number
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
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

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
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl BHBingoNumber(ObjectMaster *a1)
{
	a1->MainSub = &BHBingoNumber_Main;
	a1->DisplaySub = &BHBingoNumber_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Giant Chips
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
	if (IsPlayerInsideSphere(&a1->Data1->Position, 4400.0f)) {

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
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl BHGiantTokens(ObjectMaster *a1)
{
	a1->MainSub = &BHGiantTokens_Main;
	a1->DisplaySub = &BHGiantTokens_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Signs
void BHSigns_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
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

void BHSigns(ObjectMaster *a1) {
	a1->DisplaySub = BHSigns_Display;
	a1->MainSub = BHSigns_Display;
}
#pragma endregion

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
{ 2, 3, 1, 1660000, 0, (ObjectFuncPtr)&CPDICE, "CPDICE" } /* "Moving Dice" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPBOBINAIR, "CPBOBINAIR" } /* "Round bumpers" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPSLOTS, "CPSLOTS" } /* "Small slot machine" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPSLOTL, "CPSLOTL" } /* "Large slot machine" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPDOOR, "CPDOOR" } /* "Sliding X door" */, //65
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&ObjCannon, "CANNON" } /* "SH Cannon" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)&SHCameraSwitch, "SH CAM SW" } /* "Camera flag switcher" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&BHBingoCard, "BING CARD" } /* "Giant Bingo Card" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)&BHBingoHandler, "BINGHANDLE" } /* "Bingo initializer & handler" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&BHBingoNumber, "BINGO NB" } /* "Bingo Number" */, //70
{ 2, 3, 1, 14360000, 0, (ObjectFuncPtr)&BHGiantTokens, "BHGIANTCHP" } /* "Giant Chips" */,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&CPRoulette, "CPRoulette" } /* "Turning roulette" */,
{ 2, 3, 1, 1060000, 0, &ObjFan, "OBJFAN" } /* "SH FANS" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
};
ObjectList BingoHighwayObjectList = { arraylengthandptrT(BingoHighwayObjectList_list, int) };

void BingoHighwayObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB78, BingoHighwayObjectTextures);
	WriteData((ObjectList**)0x974B78, &BingoHighwayObjectList); //974B7C 974B80
}