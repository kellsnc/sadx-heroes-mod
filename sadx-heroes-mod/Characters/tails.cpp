#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	TA_WALK,
	TA_WALK_PULL,
	TA_WALK_PUSH,
	TA_TURN_L,
	TA_TURN_R,
	TA_SLOW_RUN,
	TA_MID_RUN,
	TA_TOP_RUN,
	TA_START,
	TA_IDLE,
	TA_IDLE_C,
	TA_IDLE_D_HALF,
	TA_DASH_PUSH,
	TA_FLY_IDLE_B,
	TA_POW_ROT,
	TA_ATC_BOMB,
	TA_FW_JUMP,
	TERO_TA,
	TA_JUMP_A,
	TA_JUMP_B,
	TA_JUMP_C,
	TA_JUMP_D,
	TA_JUMP_E,
	TA_JUMP_F,
	TA_JUMP_TRNGL,
	TA_JUMP_GLIND,
	TA_GLIND,
	TA_GLIND_BK,
	TA_GLIND_BK_L,
	TA_GLIND_BK_R,
	TA_GLIND_FLIP_BK,
	TA_GLIND_FLIP_FR,
	TA_GLIND_L,
	TA_GLIND_R,
	TA_FLY_IDLE,
	TA_FLY_SLOW,
	TA_FLY_UP,
	TA_FLY_PULL,
	TA_FLY_PUSH,
	TA_FLY_KICK,
	TA_FLY_HANG_IDLE,
	TA_FLY_HANG_OFF,
	TA_FLY_HANG_ON,
	TA_HANG_OFF,
	TA_HANG_ON,
	TA_BREAK_A,
	TA_BREAK_B,
	TA_BREAK_C,
	TA_BREAK_TURN_L,
	TA_BREAK_TURN_R,
	TA_BRA_MID,
	TA_BRA_TOP,
	TA_BOB,
	TA_FLORT,
	TA_DAM_M_A,
	TA_DAM_M_B,
	TA_DAM_M_C,
	TA_EDGE_OTTO_A,
	TA_EDGE_OTTO_B,
	TA_EDGE_OTTO_C,
	TA_WIN_B,
	TA_TRAP_JUMP,
};

enum {
	TA_WALK_TA,
	TA_SLOW_RUN_TA,
	TA_MID_RUN_TA,
	TA_TOP_RUN_TA,
	TA_START_TA,
	TA_IDLE_TA,
	TA_JUMP_TA_A,
	TA_JUMP_TA_B,
	TA_JUMP_TA_C,
	TA_JUMP_TA_D,
	TA_JUMP_TA_E,
	TA_JUMP_TRNGL_TA,
	TA_JUMP_GLIND_TA,
	TA_FW_JUMP_TA,
	TA_TRAP_JUMP_TA,
	TA_FLY_IDLE_TA,
	TA_FLY_KICK_TA,
	TA_FLY_SLOW_TA,
	TA_GLIND_TA,
	TA_GLIND_BK_TA,
	TA_GLIND_FLIP_BK_TA,
	TA_GLIND_FLIP_FR_TA,
	TA_FLORT_TA,
	TA_DASH_PUSH_TA,
	TA_DAM_M_TA_A,
	TA_DAM_M_TA_B,
	TA_DAM_M_TA_C,
	TA_BREAK_TA_A,
	TA_BREAK_TA_B,
	TA_BREAK_TA_C,
	TA_BRA_MID_TA,
	TA_ATC_BOMB_TA,
	TA_WIN_B_TA,
	HERO_TA_TA,
};

ModelInfo* TailsMdls[3];
AnimationFile* TailsAnms[62];
AnimationFile* SippoAnms[34];
NJS_ACTION TailsActs[62];
NJS_ACTION SippoActs[34];

