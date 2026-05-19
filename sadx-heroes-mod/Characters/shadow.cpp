#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	SH_WALK,
	SH_WALK_PULL,
	SH_WALK_PUSH,
	SH_TURN_L,
	SH_TURN_R,
	SH_SLOW_RUN,
	SH_SLOW_SKATE,
	SH_MID_SKATE,
	SH_START,
	SH_JUMP_WALL,
	SH_JUMP_TRIC_A,
	SH_JUMP_TRIC_B,
	SH_JUMP_TRIC_C,
	SH_JUMP_A,
	SH_JUMP_B,
	SH_JUMP_C,
	SH_JUMP_D,
	SH_JUMP_E,
	SH_JUMP_F,
	SH_JUMP_TRNGL,
	SH_JUMP_GLIND,
	SH_GLIND,
	SH_GLIND_BK,
	SH_GLIND_BK_L,
	SH_GLIND_BK_R,
	SH_GLIND_FLIP_B,
	SH_GLIND_FLIP_FR,
	SH_GLIND_L,
	SH_GLIND_R,
	SH_FLY_IDLE,
	SH_FLY_SLOW,
	SH_FLY_GLIND,
	SH_HANG_OFF,
	SH_HANG_ON,
	SH_BREAK_A,
	SH_BREAK_B,
	SH_BREAK_C,
	SH_BREAK_TURN_L,
	SH_BREAK_TURN_R,
	SH_BRA_MID,
	SH_BRA_TOP,
	SH_FLORT,
	SH_DAM_M_A,
	SH_DAM_M_B,
	SH_DAM_M_C,
	SH_EDGE_OTTO_A,
	SH_EDGE_OTTO_B,
	SH_EDGE_OTTO_C,
	SH_FW_JUMP,
	SH_TRAP_JUMP,
	SH_POW_ROT,
	SH_WIN,
	SH_ATC_DASH,
	SH_IDLE_HALF,
	SH_IDLE_B_HALF,
	SH_IDLE_D_HALF,
	DARK_SH,
	SH_TOP_SKATE,
};

ModelInfo* ShadowMdls[3];
AnimationFile* ShadowAnms[58];
NJS_ACTION ShadowActs[58];

PL_ACTION shadow_action_heroes[] = {
	{ &ShadowActs[SH_WALK], 53, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_WALK_PULL], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_WALK_PUSH], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_TURN_L], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_TURN_R], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_SLOW_RUN], 53, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &ShadowActs[SH_SLOW_SKATE], 53, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &ShadowActs[SH_MID_SKATE], 53, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &ShadowActs[SH_START], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_WALL], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_TRIC_A], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_TRIC_B], 53, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 0.5f },
	{ &ShadowActs[SH_JUMP_TRIC_C], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_A], 53, MD_MTN_NEXT, MTN_SPD_JUMP_B, 0.25f, 0.5f },
	{ &ShadowActs[SH_JUMP_B], 53, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &ShadowActs[SH_JUMP_C], 53, MD_MTN_NEXT, MTN_SPD_JUMP_D, 0.25f, 0.5f },
	{ &ShadowActs[SH_JUMP_D], 53, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &ShadowActs[SH_JUMP_E], 53, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 0.5f },
	{ &ShadowActs[SH_JUMP_F], 53, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &ShadowActs[SH_JUMP_TRNGL], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_GLIND], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_BK], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_BK_L], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_BK_R], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_FLIP_B], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_FLIP_FR], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_L], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_GLIND_R], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_FLY_IDLE], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_FLY_SLOW], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_FLY_GLIND], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_HANG_OFF], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_HANG_ON], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BREAK_A], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BREAK_B], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BREAK_C], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BREAK_TURN_L], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BREAK_TURN_R], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BRA_MID], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_BRA_TOP], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_FLORT], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_DAM_M_A], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_DAM_M_B], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_DAM_M_C], 53, MD_MTN_NEXT, SH_JUMP_E, 0.25f, 1.0f },
	{ &ShadowActs[SH_EDGE_OTTO_A], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_EDGE_OTTO_B], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_EDGE_OTTO_C], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_FW_JUMP], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_TRAP_JUMP], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_POW_ROT], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_WIN], 53, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_ATC_DASH], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_IDLE_HALF], 53, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &ShadowActs[SH_IDLE_B_HALF], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_IDLE_D_HALF], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[DARK_SH], 53, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &ShadowActs[SH_JUMP_B], 53, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &ShadowActs[SH_TOP_SKATE], 53, MD_MTN_XSPD, 0, 0.25f, 0.1f },
};

