#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* AmyMdls[3];
AnimationFile* AmyAnms[62];
AnimData HAmyAnimData[62];

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

	*NodeCallbackFuncPtr = AmyCallback;
	njActionWeight(HAmyAnimData[amyobj->Data1->Index].Animation, amyobj->Data1->Scale.x, AmyMdls[0]->getweightinfo());
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
	
	if (amyobj->Data1->Index == 53 || amyobj->Data1->Index == 61) {
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

	if (!CharactersCommon_Init(obj, "heroes-amy", &HAMY_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[12].texaddr = HAMY_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.HSpeedCap = 17;
			playerco2->PhysicsData.MaxAccel = 3.8f;
			playerco2->PhysicsData.field_14 = 0.90f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = AmyHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (playerco2->Speed.x < 2 && PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(AmySound_Ya);
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
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

		{
			NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, HAmyAnimData, anim.y, anim.z);
		}

		break;
	case 3:
		if (data->field_A == 0) {
			data->Scale.x = 0;
			data->field_A = 1;
			playerco2->Powerups |= Powerups_Invincibility;
		}
		else if (data->field_A < 119) {
			++data->field_A;
			
		}
		else {
			data->field_A = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
			playerdata->Action = 2;
			data->Action = 2;
		}

		PlayHeroesAnimation(obj, 53, HAmyAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, HAmyAnimData, 0, 0);
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
	AmyAnms[48] =  arc.GetAnimation("AM_EDGE_OTTO_C.saanim");
	AmyAnms[49] =  arc.GetAnimation("AM_FW_JUMP.saanim");
	AmyAnms[50] =  arc.GetAnimation("AM_TRAP_JUMP.saanim");
	AmyAnms[51] =  arc.GetAnimation("AM_POW_ROT_HALF.saanim");
	AmyAnms[52] =  arc.GetAnimation("AM_WIN.saanim");
	AmyAnms[53] =  arc.GetAnimation("AM_ATC_HAM.saanim");
	AmyAnms[54] =  arc.GetAnimation("AM_IDLE_HALF.saanim");
	AmyAnms[55] =  arc.GetAnimation("AM_IDLE_B_HALF.saanim");
	AmyAnms[56] =  arc.GetAnimation("AM_IDLE_C_HALF.saanim");
	AmyAnms[57] =  arc.GetAnimation("ROSE_AM.saanim");
	AmyAnms[58] =  arc.GetAnimation("AM_IDLE_POW_HALF.saanim");
	AmyAnms[59] =  arc.GetAnimation("AM_IDLE_POW_SHAKE.saanim");
	AmyAnms[60] =  arc.GetAnimation("AM_START_IDLE.saanim");
	AmyAnms[61] =  arc.GetAnimation("AM_ATC_TRNADE.saanim");

	for (uint8_t i = 0; i < LengthOfArray(HAmyAnimData); ++i) {
		if (AmyAnms[i] == nullptr) continue;
		HAmyAnimData[i].Animation = new NJS_ACTION;
		HAmyAnimData[i].Animation->object = AmyMdls[0]->getmodel();
		HAmyAnimData[i].Animation->motion = AmyAnms[i]->getmotion();
		HAmyAnimData[i].NextAnim = i;
		HAmyAnimData[i].AnimationSpeed = 0.5f;
	}

	HAmyAnimData[13].NextAnim = 14;
	HAmyAnimData[15].NextAnim = 16;
	HAmyAnimData[18].NextAnim = 19;
	HAmyAnimData[42].Property = 1;
	HAmyAnimData[43].Property = 1;
	HAmyAnimData[27].Property = 1;
	HAmyAnimData[28].Property = 1;
	HAmyAnimData[20].Property = 1;
	HAmyAnimData[25].Property = 1;
	HAmyAnimData[26].Property = 1;
	HAmyAnimData[44].NextAnim = 17;
	HAmyAnimData[52].Property = 1;
}

void UnloadAmyFiles() {
	HelperFunctionsGlobal.Weights->DeInit(AmyMdls[0]->getweightinfo(), AmyMdls[0]->getmodel());
	FreeMDLFiles(AmyMdls, LengthOfArray(AmyMdls));
	FreeANMFiles(AmyAnms, LengthOfArray(AmyAnms));
}