#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* SonicMdls[2];
AnimationFile* SonicAnms[58];
AnimData HSonicAnimData[58];

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

	njRotateX(0, 0x4000);

	if (sonicobj->Data1->Index == 52) {
		njRotateY(0, 0xC000);
	}

	*NodeCallbackFuncPtr = SonicCallback;
	njActionWeight(HSonicAnimData[sonicobj->Data1->Index].Animation, sonicobj->Data1->Scale.x, SonicMdls[0]->getweightinfo());
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

	if (!CharactersCommon_Init(obj, "heroes-sonic", &HSONIC_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[12].texaddr = HSONIC_TEXLIST.textures[5].texaddr;
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
		PlayerPtrs[data->CharIndex]->DisplaySub = SonicHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(SonicSound_Ya);
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
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

		{
			NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, HSonicAnimData, anim.y, anim.z);
		}
		
		break;
	case 3:
		KickTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 53, HSonicAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, HSonicAnimData, 0, 0);
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
	SonicAnms[48] = arc.GetAnimation("SN_EDGE_OTTO_C.saanim");
	SonicAnms[49] = arc.GetAnimation("SN_FW_JUMP.saanim");
	SonicAnms[50] = arc.GetAnimation("SN_TRAP_JUMP.saanim");
	SonicAnms[51] = arc.GetAnimation("SN_POW_ROT.saanim");
	SonicAnms[52] = arc.GetAnimation("SN_WIN_B.saanim");
	SonicAnms[53] = arc.GetAnimation("SN_ATC_DASH.saanim");
	SonicAnms[54] = arc.GetAnimation("SN_IDLE.saanim");
	SonicAnms[55] = arc.GetAnimation("SN_IDLE_C.saanim");
	SonicAnms[56] = arc.GetAnimation("SN_IDLE_D.saanim");
	SonicAnms[57] = arc.GetAnimation("HERO_SN.saanim");

	for (uint8_t i = 0; i < LengthOfArray(HSonicAnimData); ++i) {
		if (SonicAnms[i] == nullptr) continue;
		HSonicAnimData[i].Animation = new NJS_ACTION;
		HSonicAnimData[i].Animation->object = SonicMdls[0]->getmodel();
		HSonicAnimData[i].Animation->motion = SonicAnms[i]->getmotion();
		HSonicAnimData[i].NextAnim = i;
		HSonicAnimData[i].AnimationSpeed = 0.5f;
	}

	HSonicAnimData[13].NextAnim = 14;
	HSonicAnimData[15].NextAnim = 16;
	HSonicAnimData[18].NextAnim = 19;
	HSonicAnimData[42].Property = 1;
	HSonicAnimData[43].Property = 1;
	HSonicAnimData[27].Property = 1;
	HSonicAnimData[28].Property = 1;
	HSonicAnimData[20].Property = 1;
	HSonicAnimData[25].Property = 1;
	HSonicAnimData[26].Property = 1;
	HSonicAnimData[44].NextAnim = 17;
	HSonicAnimData[52].Property = 1;
}

void UnloadSonicFiles() {
	HelperFunctionsGlobal.Weights->DeInit(SonicMdls[0]->getweightinfo(), SonicMdls[0]->getmodel());
	FreeMDLFiles(SonicMdls, LengthOfArray(SonicMdls));
	FreeANMFiles(SonicAnms, LengthOfArray(SonicAnms));
}