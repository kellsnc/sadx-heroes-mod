#include "stdafx.h"

ModelInfo* BigMdls[3];
AnimationFile* BigAnms[63];
AnimData HBigAnimData[63];

NJS_TEXNAME HBIG_TEXNAMES[7];
NJS_TEXLIST HBIG_TEXLIST = { arrayptrandlength(HBIG_TEXNAMES) };

NJS_MATRIX BigMatrix;

CollisionData Lure_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, { 5.5, 0.0f, 0.0f }, 0, 0 };

void PlayVoice_Big(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayCustomSound(BigSound_Win);
		break;
	}
}

void PlaySound_Big(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayCustomSound(BigSound_Attack);
		break;
	case 1232:
		PlayCustomSound(BigSound_Hurt2);
		break;
	case 1233:
		PlayCustomSound(BigSound_Hurt1);
		break;
	case 1503:
		PlayCustomSound(BigSound_Death);
		break;
	}
}

void BigCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)BigMdls[0]->getdata("Dummy034")) {
		memcpy(BigMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //hand
	} else if (object == (NJS_OBJECT*)BigMdls[0]->getdata("Dummy039")) {
		njRotateX(0, 0x4000);
		DrawChunkModel(BigMdls[2]->getmodel()->chunkmodel);
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

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	NJS_OBJECT* mdl = BigMdls[1]->getmodel()->child->child;
	DrawChunkModel(mdl->chunkmodel);

	Direct3D_UnsetChunkModelRenderState();
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

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = BigCallback;
	njCnkAction(HBigAnimData[knucklesobj->Data1->Index].Animation, knucklesobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, BigMatrix, sizeof(NJS_MATRIX));
	njRotateX(0, 0xC000);
	if (knucklesobj->Data1->Index == 19) DrawChunkModel(BigMdls[1]->getmodel()->chunkmodel);
	if (knucklesobj->Data1->Index == 9 || knucklesobj->Data1->Index == 11 || knucklesobj->Data1->Index == 12) DrawChunkModel(BigMdls[1]->getmodel()->child ->chunkmodel);

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void BigHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "bigh", &HBIG_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[15].texaddr = HBIG_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerdata->CollisionInfo->CollisionArray->scale.x = 8;
			playerco2->PhysicsData.CollisionSize = 8;
			playerco2->PhysicsData.RippleSize = 0.5f;
			playerco2->_struct_a3.ShadowScale = 2;

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
				PlayCustomSound(BigSound_Attack);
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayCustomSound(BigSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayCustomSound(BigSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state

		PlayHeroesAnimation(obj, anim.x, HBigAnimData, anim.y, anim.z);

		break;
	case 3:
		switch (PowerLaunchTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayCustomSound(BigSound_Combo1);
			PlayHeroesAnimation(obj, 11, HBigAnimData, 0, 0);
			break;
		case 2:
			PlayCustomSound(BigSound_Combo2);
			PlayHeroesAnimation(obj, 9, HBigAnimData, 0, 0);
			break;
		case 3:
			PlayCustomSound(BigSound_Combo3);
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
			if (data->field_A != 2) PlayCustomSound(CommonSound_FlyPunchHit);
			data->field_A = 2;
		}
		else {
			if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayCustomSound(BigSound_Trick);
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
	BigMdls[0] = LoadCharacterModel("big_main");
	BigMdls[1] = LoadCharacterModel("big_rod");
	BigMdls[2] = LoadCharacterModel("big_buckle");

	NJS_OBJECT* buckle = (NJS_OBJECT*)BigMdls[0]->getdata("Dummy039");
	buckle->evalflags |= NJD_EVAL_HIDE;

	BigAnms[0] = LoadCharacterAnim("BI_WALK");
	BigAnms[1] = LoadCharacterAnim("BI_WALK_PULL");
	BigAnms[2] = LoadCharacterAnim("BI_WALK_PUSH");
	BigAnms[3] = LoadCharacterAnim("BI_TURN_L");
	BigAnms[4] = LoadCharacterAnim("BI_TURN_R");
	BigAnms[5] = LoadCharacterAnim("BI_SLOW_RUN");
	BigAnms[6] = LoadCharacterAnim("BI_MID_RUN");
	BigAnms[7] = LoadCharacterAnim("BI_TOP_RUN");
	BigAnms[8] = LoadCharacterAnim("BI_START");
	BigAnms[9] = LoadCharacterAnim("BI_ATC_BATT");
	BigAnms[10] = LoadCharacterAnim("BI_ATC_PRESS");
	BigAnms[11] = LoadCharacterAnim("BI_ATC_START");
	BigAnms[12] = LoadCharacterAnim("BI_FISH");
	BigAnms[13] = LoadCharacterAnim("BI_JUMP_A");
	BigAnms[14] = LoadCharacterAnim("BI_JUMP_B");
	BigAnms[15] = LoadCharacterAnim("BI_JUMP_C");
	BigAnms[16] = LoadCharacterAnim("BI_JUMP_D");
	BigAnms[17] = LoadCharacterAnim("BI_JUMP_E");
	BigAnms[18] = LoadCharacterAnim("BI_JUMP_F");
	BigAnms[19] = LoadCharacterAnim("BI_JUMP_UMBRELLA");
	BigAnms[20] = LoadCharacterAnim("BI_JUMP_GLIND");
	BigAnms[21] = LoadCharacterAnim("BI_GLIND");
	BigAnms[22] = LoadCharacterAnim("BI_GLIND_BK");
	BigAnms[23] = LoadCharacterAnim("BI_GLIND_BK_L");
	BigAnms[24] = LoadCharacterAnim("BI_GLIND_BK_R");
	BigAnms[25] = LoadCharacterAnim("BI_GLIND_FLIP_BK");
	BigAnms[26] = LoadCharacterAnim("BI_GLIND_FLIP_FR");
	BigAnms[27] = LoadCharacterAnim("BI_GLIND_L");
	BigAnms[28] = LoadCharacterAnim("BI_GLIND_R");
	BigAnms[29] = LoadCharacterAnim("BI_FLY_IDLE");
	BigAnms[30] = LoadCharacterAnim("BI_FLY_SLOW");
	BigAnms[31] = LoadCharacterAnim("BI_FLY_GLIND");
	BigAnms[32] = LoadCharacterAnim("BI_HANG_OFF");
	BigAnms[33] = LoadCharacterAnim("BI_HANG_ON");
	BigAnms[34] = LoadCharacterAnim("BI_BREAK_A");
	BigAnms[35] = LoadCharacterAnim("BI_BREAK_B");
	BigAnms[36] = LoadCharacterAnim("BI_BREAK_C");
	BigAnms[37] = LoadCharacterAnim("BI_BREAK_TURN_L");
	BigAnms[38] = LoadCharacterAnim("BI_BREAK_TURN_R");
	BigAnms[39] = LoadCharacterAnim("BI_BRA_MID");
	BigAnms[40] = LoadCharacterAnim("BI_BRA_TOP");
	BigAnms[41] = LoadCharacterAnim("BI_FLORT");
	BigAnms[42] = LoadCharacterAnim("BI_DAM_M_A");
	BigAnms[43] = LoadCharacterAnim("BI_DAM_M_B");
	BigAnms[44] = LoadCharacterAnim("BI_DAM_M_C");
	BigAnms[45] = LoadCharacterAnim("BI_EDGE_OTTO_A");
	BigAnms[46] = LoadCharacterAnim("BI_EDGE_OTTO_B");
	BigAnms[47] = LoadCharacterAnim("BI_EDGE_OTTO_C");
	BigAnms[48] = LoadCharacterAnim("BI_EDGE_OTTO_C");
	BigAnms[49] = LoadCharacterAnim("BI_FW_JUMP");
	BigAnms[50] = LoadCharacterAnim("BI_TRAP_JUMP");
	BigAnms[51] = LoadCharacterAnim("ROSE_BI");
	BigAnms[52] = LoadCharacterAnim("BI_WIN");
	BigAnms[53] = LoadCharacterAnim("BI_JUMP_BATT");
	BigAnms[54] = LoadCharacterAnim("BI_IDLE_HALF");
	BigAnms[55] = LoadCharacterAnim("BI_IDLE_B_HALF");
	BigAnms[56] = LoadCharacterAnim("BI_IDLE_C_HALF");
	BigAnms[57] = LoadCharacterAnim("BI_BOB");
	BigAnms[58] = LoadCharacterAnim("BI_BOB_L");
	BigAnms[59] = LoadCharacterAnim("BI_BOB_R");
	BigAnms[60] = LoadCharacterAnim("BI_START_IDLE");
	BigAnms[61] = LoadCharacterAnim("BI_GLIND_DASH");
	BigAnms[62] = LoadCharacterAnim("BI_GLIND_BK_DASH");

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
	FreeMDLFiles(BigMdls, LengthOfArray(BigMdls));
	FreeANMFiles(BigAnms, LengthOfArray(BigAnms));
}