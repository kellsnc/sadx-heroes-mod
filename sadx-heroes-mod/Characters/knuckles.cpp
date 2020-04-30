#include "stdafx.h"

ModelInfo* KnucklesMdls[2];
AnimationFile* KnucklesAnms[60];
AnimData HKnucklesAnimData[60];

NJS_TEXNAME HKNUCKLES_TEXNAMES[4];
NJS_TEXLIST HKNUCKLES_TEXLIST = { arrayptrandlength(HKNUCKLES_TEXNAMES) };

NJS_MATRIX KnucklesMatrix;

void PlayVoice_Knuckles(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayCustomSound(KnucklesSound_Win);
		break;
	}
}

void PlaySound_Knuckles(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayCustomSound(KnucklesSound_Attack);
		break;
	case 762:
		PlayCustomSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayCustomSound(KnucklesSound_Hurt2);
		break;
	case 1233:
		PlayCustomSound(KnucklesSound_Hurt1);
		break;
	case 1503:
		PlayCustomSound(KnucklesSound_Death);
		break;
	}
}

void KnucklesCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)KnucklesMdls[0]->getdata("Dummy006")) {
		memcpy(KnucklesMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //pupils
	}
}

void KnucklesHeroes_Display(ObjectMaster *obj) {
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

	njSetTexture(&HKNUCKLES_TEXLIST);

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

	*NodeCallbackFuncPtr = KnucklesCallback;
	njCnkAction(HKnucklesAnimData[knucklesobj->Data1->Index].Animation, knucklesobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, KnucklesMatrix, sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* pupils = KnucklesMdls[1]->getmodel();

	switch (knucklesobj->Data1->InvulnerableTime) {
	case 1:
	case 5:
		DrawChunkModel(pupils->chunkmodel);
		break;
	case 2:
	case 4:
		DrawChunkModel(pupils->child->chunkmodel);
		break;
	case 3:
		DrawChunkModel(pupils->child->child->chunkmodel);
		break;
	}

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void KnucklesHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "knucks", &HKNUCKLES_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[15].texaddr = HKNUCKLES_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.HSpeedCap = 15;
			playerco2->PhysicsData.MaxAccel = 3.5f;
			playerco2->PhysicsData.field_14 = 0.85f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = KnucklesHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground && (HeldButtons2[data->CharIndex] & Buttons_A) != Buttons_A) {
				data->field_A = 0;
				playerdata->Action = 2;
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayCustomSound(KnucklesSound_Attack);
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayCustomSound(KnucklesSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayCustomSound(KnucklesSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state

		PlayHeroesAnimation(obj, anim.x, HKnucklesAnimData, anim.y, anim.z);

		break;
	case 3:
		switch (PowerComboTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayCustomSound(KnucklesSound_Combo1);
			break;
		case 2:
			PlayCustomSound(KnucklesSound_Combo2);
			break;
		case 3:
			PlayCustomSound(KnucklesSound_Combo3);
			playerco2->Speed.x = 1;
			playerco2->Speed.y = 2;
		case 4:
			if (data->Scale.x > 89) {
				ExploseEnemies(&playerdata->Position, 2);
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				data->Action = 2;
			}
			break;
		}

		PlayHeroesAnimation(obj, 11, HKnucklesAnimData, 0, 0);
		break;
	case 4:
		if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayCustomSound(KnucklesSound_Trick);

		PlayHeroesAnimation(obj, 12, HKnucklesAnimData, 0, 0);
		break;
	}

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 5) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadKnuckFiles() {
	KnucklesMdls[0] = LoadCharacterModel("knucks_main");
	KnucklesMdls[1] = LoadCharacterModel("knucks_eyelids");

	KnucklesAnms[0] = LoadCharacterAnim("KN_WALK");
	KnucklesAnms[1] = LoadCharacterAnim("KN_WALK_PULL");
	KnucklesAnms[2] = LoadCharacterAnim("KN_WALK_PUSH");
	KnucklesAnms[3] = LoadCharacterAnim("KN_TURN_L");
	KnucklesAnms[4] = LoadCharacterAnim("KN_TURN_R");
	KnucklesAnms[5] = LoadCharacterAnim("KN_SLOW_RUN");
	KnucklesAnms[6] = LoadCharacterAnim("KN_MID_RUN");
	KnucklesAnms[7] = LoadCharacterAnim("KN_TOP_RUN");
	KnucklesAnms[8] = LoadCharacterAnim("KN_START");
	KnucklesAnms[9] = LoadCharacterAnim("KN_ATC_SPIKE");
	KnucklesAnms[10] = LoadCharacterAnim("KN_ATC_01");
	KnucklesAnms[11] = LoadCharacterAnim("KN_ATC_C");
	KnucklesAnms[12] = LoadCharacterAnim("KN_ATC_D");
	KnucklesAnms[13] = LoadCharacterAnim("KN_JUMP_A");
	KnucklesAnms[14] = LoadCharacterAnim("KN_JUMP_B");
	KnucklesAnms[15] = LoadCharacterAnim("KN_JUMP_C");
	KnucklesAnms[16] = LoadCharacterAnim("KN_JUMP_D");
	KnucklesAnms[17] = LoadCharacterAnim("KN_JUMP_E");
	KnucklesAnms[18] = LoadCharacterAnim("KN_JUMP_F");
	KnucklesAnms[19] = LoadCharacterAnim("KN_JUMP_TRNGL");
	KnucklesAnms[20] = LoadCharacterAnim("KN_JUMP_GLIND");
	KnucklesAnms[21] = LoadCharacterAnim("KN_GLIND");
	KnucklesAnms[22] = LoadCharacterAnim("KN_GLIND_BK");
	KnucklesAnms[23] = LoadCharacterAnim("KN_GLIND_BK_L");
	KnucklesAnms[24] = LoadCharacterAnim("KN_GLIND_BK_R");
	KnucklesAnms[25] = LoadCharacterAnim("KN_GLIND_FLIP_B");
	KnucklesAnms[26] = LoadCharacterAnim("KN_GLIND_FLIP_FR");
	KnucklesAnms[27] = LoadCharacterAnim("KN_GLIND_L");
	KnucklesAnms[28] = LoadCharacterAnim("KN_GLIND_R");
	KnucklesAnms[29] = LoadCharacterAnim("KN_FLY_IDLE");
	KnucklesAnms[30] = LoadCharacterAnim("KN_FLY_SLOW");
	KnucklesAnms[31] = LoadCharacterAnim("KN_FLY_GLIND");
	KnucklesAnms[32] = LoadCharacterAnim("KN_HANG_OFF");
	KnucklesAnms[33] = LoadCharacterAnim("KN_HANG_ON");
	KnucklesAnms[34] = LoadCharacterAnim("KN_BREAK_A");
	KnucklesAnms[35] = LoadCharacterAnim("KN_BREAK_B");
	KnucklesAnms[36] = LoadCharacterAnim("KN_BREAK_C");
	KnucklesAnms[37] = LoadCharacterAnim("KN_BREAK_TURN_L");
	KnucklesAnms[38] = LoadCharacterAnim("KN_BREAK_TURN_R");
	KnucklesAnms[39] = LoadCharacterAnim("KN_BRA_MID");
	KnucklesAnms[40] = LoadCharacterAnim("KN_BRA_TOP");
	KnucklesAnms[41] = LoadCharacterAnim("KN_FLORT");
	KnucklesAnms[42] = LoadCharacterAnim("KN_DAM_M_A");
	KnucklesAnms[43] = LoadCharacterAnim("KN_DAM_M_B");
	KnucklesAnms[44] = LoadCharacterAnim("KN_DAM_M_C");
	KnucklesAnms[45] = LoadCharacterAnim("KN_EDGE_OTTO_A");
	KnucklesAnms[46] = LoadCharacterAnim("KN_EDGE_OTTO_B");
	KnucklesAnms[47] = LoadCharacterAnim("KN_EDGE_OTTO_C");
	KnucklesAnms[48] = LoadCharacterAnim("KN_EDGE_OTTO_C");
	KnucklesAnms[49] = LoadCharacterAnim("KN_FW_JUMP");
	KnucklesAnms[50] = LoadCharacterAnim("KN_TRAP_JUMP");
	KnucklesAnms[51] = LoadCharacterAnim("HERO_KN");
	KnucklesAnms[52] = LoadCharacterAnim("KN_WIN_B");
	KnucklesAnms[53] = LoadCharacterAnim("KN_ATC_RENDA");
	KnucklesAnms[54] = LoadCharacterAnim("KN_IDLE");
	KnucklesAnms[55] = LoadCharacterAnim("KN_IDLE_D");
	KnucklesAnms[56] = LoadCharacterAnim("KN_IDLE_E");
	KnucklesAnms[57] = LoadCharacterAnim("KN_BOB");
	KnucklesAnms[58] = LoadCharacterAnim("KN_BOB_L");
	KnucklesAnms[59] = LoadCharacterAnim("KN_BOB_R");

	for (uint8_t i = 0; i < LengthOfArray(HKnucklesAnimData); ++i) {
		if (KnucklesAnms[i] == nullptr) continue;
		HKnucklesAnimData[i].Animation = new NJS_ACTION;
		HKnucklesAnimData[i].Animation->object = KnucklesMdls[0]->getmodel();
		HKnucklesAnimData[i].Animation->motion = KnucklesAnms[i]->getmotion();
		HKnucklesAnimData[i].NextAnim = i;
		HKnucklesAnimData[i].AnimationSpeed = 0.5f;
	}

	HKnucklesAnimData[13].NextAnim = 14;
	HKnucklesAnimData[15].NextAnim = 16;
	HKnucklesAnimData[18].NextAnim = 19;
	HKnucklesAnimData[42].Property = 1;
	HKnucklesAnimData[43].Property = 1;
	HKnucklesAnimData[27].Property = 1;
	HKnucklesAnimData[33].Property = 1;
	HKnucklesAnimData[28].Property = 1;
	HKnucklesAnimData[20].Property = 1;
	HKnucklesAnimData[25].Property = 1;
	HKnucklesAnimData[26].Property = 1;
	HKnucklesAnimData[44].NextAnim = 17;
	HKnucklesAnimData[52].Property = 1;
}

void UnloadKnuckFiles() {
	FreeMDLFiles(KnucklesMdls, LengthOfArray(KnucklesMdls));
	FreeANMFiles(KnucklesAnms, LengthOfArray(KnucklesAnms));
}