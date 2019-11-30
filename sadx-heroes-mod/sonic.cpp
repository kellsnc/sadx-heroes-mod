#include "stdafx.h"

ModelInfo* SonicMdls[2];
AnimationFile* SonicAnms[57];
AnimData HSonicAnimData[57];

NJS_TEXNAME HSONIC_TEXNAMES[4];
NJS_TEXLIST HSONIC_TEXLIST = { arrayptrandlength(HSONIC_TEXNAMES) };

NJS_MATRIX SonicMatrix;

void PlayVoice_Sonic(int ID) {
	switch (ID) {
	case 1803:
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
	case 1243:
		PlayHeroesSound(SonicSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayHeroesSound(SonicSound_ThatHurts);
		else if (random < 8) PlayHeroesSound(SonicSound_Hurt1);
		else PlayHeroesSound(SonicSound_Hurt2);
		break;
	case 1465:
		PlayHeroesSound(SonicSound_Death);
		break;
	case 1453:
		PlaySound(ID, 0, 0, 0);
		break;
	}
}

void SonicCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)SonicMdls[0]->getdata("Dummy006")) {
		memcpy(SonicMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //pupils
	}
}

void SonicHeroes_Delete(ObjectMaster *obj) {
	bool IsThereSonic = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (HeroesChars[obj->Data1->CharIndex]->Data1->CharID == Characters_HeroesSonic) IsThereSonic = true;
		}
	}

	if (!IsThereSonic) {
		njReleaseTexture(&HSONIC_TEXLIST);
		CharTexsLoaded[Characters_HeroesSonic - 9] = false;
	}

	CharactersCommon_Delete(obj);
}

void SonicHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* sonicobj = HeroesChars[obj->Data1->CharIndex];
	if (!sonicobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
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

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
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
		DrawChunkModel(pupils->chunkmodel);
		break;
	case 2:
		DrawChunkModel(pupils->child->chunkmodel);
		break;
	case 3:
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

	if (data->Action == 0) {
		obj->DeleteSub = SonicHeroes_Delete;
		data->CharID = Characters_HeroesSonic;
		data->Action = 1;

		if (!CharTexsLoaded[Characters_HeroesSonic - 9]) {
			CharTexsLoaded[Characters_HeroesSonic - 9] = true;
			LoadPVM("sonich", &HSONIC_TEXLIST);
		}

		return;
	}
	else if (data->Action == 1) {
		data->Action = 2;
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!playerobj || playerobj->Data1->CharID != Characters_Sonic) { DeleteObject_(obj); return; }

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	playerco2->PhysicsData.HSpeedCap = 18;
	playerco2->PhysicsData.MaxAccel = 4.0f;
	playerco2->PhysicsData.field_14 = 0.95f;

	int anim = data->Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = SonicHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 14 && PressedButtons[data->CharIndex] & Buttons_X) {
			data->field_A = 0;
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

		if (anim != 20) {
			switch (playerco2->AnimationThing.Index) {
			case 0: //stance
			case 1:
			case 2:
			case 7:
			case 8:
				anim = 54; data->Status = 0; break;
			case 3: //idle
			case 4:
			case 5:
			case 6:
				anim = 55;
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
				if (anim < 13 || anim > 18) {
					anim = 13;
				}
				else if (anim == 14) {
					if (data->Unknown > 2) {
						anim = 15;
					}
				}
				else if (anim == 16) {
					if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 17;
				}
				break;
			case 15: //rolling
			case 16:
			case 17: //spring jump
				anim = 14;
				break;
			case 18: //fall after spring jump
				if (playerco2->Speed.x > 1 || playerdata->Action == 14) anim = 49;
				else anim = 16;
				break;
			case 19: //falling
				anim = 18;
				if (playerco2->Speed.x > 8 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500) {
					anim = 49;
					playerco2->AnimationThing.Index = 150;
				}
				break;
			case 20: //break
			case 25:
				anim = 36;
				if (playerco2->Speed.x > 6) {
					anim = 35;
				}
				else if (playerco2->Speed.x > 3) {
					anim = 34;
				}
				break;
			case 21: //push
			case 22:
				anim = 48;
				break;
			case 24: //hurt
				anim = 42;
				break;
			case 26: //updraft
			case 46:
				anim = 41;
				break;
			case 27:
			case 28:
				anim = 42;
				break;
			case 29:
				anim = 15;
				break;
			case 30:
				anim = 37;
				break;
			case 31: //ball
			case 32:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 45:
			case 83:
			case 125:
				anim = 14;
				break;
			case 44:
				anim = 9;
				break;
			case 47:
				anim = 80;
				anim = 29;
				break;
			case 48: //pull
				anim = 1;
				break;
			case 49: //shake tree
			case 50: //pickup
			case 51: //shake
			case 53:
			case 54:
			case 55:
			case 56:
			case 65: //car
			case 77: //rocket
			case 130:
			case 131:
				anim = 33;
				break;
			case 52:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:
			case 63:
			case 132:
				anim = 32;
				break;
			case 64:
			case 70:
			case 79: //rocket
				anim = 49;
				break;
			case 71: //ice
				anim = 0;
				break;
			case 75: //won
			case 76:
				anim = 52;
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
			case 82:
				anim = 42;
				speed = 0;
				break;
			case 84:
				anim = 45;
				break;
			case 85: //death
			case 86:
			case 87:
				anim = 42;
				break;
			case 88: //water current slide
				anim = 53;
				break;
			case 102: //snowboard
			case 105:
			case 109:
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
				anim = 21;
				break;
			case 103:
			case 107:
				if (playerco2->Speed.z > 0.1f) anim = 28;
				else anim = 26;
				break;
			case 104:
			case 108:
				if (playerco2->Speed.z < 0.1f) anim = 27;
				else anim = 26;
				break;
			case 106:
				anim = 20;
				break;
			case 116:
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
				anim = 25;
				break;
			case 124:
				anim = 22;
				break;
			case 126:
			case 127:
			case 128:
				anim = 56;
				break;
			}
		}

		if (anim != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}

		PlayHeroesAnimation(obj, anim, HSonicAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayHeroesSound(SonicSound_Ready);
			data->Scale.x = 0;
		}
		else if (data->field_A < 30) {
			if (data->field_A < 20) {
				++data->field_A;
			}
			else {
				data->field_A = 30;
			}
		}
		else {
			if (++data->field_A == 78) {
				data->Action = 2;
				playerdata->Action = 1;
				data->field_A = 0;
			}
		}

		PlayHeroesAnimation(obj, 15, HSonicAnimData, 0, 0);
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

		PlayHeroesAnimation(obj, 14, HSonicAnimData, 0, 0);
		break;
	}

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 4) data->InvulnerableTime = 0;
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