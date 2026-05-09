#include "stdafx.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* ShadowMdls[3];
AnimationFile* ShadowAnms[59];
AnimData HShadowAnimData[59];

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

	*NodeCallbackFuncPtr = ShadowCallback;
	njActionWeight(HShadowAnimData[sonicobj->Data1->Index].Animation, sonicobj->Data1->Scale.x, ShadowMdls[0]->getweightinfo());
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

	if (sonicobj->Data1->Index == 6 || sonicobj->Data1->Index == 7 || sonicobj->Data1->Index == 57) {
		for (int i = 0; i < 4; ++i) {
			njSetMatrix(0, ShadowMatrices[i + 1]);
			late_DrawObject(ShadowMdls[2]->getmodel()->getnode((i == 0 || i == 2) ? 5 : 2), LATE_MAT);
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

	if (!CharactersCommon_Init(obj, "shadow", &SHADOW_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[12].texaddr = SHADOW_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.HSpeedCap = 18;
			playerco2->PhysicsData.MaxAccel = 4.0f;
			playerco2->PhysicsData.field_14 = 0.95f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = ShadowHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(ShadowSound_Ya);
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
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

		{
			NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, HShadowAnimData, anim.y, anim.z);
		}

		break;
	case 3:
		KickTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 53, HShadowAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, HShadowAnimData, 0, 0);
		break;
	}

	if (FrameCounterUnpaused % 200 == 0) {
		data->InvulnerableTime = 1;
	}

	if (data->InvulnerableTime != 0 && FrameCounterUnpaused % 2 == 0) {
		if (++data->InvulnerableTime > 7) data->InvulnerableTime = 0;
	}

	RunObjectChildren(obj);
}

