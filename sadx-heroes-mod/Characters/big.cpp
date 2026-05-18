#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

enum {
	BI_WALK,
	BI_WALK_PULL,
	BI_WALK_PUSH,
	BI_TURN_L,
	BI_TURN_R,
	BI_SLOW_RUN,
	BI_MID_RUN,
	BI_TOP_RUN,
	BI_START,
	BI_ATC_BATT,
	BI_ATC_PRESS,
	BI_ATC_START,
	BI_FISH,
	BI_JUMP_A,
	BI_JUMP_B,
	BI_JUMP_C,
	BI_JUMP_D,
	BI_JUMP_E,
	BI_JUMP_F,
	BI_JUMP_UMBRELLA,
	BI_JUMP_GLIND,
	BI_GLIND,
	BI_GLIND_BK,
	BI_GLIND_BK_L,
	BI_GLIND_BK_R,
	BI_GLIND_FLIP_BK,
	BI_GLIND_FLIP_FR,
	BI_GLIND_L,
	BI_GLIND_R,
	BI_FLY_IDLE,
	BI_FLY_SLOW,
	BI_FLY_GLIND,
	BI_HANG_OFF,
	BI_HANG_ON,
	BI_BREAK_A,
	BI_BREAK_B,
	BI_BREAK_C,
	BI_BREAK_TURN_L,
	BI_BREAK_TURN_R,
	BI_BRA_MID,
	BI_BRA_TOP,
	BI_FLORT,
	BI_DAM_M_A,
	BI_DAM_M_B,
	BI_DAM_M_C,
	BI_EDGE_OTTO_A,
	BI_EDGE_OTTO_B,
	BI_EDGE_OTTO_C,
	BI_FW_JUMP,
	BI_TRAP_JUMP,
	ROSE_BI,
	BI_WIN,
	BI_JUMP_BATT,
	BI_IDLE_HALF,
	BI_IDLE_B_HALF,
	BI_IDLE_C_HALF,
	BI_BOB,
	BI_BOB_L,
	BI_BOB_R,
	BI_START_IDLE,
	BI_GLIND_DASH,
	BI_GLIND_BK_DASH,
};

ModelInfo* BigMdls[4];
AnimationFile* BigAnms[63];
NJS_ACTION BigActs[63];

PL_ACTION big_action_heroes[] = {
	{ &BigActs[BI_WALK], 47, MD_MTN_XSPD, 0, 0.25f, 1.0f },
	{ &BigActs[BI_WALK_PULL], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_WALK_PUSH], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_TURN_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_TURN_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_SLOW_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.5f },
	{ &BigActs[BI_MID_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.3f },
	{ &BigActs[BI_TOP_RUN], 47, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &BigActs[BI_START], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_ATC_BATT], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_ATC_PRESS], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_ATC_START], 47, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &BigActs[BI_FISH], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_JUMP_A], 47, MD_MTN_NEXT, MTN_POW_JUMP_B, 0.25f, 0.5f },
	{ &BigActs[BI_JUMP_B], 47, MD_MTN_WORK, 0, 1.0f, 0.25f },
	{ &BigActs[BI_JUMP_C], 47, MD_MTN_NEXT, MTN_POW_JUMP_D, 0.25f, 0.5f },
	{ &BigActs[BI_JUMP_D], 47, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &BigActs[BI_JUMP_E], 47, MD_MTN_NEXT, MTN_POW_JUMP_F, 0.25f, 0.5f },
	{ &BigActs[BI_JUMP_F], 47, MD_MTN_WORK, 0, 1.0f, 0.5f },
	{ &BigActs[BI_JUMP_UMBRELLA], 47, MD_MTN_LOOP, 0, 0.25f, 0.5f },
	{ &BigActs[BI_JUMP_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_BK], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_BK_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_BK_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_FLIP_BK], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_FLIP_FR], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_L], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_R], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_FLY_IDLE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_FLY_SLOW], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_FLY_GLIND], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_HANG_OFF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_HANG_ON], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BREAK_A], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BREAK_B], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BREAK_C], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BREAK_TURN_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BREAK_TURN_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BRA_MID], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BRA_TOP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_FLORT], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_DAM_M_A], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_DAM_M_B], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_DAM_M_C], 47, MD_MTN_NEXT, MTN_POW_JUMP_E, 0.25f, 1.0f },
	{ &BigActs[BI_EDGE_OTTO_A], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_EDGE_OTTO_B], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_EDGE_OTTO_C], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_FW_JUMP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_TRAP_JUMP], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[ROSE_BI], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_WIN], 47, MD_MTN_STOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_JUMP_BATT], 47, MD_MTN_NEXT, MTN_SPD_IDLE, 0.1f, 1.0f },
	{ &BigActs[BI_IDLE_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 0.25f },
	{ &BigActs[BI_IDLE_B_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_IDLE_C_HALF], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BOB], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BOB_L], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_BOB_R], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_JUMP_B], 47, MD_MTN_XSPD, 0, 0.25f, 0.1f },
	{ &BigActs[BI_START_IDLE], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_DASH], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
	{ &BigActs[BI_GLIND_BK_DASH], 47, MD_MTN_LOOP, 0, 0.25f, 1.0f },
};

