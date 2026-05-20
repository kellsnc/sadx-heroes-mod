#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	RO_WALK,
	RO_WALK_PULL,
	RO_WALK_PUSH,
	RO_TURN_L,
	RO_TURN_R,
	RO_SLOW_RUN,
	RO_MID_RUN,
	RO_TOP_RUN,
	RO_START,
	RO_IDLE,
	RO_IDLE_B,
	RO_IDLE_C_HALF,
	RO_WIN,
	RO_ATC_BOMB,
	RO_POW_ROT,
	RO_TRAP_JUMP,
	RO_FW_JUMP,
	DARK_RO,
	RO_JUMP_A,
	RO_JUMP_B,
	RO_JUMP_C,
	RO_JUMP_D,
	RO_JUMP_E,
	RO_JUMP_F,
	RO_JUMP_TRNGL,
	RO_JUMP_GLIND,
	RO_GLIND,
	RO_GLIND_BK,
	RO_GLIND_BK_L,
	RO_GLIND_BK_R,
	RO_GLIND_FLIP_B,
	RO_GLIND_FLIP_F,
	RO_GLIND_L,
	RO_GLIND_R,
	RO_FLY_IDLE,
	RO_FLY_SLOW,
	RO_FLY_UP,
	RO_FLY_PULL,
	RO_FLY_PUSH,
	RO_FLY_KICK,
	RO_FLY_HANG_IDLE,
	RO_FLY_HANG_OFF,
	RO_FLY_HANG_ON,
	RO_HANG_OFF,
	RO_HANG_ON,
	RO_BREAK_A,
	RO_BREAK_B,
	RO_BREAK_C,
	RO_BREAK_TURN_L,
	RO_BREAK_TURN_R,
	RO_BRA_MID,
	RO_BRA_TOP,
	RO_BOB,
	RO_FLORT,
	RO_DAM_M_A,
	RO_DAM_M_B,
	RO_DAM_M_C,
	RO_EDGE_OTTO_A,
	RO_EDGE_OTTO_B,
	RO_EDGE_OTTO_C,
	RO_ROLL
};

enum {
	RO_FLY_IDLE_WNGL,
	RO_FLY_UP_WING,
	RO_IDLE_C_HANE_HALF,
	RO_IDLE_WNGS,
	SL_HANE,
	RO_WING_WIN,
};

ModelInfo* RougeMdls[6];
AnimationFile* RougeAnms[59];
AnimationFile* HaneAnms[6];
NJS_ACTION RougeActs[60];
NJS_ACTION HaneActs[6];

PL_ACTION rouge_action_heroes[] = {
	{ &RougeActs[RO_WALK], 45, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_WALK_PULL], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_WALK_PUSH], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_TURN_L], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_TURN_R], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_SLOW_RUN], 45, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &RougeActs[RO_MID_RUN], 45, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &RougeActs[RO_TOP_RUN], 45, MD_MTN_XSPD, 0, 0.25f, 0.15f },
	{ &RougeActs[RO_START], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_IDLE], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_IDLE_B], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_IDLE_C_HALF], 45, MD_MTN_NEXT, RO_IDLE, 0.25f, 0.5f },
	{ &RougeActs[RO_WIN], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_ATC_BOMB], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_POW_ROT], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_TRAP_JUMP], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FW_JUMP], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[DARK_RO], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_JUMP_A], 45, MD_MTN_NEXT, RO_JUMP_B, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_B], 45, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_C], 45, MD_MTN_NEXT, RO_JUMP_D, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_D], 45, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_E], 45, MD_MTN_NEXT, RO_JUMP_F, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_F], 45, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &RougeActs[RO_JUMP_TRNGL], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_JUMP_GLIND], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_BK], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_BK_L], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_BK_R], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_FLIP_B], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_FLIP_F], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_L], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_GLIND_R], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_IDLE], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_SLOW], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_UP], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_PULL], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_PUSH], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_KICK], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_HANG_IDLE], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_HANG_OFF], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLY_HANG_ON], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_HANG_OFF], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_HANG_ON], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BREAK_A], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BREAK_B], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BREAK_C], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BREAK_TURN_L], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BREAK_TURN_R], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BRA_MID], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BRA_TOP], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_BOB], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_FLORT], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_DAM_M_A], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_DAM_M_B], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_DAM_M_C], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_EDGE_OTTO_A], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_EDGE_OTTO_B], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_EDGE_OTTO_C], 45, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &RougeActs[RO_JUMP_B], 45, MD_MTN_XSPD, 0, 0.25f, 0.5f },
};

