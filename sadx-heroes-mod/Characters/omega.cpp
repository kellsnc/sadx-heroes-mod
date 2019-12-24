#include "stdafx.h"

ModelInfo* OmegaMdls[4];
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

inline bool IsSameObject(NJS_OBJECT* object, const char* name) {
	return (object == (NJS_OBJECT*)OmegaMdls[0]->getdata(name) ? 1 : 0);
}

inline void DrawGammaLayerPart_Draw(const char* name) {
	NJS_OBJECT *obj = (NJS_OBJECT*)OmegaMdls[3]->getdata(name);
	DrawModel_QueueVisible(obj->basicdxmodel, QueuedModelFlagsB_EnableZWrite, 1);
}

inline bool DrawGammaLayerPart(NJS_OBJECT* object, const char* name) {
	if (IsSameObject(object, name)) {
		DrawGammaLayerPart_Draw(name);
		return true;
	}
	return false;
}

void OmegaCallback(NJS_OBJECT* object) {
	if (IsSameObject(object, "Dummy037")) {
		DrawGammaLayerPart_Draw("Dummy037");
		memcpy(OmegaMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		return;
	}
	
	if (IsSameObject(object, "Dummy022")) {
		DrawGammaLayerPart_Draw("Dummy022");
		memcpy(OmegaMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		return;
	}

	if (IsSameObject(object, "Dummy012")) {
		DrawGammaLayerPart_Draw("Dummy012");
		memcpy(OmegaMatrices[2], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		return;
	}
	
	if (IsSameObject(object, "Dummy011")) {
		DrawGammaLayerPart_Draw("Dummy011");
		memcpy(OmegaMatrices[3], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		return;
	}

	if (DrawGammaLayerPart(object, "Dummy008")) return;
	if (DrawGammaLayerPart(object, "Dummy010")) return;
	if (DrawGammaLayerPart(object, "Dummy011")) return;
	if (DrawGammaLayerPart(object, "Dummy012")) return;
	if (DrawGammaLayerPart(object, "Dummy013")) return;
	if (DrawGammaLayerPart(object, "Dummy014")) return;
	if (DrawGammaLayerPart(object, "Dummy018")) return;
	if (DrawGammaLayerPart(object, "Dummy019")) return;
	if (DrawGammaLayerPart(object, "Dummy021")) return;
	if (DrawGammaLayerPart(object, "Dummy022")) return;
	if (DrawGammaLayerPart(object, "Dummy033")) return;
	if (DrawGammaLayerPart(object, "Dummy034")) return;
	if (DrawGammaLayerPart(object, "Dummy036")) return;
	if (DrawGammaLayerPart(object, "Dummy037")) return;
	if (DrawGammaLayerPart(object, "Dummy047")) return;
	if (DrawGammaLayerPart(object, "Dummy050")) return;
	if (DrawGammaLayerPart(object, "Dummy052")) return;
	if (DrawGammaLayerPart(object, "Dummy053")) return;
	if (DrawGammaLayerPart(object, "Dummy054")) return;
	if (DrawGammaLayerPart(object, "Dummy055")) return;
	if (DrawGammaLayerPart(object, "Dummy057")) return;
	if (DrawGammaLayerPart(object, "Dummy058")) return;
	if (DrawGammaLayerPart(object, "Dummy059")) return;
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

	*NodeCallbackFuncPtr = OmegaCallback;
	njAction(OmegaAnimData[omegaobj->Data1->Index].Animation, omegaobj->Data1->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	switch (omegaobj->Data1->Index) {
	case 7: case 19:
		for (uint8_t i = 2; i < 4; ++i) {
			memcpy(_nj_current_matrix_ptr_, OmegaMatrices[i], sizeof(NJS_MATRIX));
			DrawModel_QueueVisible(OmegaMdls[2]->getmodel()->basicdxmodel, QueuedModelFlagsB_EnableZWrite, 1);
		}
		
		break;
	}

	if (omegaobj->Data1->Index == 9 || (omegaobj->Data1->Index == 11 && omegaobj->Data1->Scale.x > 60) || obj->Data1->Action == 19 || obj->Data1->Action == 20) {
		for (uint8_t i = 23; i < 47; ++i) {
			if (i > 32 && i < 38) continue;
			std::string str = "Dummy0";
			str = str + std::to_string(i);
			NJS_OBJECT* mdl = (NJS_OBJECT*)OmegaMdls[0]->getdata(str);
			mdl->evalflags |= NJD_EVAL_HIDE;
		}

		memcpy(_nj_current_matrix_ptr_, OmegaMatrices[0], sizeof(NJS_MATRIX));
		njTranslate(0, -3.5f, 0, 0);
		njScale(0, 1, 0.8f, 0.8f);
		if (obj->Data1->Action == 19 || obj->Data1->Action == 20) {
			njRotateX(0, data2->field_38);
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->basicdxmodel);
		}
		else {
			njTranslate(0, 2, 0, 0);
			njRotateX(0, data2->field_38);
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->child->child->basicdxmodel);
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->child->child->child->basicdxmodel);
			memcpy(OmegaMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		}

		memcpy(_nj_current_matrix_ptr_, OmegaMatrices[1], sizeof(NJS_MATRIX));
		njRotateY(0, 0x8000);
		njTranslate(0, -3.5f, 0, 0);
		njScale(0, 1, 0.8f, 0.8f);
		if (obj->Data1->Action == 19 || obj->Data1->Action == 20) {
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->basicdxmodel);
			njRotateX(0, data2->field_38);
		}
		else {
			njTranslate(0, 2, 0, 0);
			njRotateX(0, data2->field_38);
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->child->child->basicdxmodel);
			njDrawModel_SADX(OmegaMdls[1]->getmodel()->child->child->child->basicdxmodel);
			memcpy(OmegaMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX));
		}
	}
	else {
		NJS_OBJECT* mdl = (NJS_OBJECT*)OmegaMdls[0]->getdata("Dummy038");
		if (mdl->evalflags & NJD_EVAL_HIDE) {
			for (uint8_t i = 23; i < 47; ++i) {
				if (i > 32 && i < 38) continue;
				std::string str = "Dummy0";
				str = str + std::to_string(i);
				mdl = (NJS_OBJECT*)OmegaMdls[0]->getdata(str);
				mdl->evalflags &= ~NJD_EVAL_HIDE;
			}
		}
	}

	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void OmegaDrawMissiles(float y, float z) {
	for (uint8_t j = 0; j < 2; ++j) {
		memcpy(_nj_current_matrix_ptr_, OmegaMatrices[j], sizeof(NJS_MATRIX));
		njRotateY(0, 0x8000);
		njTranslate(0, 7, y, z);
		if (FrameCounterUnpaused % 3 == 0) njTranslate(0, 5, 0, 0);
		njRotateZ(0, 0xC000);
		njScale(0, 0.02f, 0.2f, 0.02f);
		for (uint8_t i = 0; i < 5; ++i) {
			njDrawModel_SADX(OmegaMdls[2]->getmodel()->basicdxmodel);
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

	if (!CharactersCommon_Init(obj, "omega", &OMEGA_TEXLIST)) {
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
			CON_REGULAR_TEXNAMES[15].texaddr = OMEGA_TEXLIST.textures[14].texaddr;
		}

		if (CustomPhysics) {
			playerdata->CollisionInfo->CollisionArray->scale.x = 8;
			playerco2->PhysicsData.CollisionSize = 8;
			playerco2->PhysicsData.RippleSize = 0.5f;

			playerco2->PhysicsData.HSpeedCap = 15;
			playerco2->PhysicsData.MaxAccel = 3.5f;
			playerco2->PhysicsData.field_14 = 0.85f;
		}

		data->Rotation.z = 1;
	}

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = OmegaHeroes_Display;

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

		NJS_VECTOR anim = PowerAnims(data, playerdata, playerco2); //id, speed, state

		PlayHeroesAnimation(obj, anim.x, OmegaAnimData, anim.y, anim.z);

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

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 5) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadOmegaFiles() {
	OmegaMdls[0] = LoadCharacterModel("omega_main");
	OmegaMdls[1] = LoadCharacterModel("omega_guns");
	OmegaMdls[2] = LoadCharacterModel("omega_effects");
	OmegaMdls[3] = LoadCharacterModel("omega_overlay");

	OmegaAnms[0] = LoadCharacterAnim("OM_WALK");
	OmegaAnms[1] = LoadCharacterAnim("OM_WALK_PULL");
	OmegaAnms[2] = LoadCharacterAnim("OM_WALK_PUSH");
	OmegaAnms[3] = LoadCharacterAnim("OM_TURN_L");
	OmegaAnms[4] = LoadCharacterAnim("OM_TURN_R");
	OmegaAnms[5] = LoadCharacterAnim("OM_SLOW_RUN");
	OmegaAnms[6] = LoadCharacterAnim("OM_MID_RUN");
	OmegaAnms[7] = LoadCharacterAnim("OM_TOP_RUN");
	OmegaAnms[8] = LoadCharacterAnim("OM_START");
	OmegaAnms[9] = LoadCharacterAnim("OM_ATC_GUN");
	OmegaAnms[10] = LoadCharacterAnim("OM_ATC_A");
	OmegaAnms[11] = LoadCharacterAnim("OM_ATC_A");
	OmegaAnms[12] = LoadCharacterAnim("OM_ATC_A");
	OmegaAnms[13] = LoadCharacterAnim("OM_JUMP_A");
	OmegaAnms[14] = LoadCharacterAnim("OM_JUMP_B");
	OmegaAnms[15] = LoadCharacterAnim("OM_JUMP_C");
	OmegaAnms[16] = LoadCharacterAnim("OM_JUMP_D");
	OmegaAnms[17] = LoadCharacterAnim("OM_JUMP_E");
	OmegaAnms[18] = LoadCharacterAnim("OM_JUMP_F");
	OmegaAnms[19] = LoadCharacterAnim("OM_JUMP_TRNGL");
	OmegaAnms[20] = LoadCharacterAnim("OM_JUMP_GLIND");
	OmegaAnms[21] = LoadCharacterAnim("OM_GLIND");
	OmegaAnms[22] = LoadCharacterAnim("OM_GLIND_BK");
	OmegaAnms[23] = LoadCharacterAnim("OM_GLIND_BK_L");
	OmegaAnms[24] = LoadCharacterAnim("OM_GLIND_BK_R");
	OmegaAnms[25] = LoadCharacterAnim("OM_GLIND_FLIP_B");
	OmegaAnms[26] = LoadCharacterAnim("OM_GLIND_FLIP_F");
	OmegaAnms[27] = LoadCharacterAnim("OM_GLIND_L");
	OmegaAnms[28] = LoadCharacterAnim("OM_GLIND_R");
	OmegaAnms[29] = LoadCharacterAnim("OM_FLY_IDLE");
	OmegaAnms[30] = LoadCharacterAnim("OM_FLY_SLOW");
	OmegaAnms[31] = LoadCharacterAnim("OM_FLY_GLIND");
	OmegaAnms[32] = LoadCharacterAnim("OM_HANG_OFF");
	OmegaAnms[33] = LoadCharacterAnim("OM_HANG_ON");
	OmegaAnms[34] = LoadCharacterAnim("OM_BREAK_A");
	OmegaAnms[35] = LoadCharacterAnim("OM_BREAK_B");
	OmegaAnms[36] = LoadCharacterAnim("OM_BREAK_C");
	OmegaAnms[37] = LoadCharacterAnim("OM_BREAK_TURN_L");
	OmegaAnms[38] = LoadCharacterAnim("OM_BREAK_TURN_R");
	OmegaAnms[39] = LoadCharacterAnim("OM_BRA_MID");
	OmegaAnms[40] = LoadCharacterAnim("OM_BRA_TOP");
	OmegaAnms[41] = LoadCharacterAnim("OM_FLORT");
	OmegaAnms[42] = LoadCharacterAnim("OM_DAM_M_A");
	OmegaAnms[43] = LoadCharacterAnim("OM_DAM_M_B");
	OmegaAnms[44] = LoadCharacterAnim("OM_DAM_M_C");
	OmegaAnms[45] = LoadCharacterAnim("OM_EDGE_OTTO_A");
	OmegaAnms[46] = LoadCharacterAnim("OM_EDGE_OTTO_B");
	OmegaAnms[47] = LoadCharacterAnim("OM_EDGE_OTTO_C");
	OmegaAnms[48] = LoadCharacterAnim("OM_EDGE_OTTO_C");
	OmegaAnms[49] = LoadCharacterAnim("OM_FW_JUMP");
	OmegaAnms[50] = LoadCharacterAnim("OM_TRAP_JUMP");
	OmegaAnms[51] = LoadCharacterAnim("HERO_KN");
	OmegaAnms[52] = LoadCharacterAnim("OM_WIN");
	OmegaAnms[53] = LoadCharacterAnim("OM_ATC_GUN");
	OmegaAnms[54] = LoadCharacterAnim("OM_IDLE");
	OmegaAnms[55] = LoadCharacterAnim("OM_IDLE_C");
	OmegaAnms[56] = LoadCharacterAnim("OM_IDLE_D");
	OmegaAnms[57] = LoadCharacterAnim("OM_BOB");
	OmegaAnms[58] = LoadCharacterAnim("OM_BOB_L");
	OmegaAnms[59] = LoadCharacterAnim("OM_BOB_R");

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