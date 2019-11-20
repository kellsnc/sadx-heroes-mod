#include "stdafx.h"
#include "grand-metropolis-objects.h"

ModelInfo * GM_ADVERTS;
ModelInfo * GM_FLYCARS;
ModelInfo * GM_GPISTON;
ModelInfo * GM_GRFLUID;
ModelInfo * GM_GRPLANE;
ModelInfo * GM_MCLOUDS;
ModelInfo * GM_ZEPPLIN;

void GMSky(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			a1->Data1->Action = 1;
		}

		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslate(nullptr, EntityData1Ptrs[0]->Position.x, 300, EntityData1Ptrs[0]->Position.z);
		DrawQueueDepthBias = -7000;
		njDrawModel_SADX(GM_MCLOUDS->getmodel()->child->basicdxmodel);

		njTranslate(nullptr, 0, 300, 0);
		DrawQueueDepthBias = -8000;
		njDrawModel_SADX(GM_MCLOUDS->getmodel()->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GMPistons_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(GM_Pistons); ++i) {
			if (CheckModelDisplay(GM_Pistons[i])) {
				SOI_LIST item = GM_Pistons[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -6000;

				njDrawModel_SADX(GM_GPISTON->getmodel()->basicdxmodel);
				njDrawModel_SADX(GM_GPISTON->getmodel()->child->child->basicdxmodel);

				if (i % 2 == 0) 
					njTranslate(0, 0, a1->Data1->Scale.x, 0);
				else {
					njTranslate(0, 0, 24, 0);
					njTranslate(0, 0, -a1->Data1->Scale.y, 0);
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
	++a1->Data1->Index;

	if (a1->Data1->InvulnerableTime == 125) a1->Data1->InvulnerableTime = 0;
	if (a1->Data1->InvulnerableTime < 100) a1->Data1->NextAction = 1;
	if (a1->Data1->InvulnerableTime > 99) a1->Data1->NextAction = 2;
	if (a1->Data1->NextAction == 1) a1->Data1->Scale.x += 0.24f;
	if (a1->Data1->NextAction == 2) a1->Data1->Scale.x -= 0.96f;

	if (a1->Data1->Index == 125) a1->Data1->Index = 0;
	if (a1->Data1->Index < 25) a1->Data1->CharIndex = 1;
	if (a1->Data1->Index > 24) a1->Data1->CharIndex = 2;
	if (a1->Data1->CharIndex == 1) a1->Data1->Scale.y += 0.96f;
	if (a1->Data1->CharIndex == 2) a1->Data1->Scale.y -= 0.24f;

	GMPistons_Display(a1);
}

void GMPistons(ObjectMaster *a1) {
	a1->Data1->Scale.x = 0;
	a1->Data1->Index = 0;

	a1->DisplaySub = GMPistons_Display;
	a1->MainSub = GMPistons_Main;
}

void GMAds(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (a1->Data1->Action == 0) {
			a1->DisplaySub = a1->MainSub;
			a1->Data1->Action = 1;
		}

		for (int i = 0; i < LengthOfArray(GM_Ads); ++i) {
			if (CheckModelDisplay(GM_Ads[i])) {
				SOI_LIST item = GM_Ads[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -5000;
				njDrawModel_SADX(GM_ADVERTS->getmodel()->basicdxmodel);
				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void GMCars_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(GM_FlyingObjs); ++i) {
			if (CheckModelDisplay2(GM_FlyingObjs[i])) {
				SOI_LIST2 item = GM_FlyingObjs[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
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

void GMCars(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = GMCars_Display;
		a1->Data1->Action = 1;
	}

	if (anim % 1800 == 0) a1->Data1->Scale = { 0, 0, 0 };
	else {
		a1->Data1->Scale.x += 0.5f;
		a1->Data1->Scale.y += 5.0f;
		a1->Data1->Scale.z += 10.0f;
	}

	a1->DisplaySub(a1);
}

void GMEnergyH_Display(ObjectMaster *a1)
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

void GMEnergyH(ObjectMaster *a1)
{
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = GMEnergyH_Display;
		a1->Data1->Action = 1;
	}

	if (!ClipSetObject(a1)) {
		if (a1->Data1->Scale.x == 0) a1->Data1->Scale.y = 180;
		else {
			if (IsSwitchPressed(a1->Data1->Scale.x)) {
				if (a1->Data1->Scale.y < 180) a1->Data1->Scale.y += 5;
			}
		}
		a1->DisplaySub(a1);
	}
}

void GM_EnergyDoors_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		if (a1->Data1->Rotation.x == 1) njDrawModel_SADX(s03_door_2obj.basicdxmodel);
		else njDrawModel_SADX(s03_door_1obj.basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void GM_EnergyDoors_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action != 0) {
			DynColRadius(a1, 50, 1);

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

			NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;

			if (col) {
				col->pos[0] = a1->Data1->Position.x;
				col->pos[2] = a1->Data1->Position.z;
			}
		}
		else {
			DynColRadius(a1, 50, 0);
		}

		a1->DisplaySub(a1);
	}
}

void GM_EnergyDoors(ObjectMaster *a1)
{
	if (a1->Data1->Rotation.x == 1) a1->Data1->Object = &s03_door_2obj;
	else a1->Data1->Object = &s03_door_1obj;

	a1->Data1->Action = a1->Data1->Scale.x; //Action = switch id

	if (a1->Data1->Action != 0) {
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

	a1->MainSub = &GM_EnergyDoors_Main;
	a1->DisplaySub = &GM_EnergyDoors_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void GM_EnergyPaths_Display(ObjectMaster *a1) {
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

void GM_EnergyPaths_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (IsSwitchPressed(a1->Data1->Scale.y)) {
			DynColRadius(a1, 800, 4);

			if (a1->Data1->Scale.z <= 1)
				a1->Data1->Scale.z += 0.1f;
			else if (a1->Data1->Scale.z == 0.1f)
				PlaySound(52, 0, 0, 0);
		}

		a1->DisplaySub(a1);
	}
}

void GM_EnergyPaths(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1)  a1->Data1->Object = GM_GRFLUID->getmodel()->child;
	else a1->Data1->Object = GM_GRFLUID->getmodel();

	a1->MainSub = &GM_EnergyPaths_Main;
	a1->DisplaySub = &GM_EnergyPaths_Display;
	a1->DeleteSub = &DynCol_Delete;
}

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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjReel, "OBJREEL" },
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1860000, 0, GM_EnergyPaths, "GM PATHS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2360000, 0, GM_EnergyDoors, "GM DOORS" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2360000, 0, GMEnergyH, "GM ENER" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2060000, 0, ObjBoxW, "CO_WOODBOX" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OKamome, "O KAMOME   " },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OKamomel, "O KAMOMEL   " },
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" }
};
ObjectList GrandMetropolisObjectList = { arraylengthandptrT(GrandMetropolisObjectList_list, int) };

void GrandMetropolisObjects_Init(const char *path) {
	TexLists_Obj[HeroesLevelID_GrandMetropolis] = GrandMetropolisObjectTextures;
	ObjLists[HeroesLevelID_GrandMetropolis * 8] = &GrandMetropolisObjectList;
}

void AutoPathsMovs() {
	EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
	for (uint8_t slot = 0; slot < 8; ++slot) {
		if (players[slot]) {
			CharObj2 *co2 = CharObj2Ptrs[slot];
			if (co2) {
				if (co2->SurfaceFlags == 0x8a1 || co2->SurfaceFlags == 0xA81 || 
					co2->SurfaceFlags == 0x18000001 || co2->SurfaceFlags == 0x08000001) {
					if (IsLantern) {
						set_diffuse_blend_ptr(2, 4);
						set_diffuse_blend_factor_ptr(0.3f);
					}

					EntityData1 *entity = EntityData1Ptrs[slot];
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