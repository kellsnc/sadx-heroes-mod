#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	AM_WALK,
	AM_WALK_PULL,
	AM_WALK_PUSH,
	AM_TURN_L_HALF,
	AM_TURN_R_HALF,
	AM_SLOW_RUN,
	AM_MID_RUN,
	AM_TOP_RUN,
	AM_START,
	AM_JUMP_WALL,
	AM_JUMP_TRIC_A,
	AM_JUMP_TRIC_B,
	AM_JUMP_TRIC_C,
	AM_JUMP_A,
	AM_JUMP_B_HALF,
	AM_JUMP_C,
	AM_JUMP_D,
	AM_JUMP_E,
	AM_JUMP_F,
	AM_JUMP_UMBRELLA,
	AM_JUMP_GLIND,
	AM_GLIND,
	AM_GLIND_BK,
	AM_GLIND_BK_L,
	AM_GLIND_BK_R,
	AM_GLIND_FLIP_B,
	AM_GLIND_FLIP_F,
	AM_GLIND_L,
	AM_GLIND_R,
	AM_FLY_IDLE,
	AM_FLY_SLOW,
	AM_FLY_GLIND,
	AM_HANG_OFF,
	AM_HANG_ON,
	AM_BREAK_A,
	AM_BREAK_B,
	AM_BREAK_C,
	AM_BREAK_TURN_L,
	AM_BREAK_TURN_R,
	AM_BRA_MID,
	AM_BRA_TOP,
	AM_FLORT,
	AM_DAM_M_A,
	AM_DAM_M_B,
	AM_DAM_M_C,
	AM_EDGE_OTTO_A,
	AM_EDGE_OTTO_B,
	AM_EDGE_OTTO_C,
	AM_FW_JUMP,
	AM_TRAP_JUMP,
	AM_POW_ROT_HALF,
	AM_WIN,
	AM_ATC_HAM,
	AM_IDLE_HALF,
	AM_IDLE_B_HALF,
	AM_IDLE_C_HALF,
	ROSE_AM,
	AM_IDLE_POW_HALF,
	AM_IDLE_POW_SHAKE,
	AM_START_IDLE,
	AM_ATC_TRNADE,
};

ModelInfo* AmyMdls[3];
AnimationFile* AmyAnms[61];
NJS_ACTION AmyActs[61];