PL_ACTION tails_action_heroes[] = {
	{ &TailsActs[TA_WALK], 48, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_WALK_PULL], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_WALK_PUSH], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_TURN_L], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_TURN_R], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_SLOW_RUN], 48, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_MID_RUN], 48, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_TOP_RUN], 48, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_START], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_IDLE], 48, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_IDLE_C], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_IDLE_D_HALF], 48, MD_MTN_NEXT, TA_IDLE, 0.25f, 0.5f },
	{ &TailsActs[TA_DASH_PUSH], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_IDLE_B], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_POW_ROT], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_ATC_BOMB], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FW_JUMP], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TERO_TA], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_JUMP_A], 48, MD_MTN_NEXT, TA_JUMP_B, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_B], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_C], 48, MD_MTN_NEXT, TA_JUMP_D, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_D], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_E], 48, MD_MTN_NEXT, TA_JUMP_F, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_F], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &TailsActs[TA_JUMP_TRNGL], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_JUMP_GLIND], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_BK], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_BK_L], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_BK_R], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_FLIP_BK], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_FLIP_FR], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_L], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_GLIND_R], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_IDLE], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_SLOW], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_UP], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_PULL], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_PUSH], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_KICK], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_HANG_IDLE], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_HANG_OFF], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLY_HANG_ON], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_HANG_OFF], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_HANG_ON], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BREAK_A], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BREAK_B], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BREAK_C], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BREAK_TURN_L], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BREAK_TURN_R], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BRA_MID], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BRA_TOP], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_BOB], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_FLORT], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_DAM_M_A], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_DAM_M_B], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_DAM_M_C], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_EDGE_OTTO_A], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_EDGE_OTTO_B], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_EDGE_OTTO_C], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_WIN_B], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_TRAP_JUMP], 48, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &TailsActs[TA_JUMP_B], 48, MD_MTN_XSPD, 0, 0.25f, 0.5f },
};

PL_ACTION sippo_action[] = {
	{ &SippoActs[TA_WALK_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_SLOW_RUN_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_MID_RUN_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_TOP_RUN_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_START_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_IDLE_TA], 11, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &SippoActs[TA_JUMP_TA_A], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_TA_B], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_TA_C], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_TA_D], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_TA_E], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_TRNGL_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_JUMP_GLIND_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_FW_JUMP_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_TRAP_JUMP_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_FLY_IDLE_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_FLY_KICK_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_FLY_SLOW_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_GLIND_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_GLIND_BK_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_GLIND_FLIP_BK_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_GLIND_FLIP_FR_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_FLORT_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_DASH_PUSH_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_DAM_M_TA_A], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_DAM_M_TA_B], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_DAM_M_TA_C], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_BREAK_TA_A], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_BREAK_TA_B], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_BREAK_TA_C], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_BRA_MID_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_ATC_BOMB_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[TA_WIN_B_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SippoActs[HERO_TA_TA], 11, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

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

	if (co2->AnimationThing.Index == 54 || co2->AnimationThing.Index == 55) {
		njTranslate(0, 10, 2, 0);
	}

	njRotateX(0, 0x4000);

	mtnjvwk* mtn = (mtnjvwk*)tailsobj->Data1->Object;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = TailsCallback;
	njActionWeight(actptr, mtn->nframe, TailsMdls[0]->getweightinfo());
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

	njSetMatrix(NULL, TailsMatrices[1]);
	njTranslate(0, 0, 0.2f, 0.5f);
	mtn = *(mtnjvwk**)&tailsobj->Data1->Scale.z;
	actptr = mtn->plactptr[mtn->reqaction].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
		actptr = mtn->actwkptr;
	njActionWeight(actptr, mtn->nframe, TailsMdls[2]->getweightinfo());
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void TailsAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp)
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
		anim = 11;
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
		if (flysounds && FrameCounterUnpaused % 38 == 0)
			PlayHeroesSound_Entity(TailsSound_Flying, (ObjectMaster*)pltwp->cwp->mytask, 50, 0);
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
		anim = 60;
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

