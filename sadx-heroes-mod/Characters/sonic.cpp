#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	SN_WALK,
	SN_WALK_PULL,
	SN_WALK_PUSH,
	SN_TURN_L,
	SN_TURN_R,
	SN_SLOW_RUN,
	SN_MID_RUN,
	SN_TOP_RUN,
	SN_START,
	SN_JUMP_WALL,
	SN_JUMP_TRIC_A,
	SN_JUMP_TRIC_B,
	SN_JUMP_TRIC_C,
	SN_JUMP_A,
	SN_JUMP_B,
	SN_JUMP_C,
	SN_JUMP_D,
	SN_JUMP_E,
	SN_JUMP_F,
	SN_JUMP_TRNGL,
	SN_JUMP_GLIND,
	SN_GLIND,
	SN_GLIND_BK,
	SN_GLIND_BK_L,
	SN_GLIND_BK_R,
	SN_GLIND_FLIP_B,
	SN_GLIND_FLIP_FR,
	SN_GLIND_L,
	SN_GLIND_R,
	SN_FLY_IDLE,
	SN_FLY_SLOW,
	SN_FLY_GLIND,
	SN_HANG_OFF,
	SN_HANG_ON,
	SN_BREAK_A,
	SN_BREAK_B,
	SN_BREAK_C,
	SN_BREAK_TURN_L,
	SN_BREAK_TURN_R,
	SN_BRA_MID,
	SN_BRA_TOP,
	SN_FLORT,
	SN_DAM_MID_A,
	SN_DAM_MID_B,
	SN_DAM_MID_C,
	SN_EDGE_OTTO_A,
	SN_EDGE_OTTO_B,
	SN_EDGE_OTTO_C,
	SN_FW_JUMP,
	SN_TRAP_JUMP,
	SN_POW_ROT,
	SN_WIN_B,
	SN_ATC_DASH,
	SN_IDLE,
	SN_IDLE_C,
	SN_IDLE_D,
	HERO_SN,
};

ModelInfo* SonicMdls[2];
AnimationFile* SonicAnms[57];
NJS_ACTION SonicActs[57];

PL_ACTION sonic_action_heroes[] = {
	{ &SonicActs[SN_WALK], 56, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_WALK_PULL], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_WALK_PUSH], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_TURN_L], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_TURN_R], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_SLOW_RUN], 56, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &SonicActs[SN_MID_RUN], 56, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &SonicActs[SN_TOP_RUN], 56, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &SonicActs[SN_START], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_WALL], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_TRIC_A], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_TRIC_B], 56, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 0.5f },
	{ &SonicActs[SN_JUMP_TRIC_C], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_A], 56, MD_MTN_NEXT, MTN_SPD_JUMP_B, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_B], 56, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &SonicActs[SN_JUMP_C], 56, MD_MTN_NEXT, MTN_SPD_JUMP_D, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_D], 56, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &SonicActs[SN_JUMP_E], 56, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_F], 56, MD_MTN_WORK, 0, 0.25f, 0.5f },
	{ &SonicActs[SN_JUMP_TRNGL], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_GLIND], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_BK], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_BK_L], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_BK_R], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_FLIP_B], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_FLIP_FR], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_L], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_GLIND_R], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_FLY_IDLE], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_FLY_SLOW], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_FLY_GLIND], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_HANG_OFF], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_HANG_ON], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BREAK_A], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BREAK_B], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BREAK_C], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BREAK_TURN_L], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BREAK_TURN_R], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BRA_MID], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_BRA_TOP], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_FLORT], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_DAM_MID_A], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_DAM_MID_B], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_DAM_MID_C], 56, MD_MTN_NEXT, SN_JUMP_E, 0.25f, 1.0f },
	{ &SonicActs[SN_EDGE_OTTO_A], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_EDGE_OTTO_B], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_EDGE_OTTO_C], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_FW_JUMP], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_TRAP_JUMP], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_POW_ROT], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_WIN_B], 56, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_ATC_DASH], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_IDLE], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_IDLE_C], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_IDLE_D], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[HERO_SN], 56, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &SonicActs[SN_JUMP_B], 56, MD_MTN_XSPD, 0, 0.25f, 0.5f },
};

