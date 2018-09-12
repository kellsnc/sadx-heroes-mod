#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "grand-metropolis-objects.h"

uint8_t texstate = 0;
int gmtimer;
int pistonstate;

#pragma region Energy_H
void __cdecl GMEnergyH_Display(ObjectMaster *a1)
{
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, a1->Data1->Scale.y, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(&s03_door_energy);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void __cdecl GMEnergyH_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (a1->Data1->Scale.x == 0) a1->Data1->Scale.y = 180;
		else {
			if (IsSwitchPressed(a1->Data1->Scale.x)) {
				if (a1->Data1->Scale.y < 180) a1->Data1->Scale.y += 5;
			}
		}
		GMEnergyH_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl GMEnergyH(ObjectMaster *a1)
{
	a1->MainSub = &GMEnergyH_Main;
	a1->DisplaySub = &GMEnergyH_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Energy Doors
void GM_ENERGYDOORS_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		if (a1->Data1->Rotation.x == 1) njDrawModel_SADX(s03_door_2obj.basicdxmodel);
		else njDrawModel_SADX(s03_door_1obj.basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GM_ENERGYDOORS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {

		if (a1->Data1->Action != 0) {
			if (IsSwitchPressed(a1->Data1->Action)) {
				if (a1->Data1->Scale.y != 0 && a1->Data1->Scale.z != 0) {
					if (a1->Data1->Position.x != a1->Data1->Scale.y) {
						a1->Data1->Position.x = a1->Data1->Scale.y;
						a1->Data1->Position.z = a1->Data1->Scale.z;
						a1->Data1->Object->pos[0] = a1->Data1->Scale.y;
						a1->Data1->Object->pos[2] = a1->Data1->Scale.z;
					}
				}
				else {
					if (a1->Data1->Scale.y != 0) {
						float posx = a1->Data1->Scale.y;
						int order = a1->Data1->Scale.x;

						if (order == 0) {
							if (a1->Data1->Position.x < posx) a1->Data1->Position.x += 1;
							else a1->Data1->Position.x = posx;
						}

						if (order == 1) {
							if (a1->Data1->Position.x > posx) a1->Data1->Position.x -= 1;
							else a1->Data1->Position.x = posx;
						}
					}

					if (a1->Data1->Scale.z != 0) {
						float posz = a1->Data1->Scale.z;
						int order = a1->Data1->Scale.x;

						if (order == 0) {
							if (a1->Data1->Position.z < posz) a1->Data1->Position.z += 1;
							else a1->Data1->Position.z = posz;
						}

						if (order == 1) {
							if (a1->Data1->Position.z > posz) a1->Data1->Position.z -= 1;
							else a1->Data1->Position.z = posz;
						}
					}
				}
			}
		}

		a1->Data1->Object->pos[0] = a1->Data1->Position.x;
		a1->Data1->Object->pos[2] = a1->Data1->Position.z;
		GM_ENERGYDOORS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl GM_ENERGYDOORS(ObjectMaster *a1)
{
	if (a1->Data1->Rotation.x == 1) a1->Data1->Object = &s03_door_2obj;
	else a1->Data1->Object = &s03_door_1obj;

	a1->Data1->Action = a1->Data1->Scale.x; //Action = switch id

	if (a1->Data1->Action != 0) {
		AddToCollision(a1, 1);
		if (a1->Data1->Scale.y != 0 && a1->Data1->Scale.z != 0) {

		}
		else {
			if (a1->Data1->Scale.y != 0) {
				if (a1->Data1->Rotation.x == 1) a1->Data1->Scale.x = 0;
				else a1->Data1->Scale.x = 1;
			}
			if (a1->Data1->Scale.z != 0) {
				if (a1->Data1->Position.z < a1->Data1->Scale.z) a1->Data1->Scale.x = 0;
				else a1->Data1->Scale.x = 1;
			}
		}
	}
	else AddToCollision(a1, 0);

	a1->MainSub = &GM_ENERGYDOORS_Main;
	a1->DisplaySub = &GM_ENERGYDOORS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Energy Paths
void GM_ENERGYPATHS_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, a1->Data1->Scale.z, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&s03_fluid_02);
		else njDrawModel_SADX(&s03_fluid_01);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GM_ENERGYPATHS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (a1->Data1->Scale.z <= 1) if (IsSwitchPressed(a1->Data1->Scale.y)) a1->Data1->Scale.z += 0.1f;
		if (a1->Data1->Scale.z == 0.1f) PlaySound(52, 0, 0, 0);

		if (CurrentChunk == 1 && IsSwitchPressed(3)) CurrentLandTable->Col[6].Flags = 0x000008A1;
		if (CurrentChunk == 2 && IsSwitchPressed(3)) CurrentLandTable->Col[17].Flags = 0x000008A1;
		if (CurrentChunk == 2 && IsSwitchPressed(4)) CurrentLandTable->Col[20].Flags = 0x000008A1;
		if (CurrentChunk == 2 && IsSwitchPressed(24)) CurrentLandTable->Col[16].Flags = 0x000008A1;
		if (CurrentChunk == 3 && IsSwitchPressed(24)) CurrentLandTable->Col[5].Flags = 0x000008A1;
		if (CurrentChunk == 10 && IsSwitchPressed(10)) CurrentLandTable->Col[11].Flags = 0x000008A1;

		GM_ENERGYPATHS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl GM_ENERGYPATHS(ObjectMaster *a1)
{
	a1->MainSub = &GM_ENERGYPATHS_Main;
	a1->DisplaySub = &GM_ENERGYPATHS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

PVMEntry GrandMetropolisObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

ObjectListEntry GrandMetropolisObjectList_list[] = {
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
{ 2, 3, 1, 1360000, 0, &ObjReel, "OBJREEL" } /* "Reel" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500FF0, "YASI1   " } /* "YASI1   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x501090, "YASI2   " } /* "YASI2   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500EC0, "YASI3   " } /* "YASI3   " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB710, "O BFLOOT   " } /* "O BFLOOT   " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB730, "O BUNGA   " } /* "O BUNGA   " */,
{ 10, 3, 1, 160000, 0, (ObjectFuncPtr)0x500B90, "O BGIRO   " } /* "O BGIRO   " */,
{ 15, 3, 1, 360000, 0, (ObjectFuncPtr)0x500770, "O IWA   " } /* "O IWA   " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x5001E0, "O JUMP   " } /* "O JUMP   " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB770, "O IWAPO   " } /* "O IWAPO   " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FB750, "O ROCK   " } /* "O ROCK   " */,
{ 2, 3, 1, 1860000, 0, &GM_ENERGYPATHS, "GM PATHS" } /* "GM Energy Paths" */,
{ 2, 3, 1, 1360000, 0, &GM_ENERGYDOORS, "GM DOORS" } /* "GM Energy Doors" */,
{ 2, 3, 1, 1360000, 0, &GMEnergyH, "GM ENER" } /* "GM Energy H" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
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
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF190, "E UNI A" } /* "E UNI A" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF500, "E UNI B" } /* "E UNI B" */,
{ 2, 3, 5, 250000, 0, (ObjectFuncPtr)0x4AF860, "E UNI C" } /* "E UNI C" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4B3210, "E POLICE" } /* "E POLICE" */,
};
ObjectList GrandMetropolisObjectList = { arraylengthandptrT(GrandMetropolisObjectList_list, int) };

void GrandMetropolisObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB8C, GrandMetropolisObjectTextures);
	WriteData((ObjectList**)0x974C18, &GrandMetropolisObjectList);
	WriteData((ObjectList**)0x974C1C, &GrandMetropolisObjectList);
}

void ResetCars() {
	grand_metropolis_flying_objects[1].soipos = { 5300.872f, -1894.9f, -14251.39f };
	grand_metropolis_flying_objects[2].soipos = { -3397.503f, -899.9f, -248.553f };
	grand_metropolis_flying_objects[3].soipos = { 1919.699f, -536.9f, -3675.037f };
	grand_metropolis_flying_objects[4].soipos = { 1948.472f, -692.9f, 2532.573f };
	grand_metropolis_flying_objects[5].soipos = { -3058.688f, -845.9f, 4444.6678f };
	grand_metropolis_flying_objects[6].soipos = { 6643.312f, -1630.9f, -15809.23f };
	grand_metropolis_flying_objects[7].soipos = { -3345.827f, -832.9f, -15608.49f };
	grand_metropolis_flying_objects[8].soipos = { 5645.713f, -1516.9f, -15597.63f };
	grand_metropolis_flying_objects[9].soipos = { 5615.578f, -1333.9f, -14291.76f };
	grand_metropolis_flying_objects[10].soipos = { -4707.142f, -1586.9f, -13933.31f };
	grand_metropolis_flying_objects[11].soipos = { 3939.971f, -1955.9f, -13785.85f };
	grand_metropolis_flying_objects[12].soipos = { 4825.928f, -2603.9f, -41714.32f };
	grand_metropolis_flying_objects[13].soipos = { -1538.811f, -4059.9f, -44607.31f };
	grand_metropolis_flying_objects[14].soipos = { 280.5001f, -2643.9f, -44618.79f };
	grand_metropolis_flying_objects[15].soipos = { 5462.191f, -2319.9f, -35606.65f };
	grand_metropolis_flying_objects[16].soipos = { -7574.771f, -2369.9f, -35812.54f };
	grand_metropolis_flying_objects[17].soipos = { 5667.807f, -3975.9f, -43996.86f };
}

void PistonsHandler() {
	++gmtimer;
	if (pistonstate == 0) {
		for (int i = 0; i < grand_metropolis_objects[2].soicount; ++i) {
			if (i % 2 != 0) grand_metropolis_pistons[i].soipos.y += 24;
		}
	}
	if (gmtimer == 400) gmtimer = 0;
	if (gmtimer < 200) pistonstate = 1;
	if (gmtimer > 199) pistonstate = 2;
	for (int i = 0; i < grand_metropolis_objects[2].soicount; ++i) {
		if (pistonstate == 1) {
			if (i % 2 == 0) grand_metropolis_pistons[i].soipos.y += 0.12f;
			if (i % 2 != 0) grand_metropolis_pistons[i].soipos.y -= 0.12f;
		}
		if (pistonstate == 2) {
			if (i % 2 == 0) grand_metropolis_pistons[i].soipos.y -= 0.12f;
			if (i % 2 != 0) grand_metropolis_pistons[i].soipos.y += 0.12f;
		}
	}
}

void CarsHandler() {
	if (anim % 1800 == 0) {
		ResetCars();
	}
	grand_metropolis_flying_objects[0].soipos.x -= 0.5f;
	grand_metropolis_flying_objects[0].soipos.z -= 1;
	grand_metropolis_flying_objects[2].soipos.x += 10;
	grand_metropolis_flying_objects[2].soipos.z -= 1;
	grand_metropolis_flying_objects[3].soipos.x -= 10;
	grand_metropolis_flying_objects[3].soipos.z += 10;
	grand_metropolis_flying_objects[4].soipos.x -= 10;
	grand_metropolis_flying_objects[7].soipos.x += 10;
	grand_metropolis_flying_objects[10].soipos.x += 10;
	grand_metropolis_flying_objects[11].soipos.x -= 10;
	grand_metropolis_flying_objects[11].soipos.z += 7;
}

void AutoPathsMovs() {
	EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
	for (uint8_t slot = 0; slot < 8; ++slot) {
		if (players[slot]) {
			CharObj2 *co2 = GetCharObj2(slot);
			if (co2) {
				if (co2->SurfaceFlags == 0x8a1 || co2->SurfaceFlags == 0xA81) {
					set_diffuse_blend(2, 4);
					set_diffuse_blend_factor(0.3f);

					auto entity = EntityData1Ptrs[slot];
					if (entity->Position.z > 472) entity->Position.z -= 2;
					else if (entity->Position.z > -743) {}
					else if (entity->Position.z > -1808) entity->Position.z -= 2;
					else if (entity->Position.z > -2438) { entity->Position.x -= 2; entity->Position.z -= 1.6f; }
					else if (entity->Position.z > -3266) { entity->Position.z -= 2; }
					else if (entity->Position.z > -5500) entity->Position.z -= 1;
					else if (entity->Position.z > -6163) { entity->Position.x += 2; entity->Position.z -= 1.6f; }
					else if (entity->Position.z > -7978) {}
					else if (entity->Position.z > -9848) { entity->Position.z -= 2; }
					else if (entity->Position.z > -11407) entity->Position.z -= 2;
					else if (entity->Position.z > -14359) {}
					else if (entity->Position.z > -14965) entity->Position.z -= 2;
					else if (entity->Position.z > -20276) { entity->Position.z -= 2; }
					else if (entity->Position.z > -23570) { entity->Position.x += 2; entity->Position.z -= 1.4f; }
					else if (entity->Position.z > -29198) entity->Position.z -= 2;
					else if (entity->Position.z > -34096) { entity->Position.z += 2; } //chunk 6
					else if (entity->Position.z > -39404) entity->Position.z -= 2; //chunk 8
					else if (entity->Position.z > -41125) {}
					else if (entity->Position.z < -41604) {
						if (entity->Position.x < 5816) entity->Position.x += 2;
						else if (entity->Position.x < 6385) { entity->Position.x += 1.6f; entity->Position.z -= 2; }
						else if (entity->Position.x < 7945) entity->Position.x += 2;
						else if (entity->Position.x < 9352) { entity->Position.x += 1.4f; entity->Position.z -= 2; }
						else entity->Position.x += 1;
					}
				}
				else {
					if (anim % 4 == true) set_diffuse_blend_factor(0);
				}
			}
		}
	}
}

void GrandMetropolisObjects_Reset() {
	gmtimer = 0;
	pistonstate = 0;
	grand_metropolis_pistons[0].soipos.y = -1634;
	grand_metropolis_pistons[1].soipos.y = -1614;
	grand_metropolis_pistons[2].soipos.y = -1634;
	grand_metropolis_pistons[3].soipos.y = -1614;
	grand_metropolis_pistons[4].soipos.y = -1634;
	grand_metropolis_pistons[5].soipos.y = -1614;
	grand_metropolis_pistons[6].soipos.y = -1634;
	for (int i = 7; i < 18; ++i) {
		grand_metropolis_pistons[i].soipos.y = -3309.911f;
	}
	grand_metropolis_flying_objects[0].soipos = { 1729, -234.9f, -6126 };
	ResetCars();
}

void GrandMetropolisObjects_OnFrame(EntityData1 * entity) {
	AnimateUV(GrandMetropolis_UVShift, LengthOfArray(GrandMetropolis_UVShift));

	CarsHandler();
	PistonsHandler();
	AutoPathsMovs();

	if (anim % 3) texstate += 1;
	if (texstate > 31) texstate = 0;
	if (CurrentChunk == 1 || CurrentChunk == 2 || CurrentChunk == 3 || CurrentChunk == 9 || CurrentChunk == 10 || CurrentChunk == 11) {
		matlist_fluid[0].attr_texId = 224 + texstate;
	}
	matlist_energy[0].attr_texId = 192 + texstate;

	if (anim % 4 == 0) {
		if (entity != nullptr) {
			if (entity->Position.z > 3675) {
				CharObj2 * co2 = GetCharObj2(0);
				co2->AnimationThing.Index = 13;
				entity->Action = 2;
			}

			grand_metropolis_objects_common[0].soipos.x = entity->Position.x;
			grand_metropolis_objects_common[0].soipos.z = entity->Position.z;
			grand_metropolis_objects_common[1].soipos.x = entity->Position.x;
			grand_metropolis_objects_common[1].soipos.z = entity->Position.z;
		}
	}
}