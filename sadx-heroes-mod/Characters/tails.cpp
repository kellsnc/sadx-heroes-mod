#include "stdafx.h"

ModelInfo* TailsMdls[3];
AnimationFile* TailsAnms[96];
AnimData HTailsAnimData[62];
AnimData TTailsAnimData[34];

NJS_TEXNAME TAILS_TEXNAMES[4];
NJS_TEXLIST TAILS_TEXLIST = { arrayptrandlength(TAILS_TEXNAMES) };

NJS_MATRIX TailsMatrices[2];

extern CollisionData Cheese_Col;

void PlayVoice_Tails(int ID) {
	switch (ID) {
	case 1803:
		PlayHeroesSound(TailsSound_Win);
		break;
	}
}

void PlaySound_Tails(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(TailsSound_Attack);
		break;
	case 1243:
		PlayHeroesSound(TailsSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayHeroesSound(TailsSound_ThatHurts);
		else if (random < 8) PlayHeroesSound(TailsSound_Hurt1);
		else PlayHeroesSound(TailsSound_Hurt2);
		break;
	case 1465:
		PlayHeroesSound(TailsSound_Death);
		break;
	case 1453:
		PlaySound(ID, 0, 0, 0);
		break;
	}
}

void TrapRing_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	ObjectMaster* ring;

	switch (data->Action) {
	case 0:
		data->LoopData = (Loop*)LoadObject((LoadObj)(LoadObj_UnknownB | LoadObj_Data1), 2, DroppedRing_Main);
		ring = (ObjectMaster*)data->LoopData;
		ring->Data1->Position = data->Position;
		ring->Data1->Rotation = data->Rotation;

		Collision_Init(obj, &Cheese_Col, 1, 3u);
		data->CollisionInfo->Radius = 2;
		
		data->Action = 1;
		break;
	case 1:
		ring = (ObjectMaster*)data->LoopData;
		if (!ring->Data1) {
			DeleteObject_(obj);
			return;
		}

		data->Position = ring->Data1->Position;
		AddToCollisionList(data);
		data->Position.y += 5;
		break;
	}
}

void TailsTails_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* tailsdata = obj->Parent->Data1;

	int anim = 0;

	switch (tailsdata->Index) {
	case 5: anim = 1; break;
	case 6: anim = 2; break;
	case 7: anim = 3; break;
	case 8: anim = 4; break;
	case 9: anim = 5; break;
	case 12: anim = 23; break;
	case 15: anim = 31; break;
	case 16: anim = 13; break;
	case 17: anim = 33; break;
	case 18: anim = 6; break;
	case 19: anim = 7; break;
	case 20: anim = 8; break;
	case 21: anim = 9; break;
	case 22: anim = 10; break;
	case 24: anim = 11; break;
	case 25: anim = 12; break;
	case 34:
	case 36:
	case 37:
	case 38:
	case 40:
	case 41:
	case 42:
		anim = 15;
		break;
	case 35: anim = 16; break;
	case 39: anim = 17; break;
	case 26:
	case 32:
	case 33:
		anim = 18;
		break;
	case 27:
	case 28:
	case 29:
		anim = 19;
		break;
	case 30: anim = 20; break;
	case 31: anim = 21; break;
	case 53: anim = 22; break;
	case 54: anim = 24; break;
	case 55: anim = 25; break;
	case 56: anim = 26; break;
	case 57: anim = 27; break;
	case 58: anim = 28; break;
	case 59: anim = 29; break;
	case 50: anim = 30; break;
	case 60: anim = 32; break;
	case 61: anim = 14; break;
	}

	PlayHeroesAnimation(obj, anim, TTailsAnimData, 0, 0);
}

void TailsCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)TailsMdls[0]->getdata("Dummy036")) {
		memcpy(TailsMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //pupils
	}
	else if (object == (NJS_OBJECT*)TailsMdls[0]->getdata("Dummy004")) {
		memcpy(TailsMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //tails
	}
}

void TailsHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* tailsobj = HeroesChars[obj->Data1->CharIndex];
	if (!tailsobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (tailsobj->Data1->NextAction == 0) tailsobj->Data1->NextAction = 1;
			else tailsobj->Data1->NextAction = 0;
		}
	}
	else {
		tailsobj->Data1->NextAction = 0;
	}

	if (tailsobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&TAILS_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000 + tailsobj->Data1->Rotation.y);

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = TailsCallback;
	njCnkAction(HTailsAnimData[tailsobj->Data1->Index].Animation, tailsobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, TailsMatrices[0], sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* pupils = TailsMdls[1]->getmodel();

	switch (tailsobj->Data1->InvulnerableTime) {
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

	if (tailsobj->Child) {
		memcpy(_nj_current_matrix_ptr_, TailsMatrices[1], sizeof(NJS_MATRIX));
		njTranslate(0, 0, 0.2f, 0.5f);
		njCnkAction(TTailsAnimData[tailsobj->Child->Data1->Index].Animation, tailsobj->Child->Data1->Scale.x);
	}
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void TailsHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (!CharactersCommon_Init(obj, "tails", &TAILS_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), TailsTails_Main, obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	playerco2->PhysicsData.MaxAccel = 2.7f;
	playerco2->PhysicsData.field_14 = 0.85f;
	playerco2->PhysicsData.AirAccel = 0.038999999f;

	int anim = data->Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = TailsHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground && Rings >= 3) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
			PlayHeroesSound(TailsSound_FlyAttack);
			data->field_A = 0;
			data->Action = 4;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
			if (++data->field_A == 2) {
				PlayHeroesSound(TailsSound_FlyUp);
				obj->Child->Data1->field_A = 1;
				data->field_A = 0;
				playerco2->Speed.y = 2;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(TailsSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(TailsSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		switch (playerco2->AnimationThing.Index) {
		case 0:
		case 2: 
		case 7:
		case 8:
			anim = 9; data->Status = 0; break;
		case 4:
		case 5:
		case 6:
			anim = 11;
			if (++data->Status == 100) {
				playerco2->AnimationThing.Index = 0;
				data->Status = 0;
			}
			break;
		case 9:
			data->Status = 0;
			anim = 0;
			if (playerco2->Speed.x < 0.02f) anim = 11;
			break;
		case 10:
			anim = 0;
			speed = 0.9f + playerco2->Speed.x * 0.2f;
			break;
		case 11:
			anim = 5;
			speed = 0.9f + playerco2->Speed.x * 0.2f;
			break;
		case 12:
			anim = 6;
			speed = 1.5f + playerco2->Speed.x * 0.1f;
			break;
		case 13:
			anim = 7;
			speed = 0.5f + playerco2->Speed.x * 0.1f;
			break;
		case 14: //jumping
			if (anim < 18 || anim > 23) {
				anim = 18;
			}
			else if (anim == 19) {
				if (data->Unknown > 2) {
					anim = 20;
				}
			}
			else if (anim == 21) {
				if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 22;
			}
			break;
		case 15: //rolling
		case 16:
			anim = 19;
			break;
		case 17: //spring jump
			anim = 19;
			break;
		case 18: //fall after spring jump
			anim = 21;
			break;
		case 19: //falling
			anim = 21;
			if (playerco2->Speed.x > 8 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500) {
				anim = 16;
				playerco2->AnimationThing.Index = 150;
			}
			break;
		case 20:
			anim = 23;
			break;
		case 21: //break
			anim = 47;
			if (playerco2->Speed.x > 6) {
				anim = 46;
			}
			else if (playerco2->Speed.x > 3) {
				anim = 45;
			}
			break;
		case 22:
			anim = 48;
			break;
		case 24:
			anim = 2;
			speed = 0;
			break;
		case 25:
			anim = 2;
			break;
		case 26:
			anim = 54;
			break;
		case 27:
			anim = 56;
			break;
		case 28: //hurt
		case 29:
		case 30:
		case 31:
			anim = 55;
			break;
		case 32:
			anim = 45;
			break;
		case 33: //thumbling
		case 34:
			anim = 53;
			break;
		case 35:
			anim = 55;
			break;
		case 36:
			anim = 21;
			break;
		case 37: //flying
		case 40:
		case 41:
			if (flysounds && FrameCounterUnpaused % 40 == 0)
				PlayHeroesSoundQueue(TailsSound_Flying, playerobj, 50, 0);
			anim = 34; //52 IF HOLDING PLAYER
			if (HeldButtons2[data->CharIndex] & Buttons_A) speed = 0.8;
			speed += playerco2->Speed.x * 0.5f;
			break;
		case 38: //flying falling
		case 39:
		case 42:
			anim = 35;
			break;
		case 43:
			anim = 39;
			break;
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
			anim = 39;
			data->Rotation.y += 0x100;
			break;
		case 54: //won
		case 55:
			anim = 60;
			if (data->Unknown > 1) {
				if (frame > 124) data->Unknown = 20;
				if (frame < 110) data->Unknown = 40;

				if (data->Unknown > 20 && data->Unknown < 40) {
					++data->Unknown;
					state = 124 - (data->Unknown - 20);
				}
				else if (data->Unknown > 40) {
					++data->Unknown;
					state = 110 + (data->Unknown - 40);
				}
			}
			break;
		case 56: //lost
		case 57:
			anim = 10;
			break;
		case 58:
			anim = 44;
			break;
		case 72: //holding hook
			anim = 51;
			break;
		case 89:
			anim = 9;
			break;
		case 107: //snowboard
		case 108:
		case 109:
			anim = 26;
			break;
		case 110:
		case 111:
		case 116:
			anim = 26;
			break;
		case 112:
			if (playerco2->Speed.z > 0.1f) anim = 33;
			break;
		case 113:
			if (playerco2->Speed.z < 0.1f) anim = 32;
			break;
		case 114:
		case 115:
		case 117:
		case 118:
		case 119:
			anim = 30;
			break;
		case 120:
		case 121:
		case 122:
		case 123:
		case 124:
		case 125:
		case 126:
			anim = 31;
			break;
		case 127:
			anim = 27;
			break;
		case 150:
			anim = 16;
			break;
		}
		
		if (anim != 39) {
			data->Rotation.y = 0;
		}
		if (anim != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}
		PlayHeroesAnimation(obj, anim, HTailsAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayHeroesSound(TailsSound_BombAttack);
			data->Scale.x = 0;
		}
		else if (data->field_A < 30) {
			if (data->field_A < 20) {
				++data->field_A;
			}
			else {
				data->field_A = 30;
				if (Rings >= 3) {
					AddRings(-3);
					ObjectMaster* ringbomb = LoadObject(LoadObj_Data1, 2, TrapRing_Main);
					ringbomb->Data1->Position = playerdata->Position;
					ringbomb->Data1->Rotation.y = -playerdata->Rotation.y - 0xC000;
					ringbomb = LoadObject(LoadObj_Data1, 2, TrapRing_Main);
					ringbomb->Data1->Position = playerdata->Position;
					ringbomb->Data1->Rotation.y = -playerdata->Rotation.y - 0xE000;
					ringbomb = LoadObject(LoadObj_Data1, 2, TrapRing_Main);
					ringbomb->Data1->Position = playerdata->Position;
					ringbomb->Data1->Rotation.y = -playerdata->Rotation.y - 0xA000;
				}
			}
		}
		else {
			if (++data->field_A == 78) {
				data->Action = 2;
				playerdata->Action = 1;
				data->field_A = 0;
			}
		}

		PlayHeroesAnimation(obj, 15, HTailsAnimData, 0, 0);
		break;
	case 4:
		if (++data->field_A == 48) {
			data->Action = 2;
			data->field_A = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
		}
		else {
			playerco2->Powerups |= Powerups_Invincibility;
		}

		PlayHeroesAnimation(obj, 39, HTailsAnimData, 0, 0);
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

void LoadTailsFiles(const char *path, const HelperFunctions &helperFunctions) {
	TailsMdls[0] = LoadCharacterModel("tails_main");
	TailsMdls[1] = LoadCharacterModel("tails_pupils");
	TailsMdls[2] = LoadCharacterModel("tails_tails");

	TailsAnms[0] = LoadCharacterAnim("TA_WALK");
	TailsAnms[1] = LoadCharacterAnim("TA_WALK_PULL");
	TailsAnms[2] = LoadCharacterAnim("TA_WALK_PUSH");
	TailsAnms[3] = LoadCharacterAnim("TA_TURN_L");
	TailsAnms[4] = LoadCharacterAnim("TA_TURN_R");
	TailsAnms[5] = LoadCharacterAnim("TA_SLOW_RUN");
	TailsAnms[6] = LoadCharacterAnim("TA_MID_RUN");
	TailsAnms[7] = LoadCharacterAnim("TA_TOP_RUN");
	TailsAnms[8] = LoadCharacterAnim("TA_START");
	TailsAnms[9] = LoadCharacterAnim("TA_IDLE");
	TailsAnms[10] = LoadCharacterAnim("TA_IDLE_C");
	TailsAnms[11] = LoadCharacterAnim("TA_IDLE_D_HALF");
	TailsAnms[12] = LoadCharacterAnim("TA_DASH_PUSH");
	TailsAnms[13] = LoadCharacterAnim("TA_FLY_IDLE_B");
	TailsAnms[14] = LoadCharacterAnim("TA_POW_ROT");
	TailsAnms[15] = LoadCharacterAnim("TA_ATC_BOMB");
	TailsAnms[16] = LoadCharacterAnim("TA_FW_JUMP");
	TailsAnms[17] = LoadCharacterAnim("HERO_TA");
	TailsAnms[18] = LoadCharacterAnim("TA_JUMP_A");
	TailsAnms[19] = LoadCharacterAnim("TA_JUMP_B");
	TailsAnms[20] = LoadCharacterAnim("TA_JUMP_C");
	TailsAnms[21] = LoadCharacterAnim("TA_JUMP_D");
	TailsAnms[22] = LoadCharacterAnim("TA_JUMP_E");
	TailsAnms[23] = LoadCharacterAnim("TA_JUMP_F");
	TailsAnms[24] = LoadCharacterAnim("TA_JUMP_TRNGL");
	TailsAnms[25] = LoadCharacterAnim("TA_JUMP_GLIND");
	TailsAnms[26] = LoadCharacterAnim("TA_GLIND");
	TailsAnms[27] = LoadCharacterAnim("TA_GLIND_BK");
	TailsAnms[28] = LoadCharacterAnim("TA_GLIND_BK_L");
	TailsAnms[29] = LoadCharacterAnim("TA_GLIND_BK_R");
	TailsAnms[30] = LoadCharacterAnim("TA_GLIND_FLIP_BK");
	TailsAnms[31] = LoadCharacterAnim("TA_GLIND_FLIP_FR");
	TailsAnms[32] = LoadCharacterAnim("TA_GLIND_L");
	TailsAnms[33] = LoadCharacterAnim("TA_GLIND_R");
	TailsAnms[34] = LoadCharacterAnim("TA_FLY_IDLE");
	TailsAnms[35] = LoadCharacterAnim("TA_FLY_SLOW");
	TailsAnms[36] = LoadCharacterAnim("TA_FLY_UP");
	TailsAnms[37] = LoadCharacterAnim("TA_FLY_PULL");
	TailsAnms[38] = LoadCharacterAnim("TA_FLY_PUSH");
	TailsAnms[39] = LoadCharacterAnim("TA_FLY_KICK");
	TailsAnms[40] = LoadCharacterAnim("TA_FLY_HANG_IDLE");
	TailsAnms[41] = LoadCharacterAnim("TA_FLY_HANG_OFF");
	TailsAnms[42] = LoadCharacterAnim("TA_FLY_HANG_ON");
	TailsAnms[43] = LoadCharacterAnim("TA_HANG_OFF");
	TailsAnms[44] = LoadCharacterAnim("TA_HANG_ON");
	TailsAnms[45] = LoadCharacterAnim("TA_BREAK_A");
	TailsAnms[46] = LoadCharacterAnim("TA_BREAK_B");
	TailsAnms[47] = LoadCharacterAnim("TA_BREAK_C");
	TailsAnms[48] = LoadCharacterAnim("TA_BREAK_TURN_L");
	TailsAnms[49] = LoadCharacterAnim("TA_BREAK_TURN_R");
	TailsAnms[50] = LoadCharacterAnim("TA_BRA_MID");
	TailsAnms[51] = LoadCharacterAnim("TA_BRA_TOP");
	TailsAnms[52] = LoadCharacterAnim("TA_BOB");
	TailsAnms[53] = LoadCharacterAnim("TA_FLORT");
	TailsAnms[54] = LoadCharacterAnim("TA_DAM_M_A");
	TailsAnms[55] = LoadCharacterAnim("TA_DAM_M_B");
	TailsAnms[56] = LoadCharacterAnim("TA_DAM_M_C");
	TailsAnms[57] = LoadCharacterAnim("TA_EDGE_OTTO_A");
	TailsAnms[58] = LoadCharacterAnim("TA_EDGE_OTTO_B");
	TailsAnms[59] = LoadCharacterAnim("TA_EDGE_OTTO_C");
	TailsAnms[60] = LoadCharacterAnim("TA_WIN_B");
	TailsAnms[61] = LoadCharacterAnim("TA_TRAP_JUMP");

	TailsAnms[62] = LoadCharacterAnim("TA_WALK_TA");
	TailsAnms[63] = LoadCharacterAnim("TA_SLOW_RUN_TA");
	TailsAnms[64] = LoadCharacterAnim("TA_MID_RUN_TA");
	TailsAnms[65] = LoadCharacterAnim("TA_TOP_RUN_TA");
	TailsAnms[66] = LoadCharacterAnim("TA_START_TA");
	TailsAnms[67] = LoadCharacterAnim("TA_IDLE_TA");
	TailsAnms[68] = LoadCharacterAnim("TA_JUMP_TA_A");
	TailsAnms[69] = LoadCharacterAnim("TA_JUMP_TA_B");
	TailsAnms[70] = LoadCharacterAnim("TA_JUMP_TA_C");
	TailsAnms[71] = LoadCharacterAnim("TA_JUMP_TA_D");
	TailsAnms[72] = LoadCharacterAnim("TA_JUMP_TA_E");
	TailsAnms[73] = LoadCharacterAnim("TA_JUMP_TRNGL_TA");
	TailsAnms[74] = LoadCharacterAnim("TA_JUMP_GLIND_TA");
	TailsAnms[75] = LoadCharacterAnim("TA_FW_JUMP_TA");
	TailsAnms[76] = LoadCharacterAnim("TA_TRAP_JUMP_TA");
	TailsAnms[77] = LoadCharacterAnim("TA_FLY_IDLE_TA");
	TailsAnms[78] = LoadCharacterAnim("TA_FLY_KICK_TA");
	TailsAnms[79] = LoadCharacterAnim("TA_FLY_SLOW_TA");
	TailsAnms[80] = LoadCharacterAnim("TA_GLIND_TA");
	TailsAnms[81] = LoadCharacterAnim("TA_GLIND_BK_TA");
	TailsAnms[82] = LoadCharacterAnim("TA_GLIND_FLIP_BK_TA");
	TailsAnms[83] = LoadCharacterAnim("TA_GLIND_FLIP_FR_TA");
	TailsAnms[84] = LoadCharacterAnim("TA_FLORT_TA");
	TailsAnms[85] = LoadCharacterAnim("TA_DASH_PUSH_TA");
	TailsAnms[86] = LoadCharacterAnim("TA_DAM_M_TA_A");
	TailsAnms[87] = LoadCharacterAnim("TA_DAM_M_TA_B");
	TailsAnms[88] = LoadCharacterAnim("TA_DAM_M_TA_C");
	TailsAnms[89] = LoadCharacterAnim("TA_BREAK_TA_A");
	TailsAnms[90] = LoadCharacterAnim("TA_BREAK_TA_B");
	TailsAnms[91] = LoadCharacterAnim("TA_BREAK_TA_C");
	TailsAnms[92] = LoadCharacterAnim("TA_BRA_MID_TA");
	TailsAnms[93] = LoadCharacterAnim("TA_ATC_BOMB_TA");
	TailsAnms[94] = LoadCharacterAnim("TA_WIN_B_TA");
	TailsAnms[95] = LoadCharacterAnim("HERO_TA_TA");

	for (uint8_t i = 0; i < LengthOfArray(HTailsAnimData); ++i) {
		if (TailsAnms[i] == nullptr) continue;
		HTailsAnimData[i].Animation = new NJS_ACTION;
		HTailsAnimData[i].Animation->object = TailsMdls[0]->getmodel();
		HTailsAnimData[i].Animation->motion = TailsAnms[i]->getmotion();
		HTailsAnimData[i].NextAnim = i;
		HTailsAnimData[i].AnimationSpeed = 0.5f;
	}

	HTailsAnimData[9].NextAnim = 8;
	HTailsAnimData[10].NextAnim = 8;
	HTailsAnimData[18].NextAnim = 19;
	HTailsAnimData[20].NextAnim = 21;
	HTailsAnimData[22].NextAnim = 23;
	HTailsAnimData[54].Property = 1;
	HTailsAnimData[55].Property = 1;
	HTailsAnimData[32].Property = 1;
	HTailsAnimData[25].Property = 1;
	HTailsAnimData[30].Property = 1;
	HTailsAnimData[31].Property = 1;
	HTailsAnimData[33].Property = 1;
	HTailsAnimData[56].NextAnim = 11;
	HTailsAnimData[23].AnimationSpeed = 0.25f;

	for (uint8_t i = 0; i < LengthOfArray(TTailsAnimData); ++i) {
		if (TailsAnms[i + 62] == nullptr) continue;
		TTailsAnimData[i].Animation = new NJS_ACTION;
		TTailsAnimData[i].Animation->object = TailsMdls[2]->getmodel();
		TTailsAnimData[i].Animation->motion = TailsAnms[i + 62]->getmotion();
		TTailsAnimData[i].NextAnim = i;
		TTailsAnimData[i].AnimationSpeed = 0.5f;
	}
}