void TailsSippoAnim(mtnjvwk* mtn_sub, int tails_action)
{
	anim = TA_WALK_TA;

	switch (tails_action) {
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

	mtn_sub->reqaction = anim;
}

void TailsHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];
	mtnjvwk* mtn = (mtnjvwk*)data->Object;
	mtnjvwk* mtn_sub = *(mtnjvwk**)&data->Scale.z;

	if (!playerco2 || playerdata->CharID != Characters_Tails)
	{
		return;
	}

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-tails", &TAILS_TEXLIST }, 3, tails_action_heroes);

		mtn_sub = new mtnjvwk;
		mtn_sub->plactptr = sippo_action;
		mtn_sub->mtnmode = MD_MTN_INIT;
		mtn_sub->reqaction = 0;
		mtn_sub->spdp = &playerpwp[data->CharIndex]->spd.x;
		mtn_sub->workp = &playerpwp[data->CharIndex]->work.f;
		PSetMotion(mtn_sub);
		data->Scale.z = *(float*)&mtn_sub;

		data->Action = 2;
		return;
	case 2:
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

		TailsAnimConverter(mtn, Characters_HeroesTails, (taskwk*)playerdata, (playerwk*)playerco2);

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

		mtn->reqaction = 15;
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

		mtn->reqaction = 39;
		break;
	}

	CharactersCommon_DrawBall(playerdata, data);
	TailsSippoAnim(mtn_sub, mtn->reqaction);

	PSetMotion(mtn);
	PSetMotion(mtn_sub);

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

	SippoAnms[0] = arc.GetAnimation("TA_WALK_TA.saanim");
	SippoAnms[1] = arc.GetAnimation("TA_SLOW_RUN_TA.saanim");
	SippoAnms[2] = arc.GetAnimation("TA_MID_RUN_TA.saanim");
	SippoAnms[3] = arc.GetAnimation("TA_TOP_RUN_TA.saanim");
	SippoAnms[4] = arc.GetAnimation("TA_START_TA.saanim");
	SippoAnms[5] = arc.GetAnimation("TA_IDLE_TA.saanim");
	SippoAnms[6] = arc.GetAnimation("TA_JUMP_TA_A.saanim");
	SippoAnms[7] = arc.GetAnimation("TA_JUMP_TA_B.saanim");
	SippoAnms[8] = arc.GetAnimation("TA_JUMP_TA_C.saanim");
	SippoAnms[9] = arc.GetAnimation("TA_JUMP_TA_D.saanim");
	SippoAnms[10] = arc.GetAnimation("TA_JUMP_TA_E.saanim");
	SippoAnms[11] = arc.GetAnimation("TA_JUMP_TRNGL_TA.saanim");
	SippoAnms[12] = arc.GetAnimation("TA_JUMP_GLIND_TA.saanim");
	SippoAnms[13] = arc.GetAnimation("TA_FW_JUMP_TA.saanim");
	SippoAnms[14] = arc.GetAnimation("TA_TRAP_JUMP_TA.saanim");
	SippoAnms[15] = arc.GetAnimation("TA_FLY_IDLE_TA.saanim");
	SippoAnms[16] = arc.GetAnimation("TA_FLY_KICK_TA.saanim");
	SippoAnms[17] = arc.GetAnimation("TA_FLY_SLOW_TA.saanim");
	SippoAnms[18] = arc.GetAnimation("TA_GLIND_TA.saanim");
	SippoAnms[19] = arc.GetAnimation("TA_GLIND_BK_TA.saanim");
	SippoAnms[20] = arc.GetAnimation("TA_GLIND_FLIP_BK_TA.saanim");
	SippoAnms[21] = arc.GetAnimation("TA_GLIND_FLIP_FR_TA.saanim");
	SippoAnms[22] = arc.GetAnimation("TA_FLORT_TA.saanim");
	SippoAnms[23] = arc.GetAnimation("TA_DASH_PUSH_TA.saanim");
	SippoAnms[24] = arc.GetAnimation("TA_DAM_M_TA_A.saanim");
	SippoAnms[25] = arc.GetAnimation("TA_DAM_M_TA_B.saanim");
	SippoAnms[26] = arc.GetAnimation("TA_DAM_M_TA_C.saanim");
	SippoAnms[27] = arc.GetAnimation("TA_BREAK_TA_A.saanim");
	SippoAnms[28] = arc.GetAnimation("TA_BREAK_TA_B.saanim");
	SippoAnms[29] = arc.GetAnimation("TA_BREAK_TA_C.saanim");
	SippoAnms[30] = arc.GetAnimation("TA_BRA_MID_TA.saanim");
	SippoAnms[31] = arc.GetAnimation("TA_ATC_BOMB_TA.saanim");
	SippoAnms[32] = arc.GetAnimation("TA_WIN_B_TA.saanim");
	SippoAnms[33] = arc.GetAnimation("HERO_TA_TA.saanim");

	for (int i = 0; i < LengthOfArray(TailsActs); ++i) {
		TailsActs[i].object = TailsMdls[0]->getmodel();
		TailsActs[i].motion = TailsAnms[i] ? TailsAnms[i]->getmotion() : NULL;
	}

	for (int i = 0; i < LengthOfArray(SippoActs); ++i) {
		SippoActs[i].object = TailsMdls[2]->getmodel();
		SippoActs[i].motion = SippoAnms[i] ? SippoAnms[i]->getmotion() : NULL;
	}
}

void UnloadTailsFiles() {
	HelperFunctionsGlobal.Weights->DeInit(TailsMdls[0]->getweightinfo(), TailsMdls[0]->getmodel());
	HelperFunctionsGlobal.Weights->DeInit(TailsMdls[2]->getweightinfo(), TailsMdls[2]->getmodel());
	FreeMDLFiles(TailsMdls, LengthOfArray(TailsMdls));
	FreeANMFiles(TailsAnms, LengthOfArray(TailsAnms));
}