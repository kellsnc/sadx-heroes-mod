#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "paths.h"
#include "sounds.h"
#include "characters.h"

enum {
	CR_WALK,
	CR_WALK_PULL,
	CR_WALK_PUSH,
	CR_TURN_L,
	CR_TURN_R,
	CR_SLOW_RUN,
	CR_MID_RUN,
	CR_START,
	CR_START_IDLE,
	CR_IDLE_B_HALF,
	CR_IDLE_C_HALF,
	CR_IDLE_HALF,
	CR_IDLE_POW_HALF,
	CR_IDLE_POW_SHAKE,
	CR_POW_ROT,
	CR_POW_JUMP,
	CR_FW_JUMP,
	CR_FW_POW_JUMP,
	CR_JUMP_A,
	CR_JUMP_B,
	CR_JUMP_C,
	CR_JUMP_D,
	CR_JUMP_E,
	CR_JUMP_F,
	CR_JUMP_UMBRELLA,
	CR_JUMP_GLIND,
	CR_GLIND,
	CR_GLIND_BK,
	CR_GLIND_BK_L,
	CR_GLIND_BK_R,
	CR_GLIND_FLIP_BK,
	CR_GLIND_FLIP_FR,
	CR_GLIND_L,
	CR_GLIND_R,
	CR_FLY_IDLE,
	CR_FLY_SLOW,
	CR_FLY_UP,
	CR_FLY_PULL,
	CR_FLY_PUSH,
	CR_FLY_KICK,
	CR_FLY_HANG_IDLE,
	CR_FLY_HANG_OFF,
	CR_FLY_HANG_ON,
	CR_HANG_OFF,
	CR_HANG_ON,
	CR_BREAK_A,
	CR_BREAK_B,
	CR_BREAK_C,
	CR_BREAK_TURN_L,
	CR_BREAK_TURN_R,
	CR_BRA_MID,
	CR_BRA_TOP,
	CR_BOB,
	CR_FLORT,
	CR_DAM_MID_A,
	CR_DAM_MID_B,
	CR_DAM_MID_C,
	CR_EDGE_OTTO_A,
	CR_EDGE_OTTO_B,
	CR_EDGE_OTTO_C,
	CR_WIN,
	CR_ATC_CHAO,
	ROSE_CR,
	CR_ROLL
};

enum {
	CH_IDLE,
	CH_IDLE_B,
	CH_IDLE_B_OPT,
	CH_IDLE_C,
	CH_IDLE_C_OPT,
	CH_FLY,
	CH_ATC_CHAO_A,
	CH_ATC_CHAO_B,
	CH_WIN,
	CH_CHARANGE_IDLE,
	CH_CHARANGE_SELECT,
	CH_STORY_IDLE,
	CH_STORY_SELECT,
};

ModelInfo* CreamMdls[4];
AnimationFile* CreamAnms[63];
AnimationFile* CheeseAnms[34];
NJS_ACTION CreamActs[63];
NJS_ACTION CheeseActs[34];

