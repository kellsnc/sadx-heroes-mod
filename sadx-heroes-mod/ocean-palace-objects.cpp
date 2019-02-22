#include "stdafx.h"
#include "mod.h"
#include "objects.h"

#include "ocean-palace-objects.h"

ModelInfo * OP_WATERFS;
ModelInfo * OP_FLOWERS;
ModelInfo * OP_TURFINS;
ModelInfo * OP_BOULDER;
ModelInfo * OP_POLFLAG;

static int finsstate = 0;

static uint8_t rocktrigger = 0;
static uint8_t rockstate = 0;
static float rocktimer = 0;
static uint8_t rockstate2 = 0;
static float rocktimer2 = 0;
static uint8_t rockstate3 = 0;
static float rocktimer3 = 0;

NJS_VECTOR OceanPalaceTriggers[]{
	{ -8200, 1401, -40045 },
	{ -8350, 1301, -43101 }
};

#pragma region Fins
void OPFins_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(OceanPalace_EventObjects); ++i) {
			if (OceanPalace_EventObjects[i].Model > 1 && CheckModelDisplay(OceanPalace_EventObjects[i])) {
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
	if (CurrentChunk == 5) {
		finsstate++;
		if (finsstate > 300) finsstate = 0;
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

void OPFins(ObjectMaster *a1) {
	a1->DisplaySub = OPFins_Display;
	a1->MainSub = OPFins_Main;
}
#pragma endregion

#pragma region Flowers
void OPFlowers_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(OceanPalace_Flowers); ++i) {
			if (CheckModelDisplay(OceanPalace_Flowers[i])) {
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

void OPFlowers(ObjectMaster *a1) {
	a1->DisplaySub = OPFlowers_Display;
	a1->MainSub = OPFlowers_Display;
}
#pragma endregion

#pragma region Waterfalls
void OPWaterfalls_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		if (CurrentAct == 0) {
			for (int i = 0; i < LengthOfArray(OceanPalace_Waterfalls); ++i) {
				if (CheckModelDisplay(OceanPalace_Waterfalls[i])) {
					SOI_LIST2 item = OceanPalace_Waterfalls[i];

					njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
					njPushMatrix(0);
					njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
					njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
					njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
					DrawQueueDepthBias = item.Bias;

					switch (item.Model) {
					case 0: njDrawModel_SADX(OP_WATERFS->getmodel()->basicdxmodel); break;
					case 1: njDrawModel_SADX(OP_WATERFS->getmodel()->child->basicdxmodel); break;
					case 2: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->basicdxmodel); break;
					case 3: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->basicdxmodel); break;
					case 4: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->basicdxmodel); break;
					case 5: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->basicdxmodel); break;
					case 6: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->basicdxmodel); break;
					case 7: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->basicdxmodel); break;
					case 8: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->basicdxmodel); break;
					case 9: njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->basicdxmodel); break;
					}

					DrawQueueDepthBias = 0;
					njPopMatrix(1u);
				}
			}
		}
		else {
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

void OPWaterfalls(ObjectMaster *a1) {
	a1->DisplaySub = OPWaterfalls_Display;
	a1->MainSub = OPWaterfalls_Display;
}
#pragma endregion

#pragma region Poles
void OPPOLE_Display(ObjectMaster *a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 2000.0f)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		if (a1->Data1->Scale.x == 1) njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->child->basicdxmodel);
		else njDrawModel_SADX(OP_WATERFS->getmodel()->child->child->child->child->child->child->child->child->child->child->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void OPPOLE_Main(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		OPPOLE_Display(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl OPPOLE(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = OP_POLFLAG->getmodel()->child;
	else a1->Data1->Object = OP_POLFLAG->getmodel();
	AddToCollision(a1, 0);

	a1->MainSub = &OPPOLE_Main;
	a1->DisplaySub = &OPPOLE_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Boulders
void OP_AnimFix() {
	auto entity = EntityData1Ptrs[0];
	CharObj2 *co2 = GetCharObj2(0);
	if (!SuperSonicFlag) {
		entity->Action = 2;
		co2->AnimationThing.Index = 13;
	}
	else {
		entity->Action = 76;
		co2->AnimationThing.Index = 137;
	}
}

void OPBoulders_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 2; i < 5; ++i) {
			if (i > 2 && CurrentChunk != 7) break;

			SOI_LIST2 item = OceanPalace_EventObjects[i];

			njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
			njPushMatrix(0);
			njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
			njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
			njScale(nullptr, item.Scale.x, item.Scale.y, item.Scale.z);
			DrawQueueDepthBias = item.Bias;

			njDrawModel_SADX(OP_BOULDER->getmodel()->basicdxmodel);

			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void OPBoulders_Delete(ObjectMaster *a1) {
	rocktrigger = 0;
	rockstate = 0;
	rocktimer = 0;
	rockstate2 = 0;
	rocktimer2 = 0;
	rockstate3 = 0;
	rocktimer3 = 0;

	OceanPalace_EventObjects[2].Position = { -8200.3408f, 2030, -39259.3f };
	OceanPalace_EventObjects[3].Position = { -8274.9741f, 1363, -44980.36f };
	OceanPalace_EventObjects[4].Position = { -6785.288f, 963, -46514.48f };

	OceanPalace_EventObjects[2].Rotation[1] = 32768;
	OceanPalace_EventObjects[3].Rotation[1] = 24735;
	OceanPalace_EventObjects[4].Rotation[1] = 32768;
}

void OPBoulders_Main(ObjectMaster *a1) {
	//todo: rewrite this shit

	if (CurrentChunk > 5) {
		if (IsPlayerInsideSphere(&OceanPalaceTriggers[1], 45.0f) == 1) OP_AnimFix();

		if (IsPlayerInsideSphere(&OceanPalaceTriggers[0], 45.0f) == 1) {
			rocktrigger = 1;
			OP_AnimFix();
			FreeCam = 0;
		}

		if (OceanPalace_EventObjects[2].Position.z < -44954) rocktrigger = 2;
		if (OceanPalace_EventObjects[2].Position.z < -46553) rocktrigger = 3;

		if (rocktrigger > 0) {
			if (IsPlayerInsideSphere(&OceanPalace_EventObjects[2].Position, 120.0f) == 1 || IsPlayerInsideSphere(&OceanPalace_EventObjects[3].Position, 120.0f) == 1 || IsPlayerInsideSphere(&OceanPalace_EventObjects[4].Position, 120.0f) == 1) GameState = 7;

			OceanPalace_EventObjects[2].Rotation[0] = OceanPalace_EventObjects[2].Rotation[0] + 1500;
			if (rocktimer > 1) { rocktimer = 0; rockstate++; }

			if (rockstate < LengthOfArray(OP_ROCK1) - 1) {
				rocktimer = rocktimer + (OP_ROCKS[0].totaldist / OP_ROCK1[rockstate].Distance) / OP_ROCKS[0].totaldist * 8;
				if (OP_ROCK1[rockstate].Rotation[1] != 0) {
					OceanPalace_EventObjects[2].Rotation[1] = OP_ROCK1[rockstate].Rotation[1];
				}
				OceanPalace_EventObjects[2].Position.x = (OP_ROCK1[rockstate + 1].Position.x - OP_ROCK1[rockstate].Position.x) * rocktimer + OP_ROCK1[rockstate].Position.x;
				OceanPalace_EventObjects[2].Position.y = (OP_ROCK1[rockstate + 1].Position.y - OP_ROCK1[rockstate].Position.y) * rocktimer + OP_ROCK1[rockstate].Position.y;
				OceanPalace_EventObjects[2].Position.z = (OP_ROCK1[rockstate + 1].Position.z - OP_ROCK1[rockstate].Position.z) * rocktimer + OP_ROCK1[rockstate].Position.z;
			}
			if (rocktrigger > 1) {
				OceanPalace_EventObjects[3].Rotation[0] = OceanPalace_EventObjects[3].Rotation[0] + 1500;
				if (rocktimer2 > 1) { rocktimer2 = 0; rockstate2++; }
				if (rockstate2 < LengthOfArray(OP_ROCK2) - 1) {
					rocktimer2 = rocktimer2 + (OP_ROCKS[0].totaldist / OP_ROCK2[rockstate2].Distance) / OP_ROCKS[0].totaldist * 8;
					if (OP_ROCK2[rockstate2].Rotation[1] != 0) {
						OceanPalace_EventObjects[3].Rotation[1] = OP_ROCK2[rockstate2].Rotation[1];
					}
					OceanPalace_EventObjects[3].Position.x = (OP_ROCK2[rockstate2 + 1].Position.x - OP_ROCK2[rockstate2].Position.x) * rocktimer2 + OP_ROCK2[rockstate2].Position.x;
					OceanPalace_EventObjects[3].Position.y = (OP_ROCK2[rockstate2 + 1].Position.y - OP_ROCK2[rockstate2].Position.y) * rocktimer2 + OP_ROCK2[rockstate2].Position.y;
					OceanPalace_EventObjects[3].Position.z = (OP_ROCK2[rockstate2 + 1].Position.z - OP_ROCK2[rockstate2].Position.z) * rocktimer2 + OP_ROCK2[rockstate2].Position.z;
				}
				if (rocktrigger > 2) {
					OceanPalace_EventObjects[4].Rotation[0] = OceanPalace_EventObjects[4].Rotation[0] + 1500;
					if (rocktimer3 > 1) { rocktimer3 = 0; rockstate3++; }
					if (rockstate3 < LengthOfArray(OP_ROCK3) - 1) {
						rocktimer3 = rocktimer3 + (OP_ROCKS[0].totaldist / OP_ROCK3[rockstate3].Distance) / OP_ROCKS[0].totaldist * 8;
						if (OP_ROCK3[rockstate3].Rotation[1] != 0) {
							OceanPalace_EventObjects[4].Rotation[1] = OP_ROCK3[rockstate3].Rotation[1];
						}
						OceanPalace_EventObjects[4].Position.x = (OP_ROCK3[rockstate3 + 1].Position.x - OP_ROCK3[rockstate3].Position.x) * rocktimer3 + OP_ROCK3[rockstate3].Position.x;
						OceanPalace_EventObjects[4].Position.y = (OP_ROCK3[rockstate3 + 1].Position.y - OP_ROCK3[rockstate3].Position.y) * rocktimer3 + OP_ROCK3[rockstate3].Position.y;
						OceanPalace_EventObjects[4].Position.z = (OP_ROCK3[rockstate3 + 1].Position.z - OP_ROCK3[rockstate3].Position.z) * rocktimer3 + OP_ROCK3[rockstate3].Position.z;
					}
				}
			}
		}

		OPBoulders_Display(a1);
	}
}

void OPBoulders(ObjectMaster *a1) {
	a1->DisplaySub = OPBoulders_Display;
	a1->DeleteSub = OPBoulders_Delete;
	a1->MainSub = OPBoulders_Main;
}
#pragma endregion

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
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ 0 }
};

ObjectListEntry OceanPalaceObjectList_list[] = {
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
	{ 2, 3, 1, 1360000, 0, &ObjFan, "OBJFAN" } /* "SH FANS" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x500FF0, "YASI1   " } /* "YASI1   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x501090, "YASI2   " } /* "YASI2   " */,
	{ 2, 3, 1, 1060000, 0, &ObjFan, "OBJFAN" } /* "SH FANS" */,
	{ 6, 3, 1, 360000, 0, &SHCameraSwitch, "CAMSWITCH" } /* "SH Camera Switcher" */,
	{ 6, 3, 1, 1360000, 0, &ObjBalloon, "SH BALLOON" } /* "SH Balloon" */,
	{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" */,
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
	{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF190, "E UNI A" } /* "E UNI A" */,
	{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF500, "E UNI B" } /* "E UNI B" */,
	{ 2, 3, 5, 250000, 0, (ObjectFuncPtr)0x4AF860, "E UNI C" } /* "E UNI C" */,
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
	{ 2, 3, 1, 1600000, 0, (ObjectFuncPtr)&OPPOLE, "Poles" } /* "Waterfall poles" */,
	{ 2, 3, 1, 1600000, 0, (ObjectFuncPtr)&OPPOLE, "Poles" } /* "Waterfall poles" */
};
ObjectList OceanPalaceObjectList = { arraylengthandptrT(OceanPalaceObjectList_list, int) };

void OceanPalaceObjects_Init(const char *path) {
	WriteData((PVMEntry**)0x90EB70, OceanPalaceTextures);
	WriteData((ObjectList**)0x974B38, &OceanPalaceObjectList);
	WriteData((ObjectList**)0x974B3C, &OceanPalaceObjectList);
}