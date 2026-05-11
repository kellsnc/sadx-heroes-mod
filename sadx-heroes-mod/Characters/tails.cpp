#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

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
		data->CollisionInfo->colli_range = 2.0f;
		
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
	NJS_OBJECT* base = TailsMdls[0]->getmodel();

	if(object == base->getnode(31)) {
		njGetMatrix(TailsMatrices[0]); //pupils
	}
	else if (object == base->getnode(39)) {
		njGetMatrix(TailsMatrices[1]);  //tails
	}
}

void TailsHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

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
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	njRotateX(0, 0x4000);

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	*NodeCallbackFuncPtr = TailsCallback;
	njActionWeight(HTailsAnimData[tailsobj->Data1->Index].Animation, tailsobj->Data1->Scale.x, TailsMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, TailsMatrices[0]);
	NJS_CNK_OBJECT* pupils = TailsMdls[1]->getmodel();
	switch (tailsobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		dsDrawModel(pupils->getbasicdxmodel());
		break;
	case 2:
	case 6:
		dsDrawModel(pupils->child->getbasicdxmodel());
		break;
	case 3:
	case 5:
		dsDrawModel(pupils->child->child->getbasicdxmodel());
		break;
	case 4:
		dsDrawModel(pupils->child->child->child->getbasicdxmodel());
		break;
	}

	if (tailsobj->Child) {
		njSetMatrix(NULL, TailsMatrices[1]);
		njTranslate(0, 0, 0.2f, 0.5f);
		njActionWeight(TTailsAnimData[tailsobj->Child->Data1->Index].Animation, tailsobj->Child->Data1->Scale.x, TailsMdls[2]->getweightinfo());
	}
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void TailsHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (!CharactersCommon_Init(obj, "heroes-tails", &TAILS_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!obj->Child) LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), TailsTails_Main, obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[14].texaddr = TAILS_TEXLIST.textures[3].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.MaxAccel = 2.6f;
			playerco2->PhysicsData.field_14 = 0.85f;
			playerco2->PhysicsData.AirAccel = 0.039999999f;
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
		PlayerPtrs[data->CharIndex]->DisplaySub = TailsHeroes_Display;

		if (CanDoTricks(playerdata)) {
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
			if (playerco2->Speed.x < 0.02f) anim = 9;
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
			if (flysounds && playerobj->Data1->CharIndex == EntityData1Ptrs[0]->CharIndex && FrameCounterUnpaused % 38 == 0)
				PlayHeroesSound_Entity(TailsSound_Flying, playerobj, 50, 0);
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
		case 100:
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
		
		if (anim != 39) {
			data->Rotation.y = 0;
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

void LoadTailsFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-tails.arcx"));

	TailsMdls[0] = arc.GetModel("TAILS_LOCATOR.sa1mdl");
	TailsMdls[1] = arc.GetModel("TAILS_MABUTA.sa1mdl");
	TailsMdls[2] = arc.GetModel("TA_SIPPO.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(TailsMdls[0]->getweightinfo(), TailsMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->Init(TailsMdls[2]->getweightinfo(), TailsMdls[2]->getmodel());

	TailsAnms[0] = arc.GetAnimation("TA_WALK.saanim");
	TailsAnms[1] = arc.GetAnimation("TA_WALK_PULL.saanim");
	TailsAnms[2] = arc.GetAnimation("TA_WALK_PUSH.saanim");
	TailsAnms[3] = arc.GetAnimation("TA_TURN_L.saanim");
	TailsAnms[4] = arc.GetAnimation("TA_TURN_R.saanim");
	TailsAnms[5] = arc.GetAnimation("TA_SLOW_RUN.saanim");
	TailsAnms[6] = arc.GetAnimation("TA_MID_RUN.saanim");
	TailsAnms[7] = arc.GetAnimation("TA_TOP_RUN.saanim");
	TailsAnms[8] = arc.GetAnimation("TA_START.saanim");
	TailsAnms[9] = arc.GetAnimation("TA_IDLE.saanim");
	TailsAnms[10] = arc.GetAnimation("TA_IDLE_C.saanim");
	TailsAnms[11] = arc.GetAnimation("TA_IDLE_D_HALF.saanim");
	TailsAnms[12] = arc.GetAnimation("TA_DASH_PUSH.saanim");
	TailsAnms[13] = arc.GetAnimation("TA_FLY_IDLE_B.saanim");
	TailsAnms[14] = arc.GetAnimation("TA_POW_ROT.saanim");
	TailsAnms[15] = arc.GetAnimation("TA_ATC_BOMB.saanim");
	TailsAnms[16] = arc.GetAnimation("TA_FW_JUMP.saanim");
	TailsAnms[17] = arc.GetAnimation("HERO_TA.saanim");
	TailsAnms[18] = arc.GetAnimation("TA_JUMP_A.saanim");
	TailsAnms[19] = arc.GetAnimation("TA_JUMP_B.saanim");
	TailsAnms[20] = arc.GetAnimation("TA_JUMP_C.saanim");
	TailsAnms[21] = arc.GetAnimation("TA_JUMP_D.saanim");
	TailsAnms[22] = arc.GetAnimation("TA_JUMP_E.saanim");
	TailsAnms[23] = arc.GetAnimation("TA_JUMP_F.saanim");
	TailsAnms[24] = arc.GetAnimation("TA_JUMP_TRNGL.saanim");
	TailsAnms[25] = arc.GetAnimation("TA_JUMP_GLIND.saanim");
	TailsAnms[26] = arc.GetAnimation("TA_GLIND.saanim");
	TailsAnms[27] = arc.GetAnimation("TA_GLIND_BK.saanim");
	TailsAnms[28] = arc.GetAnimation("TA_GLIND_BK_L.saanim");
	TailsAnms[29] = arc.GetAnimation("TA_GLIND_BK_R.saanim");
	TailsAnms[30] = arc.GetAnimation("TA_GLIND_FLIP_BK.saanim");
	TailsAnms[31] = arc.GetAnimation("TA_GLIND_FLIP_FR.saanim");
	TailsAnms[32] = arc.GetAnimation("TA_GLIND_L.saanim");
	TailsAnms[33] = arc.GetAnimation("TA_GLIND_R.saanim");
	TailsAnms[34] = arc.GetAnimation("TA_FLY_IDLE.saanim");
	TailsAnms[35] = arc.GetAnimation("TA_FLY_SLOW.saanim");
	TailsAnms[36] = arc.GetAnimation("TA_FLY_UP.saanim");
	TailsAnms[37] = arc.GetAnimation("TA_FLY_PULL.saanim");
	TailsAnms[38] = arc.GetAnimation("TA_FLY_PUSH.saanim");
	TailsAnms[39] = arc.GetAnimation("TA_FLY_KICK.saanim");
	TailsAnms[40] = arc.GetAnimation("TA_FLY_HANG_IDLE.saanim");
	TailsAnms[41] = arc.GetAnimation("TA_FLY_HANG_OFF.saanim");
	TailsAnms[42] = arc.GetAnimation("TA_FLY_HANG_ON.saanim");
	TailsAnms[43] = arc.GetAnimation("TA_HANG_OFF.saanim");
	TailsAnms[44] = arc.GetAnimation("TA_HANG_ON.saanim");
	TailsAnms[45] = arc.GetAnimation("TA_BREAK_A.saanim");
	TailsAnms[46] = arc.GetAnimation("TA_BREAK_B.saanim");
	TailsAnms[47] = arc.GetAnimation("TA_BREAK_C.saanim");
	TailsAnms[48] = arc.GetAnimation("TA_BREAK_TURN_L.saanim");
	TailsAnms[49] = arc.GetAnimation("TA_BREAK_TURN_R.saanim");
	TailsAnms[50] = arc.GetAnimation("TA_BRA_MID.saanim");
	TailsAnms[51] = arc.GetAnimation("TA_BRA_TOP.saanim");
	TailsAnms[52] = arc.GetAnimation("TA_BOB.saanim");
	TailsAnms[53] = arc.GetAnimation("TA_FLORT.saanim");
	TailsAnms[54] = arc.GetAnimation("TA_DAM_M_A.saanim");
	TailsAnms[55] = arc.GetAnimation("TA_DAM_M_B.saanim");
	TailsAnms[56] = arc.GetAnimation("TA_DAM_M_C.saanim");
	TailsAnms[57] = arc.GetAnimation("TA_EDGE_OTTO_A.saanim");
	TailsAnms[58] = arc.GetAnimation("TA_EDGE_OTTO_B.saanim");
	TailsAnms[59] = arc.GetAnimation("TA_EDGE_OTTO_C.saanim");
	TailsAnms[60] = arc.GetAnimation("TA_WIN_B.saanim");
	TailsAnms[61] = arc.GetAnimation("TA_TRAP_JUMP.saanim");

	TailsAnms[62] = arc.GetAnimation("TA_WALK_TA.saanim");
	TailsAnms[63] = arc.GetAnimation("TA_SLOW_RUN_TA.saanim");
	TailsAnms[64] = arc.GetAnimation("TA_MID_RUN_TA.saanim");
	TailsAnms[65] = arc.GetAnimation("TA_TOP_RUN_TA.saanim");
	TailsAnms[66] = arc.GetAnimation("TA_START_TA.saanim");
	TailsAnms[67] = arc.GetAnimation("TA_IDLE_TA.saanim");
	TailsAnms[68] = arc.GetAnimation("TA_JUMP_TA_A.saanim");
	TailsAnms[69] = arc.GetAnimation("TA_JUMP_TA_B.saanim");
	TailsAnms[70] = arc.GetAnimation("TA_JUMP_TA_C.saanim");
	TailsAnms[71] = arc.GetAnimation("TA_JUMP_TA_D.saanim");
	TailsAnms[72] = arc.GetAnimation("TA_JUMP_TA_E.saanim");
	TailsAnms[73] = arc.GetAnimation("TA_JUMP_TRNGL_TA.saanim");
	TailsAnms[74] = arc.GetAnimation("TA_JUMP_GLIND_TA.saanim");
	TailsAnms[75] = arc.GetAnimation("TA_FW_JUMP_TA.saanim");
	TailsAnms[76] = arc.GetAnimation("TA_TRAP_JUMP_TA.saanim");
	TailsAnms[77] = arc.GetAnimation("TA_FLY_IDLE_TA.saanim");
	TailsAnms[78] = arc.GetAnimation("TA_FLY_KICK_TA.saanim");
	TailsAnms[79] = arc.GetAnimation("TA_FLY_SLOW_TA.saanim");
	TailsAnms[80] = arc.GetAnimation("TA_GLIND_TA.saanim");
	TailsAnms[81] = arc.GetAnimation("TA_GLIND_BK_TA.saanim");
	TailsAnms[82] = arc.GetAnimation("TA_GLIND_FLIP_BK_TA.saanim");
	TailsAnms[83] = arc.GetAnimation("TA_GLIND_FLIP_FR_TA.saanim");
	TailsAnms[84] = arc.GetAnimation("TA_FLORT_TA.saanim");
	TailsAnms[85] = arc.GetAnimation("TA_DASH_PUSH_TA.saanim");
	TailsAnms[86] = arc.GetAnimation("TA_DAM_M_TA_A.saanim");
	TailsAnms[87] = arc.GetAnimation("TA_DAM_M_TA_B.saanim");
	TailsAnms[88] = arc.GetAnimation("TA_DAM_M_TA_C.saanim");
	TailsAnms[89] = arc.GetAnimation("TA_BREAK_TA_A.saanim");
	TailsAnms[90] = arc.GetAnimation("TA_BREAK_TA_B.saanim");
	TailsAnms[91] = arc.GetAnimation("TA_BREAK_TA_C.saanim");
	TailsAnms[92] = arc.GetAnimation("TA_BRA_MID_TA.saanim");
	TailsAnms[93] = arc.GetAnimation("TA_ATC_BOMB_TA.saanim");
	TailsAnms[94] = arc.GetAnimation("TA_WIN_B_TA.saanim");
	TailsAnms[95] = arc.GetAnimation("HERO_TA_TA.saanim");

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

void UnloadTailsFiles() {
	HelperFunctionsGlobal.Weights->DeInit(TailsMdls[0]->getweightinfo(), TailsMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(TailsMdls[2]->getweightinfo(), TailsMdls[2]->getmodel());
	FreeMDLFiles(TailsMdls, LengthOfArray(TailsMdls));
	FreeANMFiles(TailsAnms, LengthOfArray(TailsAnms));
}