PL_ACTION cream_action_heroes[] = {
	{ &CreamActs[CR_WALK], 55, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_WALK_PULL], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_WALK_PUSH], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_TURN_L], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_TURN_R], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_SLOW_RUN], 55, MD_MTN_XSPD, 0, 0.25f, 0.25f },
	{ &CreamActs[CR_MID_RUN], 55, MD_MTN_XSPD, 0, 0.25f, 0.2f },
	{ &CreamActs[CR_START], 55, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &CreamActs[CR_START_IDLE], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_IDLE_B_HALF], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_IDLE_C_HALF], 55, MD_MTN_NEXT, CR_START_IDLE, 0.25f, 0.5f },
	{ &CreamActs[CR_IDLE_HALF], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_IDLE_POW_HALF], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_IDLE_POW_SHAKE], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_POW_ROT], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_POW_JUMP], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FW_JUMP], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FW_POW_JUMP], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_JUMP_A], 48, MD_MTN_NEXT, CR_JUMP_B, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_B], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_C], 48, MD_MTN_NEXT, CR_JUMP_D, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_D], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_E], 48, MD_MTN_NEXT, CR_JUMP_F, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_F], 48, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &CreamActs[CR_JUMP_UMBRELLA], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_JUMP_GLIND], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_BK], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_BK_L], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_BK_R], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_FLIP_BK], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_FLIP_FR], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_L], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_GLIND_R], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_IDLE], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_SLOW], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_UP], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_PULL], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_PUSH], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_KICK], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_HANG_IDLE], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_HANG_OFF], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLY_HANG_ON], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_HANG_OFF], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_HANG_ON], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BREAK_A], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BREAK_B], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BREAK_C], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BREAK_TURN_L], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BREAK_TURN_R], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BRA_MID], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BRA_TOP], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_BOB], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_FLORT], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_DAM_MID_A], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_DAM_MID_B], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_DAM_MID_C], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_EDGE_OTTO_A], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_EDGE_OTTO_B], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_EDGE_OTTO_C], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_WIN], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_ATC_CHAO], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[ROSE_CR], 55, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CreamActs[CR_JUMP_B], 48, MD_MTN_XSPD, 0, 0.25f, 0.5f },
};

PL_ACTION cheese_action[] = {
	{ &CheeseActs[CH_IDLE], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_IDLE_B], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_IDLE_B_OPT], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_IDLE_C], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_IDLE_C_OPT], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_FLY], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_ATC_CHAO_A], 12, MD_MTN_NEXT, CH_ATC_CHAO_B, 0.25f, 1.0f },
	{ &CheeseActs[CH_ATC_CHAO_B], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_WIN], 12, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_CHARANGE_IDLE], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_CHARANGE_SELECT], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_STORY_IDLE], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &CheeseActs[CH_STORY_SELECT], 12, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

NJS_TEXNAME CREAM_TEXNAMES[4];
NJS_TEXLIST CREAM_TEXLIST = { arrayptrandlength(CREAM_TEXNAMES) };

CollisionData Cheese_Col = { 0, CI_FORM_SPHERE, CI_PUSH_TH_ALL, CI_DMG_SET(3, 3) | CI_DMG_ENEMY, CI_ATTR_DAMAGE, { 0.0f, 0.0f, 0.0f }, 4.5f, 0.0f, 0.0f};

void PlayVoice_Cream(int ID) {
	switch (ID) {
	case 1803:
		PlayHeroesSound(CreamSound_Win);
		break;
	}
}

