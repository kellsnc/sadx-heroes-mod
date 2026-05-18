#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	BE_WALK,
	BE_WALK_PULL,
	BE_WALK_PUSH,
	BE_TURN_L,
	BE_TURN_R,
	BE_SLOW_RUN,
	BE_MID_RUN,
	BE_TOP_RUN,
	BE_START,
	BE_IDLE,
	BE_IDLE_B,
	BE_IDLE_C_HALF,
	BE_WIN,
	BE_ATC_HARI,
	BE_POW_ROT,
	BE_TRAP_JUMP,
	BE_FW_JUMP,
	CAO_BE,
	BE_JUMP_A,
	BE_JUMP_B,
	BE_JUMP_C,
	BE_JUMP_D,
	BE_JUMP_E,
	BE_JUMP_F,
	BE_GUM,
	BE_JUMP_GLIND,
	BE_GLIND,
	BE_GLIND_BK,
	BE_GLIND_BK_L,
	BE_GLIND_BK_R,
	BE_GLIND_FLIP_BK,
	BE_GLIND_FLIP_FR,
	BE_GLIND_L,
	BE_GLIND_R,
	BE_FLY_IDLE,
	BE_FLY_SLOW,
	BE_FLY_UP,
	BE_FLY_PULL,
	BE_FLY_PUSH,
	BE_FLY_KICK,
	BE_FLY_HANG_IDLE,
	BE_FLY_HANG_OFF,
	BE_FLY_HANG_ON,
	BE_HANG_ON,
	BE_BREAK_A,
	BE_BREAK_B,
	BE_BREAK_C,
	BE_BRA_MID,
	BE_BRA_TOP,
	BE_BOB,
	BE_FLORT,
	BE_DAM_M_A,
	BE_DAM_M_B,
	BE_DAM_M_C,
	BE_EDGE_OTTO_A,
	BE_EDGE_OTTO_B,
	BE_EDGE_OTTO_C,
};

ModelInfo* CharmyMdls[3];
AnimationFile* CharmyAnms[57];
NJS_ACTION CharmyActs[57];
NJS_ACTION CharmyHaneAct;

PL_ACTION charmy_action_heroes[] = {
	{ &CharmyActs[BE_SLOW_RUN], 46, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_WALK_PULL], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_WALK_PUSH], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_TURN_L], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_TURN_R], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_SLOW_RUN], 46, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_MID_RUN], 46, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_TOP_RUN], 46, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_START], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_IDLE], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_IDLE_B], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_IDLE_C_HALF], 46, MD_MTN_NEXT, BE_IDLE, 0.25f, 0.5f },
	{ &CharmyActs[BE_WIN], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_ATC_HARI], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_POW_ROT], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_TRAP_JUMP], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FW_JUMP], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[CAO_BE], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_JUMP_A], 46, MD_MTN_NEXT, BE_JUMP_B, 0.25f, 0.5f },
	{ &CharmyActs[BE_JUMP_B], 46, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_JUMP_C], 46, MD_MTN_NEXT, BE_JUMP_D, 0.25f, 0.5f },
	{ &CharmyActs[BE_JUMP_D], 46, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_JUMP_E], 46, MD_MTN_NEXT, BE_JUMP_F, 0.25f, 0.5f },
	{ &CharmyActs[BE_JUMP_F], 46, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CharmyActs[BE_GUM], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_JUMP_GLIND], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_BK], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_BK_L], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_BK_R], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_FLIP_BK], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_FLIP_FR], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_L], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_GLIND_R], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_IDLE], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_SLOW], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_UP], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_PULL], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_PUSH], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_KICK], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_HANG_IDLE], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_HANG_OFF], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLY_HANG_ON], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_HANG_ON], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BREAK_A], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BREAK_B], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BREAK_C], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BRA_MID], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BRA_TOP], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_BOB], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_FLORT], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_DAM_M_A], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_DAM_M_B], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_DAM_M_C], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_EDGE_OTTO_A], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_EDGE_OTTO_B], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_EDGE_OTTO_C], 46, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CharmyActs[BE_JUMP_B], 46, MD_MTN_XSPD, 0, 0.25f, 0.5f },
};

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

void CharmyCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = CharmyMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(CharmyMatrices[0]); //eyelashes
	}
	else if (object == base->getnode(2)) {
		njGetMatrix(CharmyMatrices[1]); //wings
	}
}

void CharmyDrawHane(void* data)
{

}

void CharmyHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* Charmyobj = HeroesChars[obj->Data1->CharIndex];
	if (!Charmyobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)Charmyobj->UnknownB_ptr;

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

	mtnjvwk* mtn = &pwp_heroes->mm;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = CharmyCallback;
	njActionWeight(actptr, mtn->nframe, CharmyMdls[0]->getweightinfo());
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

	njSetMatrix(NULL, CharmyMatrices[1]);
	njTranslate(0, 0, 0.2f, -1.2f);
	HelperFunctionsGlobal.Weights->Apply(CharmyMdls[2]->getweightinfo(), &CharmyHaneAct, (float)FrameCounterUnpaused * 0.5f);
	late_Action(&CharmyHaneAct, (float)FrameCounterUnpaused * 0.5f, LATE_MAT);

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CharmyAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp)
{
	int anim = mtn->reqaction;

	switch (pwp->mj.reqaction) {
	case 0:
	case 2:
	case 7:
	case 8:
		anim = 9; break;
	case 4:
	case 5:
	case 6:
		anim = 10;
		break;
	case 9:
		anim = 5;
		if (pwp->spd.x < 0.02f) anim = 9;
		break;
	case 10:
		anim = 5;
		break;
	case 11:
		anim = 5;
		break;
	case 12:
		anim = 6;
		break;
	case 13:
		anim = 7;
		break;
	case 14: //jumping
		if (anim < 18 || anim > 23) {
			anim = 18;
		}
		//else if (anim == 19) {
		//	if (data->Unknown > 2) {
		//		anim = 20;
		//	}
		//}
		//else if (anim == 21) {
		//	if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 22;
		//}
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
		if (pwp->spd.x > 8.0f && pltwp->pos.y - pwp->shadow.y_bottom > 500.0f) {
			anim = 16;
			pwp->mj.reqaction = 150;
		}
		break;
	case 20:
		anim = 23;
		break;
	case 21: //break
		anim = 46;
		if (pwp->spd.x > 6) {
			anim = 45;
		}
		else if (pwp->spd.x > 3) {
			anim = 43;
		}
		break;
	case 22:
		anim = 43;
		break;
	case 24:
		anim = 2;
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
		break;
	case 54: //won
	case 55:
		anim = 12;
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
		if (pwp->spd.z > 0.1f) anim = 33;
		break;
	case 113:
		if (pwp->spd.z < 0.1f) anim = 32;
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
	mtn->reqaction = anim;
}

void CharmyHeroes_Main(ObjectMaster * obj) {
	EntityData1* data = obj->Data1;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->UnknownB_ptr;

	if (!playerco2 || playerdata->CharID != Characters_Tails)
	{
		return;
	}

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-charmy", &CHARMY_TEXLIST }, 5, charmy_action_heroes);
		data->Action = 2;
		return;
	case 2:
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

		CharmyAnimConverter(&pwp_heroes->mm, Characters_Charmy, (taskwk*)playerdata, (playerwk*)playerco2);

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

		pwp_heroes->mm.reqaction = 13;
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

		pwp_heroes->mm.reqaction = 39;
		break;
	}

	CharactersCommon_DrawBall(playerdata, data);
	PSetMotion(&pwp_heroes->mm);

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

	for (int i = 0; i < LengthOfArray(CharmyActs); ++i) {
		CharmyActs[i].object = CharmyMdls[0]->getmodel();
		CharmyActs[i].motion = CharmyAnms[i] ? CharmyAnms[i]->getmotion() : NULL;
	}

	CharmyHaneAct.object = CharmyMdls[2]->getmodel();
	CharmyHaneAct.motion = CharmyAnms[0]->getmotion();
}

void UnloadCharmyFiles() {
	HelperFunctionsGlobal.Weights->DeInit(CharmyMdls[0]->getweightinfo(), CharmyMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(CharmyMdls[2]->getweightinfo(), CharmyMdls[2]->getmodel());
	FreeMDLFiles(CharmyMdls, LengthOfArray(CharmyMdls));
	FreeANMFiles(CharmyAnms, LengthOfArray(CharmyAnms));
}