NJS_TEXNAME SHADOW_TEXNAMES[4];
NJS_TEXLIST SHADOW_TEXLIST = { arrayptrandlength(SHADOW_TEXNAMES) };

NJS_MATRIX ShadowMatrices[5];

void PlayVoice_Shadow(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(ShadowSound_Win);
		break;
	}
}

void PlaySound_Shadow(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(ShadowSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(ShadowSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(ShadowSound_Hurt1);
		break;
	case 1503:
		PlayHeroesSound(ShadowSound_Death);
		break;
	}
}

void ShadowCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = ShadowMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(ShadowMatrices[0]); //pupils
	}
	else if (object == base->getnode(52)) {
		njGetMatrix(ShadowMatrices[1]); // jet
	}
	else if (object == base->getnode(53)) {
		njGetMatrix(ShadowMatrices[2]); // jet
	}
	else if (object == base->getnode(48)) {
		njGetMatrix(ShadowMatrices[3]); // jet
	}
	else if (object == base->getnode(49)) {
		njGetMatrix(ShadowMatrices[4]); // jet
	}
}

void ShadowHeroes_Display(ObjectMaster *obj) {
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

	njSetTexture(&SHADOW_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	njRotateX(0, 0x4000);

	mtnjvwk* mtn = &pwp_heroes->mm;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = ShadowCallback;
	njActionWeight(actptr, mtn->nframe, ShadowMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, ShadowMatrices[0]);
	NJS_OBJECT* pupils = ShadowMdls[1]->getmodel();
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

	if (action == MTN_SPD_MID_RUN || action == MTN_SPD_TOP_RUN || action == MTN_SH_TOP_SKATE || action == MTN_SPD_FW_JUMP) {
		for (int i = 0; i < 4; ++i) {
			njSetMatrix(0, ShadowMatrices[i + 1]);
			njRotateX(0, 0x8000);
			late_DrawObject(ShadowMdls[2]->getmodel()->getnode((i == 0 || i == 2) ? 2 : 4), LATE_MAT);
		}
	}
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void ShadowHeroes_Main(ObjectMaster *obj) {
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
		HeroesChars_InitPlayer((task*)obj, { "heroes-shadow", &SHADOW_TEXLIST }, 3, shadow_action_heroes);
		data->Action = 2;
		return;
	case 2:
		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(ShadowSound_Ya);
				data->Action = 3;
				break;
			}

			if (playerco2->AnimationThing.Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayHeroesSound(ShadowSound_Attack);
				data->Action = 4;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(ShadowSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(ShadowSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		SonicAnimConverter(&pwp_heroes->mm, HeroesChars_Shadow, (taskwk*)playerdata, (playerwk*)playerco2);

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

void LoadShadowFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-shadow.arcx"));

	ShadowMdls[0] = arc.GetModel("SHADOW_LOCATOR.sa1mdl");
	ShadowMdls[1] = arc.GetModel("SHADOW_MABUTA.sa1mdl");
	ShadowMdls[2] = arc.GetModel("SHADOW_JET.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(ShadowMdls[0]->getweightinfo(), ShadowMdls[0]->getmodel());

	ShadowAnms[0] = arc.GetAnimation("SH_WALK.saanim");
	ShadowAnms[1] = arc.GetAnimation("SH_WALK_PULL.saanim");
	ShadowAnms[2] = arc.GetAnimation("SH_WALK_PUSH.saanim");
	ShadowAnms[3] = arc.GetAnimation("SH_TURN_L.saanim");
	ShadowAnms[4] = arc.GetAnimation("SH_TURN_R.saanim");
	ShadowAnms[5] = arc.GetAnimation("SH_SLOW_RUN.saanim");
	ShadowAnms[6] = arc.GetAnimation("SH_SLOW_SKATE.saanim");
	ShadowAnms[7] = arc.GetAnimation("SH_MID_SKATE.saanim");
	ShadowAnms[8] = arc.GetAnimation("SH_START.saanim");
	ShadowAnms[9] = arc.GetAnimation("SH_JUMP_WALL.saanim");
	ShadowAnms[10] = arc.GetAnimation("SH_JUMP_TRIC_A.saanim");
	ShadowAnms[11] = arc.GetAnimation("SH_JUMP_TRIC_B.saanim");
	ShadowAnms[12] = arc.GetAnimation("SH_JUMP_TRIC_C.saanim");
	ShadowAnms[13] = arc.GetAnimation("SH_JUMP_A.saanim");
	ShadowAnms[14] = arc.GetAnimation("SH_JUMP_B.saanim");
	ShadowAnms[15] = arc.GetAnimation("SH_JUMP_C.saanim");
	ShadowAnms[16] = arc.GetAnimation("SH_JUMP_D.saanim");
	ShadowAnms[17] = arc.GetAnimation("SH_JUMP_E.saanim");
	ShadowAnms[18] = arc.GetAnimation("SH_JUMP_F.saanim");
	ShadowAnms[19] = arc.GetAnimation("SH_JUMP_TRNGL.saanim");
	ShadowAnms[20] = arc.GetAnimation("SH_JUMP_GLIND.saanim");
	ShadowAnms[21] = arc.GetAnimation("SH_GLIND.saanim");
	ShadowAnms[22] = arc.GetAnimation("SH_GLIND_BK.saanim");
	ShadowAnms[23] = arc.GetAnimation("SH_GLIND_BK_L.saanim");
	ShadowAnms[24] = arc.GetAnimation("SH_GLIND_BK_R.saanim");
	ShadowAnms[25] = arc.GetAnimation("SH_GLIND_FLIP_B.saanim");
	ShadowAnms[26] = arc.GetAnimation("SH_GLIND_FLIP_FR.saanim");
	ShadowAnms[27] = arc.GetAnimation("SH_GLIND_L.saanim");
	ShadowAnms[28] = arc.GetAnimation("SH_GLIND_R.saanim");
	ShadowAnms[29] = arc.GetAnimation("SH_FLY_IDLE.saanim");
	ShadowAnms[30] = arc.GetAnimation("SH_FLY_SLOW.saanim");
	ShadowAnms[31] = arc.GetAnimation("SH_FLY_GLIND.saanim");
	ShadowAnms[32] = arc.GetAnimation("SH_HANG_OFF.saanim");
	ShadowAnms[33] = arc.GetAnimation("SH_HANG_ON.saanim");
	ShadowAnms[34] = arc.GetAnimation("SH_BREAK_A.saanim");
	ShadowAnms[35] = arc.GetAnimation("SH_BREAK_B.saanim");
	ShadowAnms[36] = arc.GetAnimation("SH_BREAK_C.saanim");
	ShadowAnms[37] = arc.GetAnimation("SH_BREAK_TURN_L.saanim");
	ShadowAnms[38] = arc.GetAnimation("SH_BREAK_TURN_R.saanim");
	ShadowAnms[39] = arc.GetAnimation("SH_BRA_MID.saanim");
	ShadowAnms[40] = arc.GetAnimation("SH_BRA_TOP.saanim");
	ShadowAnms[41] = arc.GetAnimation("SH_FLORT.saanim");
	ShadowAnms[42] = arc.GetAnimation("SH_DAM_M_A.saanim");
	ShadowAnms[43] = arc.GetAnimation("SH_DAM_M_B.saanim");
	ShadowAnms[44] = arc.GetAnimation("SH_DAM_M_C.saanim");
	ShadowAnms[45] = arc.GetAnimation("SH_EDGE_OTTO_A.saanim");
	ShadowAnms[46] = arc.GetAnimation("SH_EDGE_OTTO_B.saanim");
	ShadowAnms[47] = arc.GetAnimation("SH_EDGE_OTTO_C.saanim");
	ShadowAnms[48] = arc.GetAnimation("SH_FW_JUMP.saanim");
	ShadowAnms[49] = arc.GetAnimation("SH_TRAP_JUMP.saanim");
	ShadowAnms[50] = arc.GetAnimation("SH_POW_ROT.saanim");
	ShadowAnms[51] = arc.GetAnimation("SH_WIN.saanim");
	ShadowAnms[52] = arc.GetAnimation("SH_ATC_DASH.saanim");
	ShadowAnms[53] = arc.GetAnimation("SH_IDLE_HALF.saanim");
	ShadowAnms[54] = arc.GetAnimation("SH_IDLE_B_HALF.saanim");
	ShadowAnms[55] = arc.GetAnimation("SH_IDLE_D_HALF.saanim");
	ShadowAnms[56] = arc.GetAnimation("DARK_SH.saanim");
	ShadowAnms[57] = arc.GetAnimation("SH_TOP_SKATE.saanim");

	for (int i = 0; i < LengthOfArray(ShadowActs); ++i) {
		ShadowActs[i].object = ShadowMdls[0]->getmodel();
		ShadowActs[i].motion = ShadowAnms[i] ? ShadowAnms[i]->getmotion() : NULL;
	}
}

void UnloadShadowFiles() {
	HelperFunctionsGlobal.Weights->DeInit(ShadowMdls[0]->getweightinfo(), ShadowMdls[0]->getmodel());
	FreeMDLFiles(ShadowMdls, LengthOfArray(ShadowMdls));
	FreeANMFiles(ShadowAnms, LengthOfArray(ShadowAnms));
}