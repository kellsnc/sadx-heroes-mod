#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* CharmyMdls[3];
AnimationFile* CharmyAnms[58];
AnimData CharmyAnimData[57];
AnimData CWingsAnimData;

NJS_TEXNAME CHARMY_TEXNAMES[5];
NJS_TEXLIST CHARMY_TEXLIST = { arrayptrandlength(CHARMY_TEXNAMES) };

NJS_MATRIX CharmyMatrices[2];

void PlayVoice_Charmy(int ID) {
	switch (ID) {
	case 1803:
		PlayHeroesSound(CharmySound_Win);
		break;
	}
}

void PlaySound_Charmy(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(CharmySound_Trick);
		break;
	case 1243:
		PlayHeroesSound(CharmySound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayHeroesSound(CharmySound_ThatHurts);
		else if (random < 8) PlayHeroesSound(CharmySound_Hurt1);
		else PlayHeroesSound(CharmySound_Hurt2);
		break;
	case 1465:
		PlayHeroesSound(CharmySound_Death);
		break;
	case 1453:
		PlaySound(ID, 0, 0, 0);
		break;
	}
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
	NJS_OBJECT* base = CharmyMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(CharmyMatrices[0]); //eyelashes
	}
	else if (object == base->getnode(2)) {
		njGetMatrix(CharmyMatrices[1]); //wings
	}
}

void CharmyHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

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

	njRotateX(0, 0x4000);

	*NodeCallbackFuncPtr = CharmyCallback;
	njActionWeight(CharmyAnimData[Charmyobj->Data1->Index].Animation, Charmyobj->Data1->Scale.x, CharmyMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, CharmyMatrices[0]);
	NJS_CNK_OBJECT* eyelashes = CharmyMdls[1]->getmodel();
	switch (Charmyobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		dsDrawModel(eyelashes->child->getbasicdxmodel());
		break;
	case 2:
	case 6:
		dsDrawModel(eyelashes->child->child->getbasicdxmodel());
		break;
	case 3:
	case 5:
		dsDrawModel(eyelashes->child->child->child->getbasicdxmodel());
		break;
	case 4:
		dsDrawModel(eyelashes->child->child->child->getbasicdxmodel());
		break;
	}

	if (Charmyobj->Child) {
		njSetMatrix(NULL, CharmyMatrices[1]);
		njTranslate(0, 0, 0.2f, -1.2f);
		HelperFunctionsGlobal.Weights->Apply(CharmyMdls[2]->getweightinfo(), CWingsAnimData.Animation, Charmyobj->Child->Data1->Scale.x);
		late_Action(CWingsAnimData.Animation, Charmyobj->Child->Data1->Scale.x, LATE_MAT);
	}

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CharmyHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (!CharactersCommon_Init(obj, "heroes-charmy", &CHARMY_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), CharmyWings_Main, obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[14].texaddr = CHARMY_TEXLIST.textures[5].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.MaxAccel = 2.5f;
			playerco2->PhysicsData.field_14 = 0.8f;
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
		PlayerPtrs[data->CharIndex]->DisplaySub = CharmyHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 100;
				data->Action = 3;
				break;
			}

			if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
				PlayHeroesSound(CharmySound_FlyAttack);
				data->field_A = 0;
				data->Action = 4;
			}

			if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
				if (++data->field_A == 2) {
					PlayHeroesSound(CharmySound_FlyUp);
					obj->Child->Data1->field_A = 1;
					data->field_A = 0;
					playerco2->Speed.y = 2;
				}
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(CharmySound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(CharmySound_Idle2);
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
			anim = 43;
			break;
		case 72: //holding hook
			anim = 48;
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
			anim = 48;
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
		
		PlayHeroesAnimation(obj, anim, CharmyAnimData, speed, state);

		break;
	case 3:
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayHeroesSound(CharmySound_DardAttack);
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
		if (++data->InvulnerableTime > 7) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadCharmyFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-charmy.arcx"));

	CharmyMdls[0] = arc.GetModel("BEE_LOCATOR.sa1mdl");
	CharmyMdls[1] = arc.GetModel("BE_MABUTA.sa1mdl");
	CharmyMdls[2] = arc.GetModel("BEE_HANE_LOCATOR.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(CharmyMdls[0]->getweightinfo(), CharmyMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->Init(CharmyMdls[2]->getweightinfo(), CharmyMdls[2]->getmodel());

	CharmyAnms[0] = arc.GetAnimation("BEE_HANE_LOCATOR.saanim");
	CharmyAnms[1] = arc.GetAnimation("BE_WALK_PULL.saanim");
	CharmyAnms[2] = arc.GetAnimation("BE_WALK_PUSH.saanim");
	CharmyAnms[3] = arc.GetAnimation("BE_TURN_L.saanim");
	CharmyAnms[4] = arc.GetAnimation("BE_TURN_R.saanim");
	CharmyAnms[5] = arc.GetAnimation("BE_SLOW_RUN.saanim");
	CharmyAnms[6] = arc.GetAnimation("BE_MID_RUN.saanim");
	CharmyAnms[7] = arc.GetAnimation("BE_TOP_RUN.saanim");
	CharmyAnms[8] = arc.GetAnimation("BE_START.saanim");
	CharmyAnms[9] = arc.GetAnimation("BE_IDLE.saanim");
	CharmyAnms[10] = arc.GetAnimation("BE_IDLE_B.saanim");
	CharmyAnms[11] = arc.GetAnimation("BE_IDLE_C_HALF.saanim");
	CharmyAnms[12] = arc.GetAnimation("BE_WIN.saanim");
	CharmyAnms[13] = arc.GetAnimation("BE_ATC_HARI.saanim");
	CharmyAnms[14] = arc.GetAnimation("BE_POW_ROT.saanim");
	CharmyAnms[15] = arc.GetAnimation("BE_TRAP_JUMP.saanim");
	CharmyAnms[16] = arc.GetAnimation("BE_FW_JUMP.saanim");
	CharmyAnms[17] = arc.GetAnimation("CAO_BE.saanim");
	CharmyAnms[18] = arc.GetAnimation("BE_JUMP_A.saanim");
	CharmyAnms[19] = arc.GetAnimation("BE_JUMP_B.saanim");
	CharmyAnms[20] = arc.GetAnimation("BE_JUMP_C.saanim");
	CharmyAnms[21] = arc.GetAnimation("BE_JUMP_D.saanim");
	CharmyAnms[22] = arc.GetAnimation("BE_JUMP_E.saanim");
	CharmyAnms[23] = arc.GetAnimation("BE_JUMP_F.saanim");
	CharmyAnms[24] = arc.GetAnimation("BE_GUM.saanim");
	CharmyAnms[25] = arc.GetAnimation("BE_JUMP_GLIND.saanim");
	CharmyAnms[26] = arc.GetAnimation("BE_GLIND.saanim");
	CharmyAnms[27] = arc.GetAnimation("BE_GLIND_BK.saanim");
	CharmyAnms[28] = arc.GetAnimation("BE_GLIND_BK_L.saanim");
	CharmyAnms[29] = arc.GetAnimation("BE_GLIND_BK_R.saanim");
	CharmyAnms[30] = arc.GetAnimation("BE_GLIND_FLIP_BK.saanim");
	CharmyAnms[31] = arc.GetAnimation("BE_GLIND_FLIP_FR.saanim");
	CharmyAnms[32] = arc.GetAnimation("BE_GLIND_L.saanim");
	CharmyAnms[33] = arc.GetAnimation("BE_GLIND_R.saanim");
	CharmyAnms[34] = arc.GetAnimation("BE_FLY_IDLE.saanim");
	CharmyAnms[35] = arc.GetAnimation("BE_FLY_SLOW.saanim");
	CharmyAnms[36] = arc.GetAnimation("BE_FLY_UP.saanim");
	CharmyAnms[37] = arc.GetAnimation("BE_FLY_PULL.saanim");
	CharmyAnms[38] = arc.GetAnimation("BE_FLY_PUSH.saanim");
	CharmyAnms[39] = arc.GetAnimation("BE_FLY_KICK.saanim");
	CharmyAnms[40] = arc.GetAnimation("BE_FLY_HANG_IDLE.saanim");
	CharmyAnms[41] = arc.GetAnimation("BE_FLY_HANG_OFF.saanim");
	CharmyAnms[42] = arc.GetAnimation("BE_FLY_HANG_ON.saanim");
	CharmyAnms[43] = arc.GetAnimation("BE_HANG_ON.saanim");
	CharmyAnms[44] = arc.GetAnimation("BE_BREAK_A.saanim");
	CharmyAnms[45] = arc.GetAnimation("BE_BREAK_B.saanim");
	CharmyAnms[46] = arc.GetAnimation("BE_BREAK_C.saanim");
	CharmyAnms[47] = arc.GetAnimation("BE_BRA_MID.saanim");
	CharmyAnms[48] = arc.GetAnimation("BE_BRA_TOP.saanim");
	CharmyAnms[49] = arc.GetAnimation("BE_BOB.saanim");
	CharmyAnms[50] = arc.GetAnimation("BE_FLORT.saanim");
	CharmyAnms[51] = arc.GetAnimation("BE_DAM_M_A.saanim");
	CharmyAnms[52] = arc.GetAnimation("BE_DAM_M_B.saanim");
	CharmyAnms[53] = arc.GetAnimation("BE_DAM_M_C.saanim");
	CharmyAnms[54] = arc.GetAnimation("BE_EDGE_OTTO_A.saanim");
	CharmyAnms[55] = arc.GetAnimation("BE_EDGE_OTTO_B.saanim");
	CharmyAnms[56] = arc.GetAnimation("BE_EDGE_OTTO_C.saanim");

	for (uint8_t i = 1; i < LengthOfArray(CharmyAnimData); ++i) {
		if (CharmyAnms[i] == nullptr) continue;
		CharmyAnimData[i].Animation = new NJS_ACTION;
		CharmyAnimData[i].Animation->object = CharmyMdls[0]->getmodel();
		CharmyAnimData[i].Animation->motion = CharmyAnms[i]->getmotion();
		CharmyAnimData[i].NextAnim = i;
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
	CWingsAnimData.NextAnim = 0;
}

void UnloadCharmyFiles() {
	HelperFunctionsGlobal.Weights->DeInit(CharmyMdls[0]->getweightinfo(), CharmyMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(CharmyMdls[2]->getweightinfo(), CharmyMdls[2]->getmodel());
	FreeMDLFiles(CharmyMdls, LengthOfArray(CharmyMdls));
	FreeANMFiles(CharmyAnms, LengthOfArray(CharmyAnms));
}