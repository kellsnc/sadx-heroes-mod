#include "stdafx.h"
#include "mod.h"
#include "objects.h"

void _cdecl HCWARP(ObjectMaster *a1);
void __cdecl HCDOOR(ObjectMaster *a1);
void __cdecl HCWALL(ObjectMaster *a1);
void __cdecl HCPLATFORM(ObjectMaster *a1);

ModelInfo * MM_SPHERE1;
ModelInfo * MM_SPHERE2;
ModelInfo * MM_SKELFAN;
ModelInfo * MM_MYSTCAR;

#pragma region MM Sphere
void MysticSphere_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);

		if (a1->Data1->Scale.x == 0) njScale(nullptr, 1.2f, 1.2f, 1.2f);
		else njScale(nullptr, 0.3f, 0.3f, 0.3f);

		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(MM_SPHERE2->getmodel()->basicdxmodel);
		njDrawModel_SADX(MM_SPHERE1->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticSphere_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {
		if (a1->Data1->Scale.x == 0) MysticSphere_Display(a1);
		else {
			if (IsPlayerInsideSphere(&a1->Data1->Position, 500.0f)) MysticSphere_Display(a1);
		}
	}
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl MysticSphere(ObjectMaster *a1)
{
	a1->MainSub = &MysticSphere_Main;
	a1->DisplaySub = &MysticSphere_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region MM Fans
void MysticFan_Display(ObjectMaster *a1)
{
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, 0, a1->Data1->Rotation.y, 0);
		njScale(nullptr, 1, 1, 1);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(MM_SKELFAN->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void MysticFan_Main(ObjectMaster *a1)
{
	if (IsPlayerInsideSphere(&a1->Data1->Position, 1500.0f)) {

		a1->Data1->Rotation.y -= a1->Data1->Scale.y;

		int slot = IsPlayerInsideSphere(&a1->Data1->Position, 45.0f);
		if (slot > 0) {
			auto entity = EntityData1Ptrs[slot - 1];
			CharObj2 *co2 = GetCharObj2(slot - 1);
			if (co2 != NULL) {
				co2->Speed.x = 0; co2->Speed.z = 0;
				entity->Rotation.x = 0;
				entity->Rotation.z = 0;
				co2->Speed.y = a1->Data1->Scale.x;
				if (GetCharacterID(0) == Characters_Sonic && !SuperSonicFlag) {
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
	else {
		deleteSub_Global(a1);
	}
}

void __cdecl MysticFan(ObjectMaster *a1)
{
	a1->MainSub = &MysticFan_Main;
	a1->DisplaySub = &MysticFan_Display;
	a1->DeleteSub = &deleteSub_Global;
}
#pragma endregion

#pragma region Cart
void DoBall(EntityData1 * entity);
void TransformPlayer(char player, NJS_VECTOR orig, NJS_VECTOR dest, float state);
float fPositionToRotation(CharObj2 * co2, NJS_VECTOR orig, NJS_VECTOR point);

void CartDisplay(ObjectMaster * a1) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		njScale(nullptr, 1, 1, 1);
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
		a1->Data1->Rotation.x = loopdata->LoopList[1].Ang_X;
		a1->Data1->Rotation.z = loopdata->LoopList[1].Ang_Y;
		a1->DisplaySub = CartDisplay;
		entity->Action = 1;
	}

	//Check for player
	else if (entity->Action == 1) {
		entity->Index = IsPlayerInsideSphere(&loopdata->LoopList[0].Position, loopdata->Unknown_0);
		if (entity->Index != 0) {
			entity->Action = 2;
		}
	}

	//Launch found player
	else if (entity->Action == 2) {
		auto player = EntityData1Ptrs[entity->Index - 1];

		NJS_VECTOR startPos = player->Position;
		NJS_VECTOR targetPos = a1->Data1->Position;

		if (a1->Data1->NextAction < 21) a1->Data1->NextAction += 1;
		else {
			entity->NextAction = 0;
			entity->Action = 3;
		}
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

		DoBall(player);
	}

	//Launch Cart
	else if (entity->Action == 3) {
		auto player = EntityData1Ptrs[entity->Index - 1];
		CharObj2 * co2 = GetCharObj2(entity->Index - 1);

		DoBall(player);

		entity->Scale.x = entity->Scale.x + (loopdata->TotalDist / loopdata->LoopList[entity->InvulnerableTime].Dist) / loopdata->TotalDist * 22;
		TransformPlayer(entity->NextAction, loopdata->LoopList[entity->InvulnerableTime].Position, loopdata->LoopList[entity->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
		player->Position.y += 8;
		entity->Position = player->Position;
		entity->Position.y -= 6;

		entity->Rotation.x = loopdata->LoopList[entity->InvulnerableTime].Ang_X;
		entity->Rotation.y = loopdata->LoopList[entity->InvulnerableTime].Ang_Y;
		entity->Rotation.z = 0;
		player->Rotation = entity->Rotation;

		if (a1->Data1->Scale.x > 1) { entity->Scale.x = 0; entity->InvulnerableTime++; }

		if (IsPlayerInsideSphere(&loopdata->LoopList[loopdata->Count - 1].Position, 20) == entity->Index) {
			entity->Action = 4;
			player->Rotation.y = loopdata->LoopList[loopdata->Count - 1].Ang_Y;
			
			co2->Speed.y = 4;
			co2->Speed.x = 6;
		}
	}

	CartDisplay(a1);
};
#pragma endregion

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
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7AA960, "E AMEMB " } /* "E AMEMB " */,
{ 2, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A1380, "E SAITO" } /* "E SAITO" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4AD140, "E SARU  " } /* "E SARU  " */,
{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x5001E0, "O JUMP   " } /* "O JUMP   " */,
{ 15, 3, 1, 160000, 0, (ObjectFuncPtr)0x4C07D0, "O ItemBoxAir" } /* "O ItemBoxAir" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0DF0, "SPINA A" } /* "SPINA A" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B0F40, "SPINA B" } /* "SPINA B" */,
{ 2, 3, 4, 0, 0, (ObjectFuncPtr)0x4B1090, "SPINA C" } /* "SPINA C" */,
{ 2, 2, 0, 0, 0, (ObjectFuncPtr)0x7A4E50, "O SPRING B" } /* "O SPRING B" */,
{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x79F860, "O SPRING H" } /* "O SPRING H" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF190, "E UNI A" } /* "E UNI A" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4AF500, "E UNI B" } /* "E UNI B" */,
{ 2, 3, 5, 250000, 0, (ObjectFuncPtr)0x4AF860, "E UNI C" } /* "E UNI C" */,
{ 2, 3, 5, 360000, 0, (ObjectFuncPtr)0x4B3210, "E POLICE" } /* "E POLICE" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjBoxW, "OBJ BOWX" } /* "Wooden Box" 41*/,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&HCWARP, "HC WARP" } /* "HC Warp" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCDOOR, "HC DOOR" } /* "HC Door" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCWALL, "HC WALL" } /* "HC Wall" */,
{ 2, 3, 1, 2400000, 0, (ObjectFuncPtr)&HCPLATFORM, "HC PLAT" } /* "HC Moving Platform" 45*/,
{ 2, 3, 1, 1360000, 0, (ObjectFuncPtr)&ObjCannon, "CANNON" } /* "SH Cannon" */,
{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)&SHCameraSwitch, "SH CAM SW" } /* "Camera flag switcher" */,
{ 2, 3, 1, 1560000, 0, (ObjectFuncPtr)&OBJCASE, "OBJ CASE" } /* "Heroes switch case" */,
{ 2, 3, 1, 1000000, 0, (ObjectFuncPtr)&ObjReel, "Reel" } /* "Heroes reels" */,
{ 2, 3, 1, 1060000, 0, &ObjFan, "ObjFan" } /* "SH FANS" 50 */,
{ 2, 3, 1, 1060000, 0, &MysticFan, "MysticFan" } /* "MM FANS" */,
{ 2, 3, 1, 1060000, 0, &MysticSphere, "MysticSphere" } /* "MM SPHERE" */,
};
ObjectList MysticMansionObjectList = { arraylengthandptrT(MysticMansionObjectList_list, int) };

void MysticMansionObjects_Init() {
	WriteData((PVMEntry**)0x90EB90, MysticMansionObjectTextures);
	WriteData((ObjectList**)0x974C38, &MysticMansionObjectList); //974C3C 974C40
}