void PlaySound_Cream(int ID) {
	int random = rand() % 10;
	
	switch (ID) {
	case 17:
		PlayHeroesSound(CreamSound_Trick);
		break;
	case 1243:
		PlayHeroesSound(CreamSound_FlyBegin);
		break;
	case 1249:
		if (random < 4) PlayHeroesSound(CreamSound_ThatHurts);
		else if (random < 8) PlayHeroesSound(CreamSound_Hurt1);
		else PlayHeroesSound(CreamSound_Hurt2);
		break;
	case 1465:
		PlayHeroesSound(CreamSound_Death);
		break;
	case 1453:
		PlaySound(ID, 0, 0, 0);
		break;
	}
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

ObjectMaster* Cheese_GetClosestEnemy(int pnum, float max_dist)
{
	colaround* ael; // todo: multiplayer mod compatibility
	if (pnum)
		ael = around_enemy_list_p1;
	else
		ael = around_enemy_list_p0;

	taskwk* tgt_twp = NULL;
	float tgt_dst = max_dist * max_dist;

	while (ael->twp)
	{
		if (ael->dist < tgt_dst)
		{
			tgt_dst = ael->dist;
			tgt_twp = ael->twp;

		}
		++ael;
	}

	if (tgt_twp)
	{
		return (ObjectMaster*)tgt_twp->cwp->mytask;
	}

	return NULL;
}

void Cheese_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData1* playerdata = EntityData1Ptrs[obj->Parent->Data1->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[obj->Parent->Data1->CharIndex];
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->Parent->UnknownB_ptr;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	mtnjvwk* mtn = &pwp_heroes->mm_sub;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

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

		if (action == 0) {
			njRotateY(0, data2->Forward.y += 0x100);
			njTranslate(0, 2, 0, 0);
		}
		else {
			njRotateY(0, -data->Rotation.y - 0x4000);
		}
	}

	njPushMatrixEx();
	njRotateX(0, 0x4000);
	njAction(actptr, mtn->nframe);
	njPopMatrixEx();

	njTranslate(0, -data->Position.x, -data->Position.y, -data->Position.z);
	njTranslateV(0, &data2->SomeCollisionVector);
	njTranslate(0, 0, 5, -1);
	dsDrawModel(CreamMdls[3]->getmodel()->getbasicdxmodel());

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void Cheese_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData1* playerdata = EntityData1Ptrs[obj->Parent->Data1->CharIndex];
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->Parent->UnknownB_ptr;

	float dist = GetDistance(&data2->VelocityDirection, &data->Position);

	float state = 0;
	float frame = data->Scale.x;

	switch (data->Action) {
	case 0:
		obj->DisplaySub = Cheese_Display;
		data->Position = GetCheesePoint(&playerdata->Position, &playerdata->Rotation);
		Collision_Init(obj, &Cheese_Col, 1, CID_BULLET);

		pwp_heroes->mm_sub.plactptr = cheese_action;
		pwp_heroes->mm_sub.mtnmode = MD_MTN_INIT;
		pwp_heroes->mm_sub.reqaction = 0;
		PSetMotion(&pwp_heroes->mm_sub);

		data->Action = 1;
		break;
	case 1:
		data2->VelocityDirection = GetCheesePoint(&playerdata->Position, &playerdata->Rotation);
		data->Rotation = playerdata->Rotation;

		if (dist > 1000) data->Position = data2->VelocityDirection;

		if (dist < 5) {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / (100 + (400 - dist)));
			data2->SomeCollisionVector = data->Position;
			pwp_heroes->mm_sub.reqaction = 0;
			if (GetDistance(&data2->VelocityDirection, &data->Position) < 1) {
				data->Position = data2->VelocityDirection;
				data->Action = 1;
			}
		}
		else if (dist < 30) {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / 400);
			data2->SomeCollisionVector = GetPathPosition(&data->Position, &data2->VelocityDirection, (dist / 400) - 0.1f);
			data->Rotation.y = fPositionToRotation(&data->Position, &data2->VelocityDirection).y;
			pwp_heroes->mm_sub.reqaction = 5;
		}
		else {
			data->Position = GetPathPosition(&data->Position, &data2->VelocityDirection, dist / 300);
			data2->SomeCollisionVector = GetPathPosition(&data->Position, &data2->VelocityDirection, (dist / 300) - 0.1f);
			data->Rotation.y = fPositionToRotation(&data->Position, &data2->VelocityDirection).y;
			pwp_heroes->mm_sub.reqaction = 5;
		}

		switch (CharObj2Ptrs[obj->Parent->Data1->CharIndex]->AnimationThing.Index) {
		case 6:
			if (obj->Parent->Data1->Index == 8)  pwp_heroes->mm_sub.reqaction = 1;
			else  pwp_heroes->mm_sub.reqaction = 2;
			break;
		case 33:
			if (dist < 30)  pwp_heroes->mm_sub.reqaction = 3;
			break;
		case 54:
		case 55:
			pwp_heroes->mm_sub.reqaction = 8;
			break;
		case 150:
			pwp_heroes->mm_sub.reqaction = 12;
			break;
		}

		break;
	case 2: //cheese special attack, get the closest enemy
		data->LoopData = (Loop*)Cheese_GetClosestEnemy(data->CharIndex, 200.0f);
		
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
			data->LoopData = (Loop*)Cheese_GetClosestEnemy(data->CharIndex, 200.0f);
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
	PSetMotion(&pwp_heroes->mm_sub);

	obj->DisplaySub(obj);
}

NJS_MATRIX EyeLashesMatrix;

void CreamCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = CreamMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(EyeLashesMatrix);
	}
}

void CreamHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* creamobj = HeroesChars[obj->Data1->CharIndex];
	
	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)creamobj->UnknownB_ptr;

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
	njRotateY(0, -entity1->Rotation.y - 0x4000);

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

	*NodeCallbackFuncPtr = CreamCallback;
	njActionWeight(actptr, mtn->nframe, CreamMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, EyeLashesMatrix);
	switch (creamobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		dsDrawModel(CreamMdls[1]->getmodel()->child->getbasicdxmodel());
		break;
	case 2:
	case 6:
		dsDrawModel(CreamMdls[1]->getmodel()->child->child->getbasicdxmodel());
		break;
	case 3:
	case 5:
		dsDrawModel(CreamMdls[1]->getmodel()->child->child->child->getbasicdxmodel());
		break;
	case 4:
		dsDrawModel(CreamMdls[1]->getmodel()->child->child->child->child->getbasicdxmodel());
		break;
	default:
		dsDrawModel(CreamMdls[1]->getmodel()->getbasicdxmodel());
		break;
	}

	njPopMatrix(1);

	if (action == CR_JUMP_B || action == CR_ROLL)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 0);
	}
	else if (action == CR_FW_JUMP)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 1);
	}

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CreamAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp)
{
	int anim = mtn->reqaction;

	switch (pwp->mj.reqaction) {
	case 0:
	case 7:
	case 8:
		anim = 11; break;
	case 2: anim = 8; break;
	case 4:
	case 5:
	case 6:
		anim = 10;
		break;
	case 9:
		anim = 0;
		break;
	case 10:
		anim = 0;
		break;
	case 11:
		anim = 0;
		break;
	case 12:
		anim = 5;
		break;
	case 13:
		anim = 6;
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
		anim = 11;
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

void CreamHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->UnknownB_ptr;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-cream", &CREAM_TEXLIST }, 2, cream_action_heroes);
		LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), Cheese_Main, obj);
		data->Action = 2;
		return;
	case 2:
		if (playerdata->field_A < PlayerState_OnRail && playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
			playerdata->Action = 100;
			data->Action = 3;
			break;
		}

		if (CanDoTricks(playerdata)) {
			if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_X) {
				PlayHeroesSound(CreamSound_FlyAttack);
				data->field_A = 0;
				data->Action = 4;
			}

			if (anim == 34 && PressedButtons[data->CharIndex] & Buttons_A) {
				if (++data->field_A == 2) {
					PlayHeroesSound(CreamSound_FlyUp);
					data->field_A = 0;
					playerco2->Speed.y = 2;
				}
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(CreamSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(CreamSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		CreamAnimConverter(&pwp_heroes->mm, HeroesChars_Tails, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		if (data->field_A == 0) {
			PlayHeroesSound(CreamSound_CheeseGetHim);
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

		pwp_heroes->mm.reqaction = 61;
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

		pwp_heroes->mm.reqaction = 39;
		break;
	}

	PSetMotion(&pwp_heroes->mm);

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 7) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadCreamFiles() {
	ArchiveX arc_cr(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-cream.arcx"));
	ArchiveX arc_ch(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-cheese.arcx"));

	CreamMdls[0] = arc_cr.GetModel("CREAM_LOCATOR.sa1mdl");
	CreamMdls[1] = arc_cr.GetModel("CR_MABUTA.sa1mdl");
	CreamMdls[2] = arc_ch.GetModel("CHAO_LOCATOR.sa1mdl");
	CreamMdls[3] = arc_ch.GetModel("CH_KAKU_LOCATOR.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(CreamMdls[0]->getweightinfo(), CreamMdls[0]->getmodel());

	CreamAnms[0] = arc_cr.GetAnimation("CR_WALK.saanim");
	CreamAnms[1] = arc_cr.GetAnimation("CR_WALK_PULL.saanim");
	CreamAnms[2] = arc_cr.GetAnimation("CR_WALK_PUSH.saanim");
	CreamAnms[3] = arc_cr.GetAnimation("CR_TURN_L.saanim");
	CreamAnms[4] = arc_cr.GetAnimation("CR_TURN_R.saanim");
	CreamAnms[5] = arc_cr.GetAnimation("CR_SLOW_RUN.saanim");
	CreamAnms[6] = arc_cr.GetAnimation("CR_MID_RUN.saanim");
	CreamAnms[7] = arc_cr.GetAnimation("CR_START.saanim");
	CreamAnms[8] = arc_cr.GetAnimation("CR_START_IDLE.saanim");
	CreamAnms[9] = arc_cr.GetAnimation("CR_IDLE_B_HALF.saanim");
	CreamAnms[10] = arc_cr.GetAnimation("CR_IDLE_C_HALF.saanim");
	CreamAnms[11] = arc_cr.GetAnimation("CR_IDLE_HALF.saanim");
	CreamAnms[12] = arc_cr.GetAnimation("CR_IDLE_POW_HALF.saanim");
	CreamAnms[13] = arc_cr.GetAnimation("CR_IDLE_POW_SHAKE.saanim");
	CreamAnms[14] = arc_cr.GetAnimation("CR_POW_ROT.saanim");
	CreamAnms[15] = arc_cr.GetAnimation("CR_POW_JUMP.saanim");
	CreamAnms[16] = arc_cr.GetAnimation("CR_FW_JUMP.saanim");
	CreamAnms[17] = arc_cr.GetAnimation("CR_FW_POW_JUMP.saanim");
	CreamAnms[18] = arc_cr.GetAnimation("CR_JUMP_A.saanim");
	CreamAnms[19] = arc_cr.GetAnimation("CR_JUMP_B.saanim");
	CreamAnms[20] = arc_cr.GetAnimation("CR_JUMP_C.saanim");
	CreamAnms[21] = arc_cr.GetAnimation("CR_JUMP_D.saanim");
	CreamAnms[22] = arc_cr.GetAnimation("CR_JUMP_E.saanim");
	CreamAnms[23] = arc_cr.GetAnimation("CR_JUMP_F.saanim");
	CreamAnms[24] = arc_cr.GetAnimation("CR_JUMP_UMBRELLA.saanim");
	CreamAnms[25] = arc_cr.GetAnimation("CR_JUMP_GLIND.saanim");
	CreamAnms[26] = arc_cr.GetAnimation("CR_GLIND.saanim");
	CreamAnms[27] = arc_cr.GetAnimation("CR_GLIND_BK.saanim");
	CreamAnms[28] = arc_cr.GetAnimation("CR_GLIND_BK_L.saanim");
	CreamAnms[29] = arc_cr.GetAnimation("CR_GLIND_BK_R.saanim");
	CreamAnms[30] = arc_cr.GetAnimation("CR_GLIND_FLIP_BK.saanim");
	CreamAnms[31] = arc_cr.GetAnimation("CR_GLIND_FLIP_FR.saanim");
	CreamAnms[32] = arc_cr.GetAnimation("CR_GLIND_L.saanim");
	CreamAnms[33] = arc_cr.GetAnimation("CR_GLIND_R.saanim");
	CreamAnms[34] = arc_cr.GetAnimation("CR_FLY_IDLE.saanim");
	CreamAnms[35] = arc_cr.GetAnimation("CR_FLY_SLOW.saanim");
	CreamAnms[36] = arc_cr.GetAnimation("CR_FLY_UP.saanim");
	CreamAnms[37] = arc_cr.GetAnimation("CR_FLY_PULL.saanim");
	CreamAnms[38] = arc_cr.GetAnimation("CR_FLY_PUSH.saanim");
	CreamAnms[39] = arc_cr.GetAnimation("CR_FLY_KICK.saanim");
	CreamAnms[40] = arc_cr.GetAnimation("CR_FLY_HANG_IDLE.saanim");
	CreamAnms[41] = arc_cr.GetAnimation("CR_FLY_HANG_OFF.saanim");
	CreamAnms[42] = arc_cr.GetAnimation("CR_FLY_HANG_ON.saanim");
	CreamAnms[43] = arc_cr.GetAnimation("CR_HANG_OFF.saanim");
	CreamAnms[44] = arc_cr.GetAnimation("CR_HANG_ON.saanim");
	CreamAnms[45] = arc_cr.GetAnimation("CR_BREAK_A.saanim");
	CreamAnms[46] = arc_cr.GetAnimation("CR_BREAK_B.saanim");
	CreamAnms[47] = arc_cr.GetAnimation("CR_BREAK_C.saanim");
	CreamAnms[48] = arc_cr.GetAnimation("CR_BREAK_TURN_L.saanim");
	CreamAnms[49] = arc_cr.GetAnimation("CR_BREAK_TURN_R.saanim");
	CreamAnms[50] = arc_cr.GetAnimation("CR_BRA_MID.saanim");
	CreamAnms[51] = arc_cr.GetAnimation("CR_BRA_TOP.saanim");
	CreamAnms[52] = arc_cr.GetAnimation("CR_BOB.saanim");
	CreamAnms[53] = arc_cr.GetAnimation("CR_FLORT.saanim");
	CreamAnms[54] = arc_cr.GetAnimation("CR_DAM_MID_A.saanim");
	CreamAnms[55] = arc_cr.GetAnimation("CR_DAM_MID_B.saanim");
	CreamAnms[56] = arc_cr.GetAnimation("CR_DAM_MID_C.saanim");
	CreamAnms[57] = arc_cr.GetAnimation("CR_EDGE_OTTO_A.saanim");
	CreamAnms[58] = arc_cr.GetAnimation("CR_EDGE_OTTO_B.saanim");
	CreamAnms[59] = arc_cr.GetAnimation("CR_EDGE_OTTO_C.saanim");
	CreamAnms[60] = arc_cr.GetAnimation("CR_WIN.saanim");
	CreamAnms[61] = arc_cr.GetAnimation("CR_ATC_CHAO.saanim");
	CreamAnms[62] = arc_cr.GetAnimation("ROSE_CR.saanim");

	CheeseAnms[0] = arc_ch.GetAnimation("CH_IDLE.saanim");
	CheeseAnms[1] = arc_ch.GetAnimation("CH_IDLE_B.saanim");
	CheeseAnms[2] = arc_ch.GetAnimation("CH_IDLE_B_OPT.saanim");
	CheeseAnms[3] = arc_ch.GetAnimation("CH_IDLE_C.saanim");
	CheeseAnms[4] = arc_ch.GetAnimation("CH_IDLE_C_OPT.saanim");
	CheeseAnms[5] = arc_ch.GetAnimation("CH_FLY.saanim");
	CheeseAnms[6] = arc_ch.GetAnimation("CH_ATC_CHAO_A.saanim");
	CheeseAnms[7] = arc_ch.GetAnimation("CH_ATC_CHAO_B.saanim");
	CheeseAnms[8] = arc_ch.GetAnimation("CH_WIN.saanim");
	CheeseAnms[9] = arc_ch.GetAnimation("CH_CHARANGE_IDLE.saanim");
	CheeseAnms[10] = arc_ch.GetAnimation("CH_CHARANGE_SELECT.saanim");
	CheeseAnms[11] = arc_ch.GetAnimation("CH_STORY_IDLE.saanim");
	CheeseAnms[12] = arc_ch.GetAnimation("CH_STORY_SELECT.saanim");

	for (int i = 0; i < LengthOfArray(CreamActs); ++i) {
		CreamActs[i].object = CreamMdls[0]->getmodel();
		CreamActs[i].motion = CreamAnms[i] ? CreamAnms[i]->getmotion() : NULL;
	}

	for (int i = 0; i < LengthOfArray(CheeseActs); ++i) {
		CheeseActs[i].object = CreamMdls[2]->getmodel();
		CheeseActs[i].motion = CheeseAnms[i] ? CheeseAnms[i]->getmotion() : NULL;
	}
}

void UnloadCreamFiles() {
	HelperFunctionsGlobal.Weights->DeInit(CreamMdls[0]->getweightinfo(), CreamMdls[0]->getmodel());
	FreeMDLFiles(CreamMdls, LengthOfArray(CreamMdls));
	FreeANMFiles(CreamAnms, LengthOfArray(CreamAnms));
}