#include "stdafx.h"

ModelInfo* EspioMdls[5];
AnimationFile* EspioAnms[59];
AnimData EspioAnimData[59];

NJS_TEXNAME ESPIO_TEXNAMES[6];
NJS_TEXLIST ESPIO_TEXLIST = { arrayptrandlength(ESPIO_TEXNAMES) };

NJS_MATRIX EspioMatrix;

CollisionData Ninja_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, { 2.5, 0.0f, 0.0f }, 0, 0 };

void PlayVoice_Espio(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(EspioSound_Win);
		break;
	}
}

void PlaySound_Espio(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(EspioSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(EspioSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(EspioSound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(EspioSound_Death);
		break;
	}
}

void EspioCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)EspioMdls[0]->getdata("Dummy006")) {
		memcpy(EspioMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //eyelids
	}
}

void NinjaObj(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	NJS_VECTOR dir = { 0, 0, 0 };

	if (GameState != 16) {
		switch (data->Action) {
		case 0:
			obj->DisplaySub = obj->MainSub;
			data->Action = 1;

			dir.x = 10;
			dir.y = 5;
			njPushMatrix(_nj_unit_matrix_);
			njTranslateV(0, &data->Position);
			njRotateZ(0, data->Rotation.z);
			njRotateX(0, data->Rotation.x);
			njRotateY(0, -data->Rotation.y);
			njCalcPoint(0, &dir, &data->Position);
			njPopMatrix(1u);

			Collision_Init(obj, &Ninja_Col, 1, 3);
			break;
		case 1:
			dir.x = 5;
			njPushMatrix(_nj_unit_matrix_);
			njTranslateV(0, &data->Position);
			njRotateZ(0, data->Rotation.z);
			njRotateX(0, data->Rotation.x);
			njRotateY(0, -data->Rotation.y);
			njCalcPoint(0, &dir, &data->Position);
			njPopMatrix(1u);

			if (++data->InvulnerableTime > 300) {
				DeleteObject_(obj);
				return;
			}

			data->Scale.y -= 0x1000;
			AddToCollisionList(data);

			break;
		}
	}

	if (!MissedFrames) {
		njSetTexture(&ESPIO_TEXLIST);

		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateZ(0, data->Rotation.z);
		njRotateX(0, data->Rotation.x);
		njRotateY(0, data->Scale.y);
		njScale(0, 0.5f, 0.5f, 0.5f);
		SetupWorldMatrix();
		Direct3D_SetChunkModelRenderState();
		DrawChunkModel(EspioMdls[2]->getmodel()->child->chunkmodel);
		Direct3D_UnsetChunkModelRenderState();
		njPopMatrix(1);
	}
}

void EspioHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* espioobj = HeroesChars[obj->Data1->CharIndex];
	if (!espioobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (espioobj->Data1->NextAction == 0) espioobj->Data1->NextAction = 1;
			else espioobj->Data1->NextAction = 0;
		}
	}
	else {
		espioobj->Data1->NextAction = 0;
	}

	if (espioobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&ESPIO_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	if (espioobj->Data1->Index == 52) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = EspioCallback;
	njCnkAction(EspioAnimData[espioobj->Data1->Index].Animation, espioobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, EspioMatrix, sizeof(NJS_MATRIX));

	njRotateX(0, 0x4000);
	DrawChunkModel(EspioMdls[4]->getmodel()->chunkmodel);
	njRotateX(0, 0xC000);

	NJS_CNK_OBJECT* pupils = EspioMdls[1]->getmodel();
	switch (espioobj->Data1->InvulnerableTime) {
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
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void EspioHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "espio", &ESPIO_TEXLIST)) {
		return;
	}

	if (data->CharIndex == 0) {
		CON_REGULAR_TEXNAMES[12].texaddr = ESPIO_TEXLIST.textures[1].texaddr;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	playerco2->PhysicsData.HSpeedCap = 18;
	playerco2->PhysicsData.MaxAccel = 4.0f;
	playerco2->PhysicsData.field_14 = 0.95f;

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = EspioHeroes_Display;

		if (HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 5;
			PlayHeroesSound(EspioSound_Ya);
			data->Action = 3;
			break;
		}

		if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
			data->field_A = 0;
			PlayHeroesSound(EspioSound_Attack);
			data->Action = 4;
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(EspioSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(EspioSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
		
		if (anim.x != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}

		PlayHeroesAnimation(obj, anim.x, EspioAnimData, anim.y, anim.z);

		break;
	case 3:
		if (KickTrick(data, data2, playerco2, playerdata)) {
			ObjectMaster* obj = LoadObject(LoadObj_Data1, 3, NinjaObj);
			obj->Data1->Position = playerdata->Position;
			obj->Data1->Rotation = playerdata->Rotation;
			playerco2->Speed.x = 2;
		}

		PlayHeroesAnimation(obj, 53, EspioAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, EspioAnimData, 0, 0);
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

void LoadEspioFiles(const char *path, const HelperFunctions &helperFunctions) {
	EspioMdls[0] = LoadCharacterModel("espio_main");
	EspioMdls[1] = LoadCharacterModel("espio_eyelids");
	EspioMdls[2] = LoadCharacterModel("espio_attacks");
	EspioMdls[3] = LoadCharacterModel("espio_objs");
	EspioMdls[4] = LoadCharacterModel("espio_head");

	//the head lightning is wrong, since it's not weighted I replace it with a non weighted, working one
	NJS_OBJECT* head = (NJS_OBJECT*)EspioMdls[0]->getdata("Dummy006");
	head->evalflags |= NJD_EVAL_HIDE;

	EspioAnms[0] = LoadCharacterAnim("ES_WALK");
	EspioAnms[1] = LoadCharacterAnim("ES_WALK_PULL");
	EspioAnms[2] = LoadCharacterAnim("ES_WALK_PUSH");
	EspioAnms[3] = LoadCharacterAnim("ES_TURN_L");
	EspioAnms[4] = LoadCharacterAnim("ES_TURN_R");
	EspioAnms[5] = LoadCharacterAnim("ES_SLOW_RUN");
	EspioAnms[6] = LoadCharacterAnim("ES_MID_RUN");
	EspioAnms[7] = LoadCharacterAnim("ES_TOP_RUN");
	EspioAnms[8] = LoadCharacterAnim("ES_START");
	EspioAnms[9] = LoadCharacterAnim("ES_JUMP_WALL");
	EspioAnms[10] = LoadCharacterAnim("ES_JUMP_TRIC_A");
	EspioAnms[11] = LoadCharacterAnim("ES_JUMP_TRIC_B");
	EspioAnms[12] = LoadCharacterAnim("ES_JUMP_TRIC_C");
	EspioAnms[13] = LoadCharacterAnim("ES_JUMP_A");
	EspioAnms[14] = LoadCharacterAnim("ES_JUMP_B");
	EspioAnms[15] = LoadCharacterAnim("ES_JUMP_C");
	EspioAnms[16] = LoadCharacterAnim("ES_JUMP_D");
	EspioAnms[17] = LoadCharacterAnim("ES_JUMP_E");
	EspioAnms[18] = LoadCharacterAnim("ES_JUMP_F");
	EspioAnms[19] = LoadCharacterAnim("ES_JUMP_GUM");
	EspioAnms[20] = LoadCharacterAnim("ES_JUMP_GLIND");
	EspioAnms[21] = LoadCharacterAnim("ES_GLIND");
	EspioAnms[22] = LoadCharacterAnim("ES_GLIND_BK");
	EspioAnms[23] = LoadCharacterAnim("ES_GLIND_BK_L");
	EspioAnms[24] = LoadCharacterAnim("ES_GLIND_BK_R");
	EspioAnms[25] = LoadCharacterAnim("ES_GLIND_FLIP_B");
	EspioAnms[26] = LoadCharacterAnim("ES_GLIND_FLIP_FR");
	EspioAnms[27] = LoadCharacterAnim("ES_GLIND_L");
	EspioAnms[28] = LoadCharacterAnim("ES_GLIND_R");
	EspioAnms[29] = LoadCharacterAnim("ES_FLY_IDLE");
	EspioAnms[30] = LoadCharacterAnim("ES_FLY_SLOW");
	EspioAnms[31] = LoadCharacterAnim("ES_FLY_GLIND");
	EspioAnms[32] = LoadCharacterAnim("ES_HANG_OFF");
	EspioAnms[33] = LoadCharacterAnim("ES_HANG_ON");
	EspioAnms[34] = LoadCharacterAnim("ES_BREAK_A");
	EspioAnms[35] = LoadCharacterAnim("ES_BREAK_B");
	EspioAnms[36] = LoadCharacterAnim("ES_BREAK_C");
	EspioAnms[37] = LoadCharacterAnim("ES_BREAK_TURN_L");
	EspioAnms[38] = LoadCharacterAnim("ES_BREAK_TURN_R");
	EspioAnms[39] = LoadCharacterAnim("ES_BRA_MID");
	EspioAnms[40] = LoadCharacterAnim("ES_BRA_TOP");
	EspioAnms[41] = LoadCharacterAnim("ES_FLORT");
	EspioAnms[42] = LoadCharacterAnim("ES_DAM_MID_A");
	EspioAnms[43] = LoadCharacterAnim("ES_DAM_MID_B");
	EspioAnms[44] = LoadCharacterAnim("ES_DAM_MID_C");
	EspioAnms[45] = LoadCharacterAnim("ES_EDDGE_OTTO_A");
	EspioAnms[46] = LoadCharacterAnim("ES_EDDGE_OTTO_B");
	EspioAnms[47] = LoadCharacterAnim("ES_EDDGE_OTTO_C");
	EspioAnms[48] = LoadCharacterAnim("ES_EDDGE_OTTO_C");
	EspioAnms[49] = LoadCharacterAnim("ES_FW_JUMP");
	EspioAnms[50] = LoadCharacterAnim("ES_TRAP_JUMP");
	EspioAnms[51] = LoadCharacterAnim("ES_POW_ROT");
	EspioAnms[52] = LoadCharacterAnim("ES_WIN_B");
	EspioAnms[53] = LoadCharacterAnim("ES_ATC_SHURI");
	EspioAnms[54] = LoadCharacterAnim("ES_IDLE_HALF");
	EspioAnms[55] = LoadCharacterAnim("ES_IDLE_B_HALF");
	EspioAnms[56] = LoadCharacterAnim("ES_IDLE_C_HALF");
	EspioAnms[57] = LoadCharacterAnim("CAO_ES");
	EspioAnms[58] = LoadCharacterAnim("ES_KONOHA");

	for (uint8_t i = 0; i < LengthOfArray(EspioAnimData); ++i) {
		if (EspioAnms[i] == nullptr) continue;
		EspioAnimData[i].Animation = new NJS_ACTION;
		EspioAnimData[i].Animation->object = EspioMdls[0]->getmodel();
		EspioAnimData[i].Animation->motion = EspioAnms[i]->getmotion();
		EspioAnimData[i].NextAnim = i;
		EspioAnimData[i].AnimationSpeed = 0.5f;
	}

	EspioAnimData[13].NextAnim = 14;
	EspioAnimData[15].NextAnim = 16;
	EspioAnimData[18].NextAnim = 19;
	EspioAnimData[42].Property = 1;
	EspioAnimData[43].Property = 1;
	EspioAnimData[27].Property = 1;
	EspioAnimData[28].Property = 1;
	EspioAnimData[20].Property = 1;
	EspioAnimData[25].Property = 1;
	EspioAnimData[26].Property = 1;
	EspioAnimData[44].NextAnim = 17;
}