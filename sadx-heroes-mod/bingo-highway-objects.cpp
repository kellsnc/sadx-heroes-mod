#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "bingo-highway-objects.h"

extern NJS_MODEL_SADX * CASINOOBJLIST[9];
extern SH_ANIMTEXS BingoHighwayAnimTexs[37];

void _cdecl FLIPPERS(ObjectMaster *a1);
void _cdecl CPDICE(ObjectMaster *a1);
void _cdecl CPBOBINAIR(ObjectMaster *a1);
void _cdecl CPSLOTS(ObjectMaster *a1);
void _cdecl CPSLOTL(ObjectMaster *a1);
void _cdecl CPDOOR(ObjectMaster *a1);

static char BingoState[9];
static char BingoLines[8];
ObjectMaster * BingoHandlerPointer;

#pragma region Bingo Card
void BHBingoCard_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(0, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njTranslate(0, 0, 40, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(&BH_BINGOCARD);

		njTranslate(0, 0, 0, -10);

		njTranslate(0, 45, 45, 0);
		DrawModel(&BH_BINGO1);
		njTranslate(0, -45, -45, 0);

		njTranslate(0, 45, 0, 0);
		DrawModel(&BH_BINGO2);
		njTranslate(0, -45, 0, 0);

		DrawModel(&BH_BINGO3);

		njTranslate(0, 0, 45, 0);
		DrawModel(&BH_BINGO4);
		njTranslate(0, 0, -45, 0);

		njTranslate(0, 45, -45, 0);
		DrawModel(&BH_BINGO5);
		njTranslate(0, -45, 45, 0);

		njTranslate(0, -45, 45, 0);
		DrawModel(&BH_BINGO6);
		njTranslate(0, 45, -45, 0);

		njTranslate(0, -45, 0, 0);
		DrawModel(&BH_BINGO7);
		njTranslate(0, 45, 0, 0);

		njTranslate(0, 0, -45, 0);
		DrawModel(&BH_BINGO8);
		njTranslate(0, 0, 45, 0);

		njTranslate(0, -45, -45, 0);
		DrawModel(&BH_BINGO9);
		njTranslate(0, 45, 45, 0);

		njTranslate(0, 0, 0, 10);
		njRotateZ(0, 0x4000);
		if (BingoLines[0]) {
			njTranslate(0, 45, 0, 0);
			DrawModel(&BH_BINGOBAR);
			njTranslate(0, -45, 0, 0);
		}
		if (BingoLines[1]) DrawModel(&BH_BINGOBAR);
		if (BingoLines[2]) {
			njTranslate(0, -45, 0, 0);
			DrawModel(&BH_BINGOBAR);
			njTranslate(0, 45, 0, 0);
		}
		njRotateZ(0, -0x4000);

		if (BingoLines[3]) {
			njTranslate(0, -45, 0, 0);
			DrawModel(&BH_BINGOBAR);
			njTranslate(0, 45, 0, 0);
		}
		if (BingoLines[4]) DrawModel(&BH_BINGOBAR);
		if (BingoLines[5]) {
			njTranslate(0, 45, 0, 0);
			DrawModel(&BH_BINGOBAR);
			njTranslate(0, -45, 0, 0);
		}

		if (BingoLines[6]) {
			njRotateZ(0, 0x2000);
			DrawModel(&BH_BINGOBAR);
			njRotateZ(0, -0x2000);
		}
		if (BingoLines[7]) {
			njRotateZ(0, -0x2000);
			DrawModel(&BH_BINGOBAR);
			njRotateZ(0, 0x2000);
		}

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void BHBingoCard_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (BingoState[0]) BH_BINGO1.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[1]) BH_BINGO2.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[2]) BH_BINGO3.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[3]) BH_BINGO4.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[4]) BH_BINGO5.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[5]) BH_BINGO6.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[6]) BH_BINGO7.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[7]) BH_BINGO8.mats[0].diffuse.color = 0xFF5B5B5B;
		if (BingoState[8]) BH_BINGO9.mats[0].diffuse.color = 0xFF5B5B5B;

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

	BH_BINGO1.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO2.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO3.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO4.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO5.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO6.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO7.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO8.mats[0].diffuse.color = 0xFFFFFFFF;
	BH_BINGO9.mats[0].diffuse.color = 0xFFFFFFFF;

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
		if (nb == 1) DrawModel(&BH_BINGO1);
		else if (nb == 2) DrawModel(&BH_BINGO2);
		else if (nb == 3) DrawModel(&BH_BINGO3);
		else if (nb == 4) DrawModel(&BH_BINGO4);
		else if (nb == 5) DrawModel(&BH_BINGO5);
		else if (nb == 6) DrawModel(&BH_BINGO6);
		else if (nb == 7) DrawModel(&BH_BINGO7);
		else if (nb == 8) DrawModel(&BH_BINGO8);
		else if (nb == 9) DrawModel(&BH_BINGO9);

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

