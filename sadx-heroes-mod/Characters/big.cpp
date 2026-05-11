#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* BigMdls[4];
AnimationFile* BigAnms[63];
AnimData HBigAnimData[63];

NJS_TEXNAME HBIG_TEXNAMES[7];
NJS_TEXLIST HBIG_TEXLIST = { arrayptrandlength(HBIG_TEXNAMES) };

NJS_MATRIX BigMatrix;

CollisionData Lure_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, 5.5, 0.0f, 0.0f };

void PlayVoice_Big(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(BigSound_Win);
		break;
	}
}

void PlaySound_Big(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(BigSound_Attack);
		break;
	case 1232:
		PlayHeroesSound(BigSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(BigSound_Hurt1);
		break;
	case 1503:
		PlayHeroesSound(BigSound_Death);
		break;
	}
}

void BigCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = BigMdls[0]->getmodel();

	if (object == base->getnode(30)) {
		njGetMatrix(BigMatrix); //hand
	}
}

void LureObj_Display(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HBIG_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &data->Position);
	njRotateZ(0, data->Rotation.z);
	njRotateX(0, data->Rotation.x);
	njRotateY(0, -data->Rotation.y - 0x4000);
	njScale(0, data->Scale.x, data->Scale.x, data->Scale.x);

	njRotateX(0, 0x4000);

	dsDrawModel(BigMdls[3]->getmodel()->getbasicdxmodel());

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void LureObj_Main(ObjectMaster *obj) {
	ObjectMaster* HeroesObj = obj->Parent;
	EntityData1* data = HeroesObj->Data1;
	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];

	obj->Data1->Position = playerobj->Data1->Position;
	obj->Data1->Rotation = playerobj->Data1->Rotation;
	NJS_VECTOR dir = { 0, 40, 0 };
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &obj->Data1->Position);
	njRotateZ(0, obj->Data1->Rotation.z);
	njRotateX(0, obj->Data1->Rotation.x);
	njRotateY(0, -obj->Data1->Rotation.y - 0x4000);
	njCalcPoint(0, &dir, &obj->Data1->Position);
	njPopMatrix(1u);
	
	switch (obj->Data1->Action) {
	case 0:
		obj->DisplaySub = LureObj_Display;
		Collision_Init(obj, &Lure_Col, 1, 3u);
		obj->Data1->Action = 1;
		break;
	case 1:
		if (data->field_A != 2) {
			if (obj->Data1->Scale.y < 45) obj->Data1->Scale.y += 0.1f;
			if (obj->Data1->Scale.x < 0.2f) obj->Data1->Scale.x += 0.005f;
		}
		else {
			if (obj->Data1->Scale.y > 0) obj->Data1->Scale.y -= 0.04f;
			if (obj->Data1->Scale.x > 0) obj->Data1->Scale.x -= 0.005f;
			else DeleteObject_(obj);
		}
		
		obj->Data1->Scale.z -= 0x500;

		dir = { obj->Data1->Scale.y, -(obj->Data1->Scale.y / 2), 0 };
		njPushMatrix(_nj_unit_matrix_);
		njTranslateV(0, &obj->Data1->Position);
		njRotateZ(0, obj->Data1->Rotation.z);
		njRotateX(0, obj->Data1->Rotation.x);
		njRotateY(0, -obj->Data1->Scale.z - 0x4000);
		njCalcPoint(0, &dir, &obj->Data1->Position);
		njPopMatrix(1u);

		ExploseEnemies(&obj->Data1->Position, 5);

		break;
	}

	obj->DisplaySub(obj);
}

void BigHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* knucklesobj = HeroesChars[obj->Data1->CharIndex];
	if (!knucklesobj) return;
	EntityData2* data2 = (EntityData2*)knucklesobj->Data2;

	WriteData<5>((void*)0x472649, 0x90);
	WriteData<5>((void*)0x47258B, 0x90);
	WriteData<5>((void*)0x472626, 0x90);
	Knuckles_Display(obj);
	WriteCall((void*)0x472649, njAction);
	WriteCall((void*)0x47258B, njAction);
	WriteCall((void*)0x472626, njAction);

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (knucklesobj->Data1->Index == 42 || knucklesobj->Data1->Index == 43 || knucklesobj->Data1->Index == 44) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (knucklesobj->Data1->NextAction == 0) knucklesobj->Data1->NextAction = 1;
			else knucklesobj->Data1->NextAction = 0;
		}
	}
	else {
		knucklesobj->Data1->NextAction = 0;
	}

	if (knucklesobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HBIG_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);

	if (entity1->Action == 19) {
		if (data2->field_38 > -6) data2->field_38 -= 0.1f;
		njTranslate(0, 0, data2->field_38, 0);
	}
	else if (entity1->Action == 20 || entity1->Action == 21) {
		data2->field_38 = 0;
	}

	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);
	if (entity1->Action == 19) njRotateX(0, -0x1000);

	if (entity1->Action == 24) {
		njRotateX(0, -0x4000);
		njTranslate(0, 0, 0, 2);
	}

	if (knucklesobj->Data1->Index == 52) {
		njRotateY(0, 0xC000);
	}

	njRotateX(0, 0x4000);

	*NodeCallbackFuncPtr = BigCallback;
	njActionWeight(HBigAnimData[knucklesobj->Data1->Index].Animation, knucklesobj->Data1->Scale.x, BigMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, BigMatrix, sizeof(NJS_MATRIX));
	njRotateX(0, 0xC000);
	if (knucklesobj->Data1->Index == 19) dsDrawModel(BigMdls[2]->getmodel()->getbasicdxmodel());
	if (knucklesobj->Data1->Index == 9 || knucklesobj->Data1->Index == 11 || knucklesobj->Data1->Index == 12) dsDrawModel(BigMdls[1]->getmodel()->getbasicdxmodel());

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void BigHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "heroes-big", &HBIG_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[15].texaddr = HBIG_TEXLIST.textures[8].texaddr;
		}

		if (CustomPhysics) {
			playerdata->CollisionInfo->CollisionArray->a = 8.0f;
			playerco2->PhysicsData.CollisionSize = 8.0f;
			playerco2->PhysicsData.RippleSize = 0.5f;
			playerco2->_struct_a3.ShadowScale = 2.0f;

			playerco2->PhysicsData.HSpeedCap = 15;
			playerco2->PhysicsData.MaxAccel = 3.5f;
			playerco2->PhysicsData.field_14 = 0.85f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = BigHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground && (HeldButtons2[data->CharIndex] & Buttons_A) != Buttons_A) {
				data->field_A = 0;
				playerdata->Action = 2;
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				data->Scale.y = 0;
				PlayHeroesSound(BigSound_Attack);
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(BigSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(BigSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		{
			NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, HBigAnimData, anim.y, anim.z);
		}

		break;
	case 3:
		switch (PowerLaunchTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayHeroesSound(BigSound_Combo1);
			PlayHeroesAnimation(obj, 11, HBigAnimData, 0, 0);
			break;
		case 2:
			PlayHeroesSound(BigSound_Combo2);
			PlayHeroesAnimation(obj, 9, HBigAnimData, 0, 0);
			break;
		case 3:
			PlayHeroesSound(BigSound_Combo3);
			data->Scale.y = 73;
			playerco2->Speed.x = 1;
		case 4:
			data->Scale.y -= 0.25f;
			playerco2->Speed.x = 0;

			if (data->Scale.y < 50) {
				data2->field_30 = 0;
				data2->field_30 = 4;
				LoadChildObject(LoadObj_Data1, LureObj_Main, obj);
			}
			
			data->Scale.x = data->Scale.y;
			PlayHeroesAnimation(obj, 12, HBigAnimData, 0, 0);
			break;
		case 5:
			playerco2->Speed.x = 0;

			if (data->field_A == 0) {
				if (data->Scale.y != 0) data->Scale.y -= 0.25f;
				else data->field_A = 1;
			}
			else if (data->field_A == 1) {
				if (data->Scale.y != 40) data->Scale.y += 0.25f;
				else data->field_A = 2;
			}

			if (data->field_A == 2 || PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 2;
				data->Scale.y += 0.25f;
				if (data->Scale.y > 72 || PressedButtons[data->CharIndex] & Buttons_X) {
					if (obj->Child) DeleteChildObjects(obj);
					data2->field_30 = 0;
					playerco2->Powerups &= ~Powerups_Invincibility;
					data->Action = 2;
				}
			}

			data->Scale.x = data->Scale.y;
			PlayHeroesAnimation(obj, 12, HBigAnimData, 0, 0);
			break;
		default:

			PlayHeroesAnimation(obj, data->Index, HBigAnimData, 0, 0);
			break;
		}
		
		RunObjectChildren(obj);

		break;
	case 4:
		if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) {
			if (data->field_A != 2) PlayHeroesSound(CommonSound_FlyPunchHit);
			data->field_A = 2;
		}
		else {
			if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayHeroesSound(BigSound_Trick);
		}
		
		if (data->field_A != 2) {
			if (data2->field_30 == 0) {
				if (data->Scale.y != 10) data->Scale.y += 0.25f;
				else data2->field_30 = 1;
			}
			else if (data2->field_30 == 1) {
				if (data->Scale.y != 0) data->Scale.y -= 0.25f;
				else data2->field_30 = 0;
			}
			data->Scale.x = data->Scale.y;
		}
		else {
			if (data->Scale.x > 28) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				
				ExploseEnemies(&playerdata->Position, 3);
				data->Action = 2;
			}
			
		}

		PlayHeroesAnimation(obj, 10, HBigAnimData, 0, 0);
		break;
	}

	RunObjectChildren(obj);
}

void LoadBigFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-big.arcx"));

	BigMdls[0] = arc.GetModel("BIG_LOCATOR.sa1mdl");
	BigMdls[1] = arc.GetModel("FISH_STICK.sa1mdl");
	BigMdls[2] = arc.GetModel("UMBRELLA.sa1mdl");
	BigMdls[3] = arc.GetModel("UKI.sa1mdl");
	//BigMdls[4] = arc.GetModel("UKIWA.sa1mdl");
	//BigMdls[5] = arc.GetModel("UNI.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(BigMdls[0]->getweightinfo(), BigMdls[0]->getmodel());

	BigAnms[0] = arc.GetAnimation("BI_WALK.saanim");
	BigAnms[1] = arc.GetAnimation("BI_WALK_PULL.saanim");
	BigAnms[2] = arc.GetAnimation("BI_WALK_PUSH.saanim");
	BigAnms[3] = arc.GetAnimation("BI_TURN_L.saanim");
	BigAnms[4] = arc.GetAnimation("BI_TURN_R.saanim");
	BigAnms[5] = arc.GetAnimation("BI_SLOW_RUN.saanim");
	BigAnms[6] = arc.GetAnimation("BI_MID_RUN.saanim");
	BigAnms[7] = arc.GetAnimation("BI_TOP_RUN.saanim");
	BigAnms[8] = arc.GetAnimation("BI_START.saanim");
	BigAnms[9] = arc.GetAnimation("BI_ATC_BATT.saanim");
	BigAnms[10] = arc.GetAnimation("BI_ATC_PRESS.saanim");
	BigAnms[11] = arc.GetAnimation("BI_ATC_START.saanim");
	BigAnms[12] = arc.GetAnimation("BI_FISH.saanim");
	BigAnms[13] = arc.GetAnimation("BI_JUMP_A.saanim");
	BigAnms[14] = arc.GetAnimation("BI_JUMP_B.saanim");
	BigAnms[15] = arc.GetAnimation("BI_JUMP_C.saanim");
	BigAnms[16] = arc.GetAnimation("BI_JUMP_D.saanim");
	BigAnms[17] = arc.GetAnimation("BI_JUMP_E.saanim");
	BigAnms[18] = arc.GetAnimation("BI_JUMP_F.saanim");
	BigAnms[19] = arc.GetAnimation("BI_JUMP_UMBRELLA.saanim");
	BigAnms[20] = arc.GetAnimation("BI_JUMP_GLIND.saanim");
	BigAnms[21] = arc.GetAnimation("BI_GLIND.saanim");
	BigAnms[22] = arc.GetAnimation("BI_GLIND_BK.saanim");
	BigAnms[23] = arc.GetAnimation("BI_GLIND_BK_L.saanim");
	BigAnms[24] = arc.GetAnimation("BI_GLIND_BK_R.saanim");
	BigAnms[25] = arc.GetAnimation("BI_GLIND_FLIP_BK.saanim");
	BigAnms[26] = arc.GetAnimation("BI_GLIND_FLIP_FR.saanim");
	BigAnms[27] = arc.GetAnimation("BI_GLIND_L.saanim");
	BigAnms[28] = arc.GetAnimation("BI_GLIND_R.saanim");
	BigAnms[29] = arc.GetAnimation("BI_FLY_IDLE.saanim");
	BigAnms[30] = arc.GetAnimation("BI_FLY_SLOW.saanim");
	BigAnms[31] = arc.GetAnimation("BI_FLY_GLIND.saanim");
	BigAnms[32] = arc.GetAnimation("BI_HANG_OFF.saanim");
	BigAnms[33] = arc.GetAnimation("BI_HANG_ON.saanim");
	BigAnms[34] = arc.GetAnimation("BI_BREAK_A.saanim");
	BigAnms[35] = arc.GetAnimation("BI_BREAK_B.saanim");
	BigAnms[36] = arc.GetAnimation("BI_BREAK_C.saanim");
	BigAnms[37] = arc.GetAnimation("BI_BREAK_TURN_L.saanim");
	BigAnms[38] = arc.GetAnimation("BI_BREAK_TURN_R.saanim");
	BigAnms[39] = arc.GetAnimation("BI_BRA_MID.saanim");
	BigAnms[40] = arc.GetAnimation("BI_BRA_TOP.saanim");
	BigAnms[41] = arc.GetAnimation("BI_FLORT.saanim");
	BigAnms[42] = arc.GetAnimation("BI_DAM_M_A.saanim");
	BigAnms[43] = arc.GetAnimation("BI_DAM_M_B.saanim");
	BigAnms[44] = arc.GetAnimation("BI_DAM_M_C.saanim");
	BigAnms[45] = arc.GetAnimation("BI_EDGE_OTTO_A.saanim");
	BigAnms[46] = arc.GetAnimation("BI_EDGE_OTTO_B.saanim");
	BigAnms[47] = arc.GetAnimation("BI_EDGE_OTTO_C.saanim");
	BigAnms[48] = arc.GetAnimation("BI_EDGE_OTTO_C.saanim");
	BigAnms[49] = arc.GetAnimation("BI_FW_JUMP.saanim");
	BigAnms[50] = arc.GetAnimation("BI_TRAP_JUMP.saanim");
	BigAnms[51] = arc.GetAnimation("ROSE_BI.saanim");
	BigAnms[52] = arc.GetAnimation("BI_WIN.saanim");
	BigAnms[53] = arc.GetAnimation("BI_JUMP_BATT.saanim");
	BigAnms[54] = arc.GetAnimation("BI_IDLE_HALF.saanim");
	BigAnms[55] = arc.GetAnimation("BI_IDLE_B_HALF.saanim");
	BigAnms[56] = arc.GetAnimation("BI_IDLE_C_HALF.saanim");
	BigAnms[57] = arc.GetAnimation("BI_BOB.saanim");
	BigAnms[58] = arc.GetAnimation("BI_BOB_L.saanim");
	BigAnms[59] = arc.GetAnimation("BI_BOB_R.saanim");
	BigAnms[60] = arc.GetAnimation("BI_START_IDLE.saanim");
	BigAnms[61] = arc.GetAnimation("BI_GLIND_DASH.saanim");
	BigAnms[62] = arc.GetAnimation("BI_GLIND_BK_DASH.saanim");

	for (uint8_t i = 0; i < LengthOfArray(HBigAnimData); ++i) {
		if (BigAnms[i] == nullptr) continue;
		HBigAnimData[i].Animation = new NJS_ACTION;
		HBigAnimData[i].Animation->object = BigMdls[0]->getmodel();
		HBigAnimData[i].Animation->motion = BigAnms[i]->getmotion();
		HBigAnimData[i].NextAnim = i;
		HBigAnimData[i].AnimationSpeed = 0.5f;
	}

	HBigAnimData[13].NextAnim = 14;
	HBigAnimData[15].NextAnim = 16;
	HBigAnimData[18].NextAnim = 19;
	HBigAnimData[42].Property = 1;
	HBigAnimData[43].Property = 1;
	HBigAnimData[27].Property = 1;
	HBigAnimData[33].Property = 1;
	HBigAnimData[28].Property = 1;
	HBigAnimData[20].Property = 1;
	HBigAnimData[25].Property = 1;
	HBigAnimData[26].Property = 1;
	HBigAnimData[44].NextAnim = 17;
	HBigAnimData[52].Property = 1;
}

void UnloadBigFiles() {
	HelperFunctionsGlobal.Weights->DeInit(BigMdls[0]->getweightinfo(), BigMdls[0]->getmodel());
	FreeMDLFiles(BigMdls, LengthOfArray(BigMdls));
	FreeANMFiles(BigAnms, LengthOfArray(BigAnms));
}