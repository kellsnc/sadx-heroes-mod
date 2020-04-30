#include "stdafx.h"

void HCWarp(ObjectMaster *a1);
void HCDoor(ObjectMaster *a1);
void HCWall(ObjectMaster *a1);
void HCPlatform(ObjectMaster *a1);

ModelInfo * MM_SPHERE1;
ModelInfo * MM_SPHERE2;
ModelInfo * MM_SKELFAN;
ModelInfo * MM_MYSTCAR;
ModelInfo * MM_TORCHES;

extern ModelInfo * HC_HFLAMES;

int flamecount;

void MysticTorches_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -5000.0f;
		njDrawModel_SADX(MM_TORCHES->getmodel()->child->basicdxmodel);
		if (a1->Data1->Scale.x == 0) {
			njDrawModel_SADX(MM_TORCHES->getmodel()->child->child->child->basicdxmodel);
			njTranslate(0, 0, 31, 0);
			njScale(0, 1.2f, 1.2f, 1.2f);
			njRotateY(0, Camera_Data1->Rotation.y - a1->Data1->Rotation.y);
			njDrawModel_SADX(HC_HFLAMES->getmodel()->basicdxmodel);
		}
		else if (a1->Data1->Scale.x == 2) {
			njDrawModel_SADX(MM_TORCHES->getmodel()->child->child->child->child->basicdxmodel);
			njTranslate(0, 0, 31, 0);
			njScale(0, 1.2f, 1.2f, 1.2f);
			njRotateY(0, Camera_Data1->Rotation.y - a1->Data1->Rotation.y);
			njDrawModel_SADX(HC_HFLAMES->getmodel()->child->basicdxmodel);
		}
		else {
			njDrawModel_SADX(MM_TORCHES->getmodel()->child->child->basicdxmodel);
		}
		
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticTorches_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		DynColRadius(a1, 50, 0);

		if (a1->Data1->Scale.x != 1) {
			NJS_VECTOR flame = a1->Data1->Position;
			flame.y += 31;
			int player = IsPlayerInsideSphere(&flame, 8.f);
			if (player != 0) {
				if (EntityData1Ptrs[player - 1]->Status | StatusBits_Attack ||
					EntityData1Ptrs[player - 1]->Status | StatusBits_Ball) {
					a1->Data1->Scale.x = 1;
					flamecount -= 1;

					if (flamecount == 0) LoadLevelResults();
				}
				else {
					HurtCharacter(player - 1);
				}
			}
		}

		a1->DisplaySub(a1);
	}
}

void MysticTorches(ObjectMaster *a1)
{
	HC_HFLAMES->getmodel()->basicdxmodel->mats[0].attr_texId = 138;
	HC_HFLAMES->getmodel()->child->basicdxmodel->mats[0].attr_texId = 122;

	a1->Data1->Object = MM_TORCHES->getmodel();

	a1->MainSub = &MysticTorches_Main;
	a1->DisplaySub = &MysticTorches_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void MysticSphere_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		if (a1->Data1->Scale.x != 0 && IsPlayerInsideSphere(&a1->Data1->Position, 500.0f) == 0) return;

		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);

		if (a1->Data1->Scale.x == 0) njScale(nullptr, 1.2f, 1.2f, 1.2f);
		else njScale(nullptr, 0.3f, 0.3f, 0.3f);

		DrawQueueDepthBias = -20000.0f;
		DrawModel_Queue(MM_SPHERE2->getmodel()->basicdxmodel, QueuedModelFlagsB_EnableZWrite);
		DrawModel_Queue(MM_SPHERE1->getmodel()->basicdxmodel, QueuedModelFlagsB_EnableZWrite);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticSphere_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		a1->DisplaySub(a1);
	}
}

void MysticSphere(ObjectMaster *a1)
{
	a1->MainSub = &MysticSphere_Main;
	a1->DisplaySub = &MysticSphere_Display;
}

void MysticFan_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(MM_SKELFAN->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticFan_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		a1->Data1->Rotation.y -= a1->Data1->Scale.y;

		int slot = IsPlayerInsideSphere(&a1->Data1->Position, 45.0f);
		if (slot > 0) {
			EntityData1 *entity = EntityData1Ptrs[slot - 1];
			CharObj2 *co2 = CharObj2Ptrs[slot - 1];
			if (co2 != NULL) {
				co2->Speed.x = 0; co2->Speed.z = 0;
				entity->Rotation.x = 0;
				entity->Rotation.z = 0;
				co2->Speed.y = a1->Data1->Scale.x;
				if (GetCharacterID(0) == Characters_Sonic && (co2->Upgrades & Upgrades_SuperSonic) == 0) {
					co2->AnimationThing.Index = 26;
					entity->Status = 0;
				}
				else if (GetCharacterID(0) == Characters_Tails) {
					co2->AnimationThing.Index = 33;
					entity->Status = 0;
				}
				else if (GetCharacterID(0) == Characters_Knuckles) {
					co2->AnimationThing.Index = 34;
					entity->Status = 0;
				}
			}
		}

		MysticFan_Display(a1);
	}
}

