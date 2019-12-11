#include "stdafx.h"

ModelInfo* SonicMdls[2];
AnimationFile* SonicAnms[58];
AnimData HSonicAnimData[58];

NJS_TEXNAME HSONIC_TEXNAMES[4];
NJS_TEXLIST HSONIC_TEXLIST = { arrayptrandlength(HSONIC_TEXNAMES) };

NJS_MATRIX SonicMatrix;

void PlayVoice_Sonic(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(SonicSound_Win);
		break;
	}
}

void PlaySound_Sonic(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(SonicSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(SonicSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(SonicSound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(SonicSound_Death);
		break;
	}
}

void SonicCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)SonicMdls[0]->getdata("Dummy006")) {
		memcpy(SonicMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //pupils
	}
}

void SonicHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* sonicobj = HeroesChars[obj->Data1->CharIndex];
	if (!sonicobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (sonicobj->Data1->NextAction == 0) sonicobj->Data1->NextAction = 1;
			else sonicobj->Data1->NextAction = 0;
		}
	}
	else {
		sonicobj->Data1->NextAction = 0;
	}

	if (sonicobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HSONIC_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	if (sonicobj->Data1->Index == 52) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = SonicCallback;
	njCnkAction(HSonicAnimData[sonicobj->Data1->Index].Animation, sonicobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, SonicMatrix, sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* pupils = SonicMdls[1]->getmodel();

	switch (sonicobj->Data1->InvulnerableTime) {
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

void SonicHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "sonich", &HSONIC_TEXLIST)) {
		return;
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
		PlayerPtrs[data->CharIndex]->DisplaySub = SonicHeroes_Display;

		if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 5;
			PlayHeroesSound(SonicSound_Ya);
			data->Action = 3;
			break;
		}

		if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
			data->field_A = 0;
			PlayHeroesSound(SonicSound_Attack);
			data->Action = 4;
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(SonicSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(SonicSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
		
		if (anim.x != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}

		PlayHeroesAnimation(obj, anim.x, HSonicAnimData, anim.y, anim.z);

		break;
	case 3:
		KickTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 53, HSonicAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, HSonicAnimData, 0, 0);
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

void LoadSonicFiles(const char *path, const HelperFunctions &helperFunctions) {
	SonicMdls[0] = LoadCharacterModel("sonic_main");
	SonicMdls[1] = LoadCharacterModel("sonic_pupils");

	SonicAnms[0] = LoadCharacterAnim("SN_WALK");
	SonicAnms[1] = LoadCharacterAnim("SN_WALK_PULL");
	SonicAnms[2] = LoadCharacterAnim("SN_WALK_PUSH");
	SonicAnms[3] = LoadCharacterAnim("SN_TURN_L");
	SonicAnms[4] = LoadCharacterAnim("SN_TURN_R");
	SonicAnms[5] = LoadCharacterAnim("SN_SLOW_RUN");
	SonicAnms[6] = LoadCharacterAnim("SN_MID_RUN");
	SonicAnms[7] = LoadCharacterAnim("SN_TOP_RUN");
	SonicAnms[8] = LoadCharacterAnim("SN_START");
	SonicAnms[9] = LoadCharacterAnim("SN_JUMP_WALL");
	SonicAnms[10] = LoadCharacterAnim("SN_JUMP_TRIC_A");
	SonicAnms[11] = LoadCharacterAnim("SN_JUMP_TRIC_B");
	SonicAnms[12] = LoadCharacterAnim("SN_JUMP_TRIC_C");
	SonicAnms[13] = LoadCharacterAnim("SN_JUMP_A");
	SonicAnms[14] = LoadCharacterAnim("SN_JUMP_B");
	SonicAnms[15] = LoadCharacterAnim("SN_JUMP_C");
	SonicAnms[16] = LoadCharacterAnim("SN_JUMP_D");
	SonicAnms[17] = LoadCharacterAnim("SN_JUMP_E");
	SonicAnms[18] = LoadCharacterAnim("SN_JUMP_F");
	SonicAnms[19] = LoadCharacterAnim("SN_JUMP_TRNGL");
	SonicAnms[20] = LoadCharacterAnim("SN_JUMP_GLIND");
	SonicAnms[21] = LoadCharacterAnim("SN_GLIND");
	SonicAnms[22] = LoadCharacterAnim("SN_GLIND_BK");
	SonicAnms[23] = LoadCharacterAnim("SN_GLIND_BK_L");
	SonicAnms[24] = LoadCharacterAnim("SN_GLIND_BK_R");
	SonicAnms[25] = LoadCharacterAnim("SN_GLIND_FLIP_B");
	SonicAnms[26] = LoadCharacterAnim("SN_GLIND_FLIP_FR");
	SonicAnms[27] = LoadCharacterAnim("SN_GLIND_L");
	SonicAnms[28] = LoadCharacterAnim("SN_GLIND_R");
	SonicAnms[29] = LoadCharacterAnim("SN_FLY_IDLE");
	SonicAnms[30] = LoadCharacterAnim("SN_FLY_SLOW");
	SonicAnms[31] = LoadCharacterAnim("SN_FLY_GLIND");
	SonicAnms[32] = LoadCharacterAnim("SN_HANG_OFF");
	SonicAnms[33] = LoadCharacterAnim("SN_HANG_ON");
	SonicAnms[34] = LoadCharacterAnim("SN_BREAK_A");
	SonicAnms[35] = LoadCharacterAnim("SN_BREAK_B");
	SonicAnms[36] = LoadCharacterAnim("SN_BREAK_C");
	SonicAnms[37] = LoadCharacterAnim("SN_BREAK_TURN_L");
	SonicAnms[38] = LoadCharacterAnim("SN_BREAK_TURN_R");
	SonicAnms[39] = LoadCharacterAnim("SN_BRA_MID");
	SonicAnms[40] = LoadCharacterAnim("SN_BRA_TOP");
	SonicAnms[41] = LoadCharacterAnim("SN_FLORT");
	SonicAnms[42] = LoadCharacterAnim("SN_DAM_MID_A");
	SonicAnms[43] = LoadCharacterAnim("SN_DAM_MID_B");
	SonicAnms[44] = LoadCharacterAnim("SN_DAM_MID_C");
	SonicAnms[45] = LoadCharacterAnim("SN_EDGE_OTTO_A");
	SonicAnms[46] = LoadCharacterAnim("SN_EDGE_OTTO_B");
	SonicAnms[47] = LoadCharacterAnim("SN_EDGE_OTTO_C");
	SonicAnms[48] = LoadCharacterAnim("SN_EDGE_OTTO_C");
	SonicAnms[49] = LoadCharacterAnim("SN_FW_JUMP");
	SonicAnms[50] = LoadCharacterAnim("SN_TRAP_JUMP");
	SonicAnms[51] = LoadCharacterAnim("SN_POW_ROT");
	SonicAnms[52] = LoadCharacterAnim("SN_WIN_B");
	SonicAnms[53] = LoadCharacterAnim("SN_ATC_DASH");
	SonicAnms[54] = LoadCharacterAnim("SN_IDLE");
	SonicAnms[55] = LoadCharacterAnim("SN_IDLE_C");
	SonicAnms[56] = LoadCharacterAnim("SN_IDLE_D");
	SonicAnms[57] = LoadCharacterAnim("HERO_SN");

	for (uint8_t i = 0; i < LengthOfArray(HSonicAnimData); ++i) {
		if (SonicAnms[i] == nullptr) continue;
		HSonicAnimData[i].Animation = new NJS_ACTION;
		HSonicAnimData[i].Animation->object = SonicMdls[0]->getmodel();
		HSonicAnimData[i].Animation->motion = SonicAnms[i]->getmotion();
		HSonicAnimData[i].NextAnim = i;
		HSonicAnimData[i].AnimationSpeed = 0.5f;
	}

	HSonicAnimData[13].NextAnim = 14;
	HSonicAnimData[15].NextAnim = 16;
	HSonicAnimData[18].NextAnim = 19;
	HSonicAnimData[42].Property = 1;
	HSonicAnimData[43].Property = 1;
	HSonicAnimData[27].Property = 1;
	HSonicAnimData[28].Property = 1;
	HSonicAnimData[20].Property = 1;
	HSonicAnimData[25].Property = 1;
	HSonicAnimData[26].Property = 1;
	HSonicAnimData[44].NextAnim = 17;
}