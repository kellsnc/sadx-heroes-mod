#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	VE_WALK,
	VE_WALK_PULL,
	VE_WALK_PUSH,
	VE_TURN_L,
	VE_TURN_R,
	VE_SLOW_RUN,
	VE_MID_RUN,
	VE_TOP_RUN,
	VE_START,
	VE_JUMP_ATC,
	VE_ATC_DOWN,
	VE_ATC_A,
	VE_ATC_B,
	VE_JUMP_A,
	VE_JUMP_B,
	VE_JUMP_C,
	VE_JUMP_D,
	VE_JUMP_E,
	VE_JUMP_F,
	VE_GUM,
	VE_JUMP_GLIND,
	VE_GLIND,
	VE_GLIND_BK,
	VE_GLIND_BK_L,
	VE_GLIND_BK_R,
	VE_GLIND_FLIP_B,
	VE_GLIND_FLIP_F,
	VE_GLIND_L,
	VE_GLIND_R,
	VE_FLY_IDLE,
	VE_FLY_SLOW,
	VE_FLY_GLIND,
	VE_HANG_OFF,
	VE_HANG_ON,
	VE_BREAK_A,
	VE_BREAK_B,
	VE_BREAK_C,
	VE_BREAK_TURN_L,
	VE_BREAK_TURN_R,
	VE_BRA_MID,
	VE_BRA_TOP,
	VE_FLORT,
	VE_DAM_M_A,
	VE_DAM_M_B,
	VE_DAM_M_C,
	VE_EDGE_OTTO_A,
	VE_EDGE_OTTO_B,
	VE_EDGE_OTTO_C,
	VE_FW_JUMP,
	VE_TRAP_JUMP,
	CAO_VE,
	VE_WIN,
	VE_IDLE_HALF,
	VE_IDLE_B_HALF,
	VE_IDLE_C_HALF,
	VE_BOB,
	VE_BOB_L,
	VE_BOB_R,
	VE_START_IDLE,
	VE_GLIND_DASH,
	VE_GLIND_BK_DASH,
	CAO_MIC,
};

ModelInfo* VectorMdls[3];
AnimationFile* VectorAnms[63];
NJS_ACTION VectorActs[63];

PL_ACTION vector_action_heroes[] = {
	{ &VectorActs[VE_WALK], 50, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_WALK_PULL], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_WALK_PUSH], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_TURN_L], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_TURN_R], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_SLOW_RUN], 50, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &VectorActs[VE_MID_RUN], 50, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &VectorActs[VE_TOP_RUN], 50, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &VectorActs[VE_START], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_JUMP_ATC], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_ATC_DOWN], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_ATC_A], 50, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &VectorActs[VE_ATC_B], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_JUMP_A], 50, MD_MTN_NEXT, MTN_POW_JUMP_B, 0.25f, 0.5f },
	{ &VectorActs[VE_JUMP_B], 50, MD_MTN_WORK, 0, 1.0f, 0.25f },
	{ &VectorActs[VE_JUMP_C], 50, MD_MTN_NEXT, MTN_POW_JUMP_D, 0.25f, 0.5f },
	{ &VectorActs[VE_JUMP_D], 50, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &VectorActs[VE_JUMP_E], 50, MD_MTN_NEXT, MTN_POW_JUMP_F, 0.25f, 0.5f },
	{ &VectorActs[VE_JUMP_F], 50, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &VectorActs[VE_GUM], 50, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &VectorActs[VE_JUMP_GLIND], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_BK], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_BK_L], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_BK_R], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_FLIP_B], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_FLIP_F], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_L], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_R], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_FLY_IDLE], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_FLY_SLOW], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_FLY_GLIND], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_HANG_OFF], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_HANG_ON], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BREAK_A], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BREAK_B], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BREAK_C], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BREAK_TURN_L], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BREAK_TURN_R], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BRA_MID], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BRA_TOP], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_FLORT], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_DAM_M_A], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_DAM_M_B], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_DAM_M_C], 50, MD_MTN_NEXT, MTN_POW_JUMP_E, 0.25f, 1.0f },
	{ &VectorActs[VE_EDGE_OTTO_A], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_EDGE_OTTO_B], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_EDGE_OTTO_C], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_FW_JUMP], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_TRAP_JUMP], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[CAO_VE], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_WIN], 50, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_ATC_A], 50, MD_MTN_NEXT, MTN_SPD_IDLE, 0.1f, 1.0f },
	{ &VectorActs[VE_IDLE_HALF], 50, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &VectorActs[VE_IDLE_B_HALF], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_IDLE_C_HALF], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BOB], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BOB_L], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_BOB_R], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_JUMP_B], 50, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &VectorActs[VE_START_IDLE], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_DASH], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[VE_GLIND_BK_DASH], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &VectorActs[CAO_MIC], 50, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