#pragma region Giant Dice
void BHGiantTokens_Display(ObjectMaster *a1) {
	if (a1->Data1->Scale.y != 0) if (CurrentChunk != a1->Data1->Scale.y) return;



	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateY(nullptr, a1->Data1->Rotation.y);
		if (a1->Data1->Scale.z == 2) njScale(nullptr, 0.5f, 0.5f, 0.5f);
		else if (a1->Data1->Scale.z == 3 || a1->Data1->Scale.z == 4) njScale(nullptr, 0.7f, 0.7f, 0.7f);
		else njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;

		if (a1->Data1->Scale.x == 0) DrawModel(&BH_GIANTCHIP1);
		else if (a1->Data1->Scale.x == 1) DrawModel(&BH_GIANTCHIP5);
		else if (a1->Data1->Scale.x == 2) DrawModel(&BH_GIANTCHIP4);
		else if (a1->Data1->Scale.x == 3) DrawModel(&BH_GIANTCHIP2);
		else if (a1->Data1->Scale.x == 4) DrawModel(&BH_GIANTCHIP3);

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

#pragma region Roulette
void BHRoulette_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njRotateY(0, a1->Data1->Scale.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		DrawModel(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void BHRoulette_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		a1->Data1->Scale.z += 60;
		BHRoulette_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl BHRoulette(ObjectMaster *a1)
{
	a1->Data1->Object = &BH_ROURETTE;
	AddToCollision(a1, 0);

	a1->MainSub = &BHRoulette_Main;
	a1->DisplaySub = &BHRoulette_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

PVMEntry BingoHighwayObjectTextures[] = {
	{ "OBJ_HIGHWAY", &OBJ_HIGHWAY_TEXLIST },
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "kajiki", &kajiki_TEXLIST },
	{ "PEN", &PEN_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "RAKO", &RAKO_TEXLIST },
	{ "mecha", &mecha_TEXLIST },
	{ "sea_bass", &sea_bass_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "utsubo", &utsubo_TEXLIST },
	{ "GOMA", &GOMA_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "tai", &tai_TEXLIST },
	{ "WARA", &WARA_TEXLIST },
	{ "hammer", &HAMMER_TEXLIST },
	{ "same", &SAME_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ NULL, (TexList *)0x10A3130 },
	{ NULL, (TexList *)0x10C786C },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
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
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&BHRoulette, "BHRoulette" } /* "Turning roulette" */,
{ 2, 3, 1, 1060000, 0, &ObjFan, "OBJFAN" } /* "SH FANS" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
};
ObjectList BingoHighwayObjectList = { arraylengthandptrT(BingoHighwayObjectList_list, int) };

void BingoHighwayObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB78, BingoHighwayObjectTextures);
	WriteData((ObjectList**)0x974B78, &BingoHighwayObjectList); //974B7C 974B80
}

void BingoHighwayObjects_OnFrame(EntityData1 * entity) {
	AnimateObjectsTextures(BHOBJLIST, LengthOfArray(BHOBJLIST), BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));
	AnimateObjectsTextures(CASINOOBJLIST, LengthOfArray(CASINOOBJLIST), BingoHighwayAnimTexs, LengthOfArray(BingoHighwayAnimTexs));

	if (anim % 4 == 0) {
		CurrentLandTable->Col[0].Model->pos[0] = entity->Position.x;
		CurrentLandTable->Col[0].Model->pos[1] = entity->Position.y;
		CurrentLandTable->Col[0].Model->pos[2] = entity->Position.z;
	}
}