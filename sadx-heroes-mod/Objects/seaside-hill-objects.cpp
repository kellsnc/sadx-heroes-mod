#include "stdafx.h"
#include "seaside-hill-objects.h"

ModelInfo * SH_PLATFOR;
ModelInfo * SH_MORUINS;
ModelInfo * SH_POLFLAG;
ModelInfo * SH_WATERFS;
ModelInfo * SH_FLOWERS;

float ruin = 0;
static int flagtimer = 0;

void SHFlowers(ObjectMaster *a1) {
	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}

	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(SeasideHill_Flowers); ++i) {
			if (CheckModelDisplay2(SeasideHill_Flowers[i])) {
				SOI_LIST2 item = SeasideHill_Flowers[i];

				SetHeroesLeveltex();
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

void SHWaterfalls(ObjectMaster *a1) {
	if (!DroppedFrames) {
		for (int i = 0; i < LengthOfArray(SeasideHill_Waterfalls); ++i) {
			if ((CurrentAct == 0 && i > 49) || (CurrentAct == 1 && i < 50)) continue;
			SOI_LIST2 item = SeasideHill_Waterfalls[i];

			ObjectMaster *obj;
			obj = LoadObject(LoadObj_Data1, 4, WaterfallObject);
			obj->DisplaySub = WaterfallObject;
			obj->DeleteSub = DynCol_Delete;
			obj->Data1->Position = item.Position;

			obj->Data1->Rotation.x = item.Rotation[0];
			obj->Data1->Rotation.y = item.Rotation[1];
			obj->Data1->Rotation.z = item.Rotation[2];

			obj->Data1->Scale.y = item.Scale.x;
			obj->Data1->Scale.x = item.DrawDistance;
			obj->Data1->InvulnerableTime = item.Bias;
			obj->Data1->NextAction = item.Chunk;

			switch (item.Model) {
			case 0: obj->Data1->Object = SH_WATERFS->getmodel(); break;
			case 1: obj->Data1->Object = SH_WATERFS->getmodel()->child; break;
			case 2: obj->Data1->Object = SH_WATERFS->getmodel()->child->child; break;
			case 3: obj->Data1->Object = SH_WATERFS->getmodel()->child->child->child; break;
			}
		}

		DeleteObjectMaster(a1);
	}
}

void SHMovingPltfrms_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void SHMovingPltfrms_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		DynColRadiusAuto(a1, a1->Data1->NextAction);

		if (ruin != 0 && ruin == a1->Data1->Scale.y && IsPlayerInsideSphere_(&a1->Data1->Position, 1000)) {
			if (a1->Data1->Position.y != a1->Data1->Scale.z) {
				if (a1->Data1->Scale.x >= a1->Data1->Scale.z) {
					if (a1->Data1->Position.y <= a1->Data1->Scale.z) {
						a1->Data1->Position.y = a1->Data1->Scale.z;
					}
					else if (a1->Data1->Position.y > a1->Data1->Scale.z) a1->Data1->Position.y -= 2;

					if (a1->Data1->Position.y == a1->Data1->Scale.x - 2) PlayCustomSound_Entity(LevelSound_Sea_RuinEnd, a1, 1000, 0);
				}
				else if (a1->Data1->Scale.x <= a1->Data1->Scale.z) {
					if (a1->Data1->Position.y >= a1->Data1->Scale.z) {
						a1->Data1->Position.y = a1->Data1->Scale.z;
					}
					else if (a1->Data1->Position.y < a1->Data1->Scale.z) a1->Data1->Position.y += 2;

					if (a1->Data1->Position.y == a1->Data1->Scale.x + 2) PlayCustomSound_Entity(LevelSound_Sea_RuinEnd, a1, 1000, 0);
					if (a1->Data1->Position.y <= a1->Data1->Scale.z - 1 && a1->Data1->Position.y >= a1->Data1->Scale.z - 3) PlayCustomSound_Entity(LevelSound_Sea_RuinEnd, a1, 1000, 0);
				}

			}

		}

		NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;
		if (col) col->pos[1] = a1->Data1->Position.y;

		SHMovingPltfrms_Display(a1);
	}
}