NJS_TEXNAME VECTOR_TEXNAMES[7];
NJS_TEXLIST VECTOR_TEXLIST = { arrayptrandlength(VECTOR_TEXNAMES) };

NJS_MATRIX VectorMatrices[2];

void PlayVoice_Vector(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(VectorSound_Win);
		break;
	}
}

void PlaySound_Vector(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(VectorSound_Attack);
		break;
	case 1232:
		PlayHeroesSound(VectorSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(VectorSound_Hurt1);
		break;
	case 1503:
		PlayHeroesSound(VectorSound_Death);
		break;
	}
}

void VectorCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = VectorMdls[0]->getmodel();

	if (object == base->getnode(32)) {
		njGetMatrix(VectorMatrices[0]); //eyelids
	}
	else if (object == base->getnode(36)) {
		njGetMatrix(VectorMatrices[1]); //gum
	}
}

void VectorHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* knucklesobj = HeroesChars[obj->Data1->CharIndex];
	if (!knucklesobj) return;
	EntityData2* data2 = (EntityData2*)knucklesobj->Data2;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)knucklesobj->UnknownB_ptr;

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

	njSetTexture(&VECTOR_TEXLIST);

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

	mtnjvwk* mtn = &pwp_heroes->mm;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = VectorCallback;
	njActionWeight(actptr, mtn->nframe, VectorMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, VectorMatrices[0]);
	NJS_CNK_OBJECT* pupils = VectorMdls[1]->getmodel();
	switch (knucklesobj->Data1->InvulnerableTime) {
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

	if (entity1->Action == 11) {
		njSetMatrix(NULL, VectorMatrices[1]);
		njTranslate(0, 0.0f, -7.0f, 2.0f);
		dsDrawModel(VectorMdls[2]->getmodel()->getbasicdxmodel());
	}
	
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void VectorHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];
	EntityData2* playerdata2 = EntityData2Ptrs[data->CharIndex];
	CharObj2* playerco2 = CharObj2Ptrs[data->CharIndex];
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)obj->UnknownB_ptr;

	if (!playerco2 || playerdata->CharID != Characters_Knuckles)
	{
		return;
	}

	switch (data->Action)
	{
	case 0:
		HeroesChars_InitPlayer((task*)obj, { "heroes-vector", &VECTOR_TEXLIST }, 6, vector_action_heroes);
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
				data->Scale.y = 0;
				PlayHeroesSound(VectorSound_Attack);
				data->Action = 4;
			}
		}
		
		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(VectorSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(VectorSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		KnucklesAnimConverter(&pwp_heroes->mm, Characters_Vector, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		switch (PowerLaunchTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayHeroesSound(VectorSound_Combo1);
			pwp_heroes->mm.reqaction = 11;
			break;
		case 2:
			PlayHeroesSound(VectorSound_Combo2);
			pwp_heroes->mm.reqaction = 9;
			break;
		case 3:
			PlayHeroesSound(VectorSound_Combo3);
			playerco2->Speed.x = 1;
		case 4:
			playerco2->Speed.x = 0;

			ExploseEnemies(&playerdata->Position, 5);

			if (data->Scale.x > 86) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				data->Action = 2;
			}

			pwp_heroes->mm.reqaction = 12;
			break;
		}

		break;
	case 4:
		if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 30) {
			if (data->field_A != 2) PlayHeroesSound(CommonSound_FlyPunchHit);
			data->field_A = 2;
		}
		else {
			if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayHeroesSound(VectorSound_Trick);
		}

		if (data->field_A != 2) {
			if (data2->field_30 == 0) {
				if (data->Scale.y != 10) data->Scale.y += 0.25f;
				else data2->field_30 = 1;
			}
			else if (data2->field_30 == 1) {
				if (data->Scale.y != 0) data->Scale.y -= 0.25f;
				else data2->field_30 = 0;
			}
			data->Scale.x = data->Scale.y;
		}
		else {
			if (data->Scale.x > 44) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;

				ExploseEnemies(&playerdata->Position, 3);
				data->Action = 2;
			}

		}

		pwp_heroes->mm.reqaction = 10;
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

void LoadVectorFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-vector.arcx"));

	VectorMdls[0] = arc.GetModel("VECTOR_LOCATOR.sa1mdl");
	VectorMdls[1] = arc.GetModel("VE_MABUTA.sa1mdl");
	VectorMdls[2] = arc.GetModel("GUM_LOCATOR.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(VectorMdls[0]->getweightinfo(), VectorMdls[0]->getmodel());

	VectorAnms[0] = arc.GetAnimation("VE_WALK.saanim");
	VectorAnms[1] = arc.GetAnimation("VE_WALK_PULL.saanim");
	VectorAnms[2] = arc.GetAnimation("VE_WALK_PUSH.saanim");
	VectorAnms[3] = arc.GetAnimation("VE_TURN_L.saanim");
	VectorAnms[4] = arc.GetAnimation("VE_TURN_R.saanim");
	VectorAnms[5] = arc.GetAnimation("VE_SLOW_RUN.saanim");
	VectorAnms[6] = arc.GetAnimation("VE_MID_RUN.saanim");
	VectorAnms[7] = arc.GetAnimation("VE_TOP_RUN.saanim");
	VectorAnms[8] = arc.GetAnimation("VE_START.saanim");
	VectorAnms[9] = arc.GetAnimation("VE_JUMP_ATC.saanim");
	VectorAnms[10] = arc.GetAnimation("VE_ATC_DOWN.saanim");
	VectorAnms[11] = arc.GetAnimation("VE_ATC_A.saanim");
	VectorAnms[12] = arc.GetAnimation("VE_ATC_B.saanim");
	VectorAnms[13] = arc.GetAnimation("VE_JUMP_A.saanim");
	VectorAnms[14] = arc.GetAnimation("VE_JUMP_B.saanim");
	VectorAnms[15] = arc.GetAnimation("VE_JUMP_C.saanim");
	VectorAnms[16] = arc.GetAnimation("VE_JUMP_D.saanim");
	VectorAnms[17] = arc.GetAnimation("VE_JUMP_E.saanim");
	VectorAnms[18] = arc.GetAnimation("VE_JUMP_F.saanim");
	VectorAnms[19] = arc.GetAnimation("VE_GUM.saanim");
	VectorAnms[20] = arc.GetAnimation("VE_JUMP_GLIND.saanim");
	VectorAnms[21] = arc.GetAnimation("VE_GLIND.saanim");
	VectorAnms[22] = arc.GetAnimation("VE_GLIND_BK.saanim");
	VectorAnms[23] = arc.GetAnimation("VE_GLIND_BK_L.saanim");
	VectorAnms[24] = arc.GetAnimation("VE_GLIND_BK_R.saanim");
	VectorAnms[25] = arc.GetAnimation("VE_GLIND_FLIP_B.saanim");
	VectorAnms[26] = arc.GetAnimation("VE_GLIND_FLIP_F.saanim");
	VectorAnms[27] = arc.GetAnimation("VE_GLIND_L.saanim");
	VectorAnms[28] = arc.GetAnimation("VE_GLIND_R.saanim");
	VectorAnms[29] = arc.GetAnimation("VE_FLY_IDLE.saanim");
	VectorAnms[30] = arc.GetAnimation("VE_FLY_SLOW.saanim");
	VectorAnms[31] = arc.GetAnimation("VE_FLY_GLIND.saanim");
	VectorAnms[32] = arc.GetAnimation("VE_HANG_OFF.saanim");
	VectorAnms[33] = arc.GetAnimation("VE_HANG_ON.saanim");
	VectorAnms[34] = arc.GetAnimation("VE_BREAK_A.saanim");
	VectorAnms[35] = arc.GetAnimation("VE_BREAK_B.saanim");
	VectorAnms[36] = arc.GetAnimation("VE_BREAK_C.saanim");
	VectorAnms[37] = arc.GetAnimation("VE_BREAK_TURN_L.saanim");
	VectorAnms[38] = arc.GetAnimation("VE_BREAK_TURN_R.saanim");
	VectorAnms[39] = arc.GetAnimation("VE_BRA_MID.saanim");
	VectorAnms[40] = arc.GetAnimation("VE_BRA_TOP.saanim");
	VectorAnms[41] = arc.GetAnimation("VE_FLORT.saanim");
	VectorAnms[42] = arc.GetAnimation("VE_DAM_M_A.saanim");
	VectorAnms[43] = arc.GetAnimation("VE_DAM_M_B.saanim");
	VectorAnms[44] = arc.GetAnimation("VE_DAM_M_C.saanim");
	VectorAnms[45] = arc.GetAnimation("VE_EDGE_OTTO_A.saanim");
	VectorAnms[46] = arc.GetAnimation("VE_EDGE_OTTO_B.saanim");
	VectorAnms[47] = arc.GetAnimation("VE_EDGE_OTTO_C.saanim");
	VectorAnms[48] = arc.GetAnimation("VE_FW_JUMP.saanim");
	VectorAnms[49] = arc.GetAnimation("VE_TRAP_JUMP.saanim");
	VectorAnms[50] = arc.GetAnimation("CAO_VE.saanim");
	VectorAnms[51] = arc.GetAnimation("VE_WIN.saanim");
	VectorAnms[52] = arc.GetAnimation("VE_IDLE_HALF.saanim");
	VectorAnms[53] = arc.GetAnimation("VE_IDLE_B_HALF.saanim");
	VectorAnms[54] = arc.GetAnimation("VE_IDLE_C_HALF.saanim");
	VectorAnms[55] = arc.GetAnimation("VE_BOB.saanim");
	VectorAnms[56] = arc.GetAnimation("VE_BOB_L.saanim");
	VectorAnms[57] = arc.GetAnimation("VE_BOB_R.saanim");
	VectorAnms[58] = arc.GetAnimation("VE_START_IDLE.saanim");
	VectorAnms[59] = arc.GetAnimation("VE_GLIND_DASH.saanim");
	VectorAnms[60] = arc.GetAnimation("VE_GLIND_BK_DASH.saanim");
	VectorAnms[61] = arc.GetAnimation("CAO_MIC.saanim");
	
	for (int i = 0; i < LengthOfArray(VectorActs); ++i) {
		VectorActs[i].object = VectorMdls[0]->getmodel();
		VectorActs[i].motion = VectorAnms[i] ? VectorAnms[i]->getmotion() : NULL;
	}
}

void UnloadVectorFiles() {
	HelperFunctionsGlobal.Weights->DeInit(VectorMdls[0]->getweightinfo(), VectorMdls[0]->getmodel());
	FreeMDLFiles(VectorMdls, LengthOfArray(VectorMdls));
	FreeANMFiles(VectorAnms, LengthOfArray(VectorAnms));
}