NJS_TEXNAME HSONIC_TEXNAMES[4];
NJS_TEXLIST HSONIC_TEXLIST = { arrayptrandlength(HSONIC_TEXNAMES) };

NJS_MATRIX SonicMatrix;

void PlayVoice_Sonic(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(SonicSound_Win);
		break;
	}
}

void PlaySound_Sonic(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(SonicSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(SonicSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(SonicSound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(SonicSound_Death);
		break;
	}
}

void SonicCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = SonicMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(SonicMatrix); //pupils
	}
}

void SonicHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* sonicobj = HeroesChars[obj->Data1->CharIndex];
	if (!sonicobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)sonicobj->UnknownB_ptr;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (sonicobj->Data1->NextAction == 0) sonicobj->Data1->NextAction = 1;
			else sonicobj->Data1->NextAction = 0;
		}
	}
	else {
		sonicobj->Data1->NextAction = 0;
	}

	if (sonicobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HSONIC_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	if (sonicobj->Data1->Index == 52) {
		njRotateY(0, 0xC000);
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

	*NodeCallbackFuncPtr = SonicCallback;
	njActionWeight(actptr, mtn->nframe, SonicMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, SonicMatrix);
	NJS_OBJECT* pupils = SonicMdls[1]->getmodel();
	switch (sonicobj->Data1->InvulnerableTime) {
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
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void SonicHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->UnknownB_ptr;

	if (!playerco2 || playerdata->CharID != Characters_Sonic)
	{
		return;
	}

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-sonic", &HSONIC_TEXLIST }, 5, sonic_action_heroes);
		data->Action = 2;
		return;
	case 2:
		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(SonicSound_Ya);
				data->Action = 3;
				break;
			}

			if (playerco2->AnimationThing.Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayHeroesSound(SonicSound_Attack);
				data->Action = 4;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(SonicSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(SonicSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		SonicAnimConverter(&pwp_heroes->mm, Characters_HeroesSonic, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		KickTrick(data, data2, playerco2, playerdata);
		pwp_heroes->mm.reqaction = MTN_SPD_ATC;
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
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

void LoadSonicFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-sonic.arcx"));

	SonicMdls[0] = arc.GetModel("SONIC_LOCATOR.sa1mdl");
	SonicMdls[1] = arc.GetModel("SONIC_MABUTA.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(SonicMdls[0]->getweightinfo(), SonicMdls[0]->getmodel());

	SonicAnms[0] = arc.GetAnimation("SN_WALK.saanim");
	SonicAnms[1] = arc.GetAnimation("SN_WALK_PULL.saanim");
	SonicAnms[2] = arc.GetAnimation("SN_WALK_PUSH.saanim");
	SonicAnms[3] = arc.GetAnimation("SN_TURN_L.saanim");
	SonicAnms[4] = arc.GetAnimation("SN_TURN_R.saanim");
	SonicAnms[5] = arc.GetAnimation("SN_SLOW_RUN.saanim");
	SonicAnms[6] = arc.GetAnimation("SN_MID_RUN.saanim");
	SonicAnms[7] = arc.GetAnimation("SN_TOP_RUN.saanim");
	SonicAnms[8] = arc.GetAnimation("SN_START.saanim");
	SonicAnms[9] = arc.GetAnimation("SN_JUMP_WALL.saanim");
	SonicAnms[10] = arc.GetAnimation("SN_JUMP_TRIC_A.saanim");
	SonicAnms[11] = arc.GetAnimation("SN_JUMP_TRIC_B.saanim");
	SonicAnms[12] = arc.GetAnimation("SN_JUMP_TRIC_C.saanim");
	SonicAnms[13] = arc.GetAnimation("SN_JUMP_A.saanim");
	SonicAnms[14] = arc.GetAnimation("SN_JUMP_B.saanim");
	SonicAnms[15] = arc.GetAnimation("SN_JUMP_C.saanim");
	SonicAnms[16] = arc.GetAnimation("SN_JUMP_D.saanim");
	SonicAnms[17] = arc.GetAnimation("SN_JUMP_E.saanim");
	SonicAnms[18] = arc.GetAnimation("SN_JUMP_F.saanim");
	SonicAnms[19] = arc.GetAnimation("SN_JUMP_TRNGL.saanim");
	SonicAnms[20] = arc.GetAnimation("SN_JUMP_GLIND.saanim");
	SonicAnms[21] = arc.GetAnimation("SN_GLIND.saanim");
	SonicAnms[22] = arc.GetAnimation("SN_GLIND_BK.saanim");
	SonicAnms[23] = arc.GetAnimation("SN_GLIND_BK_L.saanim");
	SonicAnms[24] = arc.GetAnimation("SN_GLIND_BK_R.saanim");
	SonicAnms[25] = arc.GetAnimation("SN_GLIND_FLIP_B.saanim");
	SonicAnms[26] = arc.GetAnimation("SN_GLIND_FLIP_FR.saanim");
	SonicAnms[27] = arc.GetAnimation("SN_GLIND_L.saanim");
	SonicAnms[28] = arc.GetAnimation("SN_GLIND_R.saanim");
	SonicAnms[29] = arc.GetAnimation("SN_FLY_IDLE.saanim");
	SonicAnms[30] = arc.GetAnimation("SN_FLY_SLOW.saanim");
	SonicAnms[31] = arc.GetAnimation("SN_FLY_GLIND.saanim");
	SonicAnms[32] = arc.GetAnimation("SN_HANG_OFF.saanim");
	SonicAnms[33] = arc.GetAnimation("SN_HANG_ON.saanim");
	SonicAnms[34] = arc.GetAnimation("SN_BREAK_A.saanim");
	SonicAnms[35] = arc.GetAnimation("SN_BREAK_B.saanim");
	SonicAnms[36] = arc.GetAnimation("SN_BREAK_C.saanim");
	SonicAnms[37] = arc.GetAnimation("SN_BREAK_TURN_L.saanim");
	SonicAnms[38] = arc.GetAnimation("SN_BREAK_TURN_R.saanim");
	SonicAnms[39] = arc.GetAnimation("SN_BRA_MID.saanim");
	SonicAnms[40] = arc.GetAnimation("SN_BRA_TOP.saanim");
	SonicAnms[41] = arc.GetAnimation("SN_FLORT.saanim");
	SonicAnms[42] = arc.GetAnimation("SN_DAM_MID_A.saanim");
	SonicAnms[43] = arc.GetAnimation("SN_DAM_MID_B.saanim");
	SonicAnms[44] = arc.GetAnimation("SN_DAM_MID_C.saanim");
	SonicAnms[45] = arc.GetAnimation("SN_EDGE_OTTO_A.saanim");
	SonicAnms[46] = arc.GetAnimation("SN_EDGE_OTTO_B.saanim");
	SonicAnms[47] = arc.GetAnimation("SN_EDGE_OTTO_C.saanim");
	SonicAnms[48] = arc.GetAnimation("SN_FW_JUMP.saanim");
	SonicAnms[49] = arc.GetAnimation("SN_TRAP_JUMP.saanim");
	SonicAnms[50] = arc.GetAnimation("SN_POW_ROT.saanim");
	SonicAnms[51] = arc.GetAnimation("SN_WIN_B.saanim");
	SonicAnms[52] = arc.GetAnimation("SN_ATC_DASH.saanim");
	SonicAnms[53] = arc.GetAnimation("SN_IDLE.saanim");
	SonicAnms[54] = arc.GetAnimation("SN_IDLE_C.saanim");
	SonicAnms[55] = arc.GetAnimation("SN_IDLE_D.saanim");
	SonicAnms[56] = arc.GetAnimation("HERO_SN.saanim");

	for (int i = 0; i < LengthOfArray(SonicActs); ++i) {
		SonicActs[i].object = SonicMdls[0]->getmodel();
		SonicActs[i].motion = SonicAnms[i] ? SonicAnms[i]->getmotion() : NULL;
	}
}

void UnloadSonicFiles() {
	HelperFunctionsGlobal.Weights->DeInit(SonicMdls[0]->getweightinfo(), SonicMdls[0]->getmodel());
	FreeMDLFiles(SonicMdls, LengthOfArray(SonicMdls));
	FreeANMFiles(SonicAnms, LengthOfArray(SonicAnms));
}