void LoadShadowFiles() {
	ShadowMdls[0] = LoadCharacterModel("shadow_main");
	ShadowMdls[1] = LoadCharacterModel("shadow_pupils");
	ShadowMdls[2] = LoadCharacterModel("shadow_air");

	HelperFunctionsGlobal.Weights->Init(ShadowMdls[0]->getweightinfo(), ShadowMdls[0]->getmodel());

	ShadowAnms[0] = LoadCharacterAnim("SH_WALK");
	ShadowAnms[1] = LoadCharacterAnim("SH_WALK_PULL");
	ShadowAnms[2] = LoadCharacterAnim("SH_WALK_PUSH");
	ShadowAnms[3] = LoadCharacterAnim("SH_TURN_L");
	ShadowAnms[4] = LoadCharacterAnim("SH_TURN_R");
	ShadowAnms[5] = LoadCharacterAnim("SH_SLOW_RUN");
	ShadowAnms[6] = LoadCharacterAnim("SH_SLOW_SKATE");
	ShadowAnms[7] = LoadCharacterAnim("SH_MID_SKATE");
	ShadowAnms[8] = LoadCharacterAnim("SH_START");
	ShadowAnms[9] = LoadCharacterAnim("SH_JUMP_WALL");
	ShadowAnms[10] = LoadCharacterAnim("SH_JUMP_TRIC_A");
	ShadowAnms[11] = LoadCharacterAnim("SH_JUMP_TRIC_B");
	ShadowAnms[12] = LoadCharacterAnim("SH_JUMP_TRIC_C");
	ShadowAnms[13] = LoadCharacterAnim("SH_JUMP_A");
	ShadowAnms[14] = LoadCharacterAnim("SH_JUMP_B");
	ShadowAnms[15] = LoadCharacterAnim("SH_JUMP_C");
	ShadowAnms[16] = LoadCharacterAnim("SH_JUMP_D");
	ShadowAnms[17] = LoadCharacterAnim("SH_JUMP_E");
	ShadowAnms[18] = LoadCharacterAnim("SH_JUMP_F");
	ShadowAnms[19] = LoadCharacterAnim("SH_JUMP_TRNGL");
	ShadowAnms[20] = LoadCharacterAnim("SH_JUMP_GLIND");
	ShadowAnms[21] = LoadCharacterAnim("SH_GLIND");
	ShadowAnms[22] = LoadCharacterAnim("SH_GLIND_BK");
	ShadowAnms[23] = LoadCharacterAnim("SH_GLIND_BK_L");
	ShadowAnms[24] = LoadCharacterAnim("SH_GLIND_BK_R");
	ShadowAnms[25] = LoadCharacterAnim("SH_GLIND_FLIP_B");
	ShadowAnms[26] = LoadCharacterAnim("SH_GLIND_FLIP_FR");
	ShadowAnms[27] = LoadCharacterAnim("SH_GLIND_L");
	ShadowAnms[28] = LoadCharacterAnim("SH_GLIND_R");
	ShadowAnms[29] = LoadCharacterAnim("SH_FLY_IDLE");
	ShadowAnms[30] = LoadCharacterAnim("SH_FLY_SLOW");
	ShadowAnms[31] = LoadCharacterAnim("SH_FLY_GLIND");
	ShadowAnms[32] = LoadCharacterAnim("SH_HANG_OFF");
	ShadowAnms[33] = LoadCharacterAnim("SH_HANG_ON");
	ShadowAnms[34] = LoadCharacterAnim("SH_BREAK_A");
	ShadowAnms[35] = LoadCharacterAnim("SH_BREAK_B");
	ShadowAnms[36] = LoadCharacterAnim("SH_BREAK_C");
	ShadowAnms[37] = LoadCharacterAnim("SH_BREAK_TURN_L");
	ShadowAnms[38] = LoadCharacterAnim("SH_BREAK_TURN_R");
	ShadowAnms[39] = LoadCharacterAnim("SH_BRA_MID");
	ShadowAnms[40] = LoadCharacterAnim("SH_BRA_TOP");
	ShadowAnms[41] = LoadCharacterAnim("SH_FLORT");
	ShadowAnms[42] = LoadCharacterAnim("SH_DAM_M_A");
	ShadowAnms[43] = LoadCharacterAnim("SH_DAM_M_B");
	ShadowAnms[44] = LoadCharacterAnim("SH_DAM_M_C");
	ShadowAnms[45] = LoadCharacterAnim("SH_EDGE_OTTO_A");
	ShadowAnms[46] = LoadCharacterAnim("SH_EDGE_OTTO_B");
	ShadowAnms[47] = LoadCharacterAnim("SH_EDGE_OTTO_C");
	ShadowAnms[48] = LoadCharacterAnim("SH_EDGE_OTTO_C");
	ShadowAnms[49] = LoadCharacterAnim("SH_FW_JUMP");
	ShadowAnms[50] = LoadCharacterAnim("SH_TRAP_JUMP");
	ShadowAnms[51] = LoadCharacterAnim("SH_POW_ROT");
	ShadowAnms[52] = LoadCharacterAnim("SH_WIN");
	ShadowAnms[53] = LoadCharacterAnim("SH_ATC_DASH");
	ShadowAnms[54] = LoadCharacterAnim("SH_IDLE_HALF");
	ShadowAnms[55] = LoadCharacterAnim("SH_IDLE_B_HALF");
	ShadowAnms[56] = LoadCharacterAnim("SH_IDLE_D_HALF");
	ShadowAnms[57] = LoadCharacterAnim("SH_TOP_SKATE");
	ShadowAnms[58] = LoadCharacterAnim("DARK_SH");

	for (uint8_t i = 0; i < LengthOfArray(HShadowAnimData); ++i) {
		if (ShadowAnms[i] == nullptr) continue;
		HShadowAnimData[i].Animation = new NJS_ACTION;
		HShadowAnimData[i].Animation->object = ShadowMdls[0]->getmodel();
		HShadowAnimData[i].Animation->motion = ShadowAnms[i]->getmotion();
		HShadowAnimData[i].NextAnim = i;
		HShadowAnimData[i].AnimationSpeed = 0.5f;
	}

	HShadowAnimData[13].NextAnim = 14;
	HShadowAnimData[15].NextAnim = 16;
	HShadowAnimData[18].NextAnim = 19;
	HShadowAnimData[42].Property = 1;
	HShadowAnimData[43].Property = 1;
	HShadowAnimData[27].Property = 1;
	HShadowAnimData[28].Property = 1;
	HShadowAnimData[20].Property = 1;
	HShadowAnimData[25].Property = 1;
	HShadowAnimData[26].Property = 1;
	HShadowAnimData[44].NextAnim = 17;
}

void UnloadShadowFiles() {
	HelperFunctionsGlobal.Weights->DeInit(ShadowMdls[0]->getweightinfo(), ShadowMdls[0]->getmodel());
	FreeMDLFiles(ShadowMdls, LengthOfArray(ShadowMdls));
	FreeANMFiles(ShadowAnms, LengthOfArray(ShadowAnms));
}