NJS_TEXNAME HBIG_TEXNAMES[7];
NJS_TEXLIST HBIG_TEXLIST = { arrayptrandlength(HBIG_TEXNAMES) };

NJS_MATRIX BigMatrix;

CollisionData Lure_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, 5.5, 0.0f, 0.0f };

void PlayVoice_Big(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(BigSound_Win);
		break;
	}
}

void PlaySound_Big(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(BigSound_Attack);
		break;
	case 1232:
		PlayHeroesSound(BigSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(BigSound_Hurt1);
		break;
	case 1503:
		PlayHeroesSound(BigSound_Death);
		break;
	}
}

void BigCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = BigMdls[0]->getmodel();

	if (object == base->getnode(30)) {
		njGetMatrix(BigMatrix); //hand
	}
}

void LureObj_Display(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HBIG_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &data->Position);
	njRotateZ(0, data->Rotation.z);
	njRotateX(0, data->Rotation.x);
	njRotateY(0, -data->Rotation.y - 0x4000);
	njScale(0, data->Scale.x, data->Scale.x, data->Scale.x);

	njRotateX(0, 0x4000);

	dsDrawModel(BigMdls[3]->getmodel()->getbasicdxmodel());

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void LureObj_Main(ObjectMaster *obj) {
	ObjectMaster* HeroesObj = obj->Parent;
	EntityData1* data = HeroesObj->Data1;
	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];

	obj->Data1->Position = playerobj->Data1->Position;
	obj->Data1->Rotation = playerobj->Data1->Rotation;
	NJS_VECTOR dir = { 0, 40, 0 };
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &obj->Data1->Position);
	njRotateZ(0, obj->Data1->Rotation.z);
	njRotateX(0, obj->Data1->Rotation.x);
	njRotateY(0, -obj->Data1->Rotation.y - 0x4000);
	njCalcPoint(0, &dir, &obj->Data1->Position);
	njPopMatrix(1u);
	
	switch (obj->Data1->Action) {
	case 0:
		obj->DisplaySub = LureObj_Display;
		Collision_Init(obj, &Lure_Col, 1, 3u);
		obj->Data1->Action = 1;
		break;
	case 1:
		if (data->field_A != 2) {
			if (obj->Data1->Scale.y < 45) obj->Data1->Scale.y += 0.1f;
			if (obj->Data1->Scale.x < 0.2f) obj->Data1->Scale.x += 0.005f;
		}
		else {
			if (obj->Data1->Scale.y > 0) obj->Data1->Scale.y -= 0.04f;
			if (obj->Data1->Scale.x > 0) obj->Data1->Scale.x -= 0.005f;
			else DeleteObject_(obj);
		}
		
		obj->Data1->Scale.z -= 0x500;

		dir = { obj->Data1->Scale.y, -(obj->Data1->Scale.y / 2), 0 };
		njPushMatrix(_nj_unit_matrix_);
		njTranslateV(0, &obj->Data1->Position);
		njRotateZ(0, obj->Data1->Rotation.z);
		njRotateX(0, obj->Data1->Rotation.x);
		njRotateY(0, -obj->Data1->Scale.z - 0x4000);
		njCalcPoint(0, &dir, &obj->Data1->Position);
		njPopMatrix(1u);

		ExploseEnemies(&obj->Data1->Position, 5);

		break;
	}

	obj->DisplaySub(obj);
}

void BigHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;

	ObjectMaster* Bigobj = HeroesChars[obj->Data1->CharIndex];
	if (!Bigobj) return;
	EntityData2* data2 = (EntityData2*)Bigobj->Data2;

	WriteData<5>((void*)0x472649, 0x90);
	WriteData<5>((void*)0x47258B, 0x90);
	WriteData<5>((void*)0x472626, 0x90);
	Big_Display(obj);
	WriteCall((void*)0x472649, njAction);
	WriteCall((void*)0x47258B, njAction);
	WriteCall((void*)0x472626, njAction);

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (Bigobj->Data1->Index == 42 || Bigobj->Data1->Index == 43 || Bigobj->Data1->Index == 44) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (Bigobj->Data1->NextAction == 0) Bigobj->Data1->NextAction = 1;
			else Bigobj->Data1->NextAction = 0;
		}
	}
	else {
		Bigobj->Data1->NextAction = 0;
	}

	if (Bigobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&HBIG_TEXLIST);

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

	if (Bigobj->Data1->Index == 52) {
		njRotateY(0, 0xC000);
	}

	njRotateX(0, 0x4000);

	mtnjvwk* mtn = (mtnjvwk*)Bigobj->Data1->Object;

	int action = mtn->reqaction;
	NJS_ACTION* actptr = mtn->plactptr[action].actptr;
	if (mtn->mtnmode == MD_MTN_CHNG)
	{
		action = mtn->action;
		actptr = mtn->actwkptr;
	}

	*NodeCallbackFuncPtr = BigCallback;
	njActionWeight(actptr, mtn->nframe, BigMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	memcpy(_nj_current_matrix_ptr_, BigMatrix, sizeof(NJS_MATRIX));
	njRotateX(0, 0xC000);
	if (action == 19) dsDrawModel(BigMdls[2]->getmodel()->getbasicdxmodel());
	if (action == 9 || action == 11 || action == 12) dsDrawModel(BigMdls[1]->getmodel()->getbasicdxmodel());

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void BigHeroes_Main(ObjectMaster *obj) {
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
		HeroesChars_InitPlayer((task*)obj, { "heroes-big", &HBIG_TEXLIST }, 8, big_action_heroes);
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
				PlayHeroesSound(BigSound_Attack);
				data->Action = 4;
			}
		}

		if (playerdata->Status & Status_Ground) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(BigSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(BigSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		KnucklesAnimConverter(mtn, Characters_HeroesBig, (taskwk*)playerdata, (playerwk*)playerco2);

		break;
	case 3:
		switch (PowerLaunchTrick(data, data2, playerco2, playerdata)) {
		case 1:
			PlayHeroesSound(BigSound_Combo1);
			mtn->reqaction = 11;
			break;
		case 2:
			PlayHeroesSound(BigSound_Combo2);
			mtn->reqaction = 9;
			break;
		case 3:
			PlayHeroesSound(BigSound_Combo3);
			data->Scale.y = 73;
			playerco2->Speed.x = 1;
		case 4:
			data->Scale.y -= 0.25f;
			playerco2->Speed.x = 0;

			if (data->Scale.y < 50) {
				data2->field_30 = 0;
				data2->field_30 = 4;
				LoadChildObject(LoadObj_Data1, LureObj_Main, obj);
			}
			
			data->Scale.x = data->Scale.y;
			mtn->reqaction = 12;
			break;
		case 5:
			playerco2->Speed.x = 0;

			if (data->field_A == 0) {
				if (data->Scale.y != 0) data->Scale.y -= 0.25f;
				else data->field_A = 1;
			}
			else if (data->field_A == 1) {
				if (data->Scale.y != 40) data->Scale.y += 0.25f;
				else data->field_A = 2;
			}

			if (data->field_A == 2 || PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 2;
				data->Scale.y += 0.25f;
				if (data->Scale.y > 72 || PressedButtons[data->CharIndex] & Buttons_X) {
					if (obj->Child) DeleteChildObjects(obj);
					data2->field_30 = 0;
					playerco2->Powerups &= ~Powerups_Invincibility;
					data->Action = 2;
				}
			}

			data->Scale.x = data->Scale.y;
			mtn->reqaction = 12;
			break;
		}
		
		RunObjectChildren(obj);

		break;
	case 4:
		if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) {
			if (data->field_A != 2) PlayHeroesSound(CommonSound_FlyPunchHit);
			data->field_A = 2;
		}
		else {
			if (FlightPunchTrick(data, data2, playerco2, playerdata)) PlayHeroesSound(BigSound_Trick);
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
			if (data->Scale.x > 28) {
				data2->field_30 = 0;
				playerco2->Powerups &= ~Powerups_Invincibility;
				
				ExploseEnemies(&playerdata->Position, 3);
				data->Action = 2;
			}
			
		}

		mtn->reqaction = 10;
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

void LoadBigFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-big.arcx"));

	BigMdls[0] = arc.GetModel("BIG_LOCATOR.sa1mdl");
	BigMdls[1] = arc.GetModel("FISH_STICK.sa1mdl");
	BigMdls[2] = arc.GetModel("UMBRELLA.sa1mdl");
	BigMdls[3] = arc.GetModel("UKI.sa1mdl");
	//BigMdls[4] = arc.GetModel("UKIWA.sa1mdl");
	//BigMdls[5] = arc.GetModel("UNI.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(BigMdls[0]->getweightinfo(), BigMdls[0]->getmodel());

	BigAnms[0] = arc.GetAnimation("BI_WALK.saanim");
	BigAnms[1] = arc.GetAnimation("BI_WALK_PULL.saanim");
	BigAnms[2] = arc.GetAnimation("BI_WALK_PUSH.saanim");
	BigAnms[3] = arc.GetAnimation("BI_TURN_L.saanim");
	BigAnms[4] = arc.GetAnimation("BI_TURN_R.saanim");
	BigAnms[5] = arc.GetAnimation("BI_SLOW_RUN.saanim");
	BigAnms[6] = arc.GetAnimation("BI_MID_RUN.saanim");
	BigAnms[7] = arc.GetAnimation("BI_TOP_RUN.saanim");
	BigAnms[8] = arc.GetAnimation("BI_START.saanim");
	BigAnms[9] = arc.GetAnimation("BI_ATC_BATT.saanim");
	BigAnms[10] = arc.GetAnimation("BI_ATC_PRESS.saanim");
	BigAnms[11] = arc.GetAnimation("BI_ATC_START.saanim");
	BigAnms[12] = arc.GetAnimation("BI_FISH.saanim");
	BigAnms[13] = arc.GetAnimation("BI_JUMP_A.saanim");
	BigAnms[14] = arc.GetAnimation("BI_JUMP_B.saanim");
	BigAnms[15] = arc.GetAnimation("BI_JUMP_C.saanim");
	BigAnms[16] = arc.GetAnimation("BI_JUMP_D.saanim");
	BigAnms[17] = arc.GetAnimation("BI_JUMP_E.saanim");
	BigAnms[18] = arc.GetAnimation("BI_JUMP_F.saanim");
	BigAnms[19] = arc.GetAnimation("BI_JUMP_UMBRELLA.saanim");
	BigAnms[20] = arc.GetAnimation("BI_JUMP_GLIND.saanim");
	BigAnms[21] = arc.GetAnimation("BI_GLIND.saanim");
	BigAnms[22] = arc.GetAnimation("BI_GLIND_BK.saanim");
	BigAnms[23] = arc.GetAnimation("BI_GLIND_BK_L.saanim");
	BigAnms[24] = arc.GetAnimation("BI_GLIND_BK_R.saanim");
	BigAnms[25] = arc.GetAnimation("BI_GLIND_FLIP_BK.saanim");
	BigAnms[26] = arc.GetAnimation("BI_GLIND_FLIP_FR.saanim");
	BigAnms[27] = arc.GetAnimation("BI_GLIND_L.saanim");
	BigAnms[28] = arc.GetAnimation("BI_GLIND_R.saanim");
	BigAnms[29] = arc.GetAnimation("BI_FLY_IDLE.saanim");
	BigAnms[30] = arc.GetAnimation("BI_FLY_SLOW.saanim");
	BigAnms[31] = arc.GetAnimation("BI_FLY_GLIND.saanim");
	BigAnms[32] = arc.GetAnimation("BI_HANG_OFF.saanim");
	BigAnms[33] = arc.GetAnimation("BI_HANG_ON.saanim");
	BigAnms[34] = arc.GetAnimation("BI_BREAK_A.saanim");
	BigAnms[35] = arc.GetAnimation("BI_BREAK_B.saanim");
	BigAnms[36] = arc.GetAnimation("BI_BREAK_C.saanim");
	BigAnms[37] = arc.GetAnimation("BI_BREAK_TURN_L.saanim");
	BigAnms[38] = arc.GetAnimation("BI_BREAK_TURN_R.saanim");
	BigAnms[39] = arc.GetAnimation("BI_BRA_MID.saanim");
	BigAnms[40] = arc.GetAnimation("BI_BRA_TOP.saanim");
	BigAnms[41] = arc.GetAnimation("BI_FLORT.saanim");
	BigAnms[42] = arc.GetAnimation("BI_DAM_M_A.saanim");
	BigAnms[43] = arc.GetAnimation("BI_DAM_M_B.saanim");
	BigAnms[44] = arc.GetAnimation("BI_DAM_M_C.saanim");
	BigAnms[45] = arc.GetAnimation("BI_EDGE_OTTO_A.saanim");
	BigAnms[46] = arc.GetAnimation("BI_EDGE_OTTO_B.saanim");
	BigAnms[47] = arc.GetAnimation("BI_EDGE_OTTO_C.saanim");
	BigAnms[48] = arc.GetAnimation("BI_FW_JUMP.saanim");
	BigAnms[49] = arc.GetAnimation("BI_TRAP_JUMP.saanim");
	BigAnms[50] = arc.GetAnimation("ROSE_BI.saanim");
	BigAnms[51] = arc.GetAnimation("BI_WIN.saanim");
	BigAnms[52] = arc.GetAnimation("BI_JUMP_BATT.saanim");
	BigAnms[53] = arc.GetAnimation("BI_IDLE_HALF.saanim");
	BigAnms[54] = arc.GetAnimation("BI_IDLE_B_HALF.saanim");
	BigAnms[55] = arc.GetAnimation("BI_IDLE_C_HALF.saanim");
	BigAnms[56] = arc.GetAnimation("BI_BOB.saanim");
	BigAnms[57] = arc.GetAnimation("BI_BOB_L.saanim");
	BigAnms[58] = arc.GetAnimation("BI_BOB_R.saanim");
	BigAnms[59] = arc.GetAnimation("BI_START_IDLE.saanim");
	BigAnms[60] = arc.GetAnimation("BI_GLIND_DASH.saanim");
	BigAnms[61] = arc.GetAnimation("BI_GLIND_BK_DASH.saanim");

	for (int i = 0; i < LengthOfArray(BigActs); ++i) {
		BigActs[i].object = BigMdls[0]->getmodel();
		BigActs[i].motion = BigAnms[i] ? BigAnms[i]->getmotion() : NULL;
	}
}

void UnloadBigFiles() {
	HelperFunctionsGlobal.Weights->DeInit(BigMdls[0]->getweightinfo(), BigMdls[0]->getmodel());
	FreeMDLFiles(BigMdls, LengthOfArray(BigMdls));
	FreeANMFiles(BigAnms, LengthOfArray(BigAnms));
}