#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	KN_WALK,
	KN_WALK_PULL,
	KN_WALK_PUSH,
	KN_TURN_L,
	KN_TURN_R,
	KN_SLOW_RUN,
	KN_MID_RUN,
	KN_TOP_RUN,
	KN_START,
	KN_ATC_SPIKE,
	KN_ATC_01,
	KN_ATC_C,
	KN_ATC_D,
	KN_JUMP_A,
	KN_JUMP_B,
	KN_JUMP_C,
	KN_JUMP_D,
	KN_JUMP_E,
	KN_JUMP_F,
	KN_JUMP_TRNGL,
	KN_JUMP_GLIND,
	KN_GLIND,
	KN_GLIND_BK,
	KN_GLIND_BK_L,
	KN_GLIND_BK_R,
	KN_GLIND_FLIP_B,
	KN_GLIND_FLIP_FR,
	KN_GLIND_L,
	KN_GLIND_R,
	KN_FLY_IDLE,
	KN_FLY_SLOW,
	KN_FLY_GLIND,
	KN_HANG_OFF,
	KN_HANG_ON,
	KN_BREAK_A,
	KN_BREAK_B,
	KN_BREAK_C,
	KN_BREAK_TURN_L,
	KN_BREAK_TURN_R,
	KN_BRA_MID,
	KN_BRA_TOP,
	KN_FLORT,
	KN_DAM_M_A,
	KN_DAM_M_B,
	KN_DAM_M_C,
	KN_EDGE_OTTO_A,
	KN_EDGE_OTTO_B,
	KN_EDGE_OTTO_C,
	KN_FW_JUMP,
	KN_TRAP_JUMP,
	HERO_KN,
	KN_WIN_B,
	KN_ATC_RENDA,
	KN_IDLE,
	KN_IDLE_D,
	KN_IDLE_E,
	KN_BOB,
	KN_BOB_L,
	KN_BOB_R,
};

ModelInfo* KnucklesMdls[2];
AnimationFile* KnucklesAnms[60];
NJS_ACTION KnucklesActs[60];

PL_ACTION knuckles_action_heroes[] = {
	{ &KnucklesActs[KN_WALK], 44, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_WALK_PULL], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_WALK_PUSH], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_TURN_L], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_TURN_R], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_SLOW_RUN], 44, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &KnucklesActs[KN_MID_RUN], 44, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &KnucklesActs[KN_TOP_RUN], 44, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &KnucklesActs[KN_START], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_ATC_SPIKE], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_ATC_01], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_ATC_C], 44, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &KnucklesActs[KN_ATC_D], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_JUMP_A], 44, MD_MTN_NEXT, MTN_POW_JUMP_B, 0.25f, 0.5f },
	{ &KnucklesActs[KN_JUMP_B], 44, MD_MTN_WORK, 0, 1.0f, 0.25f },
	{ &KnucklesActs[KN_JUMP_C], 44, MD_MTN_NEXT, MTN_POW_JUMP_D, 0.25f, 0.5f },
	{ &KnucklesActs[KN_JUMP_D], 44, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &KnucklesActs[KN_JUMP_E], 44, MD_MTN_NEXT, MTN_POW_JUMP_F, 0.25f, 0.5f },
	{ &KnucklesActs[KN_JUMP_F], 44, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &KnucklesActs[KN_JUMP_TRNGL], 44, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &KnucklesActs[KN_JUMP_GLIND], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_BK], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_BK_L], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_BK_R], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_FLIP_B], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_FLIP_FR], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_L], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_GLIND_R], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_FLY_IDLE], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_FLY_SLOW], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_FLY_GLIND], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_HANG_OFF], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_HANG_ON], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BREAK_A], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BREAK_B], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BREAK_C], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BREAK_TURN_L], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BREAK_TURN_R], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BRA_MID], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BRA_TOP], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_FLORT], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_DAM_M_A], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_DAM_M_B], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_DAM_M_C], 44, MD_MTN_NEXT, MTN_POW_JUMP_E, 0.25f, 1.0f },
	{ &KnucklesActs[KN_EDGE_OTTO_A], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_EDGE_OTTO_B], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_EDGE_OTTO_C], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_FW_JUMP], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_TRAP_JUMP], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[HERO_KN], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_WIN_B], 44, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_ATC_RENDA], 44, MD_MTN_NEXT, MTN_SPD_IDLE, 0.1f, 1.0f },
	{ &KnucklesActs[KN_IDLE], 44, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &KnucklesActs[KN_IDLE_D], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_IDLE_E], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BOB], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BOB_L], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_BOB_R], 44, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &KnucklesActs[KN_JUMP_B], 44, MD_MTN_XSPD, 0, 0.25f, 0.1f },
};

