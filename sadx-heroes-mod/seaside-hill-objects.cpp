#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "seaside-hill-objects.h"

ModelInfo * SH_PLATFOR;
ModelInfo * SH_MORUINS;
ModelInfo * SH_POLFLAG;
ModelInfo * SH_WATERFS;
ModelInfo * SH_FLOWERS;

#pragma region Sun
void SHSuns_Display(ObjectMaster * a1) {
	if (!MissedFrames)
		DrawLensFlare(&a1->Data1->Position);
}

void SHSuns_Init(ObjectMaster * a1) {
	a1->DisplaySub = SHSuns_Display;
	a1->MainSub = SHSuns_Display;
}
#pragma endregion

#pragma region Flowers
void SHFlowers_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(SeasideHill_Flowers); ++i) {
			if (CheckModelDisplay(SeasideHill_Flowers[i])) {
				SOI_LIST2 item = SeasideHill_Flowers[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(SH_FLOWERS->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->basicdxmodel); break;
				case 2: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->basicdxmodel); break;
				case 4: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->child->basicdxmodel); break;
				case 5: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->child->child->basicdxmodel); break;
				case 6: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->child->child->child->basicdxmodel); break;
				case 7: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->child->child->child->child->basicdxmodel); break;
				case 8: njDrawModel_SADX(SH_FLOWERS->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void SHFlowers(ObjectMaster *a1) {
	a1->DisplaySub = SHFlowers_Display;
	a1->MainSub = SHFlowers_Display;
}
#pragma endregion

#pragma region Waterfalls
void SHWaterfalls_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(SeasideHill_Waterfalls); ++i) {
			if (CheckModelDisplay(SeasideHill_Waterfalls[i])) {
				SOI_LIST2 item = SeasideHill_Waterfalls[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(SH_WATERFS->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(SH_WATERFS->getmodel()->child->basicdxmodel); break;
				case 2: njDrawModel_SADX(SH_WATERFS->getmodel()->child->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(SH_WATERFS->getmodel()->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void SHWaterfalls(ObjectMaster *a1) {
	a1->DisplaySub = SHWaterfalls_Display;
	a1->MainSub = SHWaterfalls_Display;
}
#pragma endregion

#pragma region Moving Ruins
extern float ruin = 0;

void SH_MOVINGPLATFORMS_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void SH_MOVINGPLATFORMS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (ruin != 0 && ruin == a1->Data1->Scale.y) {

			if (a1->Data1->Position.y != a1->Data1->Scale.z) {

				if (a1->Data1->Scale.x >= a1->Data1->Scale.z) {
					if (a1->Data1->Position.y <= a1->Data1->Scale.z) {
						a1->Data1->Position.y = a1->Data1->Scale.z;
					}
					else if (a1->Data1->Position.y > a1->Data1->Scale.z) a1->Data1->Position.y -= 2;

					if (a1->Data1->Position.y == a1->Data1->Scale.x - 2) if (EnableSounds) PlaySound(41, 0, 0, 0);
				}
				else if (a1->Data1->Scale.x <= a1->Data1->Scale.z) {
					if (a1->Data1->Position.y >= a1->Data1->Scale.z) {
						a1->Data1->Position.y = a1->Data1->Scale.z;
					}
					else if (a1->Data1->Position.y < a1->Data1->Scale.z) a1->Data1->Position.y += 2;

					if (a1->Data1->Position.y == a1->Data1->Scale.x + 2) if (EnableSounds) PlaySound(41, 0, 0, 0);
					if (a1->Data1->Position.y <= a1->Data1->Scale.z - 1 && a1->Data1->Position.y >= a1->Data1->Scale.z - 3) if (EnableSounds) PlaySound(42, 0, 0, 0);
				}

			}

		}
		a1->Data1->Object->pos[1] = a1->Data1->Position.y;
		SH_MOVINGPLATFORMS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl SH_MOVINGPLATFORMS(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 0) a1->Data1->Object = SH_MORUINS->getmodel();
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = SH_MORUINS->getmodel()->child;
	if (a1->Data1->Scale.x == 2) a1->Data1->Object = SH_MORUINS->getmodel()->child->child;
	a1->Data1->Action = (char)a1->Data1->Scale.x;
	a1->Data1->Scale.x = a1->Data1->Position.y;

	if (a1->Data1->Scale.z == 0) {
		AddToCollision(a1, 0);
	}
	else {
		AddToCollision(a1, 1);
	}

	a1->MainSub = &SH_MOVINGPLATFORMS_Main;
	a1->DisplaySub = &SH_MOVINGPLATFORMS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Ruin Trigger
void __cdecl SHRUINTRIGGER_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (ruin != a1->Data1->Scale.x) {
			auto entity = EntityData1Ptrs[0];
			if (IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.y)) {
				ruin = a1->Data1->Scale.x;
			}
		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl SHRUINTRIGGER(ObjectMaster *a1)
{
	a1->MainSub = &SHRUINTRIGGER_Main;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Small stone platforms
void __cdecl SHPlatforms_Display(ObjectMaster *a1)
{
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, true);
}

void __cdecl SHPlatforms_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		SHPlatforms_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl SHPlatforms(ObjectMaster *a1)
{
	a1->Data1->Object = SH_PLATFOR->getmodel();
	AddToCollision(a1, 2);

	a1->MainSub = &SHPlatforms_Main;
	a1->DisplaySub = &SHPlatforms_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Spikes + flags
static int flagtimer = 0;

void Flags_Reset() {
	flagtimer = 0;
	SH_Flag.points[5].z = 0;
	SH_Flag.points[7].z = 0;
	SH_Flag.points[18].z = 0;
	SH_Flag.points[19].z = 0;
	SH_Flag.points[0].z = 0;
	SH_Flag.points[2].z = 0;
	SH_Flag.points[12].z = 0;
	SH_Flag.points[13].z = 0;
	SH_Flag.points[1].z = 0;
	SH_Flag.points[3].z = 0;
	SH_Flag.points[14].z = 0;
	SH_Flag.points[15].z = 0;
}

void Flags_Animate() {
	flagtimer++;
	if (flagtimer > 100) flagtimer = 1;
	if (flagtimer < 51) {
		SH_Flag.points[5].z -= 0.1f;
		SH_Flag.points[7].z -= 0.1f;
		SH_Flag.points[18].z -= 0.1f;
		SH_Flag.points[19].z -= 0.1f;
		SH_Flag.points[0].z += 0.05f;
		SH_Flag.points[2].z += 0.05f;
		SH_Flag.points[12].z += 0.05f;
		SH_Flag.points[13].z += 0.05f;
		SH_Flag.points[1].z -= 0.02f;
		SH_Flag.points[3].z -= 0.02f;
		SH_Flag.points[14].z -= 0.02f;
		SH_Flag.points[15].z -= 0.02f;
	}
	else {
		SH_Flag.points[5].z += 0.1f;
		SH_Flag.points[7].z += 0.1f;
		SH_Flag.points[18].z += 0.1f;
		SH_Flag.points[19].z += 0.1f;
		SH_Flag.points[0].z -= 0.05f;
		SH_Flag.points[2].z -= 0.05f;
		SH_Flag.points[12].z -= 0.05f;
		SH_Flag.points[13].z -= 0.05f;
		SH_Flag.points[1].z += 0.02f;
		SH_Flag.points[3].z += 0.02f;
		SH_Flag.points[14].z += 0.02f;
		SH_Flag.points[15].z += 0.02f;
	}
}

void __cdecl SHSPIKES_Display(ObjectMaster *a1)
{
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		njRotateY(0, -a1->Data1->Rotation.y + a1->Data1->Scale.z);
		njScale(nullptr, 0.8f, 1, 1);
		njDrawModel_SADX(&SH_Flag);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void __cdecl SHSPIKES_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (a1->Data1->Scale.z > 4000) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z == 0) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) a1->Data1->Scale.z += 10;
		if (a1->Data1->Scale.y == 1) a1->Data1->Scale.z -= 10;

		SHSPIKES_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl SHSPIKES(ObjectMaster *a1)
{
	a1->Data1->Object = SH_POLFLAG->getmodel();
	AddToCollision(a1, 0);
	a1->Data1->Scale.y = 0;

	a1->MainSub = &SHSPIKES_Main;
	a1->DisplaySub = &SHSPIKES_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

PVMEntry SeasideHillObjectTextures[] = {
	{ "OBJ_BEACH", &OBJ_BEACH_TEXLIST },
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
	{ "tai", &tai_TEXLIST },
	{ "hammer", &HAMMER_TEXLIST },
	{ "same", &SAME_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ NULL, (TexList *)0x10A3130 },
	{ NULL, (TexList *)0x10C786C },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ 0 }
};

ObjectListEntry SeasideHillObjectList_list[] = {
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
	{ 2, 2, 1, 40000, 0, (ObjectFuncPtr)0x7A8E50, "O BUBBLE" } /* "O BUBBLE" */,
	{ 2, 4, 1, 40000, 0, (ObjectFuncPtr)0x7A8A60, "O BUBBLES" } /* "O BUBBLES" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4AD140, "E SARU  " } /* "E SARU  " */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A1380, "E SAITO" } /* "E SAITO" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7AA960, "E AMEMB" } /* "E AMEMB" */,
	{ 2, 3, 1, 2060000, 0, &SH_MOVINGPLATFORMS, "MOV PLA" } /* "SH Moving platforms" */,
	{ 2, 3, 1, 200000, 0, &SHRUINTRIGGER, "MOV PLA T" } /* "SH Ruins Trigger" */,
	{ 2, 3, 1, 1860000, 0, &SHPlatforms, "SMALL PLA   " } /* "SH Small Platform" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500EC0, "YASI3   " } /* "YASI3   " */,
	{ 6, 3, 1, 360000, 0, &ObjBalloon, "SH BALLOON" } /* "SH Balloon" */,
	{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB730, "O BUNGA   " } /* "O BUNGA   " */,
	{ 10, 3, 1, 160000, 0, (ObjectFuncPtr)0x500B90, "O BGIRO   " } /* "O BGIRO   " */,
	{ 15, 3, 1, 360000, 0, (ObjectFuncPtr)0x500770, "O IWA   " } /* "O IWA   " */,
	{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x5001E0, "O JUMP   " } /* "O JUMP   " */,
	{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB770, "O IWAPO   " } /* "O IWAPO   " */,
	{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB750, "O ROCK   " } /* "O ROCK   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFDC0, "O FENA   " } /* "O FENA   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFE00, "O FENB   " } /* "O FENB   " */,
	{ 2, 3, 1, 422500, 0, (ObjectFuncPtr)0x4FFE40, "O FENC   " } /* "O FENC   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFE80, "O FEND   " } /* "O FEND   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD80, "O CORA   " } /* "O CORA   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFDA0, "O CORB   " } /* "O CORB   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD00, "O GRASA   " } /* "O GRASA   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD20, "O GRASB   " } /* "O GRASB   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD40, "O GRASC   " } /* "O GRASC   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD60, "O GRASD   " } /* "O GRASD   " */,
	{ 10, 3, 1, 3240000, 0, (ObjectFuncPtr)0x4FACD0, "O DOLPHIN   " } /* "O DOLPHIN   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB790, "O SEKITYU   " } /* "O SEKITYU   " */,
	{ 11, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFC30, "O KAMOME   " } /* "O KAMOME   " */,
	{ 11, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFB60, "O KAMOMEL   " } /* "O KAMOMEL   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB7E0, "O KOMOMO   " } /* "O KOMOMO   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FF5F0, "O PARASOL" } /* "O PARASOL" */,
	{ 6, 3, 1, 250000, 0, (ObjectFuncPtr)0x4FF060, "O PIER" } /* "O PIER" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FE8C0, "O GOMBAN" } /* "O GOMBAN" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4F7E90, "O ASIATO" } /* "O ASIATO" */,
	{ 10, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FD9D0, "O BKUSA" } /* "O BKUSA" */,
	{ 10, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FD770, "O MKUSA" } /* "O MKUSA" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FDA90, "O K2" } /* "O K2" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FD160, "O SAKANA8K" } /* "O SAKANA8K" */,
	{ 3, 3, 1, 1000000, 0, (ObjectFuncPtr)0x4FC550, "O BIGROCK" } /* "O BIGROCK" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FBE40, "O AO SUMMON" } /* "O AO SUMMON" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FBCE0, "O AO KILL" } /* "O AO KILL" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FBBE0, "O PO SUMMON" } /* "O PO SUMMON" */,
	{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)0x4FBA70, "O TAKI" } /* "O TAKI" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB800, "O Rock2" } /* "O Rock2" */,
	{ 3, 3, 1, 1000000, 0, (ObjectFuncPtr)0x4FB5F0, "O ARCHROCK" } /* "O ARCHROCK" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB160, "O BED" } /* "O BED" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB050, "O KASA" } /* "O KASA" */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FAE30, "O DOLSW" } /* "O DOLSW" */,
	{ 3, 3, 1, 160000, 0, (ObjectFuncPtr)0x5977F0, "S BASS " } /* "S BASS " */,
	{ 3, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A7AD0, "S KAERU" } /* "S KAERU" */,
	{ 3, 3, 1, 90000, 0, (ObjectFuncPtr)0x597660, "S GENE " } /* "S GENE " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FA3B0, "O BEWIND " } /* "O BEWIND " */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4FA320, "O FROG " } /* "O FROG " */,
	{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x4F9FB0, "O BZ " } /* "O BZ " */,
	{ 15, 3, 1, 160000, 0, (ObjectFuncPtr)0x4C07D0, "O ItemBoxAir" } /* "O ItemBoxAir" */,
	{ 2, 0, 1, 1000000, 0, (ObjectFuncPtr)0x4D4BE0, "BIGWJUMP" } /* "BIGWJUMP" */,
	{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0DF0, "SPINA A" } /* "SPINA A" */,
	{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0F40, "SPINA B" } /* "SPINA B" */,
	{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B1090, "SPINA C" } /* "SPINA C" */,
	{ 2, 2, 0, 0, 0, (ObjectFuncPtr)0x7A4E50, "O SPRING B" } /* "O SPRING B" */,
	{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x79F860, "O SPRING H" } /* "O SPRING H" */,
	{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&SHSPIKES, "SHSPIKES" } /* "SH Spikes" */,
	{ 2, 3, 1, 1900000, 0, (ObjectFuncPtr)&ObjBob, "OBJ BOB" } /* "Bobsled" */,
	{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
};

ObjectList SeasideHillObjectList = { arraylengthandptrT(SeasideHillObjectList_list, int) };

void SeasideHillObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB6C, SeasideHillObjectTextures);
	WriteData((ObjectList**)0x974B18, &SeasideHillObjectList); //Act 1
	WriteData((ObjectList**)0x974B1C, &SeasideHillObjectList); //Act 2
	WriteData((ObjectList**)0x974B20, &SeasideHillObjectList); //Act 3
}

void SeasideHillObjects_OnFrame(EntityData1 * entity) {
	Flags_Animate();
	if (!IsLoaded) Flags_Reset();
}