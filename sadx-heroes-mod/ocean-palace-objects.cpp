#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "ocean-palace-objects.h"

ModelInfo * OP_WATERFS;
ModelInfo * OP_FLOWERS;
ModelInfo * OP_TURFINS;
ModelInfo * OP_BOULDER;
ModelInfo * OP_POLFLAG;
ModelInfo * OP_SKYMDLS;

void BoulderPath(ObjectMaster *a1);

NJS_VECTOR OceanPalaceTriggers[]{
	{ -8200, 1401, -40045 },
	{ -8350, 1301, -43101 }
};

void OPFins_Display(ObjectMaster *a1) {
	if (CurrentChunk == 5 && !DroppedFrames) {
		for (int i = 0; i < LengthOfArray(OceanPalace_EventObjects); ++i) {
			if (OceanPalace_EventObjects[i].Model > 1 && CheckModelDisplay2(OceanPalace_EventObjects[i])) {
				SOI_LIST2 item = OceanPalace_EventObjects[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 2: njDrawModel_SADX(OP_TURFINS->getmodel()->basicdxmodel); break;
				case 3: njDrawModel_SADX(OP_TURFINS->getmodel()->child->basicdxmodel); break;
				case 4: njDrawModel_SADX(OP_TURFINS->getmodel()->child->child->basicdxmodel); break;
				case 5: njDrawModel_SADX(OP_TURFINS->getmodel()->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void OPFins_Main(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = OPFins_Display;
		a1->Data1->Action = 1;
	}

	if (CurrentChunk == 5) {
		a1->Data1->InvulnerableTime++;
		if (a1->Data1->InvulnerableTime > 300) a1->Data1->InvulnerableTime = 0;

		short finsstate = a1->Data1->InvulnerableTime;

		for (Int i = 9; i < 41; ++i) {
			if (finsstate <= 100) {
				OceanPalace_EventObjects[i].Rotation[1] -= 100;
			}
			if (finsstate > 100) {
				OceanPalace_EventObjects[i].Rotation[1] += 50.5f;
			}
		}
		for (Int i = 41; i < 73; ++i) {
			if (finsstate <= 100) {
				OceanPalace_EventObjects[i].Rotation[1] += 100;
			}
			if (finsstate > 100) {
				OceanPalace_EventObjects[i].Rotation[1] -= 50.5f;
			}
		}
		for (Int i = 9; i < 73; ++i) {
			if (finsstate <= 100) {
				OceanPalace_EventObjects[i].Rotation[0] += 10;
			}
			if (finsstate > 100) {
				OceanPalace_EventObjects[i].Rotation[0] -= 5;
			}
		}

		OPFins_Display(a1);
	}
}

void OPFlowers(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}
	
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(OceanPalace_Flowers); ++i) {
			if (CheckModelDisplay2(OceanPalace_Flowers[i])) {
				SOI_LIST2 item = OceanPalace_Flowers[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
				DrawQueueDepthBias = item.Bias;

				switch (item.Model) {
				case 0: njDrawModel_SADX(OP_FLOWERS->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(OP_FLOWERS->getmodel()->child->basicdxmodel); break;
				case 2: njDrawModel_SADX(OP_FLOWERS->getmodel()->child->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(OP_FLOWERS->getmodel()->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void OPWaterfalls(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (CurrentAct == 0) {
			for (int i = 0; i < LengthOfArray(OceanPalace_Waterfalls); ++i) {
				SOI_LIST2 item = OceanPalace_Waterfalls[i];

				ObjectMaster *obj;
				obj = LoadObject(LoadObj_Data1, 4, WaterfallObject);
				obj->DisplaySub = WaterfallObject;
				obj->Data1->Position = item.Position;

				obj->Data1->Rotation.x = item.Rotation[0];
				obj->Data1->Rotation.y = item.Rotation[1];
				obj->Data1->Rotation.z = item.Rotation[2];

				obj->Data1->Scale.y = item.Scale.x;
				obj->Data1->Scale.x = item.DrawDistance;
				obj->Data1->InvulnerableTime = item.Bias;
				obj->Data1->NextAction = item.Chunk;
					
				switch (item.Model) {
				case 0: obj->Data1->Object = OP_WATERFS->getmodel(); break;
				case 1: obj->Data1->Object = OP_WATERFS->getmodel()->child; break;
				case 2: obj->Data1->Object = OP_WATERFS->getmodel()->child->child; break;
				case 3: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child; break;
				case 4: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child; break;
				case 5: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child->child; break;
				case 6: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child->child->child; break;
				case 7: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child->child->child->child; break;
				case 8: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child; break;
				case 9: obj->Data1->Object = OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child; break;
				}
			}

			DeleteObjectMaster(a1);
		}
		else {
			if (a1->Data1->Action == 0) {
				a1->DisplaySub = a1->MainSub;
				a1->Data1->Action = 1;
			}

			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			DrawQueueDepthBias = -6000.f;
			njTranslate(nullptr, 0, 3000, 2900);
			njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void OPPOLE_Main(ObjectMaster *a1) {
	if (!MissedFrames && !ClipSetObject(a1)) {
		DynColRadius(a1, 200, 0);

		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);

		if (a1->Data1->Scale.x == 1)
			njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel);
		else 
			njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->basicdxmodel);

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void OPPOLE(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = OP_POLFLAG->getmodel()->child;
	else a1->Data1->Object = OP_POLFLAG->getmodel();

	a1->MainSub = &OPPOLE_Main;
	a1->DisplaySub = &OPPOLE_Main;
	a1->DeleteSub = &deleteSub_Global;
}

void OP_AnimFix() {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 *co2 = CharObj2Ptrs[0];
	if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
		entity->Action = 2;
		co2->AnimationThing.Index = 13;
	}
	else {
		entity->Action = 76;
		co2->AnimationThing.Index = 137;
	}
}

void OPBoulders_Display(ObjectMaster *a1) {
	if (CurrentChunk < 6)
		return;
	
	njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
	njPushMatrix(0);
	DrawQueueDepthBias = -6000.0f;

	switch (a1->Data1->Action) {
	case 1: 
		njTranslate(0, -8200.3408f, 2030, -39259.3);
		njRotateY(nullptr, 32768);
		break;
	case 2:
		njTranslate(0, -8274.9741f, 1363, -44980.36);
		njRotateY(nullptr, 24735);
		break;
	case 3:
		njTranslate(0, -6785.288f, 963, -46514.48);
		njRotateY(nullptr, 32768);
		break;
	}
	
	njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
	DrawQueueDepthBias = 0;
	njPopMatrix(1u);
}

void OPBoulders(ObjectMaster *a1) {
	if (CurrentChunk < 6)
		return;

	switch (a1->Data1->Action)
	{
	case 0:
		a1->Data1->Object = OP_BOULDER->getmodel();
		a1->DisplaySub = OPBoulders_Display;
		a1->Data1->Action = 1;
		break;
	case 1:
		if (IsPlayerInsideSphere(&OceanPalaceTriggers[0], 45.0f) == 1) {
			a1->Data1->Action = 2;
			SetCameraMode_(0);

			LoadChildObject(LoadObj_Data1, BoulderPath, a1)->Data1->LoopData = (Loop*)&OP_BoulderPaths[0];
		}

		break;
	case 2:
		if (a1->Child->Data1->Position.z < -44954) {
			a1->Data1->Action = 3;

			LoadChildObject(LoadObj_Data1, BoulderPath, a1)->Data1->LoopData = (Loop*)&OP_BoulderPaths[1];
		}

		if (IsPlayerInsideSphere(&OceanPalaceTriggers[0], 45.0f) == 1 ||
			IsPlayerInsideSphere(&OceanPalaceTriggers[1], 45.0f) == 1)
			OP_AnimFix();

		break;
	case 3:
		if (a1->Child->Data1->Position.z < -46583) {
			a1->Data1->Action = 0;

			LoadChildObject(LoadObj_Data1, BoulderPath, a1)->Data1->LoopData = (Loop*)&OP_BoulderPaths[2];
		}

		break;
	}

	RunObjectChildren(a1);
	a1->DisplaySub(a1);
}

PVMEntry OceanPalaceTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "GOMA", &GOMA_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ 0 }
};

ObjectListEntry OceanPalaceObjectList_list[] = {
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 3360000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 460000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 860000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 50000, 0, SHCameraSwitch, "CAMSWITCH" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO_WOODBOX" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, (ObjectFuncPtr)0x5001E0, "O JUMP   " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, ORock, "O ROCK   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFena, "O FENA   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFenb, "O FENB   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 422500, 0, OFenc, "O FENC   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFend, "O FEND   " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OGrasa, "O GRASA   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OGrasb, "O GRASB   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OGrasc, "O GRASC   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OGrasd, "O GRASD   " },
	{ LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 3240000, 0, ODolphin, "O DOLPHIN   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OSekityu, "O SEKITYU   " },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OKamome, "O KAMOME   " },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OKamomel, "O KAMOMEL   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OKomomo, "O KOMOMO   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OParasol_Load, "O PARASOL" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 250000, 0, OPier, "O PIER" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, OBkusa, "O BKUSA" },
	{ LoadObj_Data1 | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OMkusa, "O MKUSA" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data2 | LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, OBigrock, "O BIGROCK" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, ORock2, "O Rock2" },
	{ LoadObj_Data2 | LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, OArchrock, "O ARCHROCK" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OBed, "O BED" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OBewind, "O BEWIND " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, OFrog, "O FROG " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, ItemBoxAir_Main, "O ItemBoxAir" },
	{ LoadObj_Data1, ObjIndex_NoDisplay, DistObj_UseDist, 1000000, 0, BigWJump_Main, "BIGWJUMP" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerA_Main, "SPINA A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerB_Main, "SPINA B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerC_Main, "SPINA C" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, SpringH_Load, "O SPRING H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, OPPOLE, "OP_POLFLAG" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 8000000, 0, OPPOLE, "OP_POLFLAG" }
};
ObjectList OceanPalaceObjectList = { arraylengthandptrT(OceanPalaceObjectList_list, int) };

void OceanPalaceObjects_Init(const char *path) {
	TexLists_Obj[HeroesLevelID_OceanPalace] = OceanPalaceTextures;
	ObjLists[HeroesLevelID_OceanPalace * 8] =		&OceanPalaceObjectList;
	ObjLists[HeroesLevelID_OceanPalace * 8 + 1] =	&OceanPalaceObjectList;
}