#include "stdafx.h"
#include "mod.h"
#include "characters.h"
#include "objects.h"

ModelInfo* CreamMdls[4];
AnimationFile* CreamAnms[76];
AnimData CreamAnimData[63];
AnimData CheeseAnimData[13];

ObjectMaster* Creams[8];

NJS_TEXNAME CREAM_TEXNAMES[4];
NJS_TEXLIST CREAM_TEXLIST = { arrayptrandlength(CREAM_TEXNAMES) };

CollisionData Cheese_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, { 4.5, 0.0f, 0.0f }, 0, 0 };

enum CreamSounds {
	CreamSound_Attack = 10900,
	CreamSound_FlyBegin,
	CreamSound_FlyAttack,
	CreamSound_FlyUp,
	CreamSound_TeamSwap,
	CreamSound_ThatHurts,
	CreamSound_Hurt1,
	CreamSound_Hurt2,
	CreamSound_Trick,
	CreamSound_CheeseGetHim,
	CreamSound_Death,
	CreamSound_LevelUp,
	CreamSound_Win,
	CreamSound_Lose,
	CreamSound_Idle1,
	CreamSound_Idle2,
	CreamSound_Fall
};

void PlayVoice_Tails(int ID) {
	switch (ID) {
	case 1803:
		PlayVoice(CreamSound_Win);
		break;
	default:
		PlayVoice(ID);
		break;
	}
}

int PlaySound_Tails(int ID, void *a2, int a3, void *a4) {
	if (Creams[CurrentPlayer] == nullptr) {
		return PlaySound(ID, a2, a3, a4);
	}

	int random = rand() % 10;
	
	switch (ID) {
	case 17:
		PlayVoice(CreamSound_Trick);
		break;
	case 1243:
		PlayVoice(CreamSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayVoice(CreamSound_ThatHurts);
		else if (random < 8) PlayVoice(CreamSound_Hurt1);
		else PlayVoice(CreamSound_Hurt2);
		break;
	case 1465:
		PlayVoice(CreamSound_Death);
		break;
	case 1453:
		PlaySound(ID, a2, a3, a4);
		break;
	}

	return 1;
}

NJS_VECTOR GetCheesePoint(NJS_VECTOR* pos, Rotation3* rot) {
	NJS_VECTOR point;

	NJS_VECTOR dir = { -2, 10, 2 };
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, pos);
	njRotateY(0, -rot->y);
	njCalcPoint(0, &dir, &point);
	njPopMatrix(1u);
	return point;
}

ObjectMaster* Cheese_GetClosestEnemy(NJS_VECTOR* pos) {
	ObjectMaster * current = ObjectListThing[3];
	while (1) {
		if (current->MainSub == Kiki_Main || current->MainSub == RhinoTank_Main || current->MainSub == Sweep_Main) {
			float dist = GetDistance(pos, &current->Data1->Position);
			if (GetDistance(pos, &current->Data1->Position) < 200) return current;
			else {
				if (current->Next) {
					current = current->Next;
					continue;
				}
				else break;
			}
		}
		else {
			if (current->Next) current = current->Next;
			else break;
		}
	}
	return nullptr;
}

