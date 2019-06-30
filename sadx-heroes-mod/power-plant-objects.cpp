#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "power-plant-objects.h"

ModelInfo * PP_ELEVATR;
ModelInfo * PP_FLDPATH;
ModelInfo * PP_LGTSIGN;
ModelInfo * PP_MTRUCKS;
ModelInfo * PP_PLTFRMS;
ModelInfo * PP_PPCRANE;
ModelInfo * PP_SOLARPN;
ModelInfo * PP_TNKDOOR;
ModelInfo * PP_TNKSTEP;
ModelInfo * PP_SKYMDLS;

static bool alternate;

void PPSolarpnls(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}

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

void PPCranes(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}

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

void PPLights(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}

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

void PPTrucks(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = PPTrucks_Display;
		a1->Data1->Action = 1;
	}

	if (anim % 1800 == 0) a1->Data1->Scale = { 0, 0, 0 };
	else {
		a1->Data1->Scale.x += 3;
		a1->Data1->Scale.y += 4;
	}

	PPTrucks_Display(a1);
}

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

void PPEnergyPaths_Display(ObjectMaster *a1) {
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

void PPEnergyPaths_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.z < 1) if (IsSwitchPressed(a1->Data1->Scale.y)) a1->Data1->Scale.z += 0.1f;
		if (a1->Data1->Scale.z == 0.1f) PlaySound(52, 0, 0, 0);

		PathsFunction();

		PPEnergyPaths_Display(a1);
	}
}

void PPEnergyPaths(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 0)  a1->Data1->Object = PP_FLDPATH->getmodel();
	else if (a1->Data1->Scale.x == 1) a1->Data1->Object = PP_FLDPATH->getmodel()->child;
	else a1->Data1->Object = PP_FLDPATH->getmodel()->child->child;

	a1->MainSub = &PPEnergyPaths_Main;
	a1->DisplaySub = &PPEnergyPaths_Display;
	a1->DeleteSub = &deleteSub_Global;
}

void PPPlatformsV_Display(ObjectMaster *a1)
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

void PPPlatformsV_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.x != 0) {
			DynColRadius(a1, 300, 1);

			if (a1->Data1->Position.y < a1->Data1->Scale.y) {
				if (IsSwitchPressed(a1->Data1->Scale.x)) a1->Data1->Position.y += 1;
			}

			NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;
			if (col) col->pos[1] = a1->Data1->Position.y;
		}
		else {
			DynColRadius(a1, 300, 0);
		}

		PPPlatformsV_Display(a1);
	}
}

void PPPlatformsV(ObjectMaster *a1)
{
	a1->Data1->Object = PP_PLTFRMS->getmodel()->child;

	a1->MainSub = &PPPlatformsV_Main;
	a1->DisplaySub = &PPPlatformsV_Display;
	a1->DeleteSub = &deleteSub_Global;
}

void PPPlatformsH_Display(ObjectMaster *a1)
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

void PPPlatformsH_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 300, 1);
		NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;

		if (a1->Data1->Scale.y != 0) {
			if (a1->Data1->Position.x != a1->Data1->Scale.y) {
				if (IsSwitchPressed(a1->Data1->Action)) {
					if (a1->Data1->Scale.x < a1->Data1->Scale.y) {
						if (a1->Data1->Position.x < a1->Data1->Scale.y) a1->Data1->Position.x += 1;
					}
					else if (a1->Data1->Scale.x > a1->Data1->Scale.y) {
						if (a1->Data1->Position.x > a1->Data1->Scale.y) a1->Data1->Position.x -= 1;
					}
				}
			}

			if (col) col->pos[0] = a1->Data1->Position.x;
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
				}
			}

			if (col) col->pos[2] = a1->Data1->Position.z;
		}
		PPPlatformsH_Display(a1);
	}
}

void PPPlatformsH(ObjectMaster *a1)
{
	a1->Data1->Object = PP_PLTFRMS->getmodel();
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

void PPElevator_Display(ObjectMaster *a1)
{
	if (alternate) a1->Data1->Object->basicdxmodel->mats[7].diffuse.argb.a = 0x00;
	if (!alternate)  a1->Data1->Object->basicdxmodel->mats[7].diffuse.argb.a = 0xFF;
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);
}

void PPElevator_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 300, 1);

		if (anim % 120 == 0) alternate = !alternate;
		a1->DisplaySub(a1);

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

		if (IsPlayerInsideSphere(&a1->Data1->Position, 300) && a1->Data1->Position.y < a1->Data1->Scale.x)
			a1->Data1->Position.y += a1->Data1->Scale.y;

		if (a1->Data1->Position.y > a1->Data1->Scale.x) a1->Data1->Position.y = a1->Data1->Scale.x;

		NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;
		if (col) col->pos[1] = a1->Data1->Position.y;
	}
}

void PPElevator(ObjectMaster *a1)
{
	a1->Data1->Object = PP_ELEVATR->getmodel();

	a1->MainSub = &PPElevator_Main;
	a1->DisplaySub = &PPElevator_Display;
	a1->DeleteSub = &deleteSub_Global;
}

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
}

void PPStopper_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
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
}

void PPStopper(ObjectMaster *a1)
{
	a1->Data1->Object = PP_ELEVATR->getmodel()->child;
	a1->Data1->Action = 0;

	a1->MainSub = &PPStopper_Main;
	a1->DisplaySub = &PPStopper_Display;
	a1->DeleteSub = &PPStopper_Delete;
}

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
	{ LoadObj_Data1, ObjIndex_Common, DistObj_UseDist, 40000, 0, Bubble_Main, "O BUBBLE" },
	{ LoadObj_Data1, ObjIndex_4, DistObj_UseDist, 40000, 0, Bubbles_Main, "O BUBBLES" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Kiki_Load, "E SARU  " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, RhinoTank_Main, "E SAITO" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Sweep_Load, "E AMEMB" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, OBJCASE, "OBJ CASE" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OJump, "O JUMP   " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1640000, 0, PPPlatformsH, "PP Plat H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, PPPlatformsV, "PP Plat V" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, PPEnergyPaths, "PP PATHS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO_WOODBOX" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, OFrog, "O FROG " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, ItemBoxAir_Main, "O ItemBoxAir" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerA_Main, "SPINA A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerB_Main, "SPINA B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerC_Main, "SPINA C" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, SpringH_Load, "O SPRING H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" },
	{ LoadObj_Data1, 3, 1, 1000000, 0, PPElevator, "PPElevator" },
	{ LoadObj_Data1, 3, 1, 1000000, 0, ObjReel, "Reel" },
	{ LoadObj_Data1, 3, 1, 1000000, 0, PPStopper, "PPSTOPPER" }
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