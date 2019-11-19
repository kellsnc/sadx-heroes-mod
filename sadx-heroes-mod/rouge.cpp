#include "stdafx.h"
#include "mod.h"
#include "characters.h"
#include "objects.h"

ModelInfo* RougeMdls[3];
AnimationFile* RougeAnms[66];
AnimData RougeAnimData[60];
AnimData RWingsAnimData[6];

NJS_TEXNAME ROUGE_TEXNAMES[7];
NJS_TEXLIST ROUGE_TEXLIST = { arrayptrandlength(ROUGE_TEXNAMES) };

NJS_MATRIX RougeMatrices[2];

enum RougeSounds {
	RougeSound_Attack = 11000,
	RougeSound_FlyBegin,
	RougeSound_FlyAttack,
	RougeSound_FlyUp,
	RougeSound_TeamSwap,
	RougeSound_ThatHurts,
	RougeSound_Hurt1,
	RougeSound_Hurt2,
	RougeSound_Trick,
	RougeSound_BombAttack,
	RougeSound_Death,
	RougeSound_LevelUp,
	RougeSound_Win,
	RougeSound_Lose,
	RougeSound_Idle1,
	RougeSound_Idle2
};

void PlayVoice_Rouge(int ID) {
	switch (ID) {
	case 1803:
		PlayVoice(RougeSound_Win);
		break;
	}
}

int PlaySound_Rouge(int ID, void *a2, int a3, void *a4) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayVoice(RougeSound_Trick);
		break;
	case 1243:
		PlayVoice(RougeSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayVoice(RougeSound_ThatHurts);
		else if (random < 8) PlayVoice(RougeSound_Hurt1);
		else PlayVoice(RougeSound_Hurt2);
		break;
	case 1465:
		PlayVoice(RougeSound_Death);
		break;
	case 1453:
		PlaySound(ID, a2, a3, a4);
		break;
	}

	return 1;
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

void RougeHeroes_Delete(ObjectMaster *obj) {
	bool IsThereRouge = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (HeroesChars[obj->Data1->CharIndex]->Data1->CharID == Characters_Rouge) IsThereRouge = true;
		}
	}

	if (!IsThereRouge) {
		njReleaseTexture(&ROUGE_TEXLIST);
		CharTexsLoaded[Characters_Rouge - 9] = false;
	}

	CharactersCommon_Delete(obj);
}