void __cdecl SHMovingPltfrms(ObjectMaster *a1)
{
	if (a1->Data1->Scale.x == 0) a1->Data1->Object = SH_MORUINS->getmodel();
	if (a1->Data1->Scale.x == 1) a1->Data1->Object = SH_MORUINS->getmodel()->child;
	if (a1->Data1->Scale.x == 2) a1->Data1->Object = SH_MORUINS->getmodel()->child->child;

	if (a1->Data1->Scale.z == 0) a1->Data1->NextAction = 0;
	else a1->Data1->NextAction = 1;

	a1->Data1->Action = (char)a1->Data1->Scale.x;
	a1->Data1->Scale.x = a1->Data1->Position.y;

	a1->MainSub = SHMovingPltfrms_Main;
	a1->DisplaySub = SHMovingPltfrms_Display;
	a1->DeleteSub = DynCol_Delete;
}

void __cdecl SHRuinTrigger(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (ruin != a1->Data1->Scale.x) {
			EntityData1 *entity = EntityData1Ptrs[0];
			if (IsPlayerInsideSphere(&a1->Data1->Position, a1->Data1->Scale.y)) {
				ruin = a1->Data1->Scale.x;
			}
		}
	}
}

void __cdecl SHPlatforms_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 200, 2);
		DrawObjModel(a1, a1->Data1->Object->basicdxmodel, true);
	}
}

void __cdecl SHPlatforms(ObjectMaster *a1)
{
	a1->Data1->Object = SH_PLATFOR->getmodel();

	a1->MainSub = SHPlatforms_Main;
	a1->DisplaySub = SHPlatforms_Main;
	a1->DeleteSub = DynCol_Delete;
}

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

void __cdecl SHSpikes_Display(ObjectMaster *a1)
{
	if (!DroppedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		njRotateY(0, -a1->Data1->Rotation.y + a1->Data1->Scale.z);
		njScale(nullptr, 0.8f, 1, 1);
		njDrawModel_SADX(&SH_Flag);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void __cdecl SHSpikes_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 100, 0);

		if (a1->Data1->Scale.z > 4000) a1->Data1->Scale.y = 1;
		if (a1->Data1->Scale.z == 0) a1->Data1->Scale.y = 0;

		if (a1->Data1->Scale.y == 0) a1->Data1->Scale.z += 10;
		if (a1->Data1->Scale.y == 1) a1->Data1->Scale.z -= 10;

		SHSpikes_Display(a1);
	}
}

void __cdecl SHSpikes(ObjectMaster *a1)
{
	a1->Data1->Object = SH_POLFLAG->getmodel();
	a1->Data1->Scale.y = 0;

	a1->MainSub = SHSpikes_Main;
	a1->DisplaySub = SHSpikes_Display;
	a1->DeleteSub = DynCol_Delete;
}

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
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ 0 }
};

ObjectListEntry SeasideHillObjectList_list[] = {
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2460000, 0, &SHMovingPltfrms, "MOV PLA" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, &SHRuinTrigger, "MOV PLA T" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2460000, 0, &SHPlatforms, "SMALL PLA   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, &ObjReel, "OBJREEL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OJump, "O JUMP   " },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, ORock, "O ROCK   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFena, "O FENA   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFenb, "O FENB   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 422500, 0, OFenc, "O FENC   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OFend, "O FEND   " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, OBJCASE, "OBJ CASE" },
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" },
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, (ObjectFuncPtr)&SHSpikes, "SH_POLFLAG" },
	{ 0, 0, 0, 0, 0, nullptr, NULL },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO_WOODBOX" },
};

ObjectList SeasideHillObjectList = { arraylengthandptrT(SeasideHillObjectList_list, int) };

void SeasideHillObjects_Init() {
	TexLists_Obj[HeroesLevelID_SeasideHill] = SeasideHillObjectTextures;
	ObjLists[HeroesLevelID_SeasideHill * 8] =		&SeasideHillObjectList;
	ObjLists[HeroesLevelID_SeasideHill * 8 + 1] =	&SeasideHillObjectList;
}

void SeasideHillObjects_OnFrame(EntityData1 * entity) {
	Flags_Animate();
	if (!IsLoaded) Flags_Reset();
}