PL_ACTION amy_action_heroes[] = {
	{ &AmyActs[AM_WALK], 47, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_WALK_PULL], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_WALK_PUSH], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_TURN_L_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_TURN_R_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_SLOW_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &AmyActs[AM_MID_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &AmyActs[AM_TOP_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &AmyActs[AM_START], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_WALL], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_TRIC_A], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_TRIC_B], 47, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 0.5f },
	{ &AmyActs[AM_JUMP_TRIC_C], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_A], 47, MD_MTN_NEXT, MTN_SPD_JUMP_B, 0.25f, 0.5f },
	{ &AmyActs[AM_JUMP_B_HALF], 47, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &AmyActs[AM_JUMP_C], 47, MD_MTN_NEXT, MTN_SPD_JUMP_D, 0.25f, 0.5f },
	{ &AmyActs[AM_JUMP_D], 47, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &AmyActs[AM_JUMP_E], 47, MD_MTN_NEXT, MTN_SPD_JUMP_F, 0.25f, 0.5f },
	{ &AmyActs[AM_JUMP_F], 47, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &AmyActs[AM_JUMP_UMBRELLA], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_BK], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_BK_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_BK_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_FLIP_B], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_FLIP_F], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_L], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_GLIND_R], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_FLY_IDLE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_FLY_SLOW], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_FLY_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_HANG_OFF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_HANG_ON], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BREAK_A], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BREAK_B], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BREAK_C], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BREAK_TURN_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BREAK_TURN_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BRA_MID], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_BRA_TOP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_FLORT], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_DAM_M_A], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_DAM_M_B], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_DAM_M_C], 47, MD_MTN_NEXT, MTN_SPD_JUMP_E, 0.25f, 1.0f },
	{ &AmyActs[AM_EDGE_OTTO_A], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_EDGE_OTTO_B], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_EDGE_OTTO_C], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_FW_JUMP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_TRAP_JUMP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_POW_ROT_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_WIN], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_ATC_HAM], 47, MD_MTN_NEXT, MTN_SPD_IDLE, 0.1f, 1.0f },
	{ &AmyActs[AM_IDLE_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &AmyActs[AM_IDLE_B_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_IDLE_C_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[ROSE_AM], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_JUMP_B_HALF], 47, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &AmyActs[AM_IDLE_POW_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_IDLE_POW_SHAKE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_START_IDLE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &AmyActs[AM_ATC_TRNADE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

NJS_TEXNAME HAMY_TEXNAMES[4];
NJS_TEXLIST HAMY_TEXLIST = { arrayptrandlength(HAMY_TEXNAMES) };

NJS_MATRIX AmyMatrices[2];

void PlayVoice_Amy(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(AmySound_Win);
		break;
	}
}

void PlaySound_Amy(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(AmySound_Jump);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(AmySound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(AmySound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(AmySound_Death);
		break;
	}
}

void AmyCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = AmyMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(AmyMatrices[0]); //eyelashes
	}
	else if (object == base->getnode(6)) {
		njGetMatrix(AmyMatrices[1]); //hammer
	}
}

void AmyHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* amyobj = HeroesChars[obj->Data1->CharIndex];
	if (!amyobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)amyobj->UnknownB_ptr;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (amyobj->Data1->NextAction == 0) amyobj->Data1->NextAction = 1;
			else amyobj->Data1->NextAction = 0;
		}
	}
	else {
		amyobj->Data1->NextAction = 0;
	}

	if (amyobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HAMY_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	njRotateX(0, 0x4000);

	if (amyobj->Data1->Index == 52) {
		njTranslate(0, -10, 5, 0);
	}

	mtnjvwk* mtn = (mtnjvwk*)&pwp_heroes->mm;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = AmyCallback;
	njActionWeight(actptr, mtn->nframe, AmyMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, AmyMatrices[0]);
	NJS_CNK_OBJECT* pupils = AmyMdls[1]->getmodel();
	switch (amyobj->Data1->InvulnerableTime) {
	case 1:
	case 7:
		dsDrawModel(pupils->child->getbasicdxmodel());
		break;
	case 2:
	case 6:
		dsDrawModel(pupils->child->child->getbasicdxmodel());
		break;
	case 3:
	case 5:
		dsDrawModel(pupils->child->child->child->getbasicdxmodel());
		break;
	case 4:
		dsDrawModel(pupils->child->child->child->child->getbasicdxmodel());
		break;
	default:
		dsDrawModel(pupils->getbasicdxmodel());
		break;
	}
	
	if (action == MTN_SPD_ATC || action == MTN_AM_ATC_TRNADE) {
		njSetMatrix(NULL, AmyMatrices[1]);
		dsDrawObject(AmyMdls[2]->getmodel());
	}
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void AmyHeroes_Main(ObjectMaster *obj) {
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
		HeroesChars_InitPlayer((task*)obj, { "heroes-amy", &HAMY_TEXLIST }, 1, amy_action_heroes);
		data->Action = 2;
		return;
	case 2:
		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(AmySound_Ya);
				data->Action = 3;
				pwp_heroes->mm.reqaction = MTN_SPD_ATC;
				break;
			}

			if (playerco2->AnimationThing.Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayHeroesSound(AmySound_Attack);
				data->Action = 4;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(AmySound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(AmySound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		SonicAnimConverter(&pwp_heroes->mm, Characters_HeroesAmy, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		if (pwp_heroes->mm.reqaction == MTN_SPD_ATC) {
			playerco2->Powerups |= Powerups_Invincibility;
			NullifyVelocity(playerdata2, playerco2);
		}
		else {
			playerco2->Powerups &= ~Powerups_Invincibility;
			playerdata->Action = 2;
			data->Action = 2;
		}
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

void LoadAmyFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-amy.arcx"));

	AmyMdls[0] = arc.GetModel("AMY_LOCATOR.sa1mdl");
	AmyMdls[1] = arc.GetModel("AMY_MABUTA.sa1mdl");
	AmyMdls[2] = arc.GetModel("AMY_HAMMER.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(AmyMdls[0]->getweightinfo(), AmyMdls[0]->getmodel());

	AmyAnms[0] = arc.GetAnimation("AM_WALK.saanim");
	AmyAnms[1] = arc.GetAnimation("AM_WALK_PULL.saanim");
	AmyAnms[2] = arc.GetAnimation("AM_WALK_PUSH.saanim");
	AmyAnms[3] = arc.GetAnimation("AM_TURN_L_HALF.saanim");
	AmyAnms[4] = arc.GetAnimation("AM_TURN_R_HALF.saanim");
	AmyAnms[5] = arc.GetAnimation("AM_SLOW_RUN.saanim");
	AmyAnms[6] = arc.GetAnimation("AM_MID_RUN.saanim");
	AmyAnms[7] = arc.GetAnimation("AM_TOP_RUN.saanim");
	AmyAnms[8] = arc.GetAnimation("AM_START.saanim");
	AmyAnms[9] = arc.GetAnimation("AM_JUMP_WALL.saanim");
	AmyAnms[10] =  arc.GetAnimation("AM_JUMP_TRIC_A.saanim");
	AmyAnms[11] =  arc.GetAnimation("AM_JUMP_TRIC_B.saanim");
	AmyAnms[12] =  arc.GetAnimation("AM_JUMP_TRIC_C.saanim");
	AmyAnms[13] =  arc.GetAnimation("AM_JUMP_A.saanim");
	AmyAnms[14] =  arc.GetAnimation("AM_JUMP_B_HALF.saanim");
	AmyAnms[15] =  arc.GetAnimation("AM_JUMP_C.saanim");
	AmyAnms[16] =  arc.GetAnimation("AM_JUMP_D.saanim");
	AmyAnms[17] =  arc.GetAnimation("AM_JUMP_E.saanim");
	AmyAnms[18] =  arc.GetAnimation("AM_JUMP_F.saanim");
	AmyAnms[19] =  arc.GetAnimation("AM_JUMP_UMBRELLA.saanim");
	AmyAnms[20] =  arc.GetAnimation("AM_JUMP_GLIND.saanim");
	AmyAnms[21] =  arc.GetAnimation("AM_GLIND.saanim");
	AmyAnms[22] =  arc.GetAnimation("AM_GLIND_BK.saanim");
	AmyAnms[23] =  arc.GetAnimation("AM_GLIND_BK_L.saanim");
	AmyAnms[24] =  arc.GetAnimation("AM_GLIND_BK_R.saanim");
	AmyAnms[25] =  arc.GetAnimation("AM_GLIND_FLIP_B.saanim");
	AmyAnms[26] =  arc.GetAnimation("AM_GLIND_FLIP_F.saanim");
	AmyAnms[27] =  arc.GetAnimation("AM_GLIND_L.saanim");
	AmyAnms[28] =  arc.GetAnimation("AM_GLIND_R.saanim");
	AmyAnms[29] =  arc.GetAnimation("AM_FLY_IDLE.saanim");
	AmyAnms[30] =  arc.GetAnimation("AM_FLY_SLOW.saanim");
	AmyAnms[31] =  arc.GetAnimation("AM_FLY_GLIND.saanim");
	AmyAnms[32] =  arc.GetAnimation("AM_HANG_OFF.saanim");
	AmyAnms[33] =  arc.GetAnimation("AM_HANG_ON.saanim");
	AmyAnms[34] =  arc.GetAnimation("AM_BREAK_A.saanim");
	AmyAnms[35] =  arc.GetAnimation("AM_BREAK_B.saanim");
	AmyAnms[36] =  arc.GetAnimation("AM_BREAK_C.saanim");
	AmyAnms[37] =  arc.GetAnimation("AM_BREAK_TURN_L.saanim");
	AmyAnms[38] =  arc.GetAnimation("AM_BREAK_TURN_R.saanim");
	AmyAnms[39] =  arc.GetAnimation("AM_BRA_MID.saanim");
	AmyAnms[40] =  arc.GetAnimation("AM_BRA_TOP.saanim");
	AmyAnms[41] =  arc.GetAnimation("AM_FLORT.saanim");
	AmyAnms[42] =  arc.GetAnimation("AM_DAM_M_A.saanim");
	AmyAnms[43] =  arc.GetAnimation("AM_DAM_M_B.saanim");
	AmyAnms[44] =  arc.GetAnimation("AM_DAM_M_C.saanim");
	AmyAnms[45] =  arc.GetAnimation("AM_EDGE_OTTO_A.saanim");
	AmyAnms[46] =  arc.GetAnimation("AM_EDGE_OTTO_B.saanim");
	AmyAnms[47] =  arc.GetAnimation("AM_EDGE_OTTO_C.saanim");
	AmyAnms[48] = arc.GetAnimation("AM_FW_JUMP.saanim");
	AmyAnms[49] = arc.GetAnimation("AM_TRAP_JUMP.saanim");
	AmyAnms[50] = arc.GetAnimation("AM_POW_ROT_HALF.saanim");
	AmyAnms[51] = arc.GetAnimation("AM_WIN.saanim");
	AmyAnms[52] = arc.GetAnimation("AM_ATC_HAM.saanim");
	AmyAnms[53] = arc.GetAnimation("AM_IDLE_HALF.saanim");
	AmyAnms[54] = arc.GetAnimation("AM_IDLE_B_HALF.saanim");
	AmyAnms[55] = arc.GetAnimation("AM_IDLE_C_HALF.saanim");
	AmyAnms[56] = arc.GetAnimation("ROSE_AM.saanim");
	AmyAnms[57] = arc.GetAnimation("AM_IDLE_POW_HALF.saanim");
	AmyAnms[58] = arc.GetAnimation("AM_IDLE_POW_SHAKE.saanim");
	AmyAnms[59] = arc.GetAnimation("AM_START_IDLE.saanim");
	AmyAnms[60] = arc.GetAnimation("AM_ATC_TRNADE.saanim");

	for (int i = 0; i < LengthOfArray(AmyActs); ++i) {
		AmyActs[i].object = AmyMdls[0]->getmodel();
		AmyActs[i].motion = AmyAnms[i] ? AmyAnms[i]->getmotion() : NULL;
	}
}

void UnloadAmyFiles() {
	HelperFunctionsGlobal.Weights->DeInit(AmyMdls[0]->getweightinfo(), AmyMdls[0]->getmodel());
	FreeMDLFiles(AmyMdls, LengthOfArray(AmyMdls));
	FreeANMFiles(AmyAnms, LengthOfArray(AmyAnms));
}