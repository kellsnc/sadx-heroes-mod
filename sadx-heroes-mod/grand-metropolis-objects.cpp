#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "grand-metropolis-objects.h"

ModelInfo * GM_ADVERTS;
ModelInfo * GM_FLYCARS;
ModelInfo * GM_GPISTON;
ModelInfo * GM_GRFLUID;
ModelInfo * GM_GRPLANE;
ModelInfo * GM_MCLOUDS;
ModelInfo * GM_ZEPPLIN;

#pragma region Sky
void GMSky_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslate(nullptr, 0, 300, 0);
		njRotateXYZ(nullptr, 0, 0, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -7000;
		njDrawModel_SADX(GM_MCLOUDS->getmodel()->child->basicdxmodel);

		njTranslate(nullptr, 0, 300, 0);
		DrawQueueDepthBias = -8000;
		njDrawModel_SADX(GM_MCLOUDS->getmodel()->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GMSky(ObjectMaster *a1) {
	a1->DisplaySub = GMSky_Display;
	a1->MainSub = GMSky_Display;
}
#pragma endregion

#pragma region Pistons
void GMPistons_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(GM_Pistons); ++i) {
			if (CheckModelDisplay(GM_Pistons[i])) {
				SOI_LIST item = GM_Pistons[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, 1, 1, 1);
				DrawQueueDepthBias = -6000;

				njDrawModel_SADX(GM_GPISTON->getmodel()->basicdxmodel);
				njDrawModel_SADX(GM_GPISTON->getmodel()->child->child->basicdxmodel);

				if (i % 2 == 0) 
					njTranslate(0, 0, a1->Data1->Scale.x, 0);
				else {
					njTranslate(0, 0, 24, 0);
					njTranslate(0, 0, -a1->Data1->Scale.x, 0);
				}
				
				njDrawModel_SADX(GM_GPISTON->getmodel()->child->basicdxmodel);

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void GMPistons_Main(ObjectMaster *a1) {
	++a1->Data1->InvulnerableTime;

	if (a1->Data1->NextAction == 0) a1->Data1->Scale.x = 0;
	if (a1->Data1->InvulnerableTime == 400) a1->Data1->InvulnerableTime = 0;
	if (a1->Data1->InvulnerableTime < 200) a1->Data1->NextAction = 1;
	if (a1->Data1->InvulnerableTime > 199) a1->Data1->NextAction = 2;
	if (a1->Data1->NextAction == 1) a1->Data1->Scale.x += 0.12f;
	if (a1->Data1->NextAction == 2) a1->Data1->Scale.x -= 0.12f;

	GMPistons_Display(a1);
}

void GMPistons(ObjectMaster *a1) {
	a1->DisplaySub = GMPistons_Display;
	a1->MainSub = GMPistons_Main;
}
#pragma endregion

#pragma region Ads
void GMAds_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(GM_Ads); ++i) {
			if (CheckModelDisplay(GM_Ads[i])) {
				SOI_LIST item = GM_Ads[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, 1, 1, 1);
				DrawQueueDepthBias = -5000;
				njDrawModel_SADX(GM_ADVERTS->getmodel()->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void GMAds(ObjectMaster *a1) {
	a1->DisplaySub = GMAds_Display;
	a1->MainSub = GMAds_Display;
}
#pragma endregion

#pragma region Cars
void GMCars_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(GM_FlyingObjs); ++i) {
			if (CheckModelDisplay2(GM_FlyingObjs[i])) {
				SOI_LIST2 item = GM_FlyingObjs[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, 1, 1, 1);
				DrawQueueDepthBias = -6000;

				if (item.Model == 0) njTranslate(0, a1->Data1->Scale.x, 0, 0);
				else if (item.Model < 3) njTranslate(0, a1->Data1->Scale.y, 0, 0);
				else njTranslate(0, a1->Data1->Scale.z, 0, 0);

				switch (item.Model) {
				case 0: njDrawModel_SADX(GM_ZEPPLIN->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(GM_FLYCARS->getmodel()->basicdxmodel); break;
				case 2: njDrawModel_SADX(GM_FLYCARS->getmodel()->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(GM_FLYCARS->getmodel()->child->child->basicdxmodel); break;
				case 4: njDrawModel_SADX(GM_FLYCARS->getmodel()->child->child->child->basicdxmodel); break;
				case 5: njDrawModel_SADX(GM_FLYCARS->getmodel()->child->child->child->child->basicdxmodel); break;
				case 6: njRotateXYZ(0, 0, 0x4000, 0); njDrawModel_SADX(GM_GRPLANE->getmodel()->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void GMCars_Main(ObjectMaster *a1) {
	if (anim % 1800 == 0) a1->Data1->Scale = { 0, 0, 0 };
	else {
		a1->Data1->Scale.x += 0.5f;
		a1->Data1->Scale.y += 5.0f;
		a1->Data1->Scale.z += 10.0f;
	}

	GMCars_Display(a1);
}

void GMCars(ObjectMaster *a1) {
	a1->DisplaySub = GMCars_Display;
	a1->MainSub = GMCars_Main;
}
#pragma endregion

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
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, a1->Data1->Scale.z, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GM_ENERGYPATHS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (a1->Data1->Scale.z <= 1 && IsSwitchPressed(a1->Data1->Scale.y)) a1->Data1->Scale.z += 0.1f;
		if (a1->Data1->Scale.z == 0.1f) {
			PlaySound(52, 0, 0, 0);
			a1->Data1->Object->pos[1] -= 10000;
		}

		GM_ENERGYPATHS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl GM_ENERGYPATHS(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1)  a1->Data1->Object = GM_GRFLUID->getmodel()->child;
	else a1->Data1->Object = GM_GRFLUID->getmodel();
	
	AddToCollision(a1, 4);
	a1->Data1->Object->pos[1] += 10000;

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

void AutoPathsMovs() {
	EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
	for (uint8_t slot = 0; slot < 8; ++slot) {
		if (players[slot]) {
			CharObj2 *co2 = GetCharObj2(slot);
			if (co2) {
				if (co2->SurfaceFlags == 0x8a1 || co2->SurfaceFlags == 0xA81 || 
					co2->SurfaceFlags == 0x18000001 || co2->SurfaceFlags == 0x08000001) {
					if (IsLantern) {
						set_diffuse_blend_ptr(2, 4);
						set_diffuse_blend_factor_ptr(0.3f);
					}

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
					if (anim % 4 == true) set_diffuse_blend_factor_ptr(0);
				}
			}
		}
	}
}