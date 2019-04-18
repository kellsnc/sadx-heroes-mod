#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "power-plant-objects.h"

ModelInfo * PP_ELEVATR;
ModelInfo * PP_FLDPATH;
ModelInfo * PP_LGTSIGN;
ModelInfo * PP_MCLOUDS;
ModelInfo * PP_MTRUCKS;
ModelInfo * PP_PLTFRMS;
ModelInfo * PP_PPCRANE;
ModelInfo * PP_SOLARPN;
ModelInfo * PP_TNKDOOR;
ModelInfo * PP_TNKSTEP;

static bool alternate;

#pragma region Solar panels
void PPSolarpnls_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(PP_Panels); ++i) {
			if (CheckModelDisplay(PP_Panels[i])) {
				SOI_LIST item = PP_Panels[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				if (GameState != 16) a1->Data1->Scale.x += 100;
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -6000;
				njDrawModel_SADX(PP_SOLARPN->getmodel()->basicdxmodel);
				njRotateX(0, a1->Data1->Scale.x);
				njDrawModel_SADX(PP_SOLARPN->getmodel()->child->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void PPSolarpnls(ObjectMaster *a1) {
	a1->DisplaySub = PPSolarpnls_Display;
	a1->MainSub = PPSolarpnls_Display;
}
#pragma endregion

#pragma region Cranes
void PPCranes_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(PP_Cranes); ++i) {
			if (CheckModelDisplay(PP_Cranes[i])) {
				SOI_LIST item = PP_Cranes[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);

				if (GameState != 16) {
					if (anim % 1000 == 0)
						a1->Data1->Scale.y += 0.5f;

					if ((int)a1->Data1->Scale.y % 2 == 0) {
						a1->Data1->Scale.x += 20;
					}
					else {
						a1->Data1->Scale.x -= 20;
					}
				}

				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1] + a1->Data1->Scale.x, item.Rotation[2]);
				DrawQueueDepthBias = -6000;
				njDrawModel_SADX(PP_PPCRANE->getmodel()->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void PPCranes(ObjectMaster *a1) {
	a1->DisplaySub = PPCranes_Display;
	a1->MainSub = PPCranes_Display;
}
#pragma endregion

#pragma region Light signs
void PPLights_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(PP_Lights); ++i) {
			if (CheckModelDisplay(PP_Lights[i])) {
				SOI_LIST item = PP_Lights[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				if (GameState != 16) a1->Data1->Scale.x += 100;
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -6000;
				njDrawModel_SADX(PP_LGTSIGN->getmodel()->basicdxmodel);
				njRotateZ(0, a1->Data1->Scale.x);
				njDrawModel_SADX(PP_LGTSIGN->getmodel()->child->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void PPLights(ObjectMaster *a1) {
	a1->DisplaySub = PPLights_Display;
	a1->MainSub = PPLights_Display;
}
#pragma endregion

#pragma region Sky
void PPSky_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslate(nullptr, EntityData1Ptrs[0]->Position.x, 0, EntityData1Ptrs[0]->Position.z);
		DrawQueueDepthBias = -7000;
		njDrawModel_SADX(PP_MCLOUDS->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void PPSky(ObjectMaster *a1) {
	a1->DisplaySub = PPSky_Display;
	a1->MainSub = PPSky_Display;
}
#pragma endregion

#pragma region TankHandler
void PPTankHandler_Display(ObjectMaster *a1) {
	if (CurrentChunk == 11) {
		if (!DroppedFrames) {
			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslate(nullptr, a1->Data1->Position.x, 10979.9f, -12388);

			DrawQueueDepthBias = -6000;
			njDrawModel_SADX(PP_TNKDOOR->getmodel()->basicdxmodel);

			njPopMatrix(1u);
			njPushMatrix(0);

			njTranslate(nullptr, a1->Data1->Position.y, 10979.9f, -12388);
			njDrawModel_SADX(PP_TNKDOOR->getmodel()->child->basicdxmodel);

			njPopMatrix(1u);

			for (int i = 0; i < LengthOfArray(PP_Steps); ++i) {
				if (CheckModelDisplay2(PP_Steps[i])) {
					SOI_LIST2 item = PP_Steps[i];

					njPushMatrix(0);
					njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
					njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
					

					switch (item.Model) {
					case 0: njDrawModel_SADX(PP_TNKSTEP->getmodel()->basicdxmodel); break;
					case 1: njDrawModel_SADX(PP_TNKSTEP->getmodel()->child->basicdxmodel); break;
					}

					njPopMatrix(1u);
				}
			}

			DrawQueueDepthBias = 0;
		}
	}
}

void PPTankHandler_Main(ObjectMaster *a1) {
	if (CurrentChunk == 11) {
		EntityData1 *entity = EntityData1Ptrs[0];
		if (entity->Position.x > 22754) {
			if (CurrentLandTable->Col[20].Model->pos[1] < 10723) {
				CurrentLandTable->Col[20].Model->pos[1] += 0.3f;
				if (entity->Position.y > 9517) CurrentLandTable->Col[20].Model->pos[1] += 0.3f;
				if (entity->Position.y > 10207) CurrentLandTable->Col[20].Model->pos[1] += 0.1f;
			}
			if (entity->Position.y < CurrentLandTable->Col[20].Model->pos[1]) GameState = 7;
		}
		else {
			CurrentLandTable->Col[20].Model->pos[1] = 8621;
		}
	
		if (entity->Position.y > 10979) {
			if (a1->Data1->Position.x != 22930) {
				a1->Data1->Position.x -= 1;
			}
			if (a1->Data1->Position.y != 22930) {
				a1->Data1->Position.y += 1;
			}
		}

		PPTankHandler_Display(a1);
	}
}

void PPTankHandler(ObjectMaster *a1) {
	a1->Data1->Scale = { 22930.4f, 11011.5f, -12387.49f };
	a1->Data1->Position = { 22979, 22879, 0 };

	a1->DisplaySub = PPTankHandler_Display;
	a1->MainSub = PPTankHandler_Main;
}
#pragma endregion

#pragma region Cars
void PPTrucks_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(PP_Trucks); ++i) {
			if (CheckModelDisplay2(PP_Trucks[i])) {
				SOI_LIST2 item = PP_Trucks[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -6000;

				if (i == 0 || i % 2 == 0) njTranslate(0, a1->Data1->Scale.x, 0, 0);
				else njTranslate(0, a1->Data1->Scale.y, 0, 0);

				switch (item.Model) {
				case 0: njDrawModel_SADX(PP_MTRUCKS->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(PP_MTRUCKS->getmodel()->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void PPTrucks_Main(ObjectMaster *a1) {
	if (anim % 1800 == 0) a1->Data1->Scale = { 0, 0, 0 };
	else {
		a1->Data1->Scale.x += 3;
		a1->Data1->Scale.y += 4;
	}

	PPTrucks_Display(a1);
}

void PPTrucks(ObjectMaster *a1) {
	a1->DisplaySub = PPTrucks_Display;
	a1->MainSub = PPTrucks_Main;
}
#pragma endregion

#pragma region Energy Paths
void PathsFunction() {
	if (anim % 5 == 0) {
		if (CurrentChunk == 2 && (IsSwitchPressed(2) || GetCharacterID(0) == 2)) CurrentLandTable->Col[17].Flags = 0xA81;

		if (IsSwitchPressed(6) || GetCharacterID(0) == 2) {
			if (CurrentChunk == 5) CurrentLandTable->Col[14].Flags = 0xA81;
			else if (CurrentChunk == 6) CurrentLandTable->Col[7].Flags = 0xA81;
		}

		if (IsSwitchPressed(43)) {
			if (CurrentChunk == 9) CurrentLandTable->Col[13].Flags = 0xA81;
			else if (CurrentChunk == 10) CurrentLandTable->Col[22].Flags = 0xA81;
		}

		EntityData1 ** players = EntityData1Ptrs;
		for (uint8_t slot = 0; slot < 8; ++slot) {
			if (players[slot]) {
				EntityData1 *entity = EntityData1Ptrs[slot];
				bool elevate = false;
				if (CurrentChunk == 2) {
					if (IsSwitchPressed(41) && IsPlayerInBox(entity->Position, { 4819.545f, 1334.25f, -2807.884f }, { 4920.795f, 1599.75f, -2750.509f })) elevate = true;
					if (IsSwitchPressed(1) && IsPlayerInBox(entity->Position, { 4922.578f, 1746.016f, -3249.482f }, { 4969.469f, 1998.652f, -3149.415f })) elevate = true;
					if ((IsSwitchPressed(2) || GetCharacterID(0) == 2) && IsPlayerInBox(entity->Position, { 5881.667f, 2366.625f, -3249.78f }, { 5914.292f, 2619.75f, -3150.78f })) elevate = true;
					if (IsSwitchPressed(3) && IsPlayerInBox(entity->Position, { 7105.375f, 3070.897f, -3249.61f }, { 7149, 3319.522F, -3150.61F })) elevate = true;
				}
				else if (CurrentChunk == 8) {
					if (IsSwitchPressed(42) && IsPlayerInBox(entity->Position, { 15416.52f, 5784.375f, -13523.97f }, { 15516.36f, 6038.875f, -13492.27f })) elevate = true;
					if (IsSwitchPressed(12) && IsPlayerInBox(entity->Position, { 15518.6f, 6186.64f, -13974.12f }, { 15566.68f, 6438.71f, -13874.49f })) elevate = true;
				}
				else if (CurrentChunk == 11 && IsPlayerInBox(entity->Position, { 22186.03f, 8432.125f, -12439.87f }, { 22232.81f, 8689.5f, -12340.5f })) elevate = true;

				if (elevate) {
					if (IsLantern) {
						set_blend_ptr(2, 4);
						set_diffuse_blend_factor_ptr(0.6f);
						set_specular_blend_factor_ptr(0.4f);
					}
					ElevatePlayer(slot);
				}
				else {
					if (anim % 60 == true) {
						if (IsLantern) {
							set_diffuse_blend_factor_ptr(0);
							set_specular_blend_factor_ptr(0);
						}
						elevate = false;
					}
				}
			}
		}
	}
}

void PP_ENERGYPATHS_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
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

void PP_ENERGYPATHS_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		if (a1->Data1->Scale.z < 1) if (IsSwitchPressed(a1->Data1->Scale.y)) a1->Data1->Scale.z += 0.1f;
		if (a1->Data1->Scale.z == 0.1f) PlaySound(52, 0, 0, 0);

		PathsFunction();

		PP_ENERGYPATHS_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl PP_ENERGYPATHS(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 0)  a1->Data1->Object = PP_FLDPATH->getmodel();
	else if (a1->Data1->Scale.x == 1) a1->Data1->Object = PP_FLDPATH->getmodel()->child;
	else a1->Data1->Object = PP_FLDPATH->getmodel()->child->child;

	a1->MainSub = &PP_ENERGYPATHS_Main;
	a1->DisplaySub = &PP_ENERGYPATHS_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Platforms_V
void __cdecl PPPlatformsV_Display(ObjectMaster *a1)
{
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void __cdecl PPPlatformsV_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {

		if (a1->Data1->Scale.x != 0) {
			if (a1->Data1->Position.y < a1->Data1->Scale.y) {
				if (IsSwitchPressed(a1->Data1->Scale.x)) {
					a1->Data1->Position.y += 1;
					a1->Data1->Object->pos[1] = a1->Data1->Position.y;
				}
			}
		}

		PPPlatformsV_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl PPPlatformsV(ObjectMaster *a1)
{
	a1->Data1->Object = PP_PLTFRMS->getmodel()->child;
	if (a1->Data1->Scale.x == 0) AddToCollision(a1, 0);
	else AddToCollision(a1, 1);

	a1->MainSub = &PPPlatformsV_Main;
	a1->DisplaySub = &PPPlatformsV_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Platforms_H
void __cdecl PPPlatformsH_Display(ObjectMaster *a1)
{
	if (!DroppedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void __cdecl PPPlatformsH_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (a1->Data1->Scale.y != 0) {
			if (a1->Data1->Position.x != a1->Data1->Scale.y) {
				if (IsSwitchPressed(a1->Data1->Action)) {
					if (a1->Data1->Scale.x < a1->Data1->Scale.y) {
						if (a1->Data1->Position.x < a1->Data1->Scale.y) a1->Data1->Position.x += 1;
					}
					else if (a1->Data1->Scale.x > a1->Data1->Scale.y) {
						if (a1->Data1->Position.x > a1->Data1->Scale.y) a1->Data1->Position.x -= 1;
					}
					a1->Data1->Object->pos[0] = a1->Data1->Position.x;
				}
			}
		}
		else {
			if (a1->Data1->Position.z != a1->Data1->Scale.z) {
				if (IsSwitchPressed(a1->Data1->Action)) {
					if (a1->Data1->Scale.x < a1->Data1->Scale.z) {
						if (a1->Data1->Position.z < a1->Data1->Scale.z) a1->Data1->Position.z += 1;
					}
					else if (a1->Data1->Scale.x > a1->Data1->Scale.z) {
						if (a1->Data1->Position.z > a1->Data1->Scale.z) a1->Data1->Position.z -= 1;
					}
					a1->Data1->Object->pos[2] = a1->Data1->Position.z;
				}
			}
		}
		PPPlatformsH_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl PPPlatformsH(ObjectMaster *a1)
{
	a1->Data1->Object = PP_PLTFRMS->getmodel();
	AddToCollision(a1, 1);

	a1->Data1->Action = a1->Data1->Scale.x;

	if (a1->Data1->Scale.y != 0) {
		a1->Data1->Scale.x = a1->Data1->Position.x;
	}
	else {
		a1->Data1->Scale.x = a1->Data1->Position.z;
	}

	a1->MainSub = &PPPlatformsH_Main;
	a1->DisplaySub = &PPPlatformsH_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Elevator
void PPElevator_Display(ObjectMaster *a1)
{
	if (alternate) a1->Data1->Object->basicdxmodel->mats[7].diffuse.argb.a = 0x00;
	if (!alternate)  a1->Data1->Object->basicdxmodel->mats[7].diffuse.argb.a = 0xFF;
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);
}

void PPElevator_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		if (anim % 120 == 0) alternate = !alternate;
		PPElevator_Display(a1);

		for (uint16_t i = 0; i < SETTable_Count; ++i) {
			if (CurrentSetFile[i].ObjectType == 95
				&& CurrentSetFile[i].Properties.x != 1
				&& CurrentSetFile[i].Position.z == a1->Data1->Position.z
				&& CurrentSetFile[i].Position.x == a1->Data1->Position.x
				&& CurrentSetFile[i].Position.y < a1->Data1->Position.y
				&& CurrentSetFile[i].Position.y >= a1->Data1->Position.y - 1) {
				return;
			}
		}

		if (IsPlayerInsideSphere(&a1->Data1->Position, 300) && a1->Data1->Position.y < a1->Data1->Scale.x) {
			a1->Data1->Position.y += a1->Data1->Scale.y;
			a1->Data1->Object->pos[1] = a1->Data1->Position.y;
		}

		if (a1->Data1->Position.y > a1->Data1->Scale.x) a1->Data1->Position.y = a1->Data1->Scale.x;
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl PPElevator(ObjectMaster *a1)
{
	a1->Data1->Object = PP_ELEVATR->getmodel();
	AddToCollision(a1, 1);

	a1->MainSub = &PPElevator_Main;
	a1->DisplaySub = &PPElevator_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Elevator Stopper
void PPStopper_Display(ObjectMaster *a1)
{
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);
	if (alternate) a1->Data1->Object->child->basicdxmodel->mats[1].diffuse.argb.a = 0x00;
	if (!alternate) a1->Data1->Object->child->basicdxmodel->mats[1].diffuse.argb.a = 0xFF;
	DrawObjModel(a1, a1->Data1->Object->child->basicdxmodel, false);
}

void PPStopper_Delete(ObjectMaster *a1)
{
	a1->SETData.SETData->SETEntry->Properties.x = 0;
	deleteSub_Global(a1);
}

void PPStopper_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1000.0f)) {
		if (a1->Data1->Action == 0) {
			PPStopper_Display(a1);

			//check if the player is blocked
			EntityData1 *entity = EntityData1Ptrs[0];
			if (entity->Position.y > a1->Data1->Position.y - 20) {
				if (killcount >= a1->Data1->Scale.y) a1->Data1->Action = 1;
			}
		}
		else if (a1->Data1->Action == 1) {
			a1->SETData.SETData->SETEntry->Properties.x = 1;
			killcount = 0;
			a1->Data1->Action = 2;
		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl PPStopper(ObjectMaster *a1)
{
	a1->Data1->Object = PP_ELEVATR->getmodel()->child;
	a1->Data1->Action = 0;

	a1->MainSub = &PPStopper_Main;
	a1->DisplaySub = &PPStopper_Display;
	a1->DeleteSub = &PPStopper_Delete;
}
#pragma endregion

PVMEntry PowerPlantObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "UNAGI", &UNAGI_TEXLIST },
	{ "PEN", &PEN_TEXLIST },
	{ "SAKE", &SAKE_TEXLIST },
	{ "RAKO", &RAKO_TEXLIST },
	{ "same", &SAME_TEXLIST },
	{ "GOMA", &GOMA_TEXLIST },
	{ "GORI", &GORI_TEXLIST },
	{ "MECHA", &MECHA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "sea_bass", &sea_bass_TEXLIST },
	{ "hammer", &HAMMER_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ 0 }
};

ObjectListEntry PowerPlantObjectList_list[] = {
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
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)0x79B2F0, "O_GRING" } /* "O_GRING" */,
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
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500F50, "YASI0   " } /* "YASI0   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500FF0, "YASI1   " } /* "YASI1   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x501090, "YASI2   " } /* "YASI2   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500EC0, "YASI3   " } /* "YASI3   " */,
{ 6, 3, 1, 1360000, 0, &ObjBalloon, "SH BALLOON" } /* "SH Balloon" */,
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
{ 2, 3, 1, 360000, 0, nullptr, "O GRASB   " } /* "O GRASB   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD40, "O GRASC   " } /* "O GRASC   " */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4FFD60, "O GRASD   " } /* "O GRASD   " */,
{ 10, 3, 1, 1640000, 0, &PPPlatformsH, "PP Plat H" } /* "PP Horizontal platforms" */,
{ 2, 3, 1, 1000000, 0, &PPPlatformsV, "PP Plat V" } /* "PP Vertical platforms" */,
{ 11, 3, 1, 360000, 0, &PP_ENERGYPATHS, "PP PATHS" } /* "PP Energy Paths" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
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
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&PPElevator, "PPElevator" } /* "PPElevator" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjReel, "Reel" } /* "Heroes reels" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&PPStopper, "PPSTOPPER" } /* "PPElevator stopper" */
};
ObjectList PowerPlantObjectList = { arraylengthandptrT(PowerPlantObjectList_list, int) };

void PowerPlantObjects_Init(const char *path) {
	TexLists_Obj[HeroesLevelID_PowerPlant] = PowerPlantObjectTextures;
	ObjLists[HeroesLevelID_PowerPlant * 8] = &PowerPlantObjectList;
}

void PPPathsHandler() {
	EntityData1 ** players = EntityData1Ptrs;
	for (uint8_t slot = 0; slot < 8; ++slot) {
		if (players[slot]) {
			EntityData1 *entity = EntityData1Ptrs[slot];
			CharObj2 *co2 = CharObj2Ptrs[slot];

			if (co2) {
				if (co2->SurfaceFlags == 0x8A1 || co2->SurfaceFlags == 0xA81) {

					if (IsLantern) {
						set_diffuse_blend_ptr(2, 4);
						set_diffuse_blend_factor_ptr(0.3f);
					}

					if (entity->Position.x < 2404 && entity->Position.z > -1274) entity->Position.z -= 2;
					else if (entity->Position.x < 3494 && entity->Position.x > 3132) entity->Position.x += 2;
					else if (entity->Position.x < 4955 && entity->Position.z > -2014 && entity->Position.z < -1446 && entity->Position.x > 4788) entity->Position.z -= 2;
					else if (entity->Position.x < 5828) entity->Position.x += 2;
					else if (entity->Position.x < 9033 && entity->Position.x > 8466 && entity->Position.z > -4164) entity->Position.x += 2;
					else if (entity->Position.x < 8794 && entity->Position.y > 3256 && entity->Position.z < -4077) { entity->Position.z += 2; entity->Position.x += 1; }
					else if (entity->Position.x < 12681 && entity->Position.x > 12184 && entity->Position.z < -8861) entity->Position.z -= 2;
					else if (entity->Position.x < 14922 && entity->Position.z > -10897 && entity->Position.z < -10343 && entity->Position.x > 14743) entity->Position.z -= 2;
					else if (entity->Position.x < 14863 && entity->Position.z < -11313) entity->Position.z += 2;
					else if (entity->Position.x > 14561 && entity->Position.x < 15522) entity->Position.z -= 2;
					else if (entity->Position.x > 19094) entity->Position.x += 2;
				}
				else {
					if (anim % 4 == true && IsLantern) set_diffuse_blend_factor_ptr(0);
				}
			}

			bool elevate = false;

			if (CurrentChunk == 3 && IsPlayerInBox(entity->Position, { 8515.642f, 2990, -5468.255f }, { 8549.026f, 3298.125f, -5421.717f })) elevate = true;
			if (CurrentChunk == 7 && IsPlayerInBox(entity->Position, { 14766.78f, 4832.125f, -11680.85f }, { 14865.47f, 5138.778f, -11637.85f })) elevate = true;
			if (CurrentChunk == 4) {
				if (IsPlayerInBox(entity->Position, { 10179.34f, 3300, -6031.539f }, { 10209.77f, 3662.876f, -5980.835f })) elevate = true;
				else if (IsPlayerInBox(entity->Position, { 10198.59f, 3300, -6003.414f }, { 10239, 3662.876f, -5952.71f })) elevate = true;
				else if (IsPlayerInBox(entity->Position, { 10210.96f, 3300, -5975.289f }, { 10251.39f, 3662.876f, -5924.585f })) elevate = true;
			}

			if (elevate) {
				if (IsLantern) {
					set_blend_ptr(2, 4);
					set_diffuse_blend_factor_ptr(0.6f);
					set_specular_blend_factor_ptr(0.4f);
				}
				ElevatePlayer(slot);
			}
			else {
				if (anim % 60 == true) {
					if (IsLantern) {
						set_diffuse_blend_factor_ptr(0);
						set_specular_blend_factor_ptr(0);
					}
					elevate = false;
				}
			}
		}
	}
}