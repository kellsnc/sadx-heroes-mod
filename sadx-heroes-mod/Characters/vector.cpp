#include "stdafx.h"

ModelInfo* VectorMdls[3];
AnimationFile* VectorAnms[63];
AnimData VectorAnimData[63];

NJS_TEXNAME VECTOR_TEXNAMES[7];
NJS_TEXLIST VECTOR_TEXLIST = { arrayptrandlength(VECTOR_TEXNAMES) };

NJS_MATRIX VectorMatrices[2];

void PlayVoice_Vector(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayCustomSound(VectorSound_Win);
		break;
	}
}

void PlaySound_Vector(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayCustomSound(VectorSound_Attack);
		break;
	case 1232:
		PlayCustomSound(VectorSound_Hurt2);
		break;
	case 1233:
		PlayCustomSound(VectorSound_Hurt1);
		break;
	case 1503:
		PlayCustomSound(VectorSound_Death);
		break;
	}
}

void VectorCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)VectorMdls[0]->getdata("Dummy007")) {
		memcpy(VectorMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //eyelids
	}
	else if (object == (NJS_OBJECT*)VectorMdls[0]->getdata("Dummy041")) {
		memcpy(VectorMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //gum
	}
	//else if (object == (NJS_OBJECT*)VectorMdls[0]->getdata("Dummy037")) { 
	//	memcpy(VectorMatrices[2], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //listeners
	//}
}

void VectorHeroes_Display(ObjectMaster *obj) {
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

	njSetTexture(&VECTOR_TEXLIST);

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

	*NodeCallbackFuncPtr = VectorCallback;
	njCnkAction(VectorAnimData[knucklesobj->Data1->Index].Animation, knucklesobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	//memcpy(_nj_current_matrix_ptr_, VectorMatrices[2], sizeof(NJS_MATRIX));
	//njRotateX(0, 0xC000);
	//DrawChunkModel(VectorMdls[2]->getmodel()->chunkmodel);

	memcpy(_nj_current_matrix_ptr_, VectorMatrices[0], sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* pupils = VectorMdls[1]->getmodel();

	switch (knucklesobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		DrawChunkModel(pupils->chunkmodel);
		break;
	case 2:
	case 6:
		DrawChunkModel(pupils->child->chunkmodel);
		break;
	case 3:
	case 5:
		DrawChunkModel(pupils->child->child->chunkmodel);
		break;
	case 4:
		DrawChunkModel(pupils->child->child->child->chunkmodel);
		break;
	}

	if (entity1->Action == 11) {
		memcpy(_nj_current_matrix_ptr_, VectorMatrices[1], sizeof(NJS_MATRIX));
		njTranslate(0, 0, -2, -7);
 		DrawChunkModel(VectorMdls[2]->getmodel()->child->chunkmodel);
	}
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void VectorHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "vector", &VECTOR_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[15].texaddr = VECTOR_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerdata->CollisionInfo->CollisionArray->scale.x = 8;
			playerco2->PhysicsData.CollisionSize = 8;
			playerco2->PhysicsData.RippleSize = 0.5f;
			playerco2->_struct_a3.ShadowScale = 1.5f;

			playerco2->PhysicsData.HSpeedCap = 15;
			playerco2->PhysicsData.MaxAccel = 3.5f;
			playerco2->PhysicsData.field_14 = 0.85f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = VectorHeroes_Display;

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
				PlayCustomSound(VectorSound_Attack);
				data->Action = 4;
			}
		}
		
		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayCustomSound(VectorSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayCustomSound(VectorSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state

		PlayHeroesAnimation(obj, anim.x, VectorAnimData, anim.y, anim.z);

		break;
	case 3:
		switch (PowerLaunchTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayCustomSound(VectorSound_Combo1);
			PlayHeroesAnimation(obj, 11, VectorAnimData, 0, 0);
			break;
		case 2:
			PlayCustomSound(VectorSound_Combo2);
			PlayHeroesAnimation(obj, 9, VectorAnimData, 0, 0);
			break;
		case 3:
			PlayCustomSound(VectorSound_Combo3);
			playerco2->Speed.x = 1;
		case 4:
			playerco2->Speed.x = 0;

			ExploseEnemies(&playerdata->Position, 5);

			if (data->Scale.x > 86) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				data->Action = 2;
			}

			PlayHeroesAnimation(obj, 12, VectorAnimData, 1, 0);
			break;
		default:
			PlayHeroesAnimation(obj, data->Index, VectorAnimData, 0, 0);
			break;
		}

		break;
	case 4:
		if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 30) {
			if (data->field_A != 2) PlayCustomSound(CommonSound_FlyPunchHit);
			data->field_A = 2;
		}
		else {
			if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayCustomSound(VectorSound_Trick);
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
			if (data->Scale.x > 44) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;

				ExploseEnemies(&playerdata->Position, 3);
				data->Action = 2;
			}

		}

		PlayHeroesAnimation(obj, 10, VectorAnimData, 0, 0);
		break;
	}

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 7) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadVectorFiles() {
	VectorMdls[0] = LoadCharacterModel("vector_main");
	VectorMdls[1] = LoadCharacterModel("vector_eyelids");
	VectorMdls[2] = LoadCharacterModel("vector_objs");

	//NJS_OBJECT* listeners = (NJS_OBJECT*)VectorMdls[0]->getdata("Dummy037");
	//listeners->evalflags |= NJD_EVAL_HIDE;

	VectorAnms[0] = LoadCharacterAnim("VE_WALK");
	VectorAnms[1] = LoadCharacterAnim("VE_WALK_PULL");
	VectorAnms[2] = LoadCharacterAnim("VE_WALK_PUSH");
	VectorAnms[3] = LoadCharacterAnim("VE_TURN_L");
	VectorAnms[4] = LoadCharacterAnim("VE_TURN_R");
	VectorAnms[5] = LoadCharacterAnim("VE_SLOW_RUN");
	VectorAnms[6] = LoadCharacterAnim("VE_MID_RUN");
	VectorAnms[7] = LoadCharacterAnim("VE_TOP_RUN");
	VectorAnms[8] = LoadCharacterAnim("VE_START");
	VectorAnms[9] = LoadCharacterAnim("VE_JUMP_ATC");
	VectorAnms[10] = LoadCharacterAnim("VE_ATC_DOWN");
	VectorAnms[11] = LoadCharacterAnim("VE_ATC_A");
	VectorAnms[12] = LoadCharacterAnim("VE_ATC_B");
	VectorAnms[13] = LoadCharacterAnim("VE_JUMP_A");
	VectorAnms[14] = LoadCharacterAnim("VE_JUMP_B");
	VectorAnms[15] = LoadCharacterAnim("VE_JUMP_C");
	VectorAnms[16] = LoadCharacterAnim("VE_JUMP_D");
	VectorAnms[17] = LoadCharacterAnim("VE_JUMP_E");
	VectorAnms[18] = LoadCharacterAnim("VE_JUMP_F");
	VectorAnms[19] = LoadCharacterAnim("VE_GUM");
	VectorAnms[20] = LoadCharacterAnim("VE_JUMP_GLIND");
	VectorAnms[21] = LoadCharacterAnim("VE_GLIND");
	VectorAnms[22] = LoadCharacterAnim("VE_GLIND_BK");
	VectorAnms[23] = LoadCharacterAnim("VE_GLIND_BK_L");
	VectorAnms[24] = LoadCharacterAnim("VE_GLIND_BK_R");
	VectorAnms[25] = LoadCharacterAnim("VE_GLIND_FLIP_B");
	VectorAnms[26] = LoadCharacterAnim("VE_GLIND_FLIP_F");
	VectorAnms[27] = LoadCharacterAnim("VE_GLIND_L");
	VectorAnms[28] = LoadCharacterAnim("VE_GLIND_R");
	VectorAnms[29] = LoadCharacterAnim("VE_FLY_IDLE");
	VectorAnms[30] = LoadCharacterAnim("VE_FLY_SLOW");
	VectorAnms[31] = LoadCharacterAnim("VE_FLY_GLIND");
	VectorAnms[32] = LoadCharacterAnim("VE_HANG_OFF");
	VectorAnms[33] = LoadCharacterAnim("VE_HANG_ON");
	VectorAnms[34] = LoadCharacterAnim("VE_BREAK_A");
	VectorAnms[35] = LoadCharacterAnim("VE_BREAK_B");
	VectorAnms[36] = LoadCharacterAnim("VE_BREAK_C");
	VectorAnms[37] = LoadCharacterAnim("VE_BREAK_TURN_L");
	VectorAnms[38] = LoadCharacterAnim("VE_BREAK_TURN_R");
	VectorAnms[39] = LoadCharacterAnim("VE_BRA_MID");
	VectorAnms[40] = LoadCharacterAnim("VE_BRA_TOP");
	VectorAnms[41] = LoadCharacterAnim("VE_FLORT");
	VectorAnms[42] = LoadCharacterAnim("VE_DAM_M_A");
	VectorAnms[43] = LoadCharacterAnim("VE_DAM_M_B");
	VectorAnms[44] = LoadCharacterAnim("VE_DAM_M_C");
	VectorAnms[45] = LoadCharacterAnim("VE_EDGE_OTTO_A");
	VectorAnms[46] = LoadCharacterAnim("VE_EDGE_OTTO_B");
	VectorAnms[47] = LoadCharacterAnim("VE_EDGE_OTTO_C");
	VectorAnms[48] = LoadCharacterAnim("CAO_MIC");
	VectorAnms[49] = LoadCharacterAnim("VE_FW_JUMP");
	VectorAnms[50] = LoadCharacterAnim("VE_TRAP_JUMP");
	VectorAnms[51] = LoadCharacterAnim("CAO_VE");
	VectorAnms[52] = LoadCharacterAnim("VE_WIN");
	VectorAnms[53] = LoadCharacterAnim("VE_ATC_A");
	VectorAnms[54] = LoadCharacterAnim("VE_IDLE_HALF");
	VectorAnms[55] = LoadCharacterAnim("VE_IDLE_B_HALF");
	VectorAnms[56] = LoadCharacterAnim("VE_IDLE_C_HALF");
	VectorAnms[57] = LoadCharacterAnim("VE_BOB");
	VectorAnms[58] = LoadCharacterAnim("VE_BOB_L");
	VectorAnms[59] = LoadCharacterAnim("VE_BOB_R");
	VectorAnms[60] = LoadCharacterAnim("VE_START_IDLE");
	VectorAnms[61] = LoadCharacterAnim("VE_GLIND_DASH");
	VectorAnms[62] = LoadCharacterAnim("VE_GLIND_BK_DASH");

	for (uint8_t i = 0; i < LengthOfArray(VectorAnimData); ++i) {
		if (VectorAnms[i] == nullptr) continue;
		VectorAnimData[i].Animation = new NJS_ACTION;
		VectorAnimData[i].Animation->object = VectorMdls[0]->getmodel();
		VectorAnimData[i].Animation->motion = VectorAnms[i]->getmotion();
		VectorAnimData[i].NextAnim = i;
		VectorAnimData[i].AnimationSpeed = 0.5f;
	}

	VectorAnimData[13].NextAnim = 14;
	VectorAnimData[15].NextAnim = 16;
	VectorAnimData[18].NextAnim = 19;
	VectorAnimData[42].Property = 1;
	VectorAnimData[43].Property = 1;
	VectorAnimData[27].Property = 1;
	VectorAnimData[33].Property = 1;
	VectorAnimData[28].Property = 1;
	VectorAnimData[20].Property = 1;
	VectorAnimData[25].Property = 1;
	VectorAnimData[26].Property = 1;
	VectorAnimData[44].NextAnim = 17;
	VectorAnimData[52].Property = 1;
}

void UnloadVectorFiles() {
	FreeMDLFiles(VectorMdls, LengthOfArray(VectorMdls));
	FreeANMFiles(VectorAnms, LengthOfArray(VectorAnms));
}