PL_ACTION rouge_hane_action[] = {
	{ &HaneActs[RO_FLY_IDLE_WNGL], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &HaneActs[RO_FLY_UP_WING], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &HaneActs[RO_IDLE_C_HANE_HALF], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &HaneActs[RO_IDLE_WNGS], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &HaneActs[SL_HANE], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &HaneActs[RO_WING_WIN], 7, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

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

void RougeCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = RougeMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(RougeMatrices[0]); //eyelashes
	}
	else if (object == base->getnode(2)) {
		njGetMatrix(RougeMatrices[1]); //wings
	}
}

void RougeHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* rougeobj = HeroesChars[obj->Data1->CharIndex];
	if (!rougeobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)rougeobj->UnknownB_ptr;

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

	njRotateX(0, 0x4000);

	mtnjvwk* mtn = (mtnjvwk*)&pwp_heroes->mm;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = RougeCallback;
	njActionWeight(actptr, mtn->nframe, RougeMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, RougeMatrices[0]);
	NJS_CNK_OBJECT* l_mabuta = RougeMdls[1]->getmodel();
	NJS_CNK_OBJECT* r_mabuta = RougeMdls[2]->getmodel();
	switch (rougeobj->Data1->InvulnerableTime) {
	case 1:
	case 3:
		dsDrawModel(l_mabuta->child->getbasicdxmodel());
		dsDrawModel(r_mabuta->child->getbasicdxmodel());
		break;
	case 2:
		dsDrawModel(l_mabuta->child->child->getbasicdxmodel());
		dsDrawModel(r_mabuta->child->child->getbasicdxmodel());
		break;
	default:
		dsDrawModel(l_mabuta->getbasicdxmodel());
		dsDrawModel(r_mabuta->getbasicdxmodel());
		break;
	}

	njSetMatrix(NULL, RougeMatrices[1]);
	njTranslate(0, 0, 0.5f, -0.8f);
	mtnjvwk* mm_sub = &pwp_heroes->mm_sub;
	actptr = mm_sub->plactptr[mm_sub->reqaction].actptr;
	if (mm_sub->mtnmode == MD_MTN_CHNG)
		actptr = mm_sub->actwkptr;
	njActionWeight(actptr, mm_sub->nframe, RougeMdls[5]->getweightinfo());

	njPopMatrix(1);

	if (action == RO_JUMP_B || action == RO_ROLL)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 0);
	}
	else if (action == RO_FW_JUMP)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 1);
	}

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void RougeAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp)
{
	int anim = mtn->reqaction;

	switch (pwp->mj.reqaction) {
	case 0:
	case 2:
	case 7:
	case 8:
		anim = 9;
		break;
	case 4:
	case 5:
	case 6:
		anim = 10;
		break;
	case 9:
		anim = 0;
		if (pwp->spd.x < 0.02f) anim = 9;
		break;
	case 10:
		anim = 0;
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
		anim = 47;
		if (pwp->spd.x > 6) {
			anim = 46;
		}
		else if (pwp->spd.x > 3) {
			anim = 45;
		}
		break;
	case 22:
		anim = 48;
		break;
	case 24:
		anim = 2;
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

void RougeHaneAnim(mtnjvwk* mtn_sub, int tails_action)
{
	int anim = 3;

	if (tails_action >= 34 && tails_action <= 42)
	{
		anim = 0;
	}

	if (tails_action == 36)
	{
		anim = 1;
	}

	if (tails_action == 54 || tails_action == 55)
	{
		anim = 5;
	}

	mtn_sub->reqaction = anim;
}

void RougeHeroes_Main(ObjectMaster *obj) {
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
		HeroesChars_InitPlayer((task*)obj, { "heroes-rouge", &ROUGE_TEXLIST }, 6, rouge_action_heroes);

		pwp_heroes->mm_sub.plactptr = rouge_hane_action;
		pwp_heroes->mm_sub.mtnmode = MD_MTN_INIT;
		pwp_heroes->mm_sub.reqaction = 0;
		pwp_heroes->mm_sub.spdp = &playerpwp[data->CharIndex]->spd.x;
		pwp_heroes->mm_sub.workp = &playerpwp[data->CharIndex]->work.f;
		PSetMotion(&pwp_heroes->mm_sub);

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

		RougeAnimConverter(&pwp_heroes->mm, HeroesChars_Tails, (taskwk*)playerdata, (playerwk*)playerco2);

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

	RougeHaneAnim(&pwp_heroes->mm_sub, pwp_heroes->mm.reqaction);

	PSetMotion(&pwp_heroes->mm);
	PSetMotion(&pwp_heroes->mm_sub);


	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 3) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadRougeFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-rouge.arcx"));

	RougeMdls[0] = arc.GetModel("ROUGE_LOCATOR.sa1mdl");
	RougeMdls[1] = arc.GetModel("L_MABUTA.sa1mdl");
	RougeMdls[2] = arc.GetModel("R_MABUTA.sa1mdl");
	RougeMdls[3] = arc.GetModel("S_HANE.sa1mdl");
	RougeMdls[4] = arc.GetModel("SL_HANE.sa1mdl");
	RougeMdls[5] = arc.GetModel("L_HANE.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(RougeMdls[0]->getweightinfo(), RougeMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->Init(RougeMdls[3]->getweightinfo(), RougeMdls[3]->getmodel());
	HelperFunctionsGlobal.Weights->Init(RougeMdls[4]->getweightinfo(), RougeMdls[4]->getmodel());
	HelperFunctionsGlobal.Weights->Init(RougeMdls[5]->getweightinfo(), RougeMdls[5]->getmodel());

	RougeAnms[0] = arc.GetAnimation("RO_WALK.saanim");
	RougeAnms[1] = arc.GetAnimation("RO_WALK_PULL.saanim");
	RougeAnms[2] = arc.GetAnimation("RO_WALK_PUSH.saanim");
	RougeAnms[3] = arc.GetAnimation("RO_TURN_L.saanim");
	RougeAnms[4] = arc.GetAnimation("RO_TURN_R.saanim");
	RougeAnms[5] = arc.GetAnimation("RO_SLOW_RUN.saanim");
	RougeAnms[6] = arc.GetAnimation("RO_MID_RUN.saanim");
	RougeAnms[7] = arc.GetAnimation("RO_TOP_RUN.saanim");
	RougeAnms[8] = arc.GetAnimation("RO_START.saanim");
	RougeAnms[9] = arc.GetAnimation("RO_IDLE.saanim");
	RougeAnms[10] = arc.GetAnimation("RO_IDLE_B.saanim");
	RougeAnms[11] = arc.GetAnimation("RO_IDLE_C_HALF.saanim");
	RougeAnms[12] = arc.GetAnimation("RO_WIN.saanim");
	RougeAnms[13] = arc.GetAnimation("RO_ATC_BOMB.saanim");
	RougeAnms[14] = arc.GetAnimation("RO_POW_ROT.saanim");
	RougeAnms[15] = arc.GetAnimation("RO_TRAP_JUMP.saanim");
	RougeAnms[16] = arc.GetAnimation("RO_FW_JUMP.saanim");
	RougeAnms[17] = arc.GetAnimation("DARK_RO.saanim");
	RougeAnms[18] = arc.GetAnimation("RO_JUMP_A.saanim");
	RougeAnms[19] = arc.GetAnimation("RO_JUMP_B.saanim");
	RougeAnms[20] = arc.GetAnimation("RO_JUMP_C.saanim");
	RougeAnms[21] = arc.GetAnimation("RO_JUMP_D.saanim");
	RougeAnms[22] = arc.GetAnimation("RO_JUMP_E.saanim");
	RougeAnms[23] = arc.GetAnimation("RO_JUMP_F.saanim");
	RougeAnms[24] = arc.GetAnimation("RO_JUMP_TRNGL.saanim");
	RougeAnms[25] = arc.GetAnimation("RO_JUMP_GLIND.saanim");
	RougeAnms[26] = arc.GetAnimation("RO_GLIND.saanim");
	RougeAnms[27] = arc.GetAnimation("RO_GLIND_BK.saanim");
	RougeAnms[28] = arc.GetAnimation("RO_GLIND_BK_L.saanim");
	RougeAnms[29] = arc.GetAnimation("RO_GLIND_BK_R.saanim");
	RougeAnms[30] = arc.GetAnimation("RO_GLIND_FLIP_B.saanim");
	RougeAnms[31] = arc.GetAnimation("RO_GLIND_FLIP_F.saanim");
	RougeAnms[32] = arc.GetAnimation("RO_GLIND_L.saanim");
	RougeAnms[33] = arc.GetAnimation("RO_GLIND_R.saanim");
	RougeAnms[34] = arc.GetAnimation("RO_FLY_IDLE.saanim");
	RougeAnms[35] = arc.GetAnimation("RO_FLY_SLOW.saanim");
	RougeAnms[36] = arc.GetAnimation("RO_FLY_UP.saanim");
	RougeAnms[37] = arc.GetAnimation("RO_FLY_PULL.saanim");
	RougeAnms[38] = arc.GetAnimation("RO_FLY_PUSH.saanim");
	RougeAnms[39] = arc.GetAnimation("RO_FLY_KICK.saanim");
	RougeAnms[40] = arc.GetAnimation("RO_FLY_HANG_IDLE.saanim");
	RougeAnms[41] = arc.GetAnimation("RO_FLY_HANG_OFF.saanim");
	RougeAnms[42] = arc.GetAnimation("RO_FLY_HANG_ON.saanim");
	RougeAnms[43] = arc.GetAnimation("RO_HANG_OFF.saanim");
	RougeAnms[44] = arc.GetAnimation("RO_HANG_ON.saanim");
	RougeAnms[45] = arc.GetAnimation("RO_BREAK_A.saanim");
	RougeAnms[46] = arc.GetAnimation("RO_BREAK_B.saanim");
	RougeAnms[47] = arc.GetAnimation("RO_BREAK_C.saanim");
	RougeAnms[48] = arc.GetAnimation("RO_BREAK_TURN_L.saanim");
	RougeAnms[49] = arc.GetAnimation("RO_BREAK_TURN_R.saanim");
	RougeAnms[50] = arc.GetAnimation("RO_BRA_MID.saanim");
	RougeAnms[51] = arc.GetAnimation("RO_BRA_TOP.saanim");
	RougeAnms[52] = arc.GetAnimation("RO_BOB.saanim");
	RougeAnms[53] = arc.GetAnimation("RO_FLORT.saanim");
	RougeAnms[54] = arc.GetAnimation("RO_DAM_M_A.saanim");
	RougeAnms[55] = arc.GetAnimation("RO_DAM_M_B.saanim");
	RougeAnms[56] = arc.GetAnimation("RO_DAM_M_C.saanim");
	RougeAnms[57] = arc.GetAnimation("RO_EDGE_OTTO_A.saanim");
	RougeAnms[58] = arc.GetAnimation("RO_EDGE_OTTO_B.saanim");
	RougeAnms[59] = arc.GetAnimation("RO_EDGE_OTTO_C.saanim");

	HaneAnms[0] = arc.GetAnimation("RO_FLY_IDLE_WNGL.saanim");
	HaneAnms[1] = arc.GetAnimation("RO_FLY_UP_WING.saanim");
	HaneAnms[2] = arc.GetAnimation("RO_IDLE_C_HANE_HALF.saanim");
	HaneAnms[3] = arc.GetAnimation("RO_IDLE_WNGS.saanim");
	HaneAnms[4] = arc.GetAnimation("SL_HANE.saanim");
	HaneAnms[5] = arc.GetAnimation("RO_WING_WIN.saanim");

	for (int i = 0; i < LengthOfArray(RougeActs); ++i) {
		RougeActs[i].object = RougeMdls[0]->getmodel();
		RougeActs[i].motion = RougeAnms[i] ? RougeAnms[i]->getmotion() : NULL;
	}

	for (int i = 0; i < LengthOfArray(HaneActs); ++i) {
		HaneActs[i].object = RougeMdls[5]->getmodel();
		HaneActs[i].motion = HaneAnms[i] ? HaneAnms[i]->getmotion() : NULL;
	}

	HaneActs[3].object = RougeMdls[3]->getmodel();
	HaneActs[4].object = RougeMdls[4]->getmodel();
}

void UnloadRougeFiles() {
	HelperFunctionsGlobal.Weights->DeInit(RougeMdls[0]->getweightinfo(), RougeMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(RougeMdls[3]->getweightinfo(), RougeMdls[3]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(RougeMdls[4]->getweightinfo(), RougeMdls[4]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(RougeMdls[5]->getweightinfo(), RougeMdls[5]->getmodel());
	FreeMDLFiles(RougeMdls, LengthOfArray(RougeMdls));
	FreeANMFiles(RougeAnms, LengthOfArray(RougeAnms));
}