NJS_TEXNAME HKNUCKLES_TEXNAMES[4];
NJS_TEXLIST HKNUCKLES_TEXLIST = { arrayptrandlength(HKNUCKLES_TEXNAMES) };

NJS_MATRIX KnucklesMatrix;

void PlayVoice_Knuckles(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(KnucklesSound_Win);
		break;
	}
}

void PlaySound_Knuckles(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(KnucklesSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(KnucklesSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(KnucklesSound_Hurt1);
		break;
	case 1503:
		PlayHeroesSound(KnucklesSound_Death);
		break;
	}
}

void KnucklesCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = KnucklesMdls[0]->getmodel();

	if (object == base->getnode(19)) {
		njGetMatrix(KnucklesMatrix); //pupils
	}
}

void KnucklesHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;
	
	ObjectMaster* knucklesobj = HeroesChars[obj->Data1->CharIndex];
	if (!knucklesobj) return;
	EntityData2* data2 = (EntityData2*)knucklesobj->Data2;

	WriteData<5>((void*)0x472649, 0x90);
	WriteData<5>((void*)0x47258B, 0x90);
	WriteData<5>((void*)0x472626, 0x90);
	Knuckles_Display(obj);
	WriteCall((void*)0x472649, njAction);
	WriteCall((void*)0x47258B, njAction);
	WriteCall((void*)0x472626, njAction);

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (knucklesobj->Data1->Index == 42 || knucklesobj->Data1->Index == 43 || knucklesobj->Data1->Index == 44) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (knucklesobj->Data1->NextAction == 0) knucklesobj->Data1->NextAction = 1;
			else knucklesobj->Data1->NextAction = 0;
		}
	}
	else {
		knucklesobj->Data1->NextAction = 0;
	}

	if (knucklesobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HKNUCKLES_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);

	if (entity1->Action == 19) {
		if (data2->field_38 > -6) data2->field_38 -= 0.1f;
		njTranslate(0, 0, data2->field_38, 0);
	}
	else if (entity1->Action == 20 || entity1->Action == 21) {
		data2->field_38 = 0;
	}

	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	if (entity1->Action == 19) njRotateX(0, -0x1000);
	
	if (entity1->Action == 24) {
		njRotateX(0, -0x4000);
		njTranslate(0, 0, 0, 2);
	}

	if (knucklesobj->Data1->Index == 52) {
		njRotateY(0, 0xC000);
	}

	njRotateX(0, 0x4000);

	mtnjvwk* mtn = (mtnjvwk*)knucklesobj->Data1->Object;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = KnucklesCallback;
	njActionWeight(actptr, mtn->nframe, KnucklesMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, KnucklesMatrix);
	NJS_CNK_OBJECT* pupils = KnucklesMdls[1]->getmodel();
	switch (knucklesobj->Data1->InvulnerableTime) {
	case 1:
	case 5:
		dsDrawModel(pupils->getbasicdxmodel());
		break;
	case 2:
	case 4:
		dsDrawModel(pupils->child->getbasicdxmodel());
		break;
	case 3:
		dsDrawModel(pupils->child->child->getbasicdxmodel());
		break;
	}

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void KnucklesHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];
	mtnjvwk* mtn = (mtnjvwk*)data->Object;

	if (!playerco2 || playerdata->CharID != Characters_Knuckles)
	{
		return;
	}

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-knuckles", &HKNUCKLES_TEXLIST }, 2, knuckles_action_heroes);
		data->Action = 2;
		return;
	case 2:
		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground && (HeldButtons2[data->CharIndex] & Buttons_A) != Buttons_A) {
				data->field_A = 0;
				playerdata->Action = 2;
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayHeroesSound(KnucklesSound_Attack);
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(KnucklesSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(KnucklesSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		KnucklesAnimConverter(mtn, Characters_HeroesKnuckles, (taskwk*)playerdata, (playerwk*)playerco2);
		
		break;
	case 3:
		switch (PowerComboTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayHeroesSound(KnucklesSound_Combo1);
			break;
		case 2:
			PlayHeroesSound(KnucklesSound_Combo2);
			break;
		case 3:
			PlayHeroesSound(KnucklesSound_Combo3);
			playerco2->Speed.x = 1;
			playerco2->Speed.y = 2;
		case 4:
			if (data->Scale.x > 89) {
				ExploseEnemies(&playerdata->Position, 2);
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				data->Action = 2;
			}
			break;
		}
		
		mtn->reqaction = 11;
		break;
	case 4:
		if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayHeroesSound(KnucklesSound_Trick);

		mtn->reqaction = 12;
		break;
	}

	CharactersCommon_DrawBall(playerdata, data);
	PSetMotion(mtn);

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 5) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadKnuckFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-knuckles.arcx"));

	KnucklesMdls[0] = arc.GetModel("KNUCKLES_LOCATOR.sa1mdl");
	KnucklesMdls[1] = arc.GetModel("KN_MABUTA.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(KnucklesMdls[0]->getweightinfo(), KnucklesMdls[0]->getmodel());

	KnucklesAnms[0] = arc.GetAnimation("KN_WALK.saanim");
	KnucklesAnms[1] = arc.GetAnimation("KN_WALK_PULL.saanim");
	KnucklesAnms[2] = arc.GetAnimation("KN_WALK_PUSH.saanim");
	KnucklesAnms[3] = arc.GetAnimation("KN_TURN_L.saanim");
	KnucklesAnms[4] = arc.GetAnimation("KN_TURN_R.saanim");
	KnucklesAnms[5] = arc.GetAnimation("KN_SLOW_RUN.saanim");
	KnucklesAnms[6] = arc.GetAnimation("KN_MID_RUN.saanim");
	KnucklesAnms[7] = arc.GetAnimation("KN_TOP_RUN.saanim");
	KnucklesAnms[8] = arc.GetAnimation("KN_START.saanim");
	KnucklesAnms[9] = arc.GetAnimation("KN_ATC_SPIKE.saanim");
	KnucklesAnms[10] = arc.GetAnimation("KN_ATC_01.saanim");
	KnucklesAnms[11] = arc.GetAnimation("KN_ATC_C.saanim");
	KnucklesAnms[12] = arc.GetAnimation("KN_ATC_D.saanim");
	KnucklesAnms[13] = arc.GetAnimation("KN_JUMP_A.saanim");
	KnucklesAnms[14] = arc.GetAnimation("KN_JUMP_B.saanim");
	KnucklesAnms[15] = arc.GetAnimation("KN_JUMP_C.saanim");
	KnucklesAnms[16] = arc.GetAnimation("KN_JUMP_D.saanim");
	KnucklesAnms[17] = arc.GetAnimation("KN_JUMP_E.saanim");
	KnucklesAnms[18] = arc.GetAnimation("KN_JUMP_F.saanim");
	KnucklesAnms[19] = arc.GetAnimation("KN_JUMP_TRNGL.saanim");
	KnucklesAnms[20] = arc.GetAnimation("KN_JUMP_GLIND.saanim");
	KnucklesAnms[21] = arc.GetAnimation("KN_GLIND.saanim");
	KnucklesAnms[22] = arc.GetAnimation("KN_GLIND_BK.saanim");
	KnucklesAnms[23] = arc.GetAnimation("KN_GLIND_BK_L.saanim");
	KnucklesAnms[24] = arc.GetAnimation("KN_GLIND_BK_R.saanim");
	KnucklesAnms[25] = arc.GetAnimation("KN_GLIND_FLIP_B.saanim");
	KnucklesAnms[26] = arc.GetAnimation("KN_GLIND_FLIP_FR.saanim");
	KnucklesAnms[27] = arc.GetAnimation("KN_GLIND_L.saanim");
	KnucklesAnms[28] = arc.GetAnimation("KN_GLIND_R.saanim");
	KnucklesAnms[29] = arc.GetAnimation("KN_FLY_IDLE.saanim");
	KnucklesAnms[30] = arc.GetAnimation("KN_FLY_SLOW.saanim");
	KnucklesAnms[31] = arc.GetAnimation("KN_FLY_GLIND.saanim");
	KnucklesAnms[32] = arc.GetAnimation("KN_HANG_OFF.saanim");
	KnucklesAnms[33] = arc.GetAnimation("KN_HANG_ON.saanim");
	KnucklesAnms[34] = arc.GetAnimation("KN_BREAK_A.saanim");
	KnucklesAnms[35] = arc.GetAnimation("KN_BREAK_B.saanim");
	KnucklesAnms[36] = arc.GetAnimation("KN_BREAK_C.saanim");
	KnucklesAnms[37] = arc.GetAnimation("KN_BREAK_TURN_L.saanim");
	KnucklesAnms[38] = arc.GetAnimation("KN_BREAK_TURN_R.saanim");
	KnucklesAnms[39] = arc.GetAnimation("KN_BRA_MID.saanim");
	KnucklesAnms[40] = arc.GetAnimation("KN_BRA_TOP.saanim");
	KnucklesAnms[41] = arc.GetAnimation("KN_FLORT.saanim");
	KnucklesAnms[42] = arc.GetAnimation("KN_DAM_M_A.saanim");
	KnucklesAnms[43] = arc.GetAnimation("KN_DAM_M_B.saanim");
	KnucklesAnms[44] = arc.GetAnimation("KN_DAM_M_C.saanim");
	KnucklesAnms[45] = arc.GetAnimation("KN_EDGE_OTTO_A.saanim");
	KnucklesAnms[46] = arc.GetAnimation("KN_EDGE_OTTO_B.saanim");
	KnucklesAnms[47] = arc.GetAnimation("KN_EDGE_OTTO_C.saanim");
	KnucklesAnms[48] = arc.GetAnimation("KN_FW_JUMP.saanim");
	KnucklesAnms[49] = arc.GetAnimation("KN_TRAP_JUMP.saanim");
	KnucklesAnms[50] = arc.GetAnimation("HERO_KN.saanim");
	KnucklesAnms[51] = arc.GetAnimation("KN_WIN_B.saanim");
	KnucklesAnms[52] = arc.GetAnimation("KN_ATC_RENDA.saanim");
	KnucklesAnms[53] = arc.GetAnimation("KN_IDLE.saanim");
	KnucklesAnms[54] = arc.GetAnimation("KN_IDLE_D.saanim");
	KnucklesAnms[55] = arc.GetAnimation("KN_IDLE_E.saanim");
	KnucklesAnms[56] = arc.GetAnimation("KN_BOB.saanim");
	KnucklesAnms[57] = arc.GetAnimation("KN_BOB_L.saanim");
	KnucklesAnms[58] = arc.GetAnimation("KN_BOB_R.saanim");

	for (int i = 0; i < LengthOfArray(KnucklesActs); ++i) {
		KnucklesActs[i].object = KnucklesMdls[0]->getmodel();
		KnucklesActs[i].motion = KnucklesAnms[i] ? KnucklesAnms[i]->getmotion() : NULL;
	}
}

void UnloadKnuckFiles() {
	HelperFunctionsGlobal.Weights->DeInit(KnucklesMdls[0]->getweightinfo(), KnucklesMdls[0]->getmodel());
	FreeMDLFiles(KnucklesMdls, LengthOfArray(KnucklesMdls));
	FreeANMFiles(KnucklesAnms, LengthOfArray(KnucklesAnms));
}