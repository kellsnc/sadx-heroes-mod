#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	OM_WALK,
	OM_WALK_PULL,
	OM_WALK_PUSH,
	OM_TURN_L,
	OM_TURN_R,
	OM_SLOW_RUN,
	OM_MID_RUN,
	OM_TOP_RUN,
	OM_START,
	OM_ATC_GUN,
	OM_ATC_A,
	OM_ATC_A2,
	OM_ATC_A3,
	OM_JUMP_A,
	OM_JUMP_B,
	OM_JUMP_C,
	OM_JUMP_D,
	OM_JUMP_E,
	OM_JUMP_F,
	OM_JUMP_TRNGL,
	OM_JUMP_GLIND,
	OM_GLIND,
	OM_GLIND_BK,
	OM_GLIND_BK_L,
	OM_GLIND_BK_R,
	OM_GLIND_FLIP_B,
	OM_GLIND_FLIP_F,
	OM_GLIND_L,
	OM_GLIND_R,
	OM_FLY_IDLE,
	OM_FLY_SLOW,
	OM_FLY_GLIND,
	OM_HANG_OFF,
	OM_HANG_ON,
	OM_BREAK_A,
	OM_BREAK_B,
	OM_BREAK_C,
	OM_BREAK_TURN_L,
	OM_BREAK_TURN_R,
	OM_BRA_MID,
	OM_BRA_TOP,
	OM_FLORT,
	OM_DAM_M_A,
	OM_DAM_M_B,
	OM_DAM_M_C,
	OM_EDGE_OTTO_A,
	OM_EDGE_OTTO_B,
	OM_EDGE_OTTO_C,
	OM_FW_JUMP,
	OM_TRAP_JUMP,
	DARK_OM,
	OM_WIN,
	OM_IDLE,
	OM_IDLE_C,
	OM_IDLE_D,
	OM_BOB,
	OM_BOB_L,
	OM_BOB_R,
};

ModelInfo* OmegaMdls[5];
AnimationFile* OmegaAnms[60];
NJS_ACTION OmegaActs[60];

PL_ACTION omega_action_heroes[] = {
	{ &OmegaActs[OM_WALK], 57, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_WALK_PULL], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_WALK_PUSH], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_TURN_L], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_TURN_R], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_SLOW_RUN], 57, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &OmegaActs[OM_MID_RUN], 57, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &OmegaActs[OM_TOP_RUN], 57, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &OmegaActs[OM_START], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_ATC_GUN], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_ATC_A], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_ATC_A], 57, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &OmegaActs[OM_ATC_A], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_JUMP_A], 57, MD_MTN_NEXT, MTN_POW_JUMP_B, 0.25f, 0.5f },
	{ &OmegaActs[OM_JUMP_B], 57, MD_MTN_WORK, 0, 1.0f, 0.25f },
	{ &OmegaActs[OM_JUMP_C], 57, MD_MTN_NEXT, MTN_POW_JUMP_D, 0.25f, 0.5f },
	{ &OmegaActs[OM_JUMP_D], 57, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &OmegaActs[OM_JUMP_E], 57, MD_MTN_NEXT, MTN_POW_JUMP_F, 0.25f, 0.5f },
	{ &OmegaActs[OM_JUMP_F], 57, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &OmegaActs[OM_JUMP_TRNGL], 57, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &OmegaActs[OM_JUMP_GLIND], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_BK], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_BK_L], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_BK_R], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_FLIP_B], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_FLIP_F], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_L], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_GLIND_R], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_FLY_IDLE], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_FLY_SLOW], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_FLY_GLIND], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_HANG_OFF], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_HANG_ON], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BREAK_A], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BREAK_B], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BREAK_C], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BREAK_TURN_L], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BREAK_TURN_R], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BRA_MID], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BRA_TOP], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_FLORT], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_DAM_M_A], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_DAM_M_B], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_DAM_M_C], 57, MD_MTN_NEXT, MTN_POW_JUMP_E, 0.25f, 1.0f },
	{ &OmegaActs[OM_EDGE_OTTO_A], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_EDGE_OTTO_B], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_EDGE_OTTO_C], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_FW_JUMP], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_TRAP_JUMP], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[DARK_OM], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_WIN], 57, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_ATC_GUN], 57, MD_MTN_NEXT, MTN_SPD_IDLE, 0.1f, 1.0f },
	{ &OmegaActs[OM_IDLE], 57, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &OmegaActs[OM_IDLE_C], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_IDLE_D], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BOB], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BOB_L], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_BOB_R], 57, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &OmegaActs[OM_JUMP_B], 57, MD_MTN_XSPD, 0, 0.25f, 0.1f },
};

