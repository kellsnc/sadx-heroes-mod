#include "stdafx.h"
#include "mod.h"
#include "characters.h"
#include "objects.h"

ModelInfo* CharmyMdls[4];
AnimationFile* CharmyAnms[58];
AnimData CharmyAnimData[57];
AnimData CWingsAnimData;

NJS_TEXNAME CHARMY_TEXNAMES[5];
NJS_TEXLIST CHARMY_TEXLIST = { arrayptrandlength(CHARMY_TEXNAMES) };

NJS_MATRIX CharmyMatrices[2];

enum CharmySounds {
	CharmySound_Attack = 11100,
	CharmySound_FlyBegin,
	CharmySound_FlyAttack,
	CharmySound_FlyUp,
	CharmySound_TeamSwap,
	CharmySound_ThatHurts,
	CharmySound_Hurt1,
	CharmySound_Hurt2,
	CharmySound_Trick,
	CharmySound_DardAttack,
	CharmySound_Death,
	CharmySound_LevelUp,
	CharmySound_Win,
	CharmySound_Lose,
	CharmySound_Idle1,
	CharmySound_Idle2
};

void PlayVoice_Charmy(int ID) {
	switch (ID) {
	case 1803:
		PlayVoice(CharmySound_Win);
		break;
	}
}

int PlaySound_Charmy(int ID, void *a2, int a3, void *a4) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayVoice(CharmySound_Trick);
		break;
	case 1243:
		PlayVoice(CharmySound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayVoice(CharmySound_ThatHurts);
		else if (random < 8) PlayVoice(CharmySound_Hurt1);
		else PlayVoice(CharmySound_Hurt2);
		break;
	case 1465:
		PlayVoice(CharmySound_Death);
		break;
	case 1453:
		PlaySound(ID, a2, a3, a4);
		break;
	}

	return 1;
}

void CharmyWings_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* charmydata = obj->Parent->Data1;
	CharObj2* co2 = CharObj2Ptrs[charmydata->CharIndex];

	float speed = 0.7f;
	speed += fmax(co2->Speed.x, co2->Speed.y) / 0.3f;

	PlayHeroesAnimation(obj, 0, &CWingsAnimData, speed, 0);
}

void CharmyCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)CharmyMdls[0]->getdata("Dummy008")) {
		memcpy(CharmyMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //eyelashes
	}
	else if (object == (NJS_OBJECT*)CharmyMdls[0]->getdata("Dummy005")) {
		memcpy(CharmyMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //wings
	}
}

void CharmyHeroes_Delete(ObjectMaster *obj) {
	bool IsThereCharmy = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (HeroesChars[obj->Data1->CharIndex]->Data1->CharID == Characters_Charmy) IsThereCharmy = true;
		}
	}

	if (!IsThereCharmy) {
		njReleaseTexture(&CHARMY_TEXLIST);
		CharTexsLoaded[Characters_Charmy - 9] = false;
	}

	CharactersCommon_Delete(obj);
}

void CharmyHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* Charmyobj = HeroesChars[obj->Data1->CharIndex];
	if (!Charmyobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (Charmyobj->Data1->NextAction == 0) Charmyobj->Data1->NextAction = 1;
			else Charmyobj->Data1->NextAction = 0;
		}
	}
	else {
		Charmyobj->Data1->NextAction = 0;
	}

	if (Charmyobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CHARMY_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x + Charmyobj->Data1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000 + Charmyobj->Data1->Rotation.y);

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = CharmyCallback;
	njCnkAction(CharmyAnimData[Charmyobj->Data1->Index].Animation, Charmyobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, CharmyMatrices[0], sizeof(NJS_MATRIX));
	NJS_CNK_OBJECT* eyelashes = CharmyMdls[1]->getmodel();

	switch (Charmyobj->Data1->InvulnerableTime) {
	case 1:
		DrawChunkModel(eyelashes->child->chunkmodel);
		break;
	case 2:
		DrawChunkModel(eyelashes->child->child->chunkmodel);
		break;
	case 3:
		DrawChunkModel(eyelashes->child->child->child->chunkmodel);
		break;
	case 4:
		DrawChunkModel(eyelashes->child->child->child->chunkmodel);
		break;
	}

	if (Charmyobj->Child) {
		memcpy(_nj_current_matrix_ptr_, CharmyMatrices[1], sizeof(NJS_MATRIX));
		njTranslate(0, 0, 1.2f, 0.5f);
		njCnkAction(CWingsAnimData.Animation, Charmyobj->Child->Data1->Scale.x);
		njTranslate(0, 0, -4.8f, -0.5f);
		DrawChunkModel(CharmyMdls[3]->getmodel()->child->sibling->sibling->chunkmodel);
	}

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CharmyHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DeleteSub = CharmyHeroes_Delete;
		data->CharID = Characters_Charmy;
		data->Action = 1;

		if (!CharTexsLoaded[Characters_Charmy - 9]) {
			CharTexsLoaded[Characters_Charmy - 9] = true;
			LoadPVM("charmy", &CHARMY_TEXLIST);
		}

		return;
	}
	else if (data->Action == 1) {
		if (GameState == 4 || GameState == 15) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), CharmyWings_Main, obj);
		data->Action = 2;
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!playerobj || playerobj->Data1->CharID != Characters_Tails) { DeleteObject_(obj); return; }
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), CharmyWings_Main, obj);

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

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = CharmyHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
			PlayVoice(CharmySound_FlyAttack);
			data->field_A = 0;
			data->Action = 4;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
			if (++data->field_A == 2) {
				PlayVoice(CharmySound_FlyUp);
				obj->Child->Data1->field_A = 1;
				data->field_A = 0;
				playerco2->Speed.y = 2;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (anim == 19 && playerco2->AnimationThing.Index < 6) {
			anim = 20; //just stopped rolling
		}

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayVoice(CharmySound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayVoice(CharmySound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		if (anim != 20) {
			switch (playerco2->AnimationThing.Index) {
			case 0:
			case 2:
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
			case 7:
			case 8:
				anim = 9; data->Status = 0; break;
			case 9:
				data->Status = 0;
				anim = 5;
				if (playerco2->Speed.x < 0.02f) anim = 9;
				break;
			case 10:
				anim = 5;
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
				anim = 46;
				if (playerco2->Speed.x > 6) {
					anim = 45;
				}
				else if (playerco2->Speed.x > 3) {
					anim = 43;
				}
				break;
			case 22:
				anim = 43;
				break;
			case 24:
				anim = 2;
				speed = 0;
				break;
			case 25:
				anim = 2;
				break;
			case 26:
				anim = 51;
				break;
			case 27:
				anim = 53;
				break;
			case 28: //hurt
			case 29:
			case 30:
			case 31:
				anim = 52;
				break;
			case 32:
				anim = 45;
				break;
			case 33: //thumbling
			case 34:
				anim = 50;
				break;
			case 35:
				anim = 52;
				break;
			case 36:
				anim = 11;
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
				anim = 43;
				break;
			case 72: //holding hook
				anim = 48;
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
		}

		if (anim != 39) {
			data->Rotation.y = 0;
		}
		if (anim != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}
		PlayHeroesAnimation(obj, anim, CharmyAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayVoice(CharmySound_DardAttack);
			data->Scale.x = 0;
		}
		else {
			++data->field_A;
			data->Rotation.x += 50;
			playerco2->Powerups |= Powerups_Invincibility;

			if (data->field_A == 3) {
				playerdata->Action = 1;
				playerco2->Speed.x = 3;
			}
			else if (data->field_A > 20) {
				data->Action = 2;
				data->Rotation.x = 0;
				data->field_A = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
			}
		}

		PlayHeroesAnimation(obj, 13, CharmyAnimData, 0, 0);
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

		PlayHeroesAnimation(obj, 39, CharmyAnimData, 0, 0);
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

void LoadCharmyFiles(const char *path, const HelperFunctions &helperFunctions) {
	CharmyMdls[0] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\charmy_main.sa2mdl"));
	CharmyMdls[1] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\charmy_eyelashes.sa2mdl"));
	CharmyMdls[2] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\charmy_wings.sa2mdl"));
	CharmyMdls[3] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\charmy_objs.sa2mdl"));

	CharmyAnms[0] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BEE_HANE_LOCATOR.saanim"));

	CharmyAnms[1] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_WALK_PULL.saanim"));
	CharmyAnms[2] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_WALK_PUSH.saanim"));
	CharmyAnms[3] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_TURN_L.saanim"));
	CharmyAnms[4] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_TURN_R.saanim"));
	CharmyAnms[5] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_SLOW_RUN.saanim"));
	CharmyAnms[6] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_MID_RUN.saanim"));
	CharmyAnms[7] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_TOP_RUN.saanim"));
	CharmyAnms[8] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_START.saanim"));
	CharmyAnms[9] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_IDLE.saanim"));
	CharmyAnms[10] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_IDLE_B.saanim"));
	CharmyAnms[11] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_IDLE_C_HALF.saanim"));
	CharmyAnms[12] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_WIN.saanim"));
	CharmyAnms[13] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_ATC_HARI.saanim"));
	CharmyAnms[14] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_POW_ROT.saanim"));
	CharmyAnms[15] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_TRAP_JUMP.saanim"));
	CharmyAnms[16] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FW_JUMP.saanim"));
	CharmyAnms[17] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CAO_BE.saanim"));
	CharmyAnms[18] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_A.saanim"));
	CharmyAnms[19] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_B.saanim"));
	CharmyAnms[20] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_C.saanim"));
	CharmyAnms[21] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_D.saanim"));
	CharmyAnms[22] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_E.saanim"));
	CharmyAnms[23] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_F.saanim"));
	CharmyAnms[24] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GUM.saanim"));
	CharmyAnms[25] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_JUMP_GLIND.saanim"));
	CharmyAnms[26] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND.saanim"));
	CharmyAnms[27] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_BK.saanim"));
	CharmyAnms[28] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_BK_L.saanim"));
	CharmyAnms[29] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_BK_R.saanim"));
	CharmyAnms[30] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_FLIP_BR.saanim"));
	CharmyAnms[31] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_FLIP_FR.saanim"));
	CharmyAnms[32] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_L.saanim"));
	CharmyAnms[33] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_GLIND_R.saanim"));
	CharmyAnms[34] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_IDLE.saanim"));
	CharmyAnms[35] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_SLOW.saanim"));
	CharmyAnms[36] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_UP.saanim"));
	CharmyAnms[37] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_PULL.saanim"));
	CharmyAnms[38] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_PUSH.saanim"));
	CharmyAnms[39] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_KICK.saanim"));
	CharmyAnms[40] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_HANG_IDLE.saanim"));
	CharmyAnms[41] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_HANG_OFF.saanim"));
	CharmyAnms[42] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLY_HANG_ON.saanim"));
	CharmyAnms[43] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_HANG_ON.saanim"));
	CharmyAnms[44] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BREAK_A.saanim"));
	CharmyAnms[45] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BREAK_B.saanim"));
	CharmyAnms[46] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BREAK_C.saanim"));
	CharmyAnms[47] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BRA_MID.saanim"));
	CharmyAnms[48] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BRA_TOP.saanim"));
	CharmyAnms[49] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_BOB.saanim"));
	CharmyAnms[50] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_FLORT.saanim"));
	CharmyAnms[51] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_DAM_M_A.saanim"));
	CharmyAnms[52] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_DAM_M_B.saanim"));
	CharmyAnms[53] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_DAM_M_C.saanim"));
	CharmyAnms[54] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_EDGE_OTTO_A.saanim"));
	CharmyAnms[55] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_EDGE_OTTO_B.saanim"));
	CharmyAnms[56] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\BE_EDGE_OTTO_C.saanim"));

	for (uint8_t i = 1; i < LengthOfArray(CharmyAnimData); ++i) {
		CharmyAnimData[i].Animation = new NJS_ACTION;
		CharmyAnimData[i].Animation->object = CharmyMdls[0]->getmodel();
		CharmyAnimData[i].Animation->motion = CharmyAnms[i]->getmotion();
		CharmyAnimData[i].Instance = 68;
		CharmyAnimData[i].NextAnim = i;
		CharmyAnimData[i].Property = 3;
		CharmyAnimData[i].TransitionSpeed = 0.25f;
		CharmyAnimData[i].AnimationSpeed = 0.5f;
	}
	
	CharmyAnimData[0] = CharmyAnimData[5];

	CharmyAnimData[9].NextAnim = 8;
	CharmyAnimData[10].NextAnim = 8;
	CharmyAnimData[18].NextAnim = 19;
	CharmyAnimData[20].NextAnim = 21;
	CharmyAnimData[22].NextAnim = 23;
	CharmyAnimData[51].Property = 1;
	CharmyAnimData[52].Property = 1;
	CharmyAnimData[32].Property = 1;
	CharmyAnimData[25].Property = 1;
	CharmyAnimData[30].Property = 1;
	CharmyAnimData[31].Property = 1;
	CharmyAnimData[33].Property = 1;
	CharmyAnimData[53].NextAnim = 11;
	CharmyAnimData[23].AnimationSpeed = 0.25f;

	CharmyAnimData[18].AnimationSpeed = 1;
	CharmyAnimData[19].AnimationSpeed = 1;
	CharmyAnimData[20].AnimationSpeed = 1;
	CharmyAnimData[21].AnimationSpeed = 1;
	CharmyAnimData[22].AnimationSpeed = 1;
	CharmyAnimData[23].AnimationSpeed = 1;

	CWingsAnimData.Animation = new NJS_ACTION;
	CWingsAnimData.Animation->object = CharmyMdls[2]->getmodel();
	CWingsAnimData.Animation->motion = CharmyAnms[0]->getmotion();
	CWingsAnimData.Instance = 68;
	CWingsAnimData.NextAnim = 0;
	CWingsAnimData.Property = 3;
	CWingsAnimData.TransitionSpeed = 0.25f;
	CWingsAnimData.AnimationSpeed = 0.5f;
}