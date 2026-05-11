#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* OmegaMdls[5];
AnimationFile* OmegaAnms[60];
AnimData OmegaAnimData[60];

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

	bool show_gun_l = omegaobj->Data1->Index == 9 || (omegaobj->Data1->Index == 11 && omegaobj->Data1->Scale.x > 60) || obj->Data1->Action == 19 || obj->Data1->Action == 20;
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
	njAction(OmegaAnimData[omegaobj->Data1->Index].Animation, omegaobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	switch (omegaobj->Data1->Index) {
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

	if (!CharactersCommon_Init(obj, "heroes-omega", &OMEGA_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (playerdata->Action == 19 || playerdata->Action == 20) {
		data2->field_38 += 0x1000;
	}

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[15].texaddr = OMEGA_TEXLIST.textures[9].texaddr;
		}

		if (CustomPhysics) {
			playerdata->CollisionInfo->CollisionArray->a = 8.0f;
			playerco2->PhysicsData.CollisionSize = 8.0f;
			playerco2->PhysicsData.RippleSize = 0.5f;
			playerco2->_struct_a3.ShadowScale = 1.5f;

			playerco2->PhysicsData.HSpeedCap = 15;
			playerco2->PhysicsData.MaxAccel = 3.5f;
			playerco2->PhysicsData.field_14 = 0.85f;
		}

		data->Rotation.z = 1;
	}

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = OmegaHeroes_Display;

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

		{
			NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, OmegaAnimData, anim.y, anim.z);
		}

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

		PlayHeroesAnimation(obj, 11, OmegaAnimData, 0, 0);
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
		

		PlayHeroesAnimation(obj, 9, OmegaAnimData, 0, 0);
		break;
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
	OmegaAnms[48] = arc.GetAnimation("OM_EDGE_OTTO_C.saanim");
	OmegaAnms[49] = arc.GetAnimation("OM_FW_JUMP.saanim");
	OmegaAnms[50] = arc.GetAnimation("OM_TRAP_JUMP.saanim");
	OmegaAnms[51] = arc.GetAnimation("HERO_KN.saanim");
	OmegaAnms[52] = arc.GetAnimation("OM_WIN.saanim");
	OmegaAnms[53] = arc.GetAnimation("OM_ATC_GUN.saanim");
	OmegaAnms[54] = arc.GetAnimation("OM_IDLE.saanim");
	OmegaAnms[55] = arc.GetAnimation("OM_IDLE_C.saanim");
	OmegaAnms[56] = arc.GetAnimation("OM_IDLE_D.saanim");
	OmegaAnms[57] = arc.GetAnimation("OM_BOB.saanim");
	OmegaAnms[58] = arc.GetAnimation("OM_BOB_L.saanim");
	OmegaAnms[59] = arc.GetAnimation("OM_BOB_R.saanim");

	for (uint8_t i = 0; i < LengthOfArray(OmegaAnimData); ++i) {
		if (OmegaAnms[i] == nullptr) continue;
		OmegaAnimData[i].Animation = new NJS_ACTION;
		OmegaAnimData[i].Animation->object = OmegaMdls[0]->getmodel();
		OmegaAnimData[i].Animation->motion = OmegaAnms[i]->getmotion();
		OmegaAnimData[i].NextAnim = i;
		OmegaAnimData[i].AnimationSpeed = 0.5f;
	}

	OmegaAnimData[13].NextAnim = 14;
	OmegaAnimData[15].NextAnim = 16;
	OmegaAnimData[18].NextAnim = 19;
	OmegaAnimData[42].Property = 1;
	OmegaAnimData[43].Property = 1;
	OmegaAnimData[27].Property = 1;
	OmegaAnimData[33].Property = 1;
	OmegaAnimData[28].Property = 1;
	OmegaAnimData[20].Property = 1;
	OmegaAnimData[25].Property = 1;
	OmegaAnimData[26].Property = 1;
	OmegaAnimData[44].NextAnim = 17;
	OmegaAnimData[52].Property = 1;
}

void UnloadOmegaFiles() {
	FreeMDLFiles(OmegaMdls, LengthOfArray(OmegaMdls));
	FreeANMFiles(OmegaAnms, LengthOfArray(OmegaAnms));
}