void Cheese_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData1* playerdata = EntityData1Ptrs[obj->Parent->Data1->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[obj->Parent->Data1->CharIndex];

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	if (playerco2 && (playerco2->AnimationThing.Index == 54 || playerco2->AnimationThing.Index == 55)) {
		njTranslateV(0, &playerdata->Position);
		njTranslate(0, 10, 12, 0);
		njRotateZ(0, playerdata->Rotation.z);
		njRotateX(0, playerdata->Rotation.x);
		njRotateY(0, -playerdata->Rotation.y - 0x4000);
	}
	else {
		njTranslateV(0, &data->Position);
		njRotateZ(0, data->Rotation.z);
		njRotateX(0, data->Rotation.x);

		if (data->Index == 0) {
			njRotateY(0, data2->Forward.y += 0x100);
			njTranslate(0, 2, 0, 0);
		}
		else {
			njRotateY(0, -data->Rotation.y - 0x4000);
		}
	}
	
	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	njCnkAction(CheeseAnimData[data->Index].Animation, data->Scale.x);

	njTranslate(0, -data->Position.x, -data->Position.y, -data->Position.z);
	njTranslateV(0, &data2->SomeCollisionVector);
	njTranslate(0, 0, 5, -1);
	DrawChunkModel(CreamMdls[3]->getmodel()->child->chunkmodel);

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void Cheese_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData1* playerdata = EntityData1Ptrs[obj->Parent->Data1->CharIndex];

	float dist = GetDistance(&data2->VelocityDirection, &data->Position);
	uint8_t anim = data->Index;

	float state = 0;
	float frame = data->Scale.x;

	switch (data->Action) {
	case 0:
		obj->DisplaySub = Cheese_Display;
		data->Position = GetCheesePoint(&playerdata->Position, &playerdata->Rotation);
		Collision_Init(obj, &Cheese_Col, 1, 3u);
		data->Action = 1;
		break;
	case 1:
		data2->VelocityDirection = GetCheesePoint(&playerdata->Position, &playerdata->Rotation);
		data->Rotation = playerdata->Rotation;

		if (dist < 5) {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / (100 + (400 - dist)));
			data2->SomeCollisionVector = data->Position;
			anim = 0;
			if (GetDistance(&data2->VelocityDirection, &data->Position) < 1) {
				data->Position = data2->VelocityDirection;
				data->Action = 1;
			}
		}
		else if (dist < 30) {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / 400);
			data2->SomeCollisionVector = GetPathPosition(&data->Position, &data2->VelocityDirection, (dist / 400) - 0.1f);
			data->Rotation.y = fPositionToRotation(&data->Position, &data2->VelocityDirection).y;
			anim = 5;
		}
		else {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / 300);
			data2->SomeCollisionVector = GetPathPosition(&data->Position, &data2->VelocityDirection, (dist / 300) - 0.1f);
			data->Rotation.y = fPositionToRotation(&data->Position, &data2->VelocityDirection).y;
			anim = 5;
		}

		switch (CharObj2Ptrs[obj->Parent->Data1->CharIndex]->AnimationThing.Index) {
		case 6:
			if (obj->Parent->Data1->Index == 8) anim = 1;
			else anim = 2;
			break;
		case 33:
			if (dist < 30) anim = 3;
			break;
		case 54:
		case 55:
			anim = 8;
			if (data->Unknown > 1) {
				if (frame > 154) data->Unknown = 20;
				if (frame < 114) data->Unknown = 40;

				if (data->Unknown > 20 && data->Unknown < 40) {
					++data->Unknown;
					state = 154 - (data->Unknown - 20);
				}
				else if (data->Unknown > 40) {
					++data->Unknown;
					state = 114 + (data->Unknown - 40);
				}
			}
			break;
		case 150:
			anim = 12;
			break;
		}

		break;
	case 2: //cheese special attack, get the closest enemy
		data->LoopData = (Loop*)Cheese_GetClosestEnemy(&playerdata->Position);
		
		if (data->LoopData) {
			data->NextAction = 0;
			data->Action = 3;
		}
		else {
			data->Action = 1;
		}

		break;
	case 3: //attack enemy
		ObjectMaster* enemy = (ObjectMaster*)data->LoopData;
		EntityData1* enemydata = enemy->Data1;

		if (data->InvulnerableTime < 50) {
			anim = 6;
			++data->InvulnerableTime;
			break;
		}

		if (!enemydata) {
			data->InvulnerableTime = 0;
			data->LoopData = (Loop*)Cheese_GetClosestEnemy(&playerdata->Position);
			if (data->LoopData) {
				data->NextAction = 0;
				break;
			}
			else {
				data->Action = 1;
			}
		}
		else {
			data->Position = GetPathPosition(&data->Position, &enemydata->Position, dist / 500);
			data2->SomeCollisionVector = GetPathPosition(&data->Position, &enemydata->Position, (dist / 500) - 0.1f);
		}
		
		break;
	}
	
	AddToCollisionList(data);
	PlayHeroesAnimation(obj, anim, CheeseAnimData, 0, state);

	obj->DisplaySub(obj);
}