NJS_TEXNAME OMEGA_TEXNAMES[15];
NJS_TEXLIST OMEGA_TEXLIST = { arrayptrandlength(OMEGA_TEXNAMES) };

NJS_MATRIX OmegaMatrices[4];

void PlayVoice_Omega(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(OmegaSound_Win);
		break;
	}
}

void PlaySound_Omega(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(OmegaSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		break;
	case 1233:
		break;
	case 1503:
		PlayHeroesSound(OmegaSound_Death);
		break;
	}
}

void OmegaCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = OmegaMdls[0]->getmodel();

	if (object == base->getnode(17)) {
		njGetMatrix(OmegaMatrices[0]);
		return;
	}

	if (object == base->getnode(33)) {
		njGetMatrix(OmegaMatrices[1]);
		return;
	}

	if (object == base->getnode(6)) {
		njGetMatrix(OmegaMatrices[2]);
		return;
	}
	
	if (object == base->getnode(7)) {
		njGetMatrix(OmegaMatrices[3]);
		return;
	}
}

void HideNode(NJS_OBJECT* object)
{
	while (object)
	{
		object->evalflags |= NJD_EVAL_HIDE;
		if (object->child)
		{
			HideNode(object->child);
		}
		object = object->sibling;
	}
}

void ShowNode(NJS_OBJECT* object)
{
	while (object)
	{
		object->evalflags |= NJD_EVAL_HIDE;
		if (object->child)
		{
			HideNode(object->child);
		}
		object = object->sibling;
	}
}

void OmegaHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* omegaobj = HeroesChars[obj->Data1->CharIndex];
	if (!omegaobj) return;
	EntityData2* data2 = (EntityData2*)omegaobj->Data2;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;
	playerwk_heroes* pwp_heroes = (playerwk_heroes*)omegaobj->UnknownB_ptr;

	if (omegaobj->Data1->Index == 42 || omegaobj->Data1->Index == 43 || omegaobj->Data1->Index == 44) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (omegaobj->Data1->NextAction == 0) omegaobj->Data1->NextAction = 1;
			else omegaobj->Data1->NextAction = 0;
		}
	}
	else {
		omegaobj->Data1->NextAction = 0;
	}

	if (omegaobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&OMEGA_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);

	if (entity1->Action == 19) {
		if (data2->field_34 > -6) data2->field_34 -= 0.1f;
		njTranslate(0, 0, data2->field_34, 0);
	}
	else if (entity1->Action == 20 || entity1->Action == 21) {
		data2->field_34 = 0;
	}

	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);
	if (entity1->Action == 19) njRotateX(0, -0x1000);

	if (entity1->Action == 24) {
		njRotateX(0, -0x4000);
		njTranslate(0, 0, 0, 2);
	}

	if (omegaobj->Data1->Index == 52) {
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

	bool show_gun_l = action == MTN_POW_ATC_A || (action == MTN_POW_ATC_C && omegaobj->Data1->Scale.x > 60) || obj->Data1->Action == 19 || obj->Data1->Action == 20;
	bool show_gun_r = show_gun_l;

	if (show_gun_l)
	{
		HideNode(OmegaMdls[0]->getmodel()->getnode(34));
	}

	if (show_gun_r)
	{
		HideNode(OmegaMdls[0]->getmodel()->getnode(18));
	}

	*NodeCallbackFuncPtr = OmegaCallback;
	njAction(actptr, mtn->nframe);
	*NodeCallbackFuncPtr = nullptr;

	switch (action) {
	case 7:
	case 19:
		for (int i = 2; i < 4; ++i) {
			njSetMatrix(NULL, OmegaMatrices[i]);
			late_DrawModel(OmegaMdls[1]->getmodel()->getbasicdxmodel(), LATE_MAT);
		}
		break;
	}

	if (show_gun_l)
	{
		ShowNode(OmegaMdls[0]->getmodel()->getnode(34));

		njSetMatrix(NULL, OmegaMatrices[1]);
		if (obj->Data1->Action == 19 || obj->Data1->Action == 20) {
			njRotateX(0, data2->field_38);
			dsDrawObject(OmegaMdls[4]->getmodel());
		}
		else {
			njRotateX(0, data2->field_38);
			dsDrawObject(OmegaMdls[2]->getmodel());
			njGetMatrix(OmegaMatrices[1]);
		}
	}

	if (show_gun_r)
	{
		ShowNode(OmegaMdls[0]->getmodel()->getnode(18));

		njSetMatrix(NULL, OmegaMatrices[0]);
		if (obj->Data1->Action == 19 || obj->Data1->Action == 20) {
			njRotateX(0, data2->field_38);
			dsDrawObject(OmegaMdls[4]->getmodel());
		}
		else {
			njRotateX(0, data2->field_38);
			dsDrawObject(OmegaMdls[3]->getmodel());
			njGetMatrix(OmegaMatrices[0]);
		}
	}

	njPopMatrix(1);

	if (action == MTN_POW_JUMP_B || action == MTN_POW_ROLL)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 0);
	}
	else if (action == MTN_POW_FW_JUMP)
	{
		HeroesChars_EffBall((taskwk*)entity1, pwp_heroes, 1);
	}

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void OmegaDrawMissiles(float y, float z) {
	for (uint8_t j = 0; j < 2; ++j) {
		njSetMatrix(NULL, OmegaMatrices[j]);
		njRotateY(0, 0x8000);
		njTranslate(0, 7, y, z);
		if (FrameCounterUnpaused % 3 == 0) njTranslate(0, 5, 0, 0);
		njRotateZ(0, 0xC000);
		njScale(0, 0.02f, 0.2f, 0.02f);
		for (uint8_t i = 0; i < 5; ++i) {
			dsDrawModel(OmegaMdls[2]->getmodel()->getbasicdxmodel());
			njTranslate(0, 0, 20, 0);
		}
	}
}

void OmegaDrawMissilesList() {
	njSetTexture(&OMEGA_TEXLIST);
	njPushMatrixEx();
	OmegaDrawMissiles(1, 0);
	OmegaDrawMissiles(0.5, 0.5);
	OmegaDrawMissiles(0, 1);
	OmegaDrawMissiles(0.5, -0.5);
	OmegaDrawMissiles(0, -1);
	OmegaDrawMissiles(-0.5, -0.5);
	OmegaDrawMissiles(-1, -1);
	OmegaDrawMissiles(-0.5, 0.5);
	njPopMatrix(1);
}