void RougeHeroes_Display(ObjectMaster *obj) {
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

	if (data->Action == 0) {
		obj->DeleteSub = RougeHeroes_Delete;
		data->CharID = Characters_Rouge;
		data->Action = 1;

		if (!CharTexsLoaded[Characters_Rouge - 9]) {
			CharTexsLoaded[Characters_Rouge - 9] = true;
			LoadPVM("rouge", &ROUGE_TEXLIST);
		}

		return;
	}
	else if (data->Action == 1) {
		if (GameState == 4 || GameState == 15) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), RougeWings_Main, obj);
		data->Action = 2;
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!playerobj || playerobj->Data1->CharID != Characters_Tails) { DeleteObject_(obj); return; }
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), RougeWings_Main, obj);

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
		PlayerPtrs[data->CharIndex]->DisplaySub = RougeHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
			PlayVoice(RougeSound_FlyAttack);
			data->field_A = 0;
			data->Action = 4;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
			if (++data->field_A == 2) {
				PlayVoice(RougeSound_FlyUp);
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
				PlayVoice(RougeSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayVoice(RougeSound_Idle2);
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
				anim = 0;
				if (playerco2->Speed.x < 0.02f) anim = 11;
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
				anim = 44;
				break;
			case 72: //holding hook
				anim = 51;
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
		PlayHeroesAnimation(obj, anim, RougeAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayVoice(RougeSound_BombAttack);
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
		if (++data->InvulnerableTime > 2) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadRougeFiles(const char *path, const HelperFunctions &helperFunctions) {
	RougeMdls[0] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\rouge_main.sa2mdl"));
	RougeMdls[1] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\rouge_eyelashes.sa2mdl"));
	RougeMdls[2] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\rouge_wings.sa2mdl"));

	RougeAnms[0] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_WALK.saanim"));
	RougeAnms[1] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_WALK_PULL.saanim"));
	RougeAnms[2] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_WALK_PUSH.saanim"));
	RougeAnms[3] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_TURN_L.saanim"));
	RougeAnms[4] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_TURN_R.saanim"));
	RougeAnms[5] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_SLOW_RUN.saanim"));
	RougeAnms[6] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_MID_RUN.saanim"));
	RougeAnms[7] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_TOP_RUN.saanim"));
	RougeAnms[8] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_START.saanim"));
	RougeAnms[9] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_IDLE.saanim"));
	RougeAnms[10] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_IDLE_B.saanim"));
	RougeAnms[11] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_IDLE_C_HALF.saanim"));
	RougeAnms[12] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_WIN.saanim"));
	RougeAnms[13] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_ATC_BOMB.saanim"));
	RougeAnms[14] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_POW_ROT.saanim"));
	RougeAnms[15] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_TRAP_JUMP.saanim"));
	RougeAnms[16] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FW_JUMP.saanim"));
	RougeAnms[17] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\DARK_RO.saanim"));
	RougeAnms[18] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_A.saanim"));
	RougeAnms[19] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_B.saanim"));
	RougeAnms[20] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_C.saanim"));
	RougeAnms[21] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_D.saanim"));
	RougeAnms[22] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_E.saanim"));
	RougeAnms[23] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_F.saanim"));
	RougeAnms[24] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_TRNGL.saanim"));
	RougeAnms[25] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_JUMP_GLIND.saanim"));
	RougeAnms[26] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND.saanim"));
	RougeAnms[27] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_BK.saanim"));
	RougeAnms[28] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_BK_L.saanim"));
	RougeAnms[29] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_BK_R.saanim"));
	RougeAnms[30] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_FLIP_B.saanim"));
	RougeAnms[31] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_FLIP_F.saanim"));
	RougeAnms[32] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_L.saanim"));
	RougeAnms[33] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_GLIND_R.saanim"));
	RougeAnms[34] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_IDLE.saanim"));
	RougeAnms[35] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_SLOW.saanim"));
	RougeAnms[36] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_UP.saanim"));
	RougeAnms[37] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_PULL.saanim"));
	RougeAnms[38] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_PUSH.saanim"));
	RougeAnms[39] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_KICK.saanim"));
	RougeAnms[40] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_HANG_IDLE.saanim"));
	RougeAnms[41] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_HANG_OFF.saanim"));
	RougeAnms[42] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_HANG_ON.saanim"));
	RougeAnms[43] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_HANG_OFF.saanim"));
	RougeAnms[44] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_HANG_ON.saanim"));
	RougeAnms[45] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BREAK_A.saanim"));
	RougeAnms[46] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BREAK_B.saanim"));
	RougeAnms[47] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BREAK_C.saanim"));
	RougeAnms[48] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BREAK_TURN_L.saanim"));
	RougeAnms[49] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BREAK_TURN_R.saanim"));
	RougeAnms[50] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BRA_MID.saanim"));
	RougeAnms[51] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BRA_TOP.saanim"));
	RougeAnms[52] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_BOB.saanim"));
	RougeAnms[53] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLORT.saanim"));
	RougeAnms[54] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_DAM_M_A.saanim"));
	RougeAnms[55] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_DAM_M_B.saanim"));
	RougeAnms[56] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_DAM_M_C.saanim"));
	RougeAnms[57] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_EDGE_OTTO_A.saanim"));
	RougeAnms[58] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_EDGE_OTTO_B.saanim"));
	RougeAnms[59] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_EDGE_OTTO_C.saanim"));

	RougeAnms[60] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_IDLE_WNGL.saanim"));
	RougeAnms[61] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_FLY_UP_WING.saanim"));
	RougeAnms[62] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_IDLE_C_HANE_HALF.saanim"));
	RougeAnms[63] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_IDLE_WNGS.saanim"));
	RougeAnms[64] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\SL_HANE.saanim"));
	RougeAnms[65] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\RO_WING_WIN.saanim"));

	for (uint8_t i = 0; i < LengthOfArray(RougeAnimData); ++i) {
		RougeAnimData[i].Animation = new NJS_ACTION;
		RougeAnimData[i].Animation->object = RougeMdls[0]->getmodel();
		RougeAnimData[i].Animation->motion = RougeAnms[i]->getmotion();
		RougeAnimData[i].Instance = 68;
		RougeAnimData[i].NextAnim = i;
		RougeAnimData[i].Property = 3;
		RougeAnimData[i].TransitionSpeed = 0.25f;
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
		RWingsAnimData[i].Animation = new NJS_ACTION;
		RWingsAnimData[i].Animation->object = RougeMdls[2]->getmodel();
		RWingsAnimData[i].Animation->motion = RougeAnms[i + 60]->getmotion();
		RWingsAnimData[i].Instance = 68;
		RWingsAnimData[i].NextAnim = i;
		RWingsAnimData[i].Property = 3;
		RWingsAnimData[i].TransitionSpeed = 0.25f;
		RWingsAnimData[i].AnimationSpeed = 0.5f;
	}

	RWingsAnimData[6].Property = 1;
}