NJS_MATRIX EyeLashesMatrix;

void CreamCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)CreamMdls[0]->getdata("Dummy019")) {
		memcpy(EyeLashesMatrix, _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
	}
}

void CreamHeroes_Delete(ObjectMaster *obj) {
	bool IsThereCream = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (Creams[player]) IsThereCream = true;
		}
	}

	if (!IsThereCream) {
		njReleaseTexture(&CREAM_TEXLIST);
		CharTexsLoaded[9 - Characters_Cream] = false;
	}

	Creams[obj->Data1->CharIndex] = nullptr;

	ObjectMaster* playerobj = PlayerPtrs[obj->Data1->CharIndex];
	if (playerobj) {
		EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
		CharObj2* playerco2 = playerdata2->CharacterData;

		playerobj->DisplaySub = Tails_Display;

		playerco2->PhysicsData.MaxAccel = PhysicsArray[2].MaxAccel;
		playerco2->PhysicsData.field_14 = PhysicsArray[2].field_14;
		playerco2->PhysicsData.AirAccel = PhysicsArray[2].AirAccel;
	}
}

void CreamHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* creamobj = Creams[obj->Data1->CharIndex];
	if (!creamobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (creamobj->Data1->NextAction == 0) creamobj->Data1->NextAction = 1;
			else creamobj->Data1->NextAction = 0;
		}
	}
	else {
		creamobj->Data1->NextAction = 0;
	}

	if (creamobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000 + creamobj->Data1->Rotation.y);

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	*NodeCallbackFuncPtr = CreamCallback;
	njCnkAction(CreamAnimData[creamobj->Data1->Index].Animation, creamobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, EyeLashesMatrix, sizeof(NJS_MATRIX));

	switch (creamobj->Data1->InvulnerableTime) {
	case 1:
		DrawChunkModel(CreamMdls[1]->getmodel()->child->chunkmodel);
		break;
	case 2:
		DrawChunkModel(CreamMdls[1]->getmodel()->child->child->chunkmodel);
		break;
	case 3:
		DrawChunkModel(CreamMdls[1]->getmodel()->child->child->child->chunkmodel);
		break;
	case 4:
		DrawChunkModel(CreamMdls[1]->getmodel()->child->child->child->child->chunkmodel);
		break;
	default:
		DrawChunkModel(CreamMdls[1]->getmodel()->chunkmodel);
		break;
	}

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CreamHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DeleteSub = CreamHeroes_Delete;
		data->CharID = Characters_Cream;
		data->Action = 1;

		if (!CharTexsLoaded[9 - Characters_Cream]) {
			CharTexsLoaded[9 - Characters_Cream] = true;
			LoadPVM("cream", &CREAM_TEXLIST);
		}

		return;
	}
	else if (data->Action == 1) {
		if (GameState == 4 || GameState == 15) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), Cheese_Main, obj);
		data->Action = 2;
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!playerobj || playerobj->Data1->CharID != Characters_Tails) {DeleteObject_(obj); return; }
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), Cheese_Main, obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	playerco2->PhysicsData.MaxAccel = 2.5f;
	playerco2->PhysicsData.field_14 = 0.8f;
	playerco2->PhysicsData.AirAccel = 0.035999999f;

	int anim = data->Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = CreamHeroes_Display;

		if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
			PlayVoice(CreamSound_FlyAttack);
			data->field_A = 0;
			data->Action = 4;
		}

		if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
			if (++data->field_A == 2) {
				PlayVoice(CreamSound_FlyUp);
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
				PlayVoice(CreamSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayVoice(CreamSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		if (anim != 20) {
			switch (playerco2->AnimationThing.Index) {
			case 0: anim = 11; data->Status = 0; break;
			case 2: anim = 8; data->Status = 0; break;
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
				if (playerco2->Speed.x < 0.02f) anim = 11;
				break;
			case 10:
				anim = 0;
				speed = 0.9f + playerco2->Speed.x * 0.2f;
				break;
			case 11:
				anim = 0;
				speed = 0.9f + playerco2->Speed.x * 0.2f;
				break;
			case 12:
				anim = 5;
				speed = 1.5f + playerco2->Speed.x * 0.1f;
				break;
			case 13:
				anim = 6;
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
		PlayHeroesAnimation(obj, anim, CreamAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			PlayVoice(CreamSound_CheeseGetHim);
			data->field_A = 1;
			data->Scale.x = 0;
		}
		else if (data->field_A < 30) {
			if (data->field_A < 20) {
				++data->field_A;
			}
			else {
				obj->Child->Data1->Action = 2;
				data->field_A = 30;
			}
		}
		else {
			if (++data->field_A == 120) {
				data->Action = 2;
				playerdata->Action = 1;
				data->field_A = 0;
			}
		}

		PlayHeroesAnimation(obj, 61, CreamAnimData, 0, 0);
		break;
	case 4:
		if (++data->field_A == 100) {
			data->Action = 2;
			data->field_A = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
		}
		else {
			playerco2->Powerups |= Powerups_Invincibility;
		}
		
		PlayHeroesAnimation(obj, 39, CreamAnimData, 0, 0);
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

void LoadCreamFiles(const char *path, const HelperFunctions &helperFunctions) {
	CreamMdls[0] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cream_main.sa2mdl"));
	CreamMdls[1] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cream_eyelashes.sa2mdl"));
	CreamMdls[2] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cheese_main.sa2mdl"));
	CreamMdls[3] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cheese_ball.sa2mdl"));

	CreamAnms[0] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK.saanim"));
	CreamAnms[1] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK_PULL.saanim"));
	CreamAnms[2] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK_PUSH.saanim"));
	CreamAnms[3] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_TURN_L.saanim"));
	CreamAnms[4] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_TURN_R.saanim"));
	CreamAnms[5] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_SLOW_RUN.saanim"));
	CreamAnms[6] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_MID_RUN.saanim"));
	CreamAnms[7] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_START.saanim"));
	CreamAnms[8] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_START_IDLE.saanim"));
	CreamAnms[9] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_B_HALF.saanim"));
	CreamAnms[10] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_C_HALF.saanim"));
	CreamAnms[11] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_HALF.saanim"));
	CreamAnms[12] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_POW_HALF.saanim"));
	CreamAnms[13] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_POW_SHAKE.saanim"));
	CreamAnms[14] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_POW_ROT.saanim"));
	CreamAnms[15] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_POW_JUMP.saanim"));
	CreamAnms[16] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FW_JUMP.saanim"));
	CreamAnms[17] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FW_POW_JUMP.saanim"));
	CreamAnms[18] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_A.saanim"));
	CreamAnms[19] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_B.saanim"));
	CreamAnms[20] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_C.saanim"));
	CreamAnms[21] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_D.saanim"));
	CreamAnms[22] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_E.saanim"));
	CreamAnms[23] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_F.saanim"));
	CreamAnms[24] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_UMBRELLA.saanim"));
	CreamAnms[25] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_GLIND.saanim"));
	CreamAnms[26] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND.saanim"));
	CreamAnms[27] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK.saanim"));
	CreamAnms[28] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK_L.saanim"));
	CreamAnms[29] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK_R.saanim"));
	CreamAnms[30] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_FLIP_BK.saanim"));
	CreamAnms[31] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_FLIP_FR.saanim"));
	CreamAnms[32] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_L.saanim"));
	CreamAnms[33] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_R.saanim"));
	CreamAnms[34] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_IDLE.saanim"));
	CreamAnms[35] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_SLOW.saanim"));
	CreamAnms[36] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_UP.saanim"));
	CreamAnms[37] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_PULL.saanim"));
	CreamAnms[38] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_PUSH.saanim"));
	CreamAnms[39] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_KICK.saanim"));
	CreamAnms[40] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_IDLE.saanim"));
	CreamAnms[41] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_OFF.saanim"));
	CreamAnms[42] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_ON.saanim"));
	CreamAnms[43] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_HANG_OFF.saanim"));
	CreamAnms[44] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_HANG_ON.saanim"));
	CreamAnms[45] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_A.saanim"));
	CreamAnms[46] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_B.saanim"));
	CreamAnms[47] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_C.saanim"));
	CreamAnms[48] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_TURN_L.saanim"));
	CreamAnms[49] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_TURN_R.saanim"));
	CreamAnms[50] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BRA_MID.saanim"));
	CreamAnms[51] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BRA_TOP.saanim"));
	CreamAnms[52] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BOB.saanim"));
	CreamAnms[53] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLORT.saanim"));
	CreamAnms[54] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_A.saanim"));
	CreamAnms[55] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_B.saanim"));
	CreamAnms[56] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_C.saanim"));
	CreamAnms[57] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_A.saanim"));
	CreamAnms[58] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_B.saanim"));
	CreamAnms[59] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_C.saanim"));
	CreamAnms[60] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WIN.saanim"));
	CreamAnms[61] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_ATC_CHAO.saanim"));
	CreamAnms[62] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\ROSE_CR.saanim"));

	CreamAnms[63] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_IDLE.saanim"));
	CreamAnms[64] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_IDLE_B.saanim"));
	CreamAnms[65] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_IDLE_B_OPT.saanim"));
	CreamAnms[66] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_IDLE_C.saanim"));
	CreamAnms[67] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_IDLE_C_OPT.saanim"));
	CreamAnms[68] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_FLY.saanim"));
	CreamAnms[69] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_ATC_CHAO_A.saanim"));
	CreamAnms[70] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_ATC_CHAO_B.saanim"));
	CreamAnms[71] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_WIN.saanim"));
	CreamAnms[72] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_CHARANGE_IDLE.saanim"));
	CreamAnms[73] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_CHARANGE_SELECT.saanim"));
	CreamAnms[74] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_STORY_IDLE.saanim"));
	CreamAnms[75] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CH_STORY_SELECT.saanim"));

	for (uint8_t i = 0; i < LengthOfArray(CreamAnimData); ++i) {
		CreamAnimData[i].Animation = new NJS_ACTION;
		CreamAnimData[i].Animation->object = CreamMdls[0]->getmodel();
		CreamAnimData[i].Animation->motion = CreamAnms[i]->getmotion();
		CreamAnimData[i].Instance = 68;
		CreamAnimData[i].NextAnim = i;
		CreamAnimData[i].Property = 3;
		CreamAnimData[i].TransitionSpeed = 0.25f;
		CreamAnimData[i].AnimationSpeed = 0.5f;
	}

	CreamAnimData[9].NextAnim = 8;
	CreamAnimData[10].NextAnim = 8;
	CreamAnimData[18].NextAnim = 19;
	CreamAnimData[20].NextAnim = 21;
	CreamAnimData[22].NextAnim = 23;
	CreamAnimData[54].Property = 1;
	CreamAnimData[55].Property = 1;
	CreamAnimData[60].Property = 1;
	CreamAnimData[32].Property = 1;
	CreamAnimData[25].Property = 1;
	CreamAnimData[30].Property = 1;
	CreamAnimData[31].Property = 1;
	CreamAnimData[33].Property = 1;
	CreamAnimData[56].NextAnim = 11;
	CreamAnimData[61].Property = 1;
	CreamAnimData[23].AnimationSpeed = 0.25f;

	for (uint8_t i = 0; i < LengthOfArray(CheeseAnimData); ++i) {
		CheeseAnimData[i].Animation = new NJS_ACTION;
		CheeseAnimData[i].Animation->object = CreamMdls[2]->getmodel();
		CheeseAnimData[i].Animation->motion = CreamAnms[i + 63]->getmotion();
		CheeseAnimData[i].Instance = 68;
		CheeseAnimData[i].NextAnim = i;
		CheeseAnimData[i].Property = 3;
		CheeseAnimData[i].TransitionSpeed = 0.25f;
		CheeseAnimData[i].AnimationSpeed = 0.5f;
	}

	CheeseAnimData[6].NextAnim = 7;
	CheeseAnimData[8].Property = 1;
}