void OmegaHeroes_Main(ObjectMaster *obj) {
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
		HeroesChars_InitPlayer((task*)obj, { "heroes-omega", &OMEGA_TEXLIST }, 9, omega_action_heroes);
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
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(OmegaSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(OmegaSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		KnucklesAnimConverter(&pwp_heroes->mm, HeroesChars_Omega, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		switch (PowerComboTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayHeroesSound(OmegaSound_Combo1);
			break;
		case 2:
			PlayHeroesSound(OmegaSound_Combo2);
			break;
		case 3:
			PlayHeroesSound(OmegaSound_Combo3);
		case 4:
			playerco2->Speed = { 0, 0, 0 };

			data->Scale.x += 1;
			data2->field_38 += 0x1000; //rotate guns
			ExploseEnemies(&playerdata->Position, 5);

			OmegaDrawMissilesList();

			if (data->Scale.x > 125 || (data->Scale.x > 100 && PressedButtons[playerdata->CharIndex] & Buttons_X)) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				data->Action = 2;
			}
			break;
		}

		pwp_heroes->mm.reqaction = 11;
		break;
	case 4:
		if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayHeroesSound(OmegaSound_Trick);
		
		data2->field_38 += 0x1000;

		if (data->Scale.x > 10 && data->Scale.x < 20) {
			NJS_VECTOR pos;
			njPushMatrix(_nj_unit_matrix_);
			njTranslateV(0, &playerdata->Position);
			njRotateY(0, -playerdata->Rotation.y);
			njTranslate(0, 15, 0, 0);
			njSetTexture(&OMEGA_TEXLIST);
			njGetTranslation(_nj_current_matrix_ptr_, &pos);
			njPopMatrix(1);

			ExploseEnemies(&pos, 5);

			OmegaDrawMissilesList();
		}

		pwp_heroes->mm.reqaction = 9;
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

void LoadOmegaFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-omega.arcx"));

	OmegaMdls[0] = arc.GetModel("OMEGA_LOCATOR.sa1mdl");
	OmegaMdls[1] = arc.GetModel("EF_OMG_JET.sa1mdl");
	OmegaMdls[2] = arc.GetModel("OMEGA_L_GUN.sa1mdl");
	OmegaMdls[3] = arc.GetModel("OMEGA_R_GUN.sa1mdl");
	OmegaMdls[4] = arc.GetModel("DRILL.sa1mdl");

	OmegaAnms[0] = arc.GetAnimation("OM_WALK.saanim");
	OmegaAnms[1] = arc.GetAnimation("OM_WALK_PULL.saanim");
	OmegaAnms[2] = arc.GetAnimation("OM_WALK_PUSH.saanim");
	OmegaAnms[3] = arc.GetAnimation("OM_TURN_L.saanim");
	OmegaAnms[4] = arc.GetAnimation("OM_TURN_R.saanim");
	OmegaAnms[5] = arc.GetAnimation("OM_SLOW_RUN.saanim");
	OmegaAnms[6] = arc.GetAnimation("OM_MID_RUN.saanim");
	OmegaAnms[7] = arc.GetAnimation("OM_TOP_RUN.saanim");
	OmegaAnms[8] = arc.GetAnimation("OM_START.saanim");
	OmegaAnms[9] = arc.GetAnimation("OM_ATC_GUN.saanim");
	OmegaAnms[10] = arc.GetAnimation("OM_ATC_A.saanim");
	OmegaAnms[11] = arc.GetAnimation("OM_ATC_A.saanim");
	OmegaAnms[12] = arc.GetAnimation("OM_ATC_A.saanim");
	OmegaAnms[13] = arc.GetAnimation("OM_JUMP_A.saanim");
	OmegaAnms[14] = arc.GetAnimation("OM_JUMP_B.saanim");
	OmegaAnms[15] = arc.GetAnimation("OM_JUMP_C.saanim");
	OmegaAnms[16] = arc.GetAnimation("OM_JUMP_D.saanim");
	OmegaAnms[17] = arc.GetAnimation("OM_JUMP_E.saanim");
	OmegaAnms[18] = arc.GetAnimation("OM_JUMP_F.saanim");
	OmegaAnms[19] = arc.GetAnimation("OM_JUMP_TRNGL.saanim");
	OmegaAnms[20] = arc.GetAnimation("OM_JUMP_GLIND.saanim");
	OmegaAnms[21] = arc.GetAnimation("OM_GLIND.saanim");
	OmegaAnms[22] = arc.GetAnimation("OM_GLIND_BK.saanim");
	OmegaAnms[23] = arc.GetAnimation("OM_GLIND_BK_L.saanim");
	OmegaAnms[24] = arc.GetAnimation("OM_GLIND_BK_R.saanim");
	OmegaAnms[25] = arc.GetAnimation("OM_GLIND_FLIP_B.saanim");
	OmegaAnms[26] = arc.GetAnimation("OM_GLIND_FLIP_F.saanim");
	OmegaAnms[27] = arc.GetAnimation("OM_GLIND_L.saanim");
	OmegaAnms[28] = arc.GetAnimation("OM_GLIND_R.saanim");
	OmegaAnms[29] = arc.GetAnimation("OM_FLY_IDLE.saanim");
	OmegaAnms[30] = arc.GetAnimation("OM_FLY_SLOW.saanim");
	OmegaAnms[31] = arc.GetAnimation("OM_FLY_GLIND.saanim");
	OmegaAnms[32] = arc.GetAnimation("OM_HANG_OFF.saanim");
	OmegaAnms[33] = arc.GetAnimation("OM_HANG_ON.saanim");
	OmegaAnms[34] = arc.GetAnimation("OM_BREAK_A.saanim");
	OmegaAnms[35] = arc.GetAnimation("OM_BREAK_B.saanim");
	OmegaAnms[36] = arc.GetAnimation("OM_BREAK_C.saanim");
	OmegaAnms[37] = arc.GetAnimation("OM_BREAK_TURN_L.saanim");
	OmegaAnms[38] = arc.GetAnimation("OM_BREAK_TURN_R.saanim");
	OmegaAnms[39] = arc.GetAnimation("OM_BRA_MID.saanim");
	OmegaAnms[40] = arc.GetAnimation("OM_BRA_TOP.saanim");
	OmegaAnms[41] = arc.GetAnimation("OM_FLORT.saanim");
	OmegaAnms[42] = arc.GetAnimation("OM_DAM_M_A.saanim");
	OmegaAnms[43] = arc.GetAnimation("OM_DAM_M_B.saanim");
	OmegaAnms[44] = arc.GetAnimation("OM_DAM_M_C.saanim");
	OmegaAnms[45] = arc.GetAnimation("OM_EDGE_OTTO_A.saanim");
	OmegaAnms[46] = arc.GetAnimation("OM_EDGE_OTTO_B.saanim");
	OmegaAnms[47] = arc.GetAnimation("OM_EDGE_OTTO_C.saanim");
	OmegaAnms[48] = arc.GetAnimation("OM_FW_JUMP.saanim");
	OmegaAnms[49] = arc.GetAnimation("OM_TRAP_JUMP.saanim");
	OmegaAnms[50] = arc.GetAnimation("DARK_OM.saanim");
	OmegaAnms[51] = arc.GetAnimation("OM_WIN.saanim");
	OmegaAnms[52] = arc.GetAnimation("OM_IDLE.saanim");
	OmegaAnms[53] = arc.GetAnimation("OM_IDLE_C.saanim");
	OmegaAnms[54] = arc.GetAnimation("OM_IDLE_D.saanim");
	OmegaAnms[55] = arc.GetAnimation("OM_BOB.saanim");
	OmegaAnms[56] = arc.GetAnimation("OM_BOB_L.saanim");
	OmegaAnms[57] = arc.GetAnimation("OM_BOB_R.saanim");

	for (int i = 0; i < LengthOfArray(OmegaActs); ++i) {
		OmegaActs[i].object = OmegaMdls[0]->getmodel();
		OmegaActs[i].motion = OmegaAnms[i] ? OmegaAnms[i]->getmotion() : NULL;
	}
}

void UnloadOmegaFiles() {
	FreeMDLFiles(OmegaMdls, LengthOfArray(OmegaMdls));
	FreeANMFiles(OmegaAnms, LengthOfArray(OmegaAnms));
}