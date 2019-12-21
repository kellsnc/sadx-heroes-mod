#include "stdafx.h"

ModelInfo* RougeMdls[3];
AnimationFile* RougeAnms[66];
AnimData RougeAnimData[60];
AnimData RWingsAnimData[6];

NJS_TEXNAME ROUGE_TEXNAMES[7];
NJS_TEXLIST ROUGE_TEXLIST = { arrayptrandlength(ROUGE_TEXNAMES) };

NJS_MATRIX RougeMatrices[2];

void PlayVoice_Rouge(int ID) {
	switch (ID) {
	case 1803:
		PlayHeroesSound(RougeSound_Win);
		break;
	}
}

void PlaySound_Rouge(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(RougeSound_Trick);
		break;
	case 1243:
		PlayHeroesSound(RougeSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayHeroesSound(RougeSound_ThatHurts);
		else if (random < 8) PlayHeroesSound(RougeSound_Hurt1);
		else PlayHeroesSound(RougeSound_Hurt2);
		break;
	case 1465:
		PlayHeroesSound(RougeSound_Death);
		break;
	case 1453:
		PlaySound(ID, 0, 0, 0);
		break;
	}
}

void RougeWings_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* rougedata = obj->Parent->Data1;

	int anim;

	anim = 2;

	if (data->field_A) {
		anim = 1;
		if (++data->field_A >= 70) data->field_A = 0;
	}

	if (rougedata->Index > 8) anim = 0;

	if (rougedata->Index == 54 || rougedata->Index == 55) anim = 5;

	PlayHeroesAnimation(obj, anim, RWingsAnimData, 0, 0);
}

void RougeCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)RougeMdls[0]->getdata("Rouge031")) {
		memcpy(RougeMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //eyelashes
	}
	else if (object == (NJS_OBJECT*)RougeMdls[0]->getdata("Rouge002")) {
		memcpy(RougeMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //wings
	}
}

void RougeHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* rougeobj = HeroesChars[obj->Data1->CharIndex];
	if (!rougeobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (rougeobj->Data1->NextAction == 0) rougeobj->Data1->NextAction = 1;
			else rougeobj->Data1->NextAction = 0;
		}
	}
	else {
		rougeobj->Data1->NextAction = 0;
	}

	if (rougeobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&ROUGE_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000 + rougeobj->Data1->Rotation.y);

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = RougeCallback;
	njCnkAction(RougeAnimData[rougeobj->Data1->Index].Animation, rougeobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, RougeMatrices[0], sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* eyelashes = RougeMdls[1]->getmodel()->child;

	switch (rougeobj->Data1->InvulnerableTime) {
	case 1:
	case 3:
		DrawChunkModel(eyelashes->child->chunkmodel);
		DrawChunkModel(eyelashes->sibling->child->chunkmodel);
		break;
	case 2:
		DrawChunkModel(eyelashes->child->child->chunkmodel);
		DrawChunkModel(eyelashes->sibling->child->child->chunkmodel);
		break;
	default:
		DrawChunkModel(eyelashes->chunkmodel);
		DrawChunkModel(eyelashes->sibling->chunkmodel);
		break;
	}

	if (rougeobj->Child) {
		memcpy(_nj_current_matrix_ptr_, RougeMatrices[1], sizeof(NJS_MATRIX));
		njTranslate(0, 0, 0.8f, 0.5f);
		njCnkAction(RWingsAnimData[rougeobj->Child->Data1->Index].Animation, rougeobj->Child->Data1->Scale.x);
	}
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void RougeHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (!CharactersCommon_Init(obj, "rouge", &ROUGE_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), RougeWings_Main, obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[14].texaddr = ROUGE_TEXLIST.textures[5].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.MaxAccel = 2.7f;
			playerco2->PhysicsData.field_14 = 0.85f;
			playerco2->PhysicsData.AirAccel = 0.038999999f;
		}

		data->Rotation.z = 1;
	}

	int anim = data->Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = RougeHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
			PlayHeroesSound(RougeSound_FlyAttack);
			data->field_A = 0;
			data->Action = 4;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
			if (++data->field_A == 2) {
				PlayHeroesSound(RougeSound_FlyUp);
				obj->Child->Data1->field_A = 1;
				data->field_A = 0;
				playerco2->Speed.y = 2;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(RougeSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(RougeSound_Idle2);
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
			anim = 10;
			if (++data->Status == 100) {
				playerco2->AnimationThing.Index = 0;
				data->Status = 0;
			}
			break;
		case 9:
			data->Status = 0;
			anim = 0;
			if (playerco2->Speed.x < 0.02f) anim = 9;
			break;
		case 10:
			anim = 0;
			speed = 0.9f + playerco2->Speed.x * 0.5f;
			break;
		case 11:
			anim = 5;
			speed = 0.9f + playerco2->Speed.x * 0.3f;
			break;
		case 12:
			anim = 6;
			speed = 1.5f + playerco2->Speed.x * 0.2f;
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
			anim = 12;
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
		case 100:
			anim = 51;
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
		
		PlayHeroesAnimation(obj, anim, RougeAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayHeroesSound(RougeSound_BombAttack);
			data->Scale.x = 0;
		}
		else if (data->field_A < 30) {
			if (data->field_A < 20) {
				++data->field_A;
			}
			else {
				data->field_A = 30;
				ObjectMaster* temp = LoadObject(LoadObj_Data1, 3, (ObjectFuncPtr)0x4AC920);
				temp->Data1->Rotation.y = -playerdata->Rotation.y;
				temp->Data1->Position = playerdata->Position;
				temp->Data1->Position.y += 5;
				temp->Parent = obj;
			}
		}
		else {
			if (++data->field_A == 88) {
				data->Action = 2;
				playerdata->Action = 1;
				data->field_A = 0;
			}
		}

		PlayHeroesAnimation(obj, 13, RougeAnimData, 0, 0);
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

		PlayHeroesAnimation(obj, 39, RougeAnimData, 0, 0);
		break;
	}

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 3) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadRougeFiles() {
	RougeMdls[0] = LoadCharacterModel("rouge_main");
	RougeMdls[1] = LoadCharacterModel("rouge_eyelashes");
	RougeMdls[2] = LoadCharacterModel("rouge_wings");

	RougeAnms[0] = LoadCharacterAnim("RO_WALK");
	RougeAnms[1] = LoadCharacterAnim("RO_WALK_PULL");
	RougeAnms[2] = LoadCharacterAnim("RO_WALK_PUSH");
	RougeAnms[3] = LoadCharacterAnim("RO_TURN_L");
	RougeAnms[4] = LoadCharacterAnim("RO_TURN_R");
	RougeAnms[5] = LoadCharacterAnim("RO_SLOW_RUN");
	RougeAnms[6] = LoadCharacterAnim("RO_MID_RUN");
	RougeAnms[7] = LoadCharacterAnim("RO_TOP_RUN");
	RougeAnms[8] = LoadCharacterAnim("RO_START");
	RougeAnms[9] = LoadCharacterAnim("RO_IDLE");
	RougeAnms[10] = LoadCharacterAnim("RO_IDLE_B");
	RougeAnms[11] = LoadCharacterAnim("RO_IDLE_C_HALF");
	RougeAnms[12] = LoadCharacterAnim("RO_WIN");
	RougeAnms[13] = LoadCharacterAnim("RO_ATC_BOMB");
	RougeAnms[14] = LoadCharacterAnim("RO_POW_ROT");
	RougeAnms[15] = LoadCharacterAnim("RO_TRAP_JUMP");
	RougeAnms[16] = LoadCharacterAnim("RO_FW_JUMP");
	RougeAnms[17] = LoadCharacterAnim("DARK_RO");
	RougeAnms[18] = LoadCharacterAnim("RO_JUMP_A");
	RougeAnms[19] = LoadCharacterAnim("RO_JUMP_B");
	RougeAnms[20] = LoadCharacterAnim("RO_JUMP_C");
	RougeAnms[21] = LoadCharacterAnim("RO_JUMP_D");
	RougeAnms[22] = LoadCharacterAnim("RO_JUMP_E");
	RougeAnms[23] = LoadCharacterAnim("RO_JUMP_F");
	RougeAnms[24] = LoadCharacterAnim("RO_JUMP_TRNGL");
	RougeAnms[25] = LoadCharacterAnim("RO_JUMP_GLIND");
	RougeAnms[26] = LoadCharacterAnim("RO_GLIND");
	RougeAnms[27] = LoadCharacterAnim("RO_GLIND_BK");
	RougeAnms[28] = LoadCharacterAnim("RO_GLIND_BK_L");
	RougeAnms[29] = LoadCharacterAnim("RO_GLIND_BK_R");
	RougeAnms[30] = LoadCharacterAnim("RO_GLIND_FLIP_B");
	RougeAnms[31] = LoadCharacterAnim("RO_GLIND_FLIP_F");
	RougeAnms[32] = LoadCharacterAnim("RO_GLIND_L");
	RougeAnms[33] = LoadCharacterAnim("RO_GLIND_R");
	RougeAnms[34] = LoadCharacterAnim("RO_FLY_IDLE");
	RougeAnms[35] = LoadCharacterAnim("RO_FLY_SLOW");
	RougeAnms[36] = LoadCharacterAnim("RO_FLY_UP");
	RougeAnms[37] = LoadCharacterAnim("RO_FLY_PULL");
	RougeAnms[38] = LoadCharacterAnim("RO_FLY_PUSH");
	RougeAnms[39] = LoadCharacterAnim("RO_FLY_KICK");
	RougeAnms[40] = LoadCharacterAnim("RO_FLY_HANG_IDLE");
	RougeAnms[41] = LoadCharacterAnim("RO_FLY_HANG_OFF");
	RougeAnms[42] = LoadCharacterAnim("RO_FLY_HANG_ON");
	RougeAnms[43] = LoadCharacterAnim("RO_HANG_OFF");
	RougeAnms[44] = LoadCharacterAnim("RO_HANG_ON");
	RougeAnms[45] = LoadCharacterAnim("RO_BREAK_A");
	RougeAnms[46] = LoadCharacterAnim("RO_BREAK_B");
	RougeAnms[47] = LoadCharacterAnim("RO_BREAK_C");
	RougeAnms[48] = LoadCharacterAnim("RO_BREAK_TURN_L");
	RougeAnms[49] = LoadCharacterAnim("RO_BREAK_TURN_R");
	RougeAnms[50] = LoadCharacterAnim("RO_BRA_MID");
	RougeAnms[51] = LoadCharacterAnim("RO_BRA_TOP");
	RougeAnms[52] = LoadCharacterAnim("RO_BOB");
	RougeAnms[53] = LoadCharacterAnim("RO_FLORT");
	RougeAnms[54] = LoadCharacterAnim("RO_DAM_M_A");
	RougeAnms[55] = LoadCharacterAnim("RO_DAM_M_B");
	RougeAnms[56] = LoadCharacterAnim("RO_DAM_M_C");
	RougeAnms[57] = LoadCharacterAnim("RO_EDGE_OTTO_A");
	RougeAnms[58] = LoadCharacterAnim("RO_EDGE_OTTO_B");
	RougeAnms[59] = LoadCharacterAnim("RO_EDGE_OTTO_C");

	RougeAnms[60] = LoadCharacterAnim("RO_FLY_IDLE_WNGL");
	RougeAnms[61] = LoadCharacterAnim("RO_FLY_UP_WING");
	RougeAnms[62] = LoadCharacterAnim("RO_IDLE_C_HANE_HALF");
	RougeAnms[63] = LoadCharacterAnim("RO_IDLE_WNGS");
	RougeAnms[64] = LoadCharacterAnim("SL_HANE");
	RougeAnms[65] = LoadCharacterAnim("RO_WING_WIN");

	for (uint8_t i = 0; i < LengthOfArray(RougeAnimData); ++i) {
		if (RougeAnms[i] == nullptr) continue;
		RougeAnimData[i].Animation = new NJS_ACTION;
		RougeAnimData[i].Animation->object = RougeMdls[0]->getmodel();
		RougeAnimData[i].Animation->motion = RougeAnms[i]->getmotion();
		RougeAnimData[i].NextAnim = i;
		RougeAnimData[i].AnimationSpeed = 0.5f;
	}

	RougeAnimData[9].NextAnim = 8;
	RougeAnimData[10].NextAnim = 8;
	RougeAnimData[18].NextAnim = 19;
	RougeAnimData[20].NextAnim = 21;
	RougeAnimData[22].NextAnim = 23;
	RougeAnimData[54].Property = 1;
	RougeAnimData[55].Property = 1;
	RougeAnimData[32].Property = 1;
	RougeAnimData[25].Property = 1;
	RougeAnimData[30].Property = 1;
	RougeAnimData[31].Property = 1;
	RougeAnimData[33].Property = 1;
	RougeAnimData[56].NextAnim = 11;
	RougeAnimData[23].AnimationSpeed = 0.25f;

	for (uint8_t i = 0; i < LengthOfArray(RWingsAnimData); ++i) {
		if (RougeAnms[i + 60] == nullptr) continue;
		RWingsAnimData[i].Animation = new NJS_ACTION;
		RWingsAnimData[i].Animation->object = RougeMdls[2]->getmodel();
		RWingsAnimData[i].Animation->motion = RougeAnms[i + 60]->getmotion();
		RWingsAnimData[i].NextAnim = i;
		RWingsAnimData[i].AnimationSpeed = 0.5f;
	}

	RWingsAnimData[6].Property = 1;
}