void MysticFan(ObjectMaster *a1)
{
	a1->MainSub = &MysticFan_Main;
	a1->DisplaySub = &MysticFan_Display;
}

void CartActions(ObjectMaster * a1) {
	EntityData1 * entity = a1->Data1;
	auto player = EntityData1Ptrs[entity->Index - 1];

	if (entity->NextAction == 0 && ControllerPointers[entity->Index - 1]->PressedButtons & Buttons_A) entity->NextAction = 1;
	
	if (entity->NextAction > 0) {
		entity->NextAction += 1;

		if (entity->NextAction == 40) entity->NextAction = 0;
		if (entity->NextAction > 20) a1->Data1->Position.y += abs(entity->NextAction - 40);
		else a1->Data1->Position.y += entity->NextAction;

		player->Position.y = entity->Position.y;
	}
}

void CartDisplay(ObjectMaster * a1) {
	if (!MissedFrames && IsPlayerInsideSphere(&a1->Data1->Position, 1000)) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		
		njRotateY(nullptr, a1->Data1->Rotation.y);
		if (a1->Data1->Action > 2) njRotateY(0, 0x4000);
		njRotateX(0, a1->Data1->Rotation.x);

		njTranslate(0, -1.5f, 0, 0);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(MM_MYSTCAR->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticCart(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData;
	EntityData1 * entity = a1->Data1;
	
	//Initialize
	if (entity->Action == 0) {
		a1->Data1->Position = loopdata->LoopList[0].Position;
		a1->Data1->Rotation.y = loopdata->LoopList[0].Ang_Y;
		a1->DisplaySub = CartDisplay;
		entity->Action = 1;
	}

	//Check for player
	else if (entity->Action == 1) {
		entity->Index = IsPlayerInsideSphere(&loopdata->LoopList[0].Position, loopdata->Unknown_0);
		if (entity->Index != 0) {
			entity->Action = 2;
			PlayCustomSound_EntityAndPos(LevelSound_Mys_Char, a1, &EntityData1Ptrs[entity->Index - 1]->Position, 300, 0.5f, true);
		}
	}
	
	//Launch found player
	else if (entity->Action == 2) {
		auto player = EntityData1Ptrs[entity->Index - 1];

		NJS_VECTOR startPos = player->Position;
		NJS_VECTOR targetPos = a1->Data1->Position;

		float timer = (float)entity->NextAction / 100;

		float x0 = startPos.x;
		float x1 = targetPos.x;
		float y0 = startPos.y;
		float y1 = targetPos.y + 28;
		float z0 = startPos.z;
		float z1 = targetPos.z;

		float distx = x1 - x0;
		float disty = y1 - y0;
		float distz = z1 - z0;

		player->Position.x = (distx)* timer + x0;
		player->Position.y = (disty)* timer + y0;
		player->Position.z = (distz)* timer + z0;

		DoBall(entity->Index - 1);

		if (a1->Data1->NextAction < 21) a1->Data1->NextAction += 1;
		else {
			entity->NextAction = 0;
			entity->Action = 3;
			PlayCustomSound_EntityAndVolume(LevelSound_Mys_Char, a1, 300, 2, true);
		}
	}

	//Launch Cart
	else if (entity->Action == 3) {
		auto player = EntityData1Ptrs[entity->Index - 1];
		CharObj2 * co2 = CharObj2Ptrs[entity->Index - 1];

		DoBall(entity->Index - 1);

		int speed = 22;
		if (ControllerPointers[entity->Index - 1]->HeldButtons & Buttons_X) speed += 6;
		if (ControllerPointers[entity->Index - 1]->HeldButtons & Buttons_Down) speed -= 6;

		entity->Scale.x = entity->Scale.x + (loopdata->TotalDist / loopdata->LoopList[entity->InvulnerableTime].Dist) / loopdata->TotalDist * speed;
		TransformSpline(a1->Data1, loopdata->LoopList[entity->InvulnerableTime].Position, loopdata->LoopList[entity->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
		a1->Data1->Rotation.y = fPositionToRotation(&loopdata->LoopList[entity->InvulnerableTime].Position, &loopdata->LoopList[entity->InvulnerableTime + 1].Position).y;
		a1->Data1->Rotation.z = 0;

		player->Position = entity->Position;
		entity->Position.y += 2;
		player->Position.y += 6;
		
		player->Rotation = entity->Rotation;

		CartActions(a1);

		if (a1->Data1->Scale.x > 1) { entity->Scale.x = 0; entity->InvulnerableTime++; }

		if (IsPlayerInsideSphere(&loopdata->LoopList[loopdata->Count - 1].Position, 20) == entity->Index) {
			entity->Action = 4;
			entity->Status = Status_KillSound;
			PlayCustomSound(CommonSound_CannonLch);

			co2->Speed.y = 1;
			co2->Speed.x = 4;
		}
	}

	CartDisplay(a1);
};

PVMEntry MysticMansionObjectTextures[] = {
	{ "E_SAI", &E_SAI_TEXLIST },
	{ "E_AMENBO", &E_AMENBO_TEXLIST },
	{ "ishidai", &ishidai_TEXLIST },
	{ "E_SARU", &E_SARU_TEXLIST },
	{ "big_kaeru", &big_kaeru_TEXLIST },
	{ "TOGEBALL_TOGEBALL", &TOGEBALL_TOGEBALL_TEXLIST },
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ "SUPI_SUPI", &SUPI_SUPI_TEXLIST },
	{ "UNI_C_UNIBODY", &UNI_C_UNIBODY_TEXLIST },
	{ "UNI_A_UNIBODY", &UNI_A_UNIBODY_TEXLIST },
	{ "E_LEON", (TexList *)0x9599F4 },
	{ "GOMA", &GOMA_TEXLIST },
	{ "PEN", &PEN_TEXLIST },
	{ "TUBA", &TUBA_TEXLIST },
	{ "KUJA", &KUJA_TEXLIST },
	{ "OUM", &OUM_TEXLIST },
	{ "KOAR", &KOAR_TEXLIST },
	{ "WARA", &WARA_TEXLIST },
	{ "BANB", &BANB_TEXLIST },
	{ "USA", &USA_TEXLIST },
	{ "MOGU", &MOGU_TEXLIST },
	{ "ZOU", &ZOU_TEXLIST },
	{ "LION", &LION_TEXLIST },
	{ "GORI", &GORI_TEXLIST },
	{ "SUKA", &SUKA_TEXLIST },
	{ "RAKO", &RAKO_TEXLIST },
	{ 0 }
};

ObjectListEntry MysticMansionObjectList_list[] = {
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
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Kiki_Load, "E SARU  " },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 160000, 0, RhinoTank_Main, "E SAITO" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, Sweep_Load, "E AMEMB" },
	{ LoadObj_Data1 | LoadObj_UnknownA, ObjIndex_Stage, DistObj_UseDist, 360000, 0, OJump, "O JUMP   " },
	{ LoadObj_Data2 | LoadObj_Data1 | LoadObj_UnknownA | LoadObj_UnknownB, ObjIndex_Stage, DistObj_UseDist, 160000, 0, ItemBoxAir_Main, "O ItemBoxAir" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerA_Main, "SPINA A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerB_Main, "SPINA B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown4, 0, 0, SpinnerC_Main, "SPINA C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjBalloon, "SH BALLOON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Default, 0, 0, SpringH_Load, "O SPRING H" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusA_Main, "E UNI A" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, UnidusB_Main, "E UNI B" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 250000, 0, UnidusC_Main, "E UNI C" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_Unknown5, 360000, 0, EPolice, "E POLICE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, ObjBoxW, "CO_WOODBOX" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1000000, 0, HCWarp, "HC WARP" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCDoor, "HC DOOR" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCWall, "HC WALL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 2400000, 0, HCPlatform, "HC PLAT" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, ObjCannon, "CANNON" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 360000, 0, SHCameraSwitch, "SH CAM SW" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1560000, 0, OBJCASE, "OBJ CASE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1360000, 0, &ObjReel, "OBJREEL" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, ObjFan, "OBJFAN" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, MysticFan, "MysticFan" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, MysticSphere, "MMSPHERE" },
	{ LoadObj_Data1, ObjIndex_Stage, DistObj_UseDist, 1060000, 0, MysticTorches, "MMTORCHES" }
};
ObjectList MysticMansionObjectList = { arraylengthandptrT(MysticMansionObjectList_list, int) };

void MysticMansionObjects_Init() {
	TexLists_Obj[HeroesLevelID_MysticMansion] = MysticMansionObjectTextures;
	ObjLists[HeroesLevelID_MysticMansion * 8] = &MysticMansionObjectList;
}