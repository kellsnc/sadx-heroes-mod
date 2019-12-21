#include "stdafx.h"

ModelInfo* AmyMdls[3];
AnimationFile* AmyAnms[62];
AnimData HAmyAnimData[62];

NJS_TEXNAME HAMY_TEXNAMES[4];
NJS_TEXLIST HAMY_TEXLIST = { arrayptrandlength(HAMY_TEXNAMES) };

NJS_MATRIX AmyMatrices[2];

void PlayVoice_Amy(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(AmySound_Win);
		break;
	}
}

void PlaySound_Amy(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(AmySound_Jump);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(AmySound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(AmySound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(AmySound_Death);
		break;
	}
}

void AmyCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)AmyMdls[0]->getdata("Dummy006")) {
		memcpy(AmyMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //eyelashes
	}
	else if (object == (NJS_OBJECT*)AmyMdls[0]->getdata("Dummy011")) {
		memcpy(AmyMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //hammer
	}
}

void AmyHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* amyobj = HeroesChars[obj->Data1->CharIndex];
	if (!amyobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (amyobj->Data1->NextAction == 0) amyobj->Data1->NextAction = 1;
			else amyobj->Data1->NextAction = 0;
		}
	}
	else {
		amyobj->Data1->NextAction = 0;
	}

	if (amyobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HAMY_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	if (amyobj->Data1->Index == 52) {
		njTranslate(0, -10, 5, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = AmyCallback;
	njCnkAction(HAmyAnimData[amyobj->Data1->Index].Animation, amyobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, AmyMatrices[0], sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* pupils = AmyMdls[1]->getmodel();

	switch (amyobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		DrawChunkModel(pupils->child->chunkmodel);
		break;
	case 2:
	case 6:
		DrawChunkModel(pupils->child->child->chunkmodel);
		break;
	case 3:
	case 5:
		DrawChunkModel(pupils->child->child->child->chunkmodel);
		break;
	case 4:
		DrawChunkModel(pupils->child->child->child->child->chunkmodel);
		break;
	default:
		DrawChunkModel(pupils->chunkmodel);
		break;
	}
	
	if (amyobj->Data1->Index == 53 || amyobj->Data1->Index == 61) {
		memcpy(_nj_current_matrix_ptr_, AmyMatrices[1], sizeof(NJS_MATRIX));
		njTranslate(0, 0, 0, 0.5f);
		njRotateZ(0, 0xC000);
		DrawChunkModel(AmyMdls[2]->getmodel()->child->chunkmodel);
	}
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void AmyHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "amyh", &HAMY_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[12].texaddr = HAMY_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.HSpeedCap = 17;
			playerco2->PhysicsData.MaxAccel = 3.8f;
			playerco2->PhysicsData.field_14 = 0.90f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = AmyHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 5;
			PlayHeroesSound(AmySound_Ya);
			data->Action = 3;
			break;
		}

		if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
			data->field_A = 0;
			PlayHeroesSound(AmySound_Attack);
			data->Action = 4;
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(AmySound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(AmySound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
		
		PlayHeroesAnimation(obj, anim.x, HAmyAnimData, anim.y, anim.z);

		break;
	case 3:
		if (data->field_A == 0) {
			data->Scale.x = 0;
			data->field_A = 1;
			playerco2->Powerups |= Powerups_Invincibility;
		}
		else if (data->field_A < 119) {
			++data->field_A;
			
		}
		else {
			data->field_A = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
			playerdata->Action = 2;
			data->Action = 2;
		}

		PlayHeroesAnimation(obj, 53, HAmyAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, HAmyAnimData, 0, 0);
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

void LoadAmyFiles() {
	AmyMdls[0] = LoadCharacterModel("amy_main");
	AmyMdls[1] = LoadCharacterModel("amy_eyelashes");
	AmyMdls[2] = LoadCharacterModel("amy_hammer");

	AmyAnms[0] = LoadCharacterAnim("AM_WALK");
	AmyAnms[1] = LoadCharacterAnim("AM_WALK_PULL");
	AmyAnms[2] = LoadCharacterAnim("AM_WALK_PUSH");
	AmyAnms[3] = LoadCharacterAnim("AM_TURN_L_HALF");
	AmyAnms[4] = LoadCharacterAnim("AM_TURN_R_HALF");
	AmyAnms[5] = LoadCharacterAnim("AM_SLOW_RUN");
	AmyAnms[6] = LoadCharacterAnim("AM_MID_RUN");
	AmyAnms[7] = LoadCharacterAnim("AM_TOP_RUN");
	AmyAnms[8] = LoadCharacterAnim("AM_START");
	AmyAnms[9] = LoadCharacterAnim("AM_JUMP_WALL");
	AmyAnms[10] = LoadCharacterAnim("AM_JUMP_TRIC_A");
	AmyAnms[11] = LoadCharacterAnim("AM_JUMP_TRIC_B");
	AmyAnms[12] = LoadCharacterAnim("AM_JUMP_TRIC_C");
	AmyAnms[13] = LoadCharacterAnim("AM_JUMP_A");
	AmyAnms[14] = LoadCharacterAnim("AM_JUMP_B_HALF");
	AmyAnms[15] = LoadCharacterAnim("AM_JUMP_C");
	AmyAnms[16] = LoadCharacterAnim("AM_JUMP_D");
	AmyAnms[17] = LoadCharacterAnim("AM_JUMP_E");
	AmyAnms[18] = LoadCharacterAnim("AM_JUMP_F");
	AmyAnms[19] = LoadCharacterAnim("AM_JUMP_UMBRELLA");
	AmyAnms[20] = LoadCharacterAnim("AM_JUMP_GLIND");
	AmyAnms[21] = LoadCharacterAnim("AM_GLIND");
	AmyAnms[22] = LoadCharacterAnim("AM_GLIND_BK");
	AmyAnms[23] = LoadCharacterAnim("AM_GLIND_BK_L");
	AmyAnms[24] = LoadCharacterAnim("AM_GLIND_BK_R");
	AmyAnms[25] = LoadCharacterAnim("AM_GLIND_FLIP_B");
	AmyAnms[26] = LoadCharacterAnim("AM_GLIND_FLIP_F");
	AmyAnms[27] = LoadCharacterAnim("AM_GLIND_L");
	AmyAnms[28] = LoadCharacterAnim("AM_GLIND_R");
	AmyAnms[29] = LoadCharacterAnim("AM_FLY_IDLE");
	AmyAnms[30] = LoadCharacterAnim("AM_FLY_SLOW");
	AmyAnms[31] = LoadCharacterAnim("AM_FLY_GLIND");
	AmyAnms[32] = LoadCharacterAnim("AM_HANG_OFF");
	AmyAnms[33] = LoadCharacterAnim("AM_HANG_ON");
	AmyAnms[34] = LoadCharacterAnim("AM_BREAK_A");
	AmyAnms[35] = LoadCharacterAnim("AM_BREAK_B");
	AmyAnms[36] = LoadCharacterAnim("AM_BREAK_C");
	AmyAnms[37] = LoadCharacterAnim("AM_BREAK_TURN_L");
	AmyAnms[38] = LoadCharacterAnim("AM_BREAK_TURN_R");
	AmyAnms[39] = LoadCharacterAnim("AM_BRA_MID");
	AmyAnms[40] = LoadCharacterAnim("AM_BRA_TOP");
	AmyAnms[41] = LoadCharacterAnim("AM_FLORT");
	AmyAnms[42] = LoadCharacterAnim("AM_DAM_M_A");
	AmyAnms[43] = LoadCharacterAnim("AM_DAM_M_B");
	AmyAnms[44] = LoadCharacterAnim("AM_DAM_M_C");
	AmyAnms[45] = LoadCharacterAnim("AM_EDGE_OTTO_A");
	AmyAnms[46] = LoadCharacterAnim("AM_EDGE_OTTO_B");
	AmyAnms[47] = LoadCharacterAnim("AM_EDGE_OTTO_C");
	AmyAnms[48] = LoadCharacterAnim("AM_EDGE_OTTO_C");
	AmyAnms[49] = LoadCharacterAnim("AM_FW_JUMP");
	AmyAnms[50] = LoadCharacterAnim("AM_TRAP_JUMP");
	AmyAnms[51] = LoadCharacterAnim("AM_POW_ROT_HALF");
	AmyAnms[52] = LoadCharacterAnim("AM_WIN");
	AmyAnms[53] = LoadCharacterAnim("AM_ATC_HAM");
	AmyAnms[54] = LoadCharacterAnim("AM_IDLE_HALF");
	AmyAnms[55] = LoadCharacterAnim("AM_IDLE_B_HALF");
	AmyAnms[56] = LoadCharacterAnim("AM_IDLE_C_HALF");
	AmyAnms[57] = LoadCharacterAnim("ROSE_AM");
	AmyAnms[58] = LoadCharacterAnim("AM_IDLE_POW_HALF");
	AmyAnms[59] = LoadCharacterAnim("AM_IDLE_POW_SHAKE");
	AmyAnms[60] = LoadCharacterAnim("AM_START_IDLE");
	AmyAnms[61] = LoadCharacterAnim("AM_ATC_TRNADE");

	for (uint8_t i = 0; i < LengthOfArray(HAmyAnimData); ++i) {
		if (AmyAnms[i] == nullptr) continue;
		HAmyAnimData[i].Animation = new NJS_ACTION;
		HAmyAnimData[i].Animation->object = AmyMdls[0]->getmodel();
		HAmyAnimData[i].Animation->motion = AmyAnms[i]->getmotion();
		HAmyAnimData[i].NextAnim = i;
		HAmyAnimData[i].AnimationSpeed = 0.5f;
	}

	HAmyAnimData[13].NextAnim = 14;
	HAmyAnimData[15].NextAnim = 16;
	HAmyAnimData[18].NextAnim = 19;
	HAmyAnimData[42].Property = 1;
	HAmyAnimData[43].Property = 1;
	HAmyAnimData[27].Property = 1;
	HAmyAnimData[28].Property = 1;
	HAmyAnimData[20].Property = 1;
	HAmyAnimData[25].Property = 1;
	HAmyAnimData[26].Property = 1;
	HAmyAnimData[44].NextAnim = 17;
	HAmyAnimData[52].Property = 1;
}

void UnloadAmyFiles() {
	FreeMDLFiles(AmyMdls, LengthOfArray(AmyMdls));
	FreeANMFiles(AmyAnms, LengthOfArray(AmyAnms));
}