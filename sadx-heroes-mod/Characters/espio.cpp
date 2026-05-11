#include "stdafx.h"
#include "ArchiveX.h"
#include "mod.h"
#include "utils.h"
#include "sounds.h"
#include "characters.h"

ModelInfo* EspioMdls[4];
AnimationFile* EspioAnms[59];
AnimData EspioAnimData[59];

NJS_TEXNAME ESPIO_TEXNAMES[6];
NJS_TEXLIST ESPIO_TEXLIST = { arrayptrandlength(ESPIO_TEXNAMES) };

NJS_MATRIX EspioMatrix;

CollisionData Ninja_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, 3.5, 0.0f, 0.0f };

void PlayVoice_Espio(int ID) {
	switch (ID) {
	case 1498:
	case 1495:
		PlayHeroesSound(EspioSound_Win);
		break;
	}
}

void PlaySound_Espio(int ID) {
	int random = rand() % 10;

	switch (ID) {
	case 17:
		PlayHeroesSound(EspioSound_Attack);
		break;
	case 762:
		PlayHeroesSound(CommonSound_HomingAttack);
		break;
	case 1232:
		PlayHeroesSound(EspioSound_Hurt2);
		break;
	case 1233:
		PlayHeroesSound(EspioSound_Hurt1); 
		break;
	case 1503:
		PlayHeroesSound(EspioSound_Death);
		break;
	}
}

void EspioCallback(NJS_OBJECT* object) {
	NJS_OBJECT* base = EspioMdls[0]->getmodel();

	if (object == base->getnode(31)) {
		njGetMatrix(EspioMatrix); //eyelids
	}
}

void NinjaObj(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	NJS_VECTOR dir = { 0, 0, 0 };

	if (GameState != 16) {
		switch (data->Action) {
		case 0:
			obj->DisplaySub = obj->MainSub;
			data->Action = 1;

			dir.x = 10;
			dir.y = 5;
			njPushMatrix(_nj_unit_matrix_);
			njTranslateV(0, &data->Position);
			njRotateZ(0, data->Rotation.z);
			njRotateX(0, data->Rotation.x);
			njRotateY(0, -data->Rotation.y);
			njCalcPoint(0, &dir, &data->Position);
			njPopMatrix(1u);

			Collision_Init(obj, &Ninja_Col, 1, 3);
			break;
		case 1:
			dir.x = 5;
			njPushMatrix(_nj_unit_matrix_);
			njTranslateV(0, &data->Position);
			njRotateZ(0, data->Rotation.z);
			njRotateX(0, data->Rotation.x);
			njRotateY(0, -data->Rotation.y);
			njCalcPoint(0, &dir, &data->Position);
			njPopMatrix(1u);

			if (++data->InvulnerableTime > 300) {
				DeleteObject_(obj);
				return;
			}

			data->Scale.y -= 0x1000;
			AddToCollisionList(data);

			break;
		}
	}

	if (!MissedFrames) {
		njSetTexture(&ESPIO_TEXLIST);

		njPushMatrix(0);
		njTranslateV(0, &data->Position);
		njRotateZ(0, data->Rotation.z);
		njRotateX(0, data->Rotation.x);
		njRotateY(0, data->Scale.y);
		njScale(0, 0.7f, 0.7f, 0.7f);
		SetupWorldMatrix();
		Direct3D_SetChunkModelRenderState();
		dsDrawModel(EspioMdls[2]->getmodel()->getbasicdxmodel());
		Direct3D_UnsetChunkModelRenderState();
		njPopMatrix(1);
	}
}

void EspioHeroes_Display(ObjectMaster *obj) {
	if (MissedFrames) return;
	
	ObjectMaster* espioobj = HeroesChars[obj->Data1->CharIndex];
	if (!espioobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 16 || entity1->Action == 86) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (espioobj->Data1->NextAction == 0) espioobj->Data1->NextAction = 1;
			else espioobj->Data1->NextAction = 0;
		}
	}
	else {
		espioobj->Data1->NextAction = 0;
	}

	if (espioobj->Data1->NextAction) return;

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&ESPIO_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000);

	njRotateX(0, 0x4000);

	*NodeCallbackFuncPtr = EspioCallback;
	njActionWeight(EspioAnimData[espioobj->Data1->Index].Animation, espioobj->Data1->Scale.x, EspioMdls[0]->getweightinfo());
	*NodeCallbackFuncPtr = nullptr;

	njSetMatrix(NULL, EspioMatrix);
	NJS_CNK_OBJECT* pupils = EspioMdls[1]->getmodel();
	switch (espioobj->Data1->InvulnerableTime) {
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

void EspioHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;

	if (!CharactersCommon_Init(obj, "heroes-espio", &ESPIO_TEXLIST)) {
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	if (data->Rotation.z == 0) {
		if (data->CharIndex == 0) {
			CON_REGULAR_TEXNAMES[12].texaddr = ESPIO_TEXLIST.textures[1].texaddr;
		}

		if (CustomPhysics) {
			playerco2->PhysicsData.HSpeedCap = 17;
			playerco2->PhysicsData.MaxAccel = 3.6f;
			playerco2->PhysicsData.field_14 = 0.95f;
		}

		data->Rotation.z = 1;
	}

	CharactersCommon_DrawBall(playerdata, data);

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = EspioHeroes_Display;

		if (CanDoTricks(playerdata)) {
			if (HeldButtons2[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				playerdata->Action = 5;
				PlayHeroesSound(EspioSound_Ya);
				data->Action = 3;
				break;
			}

			if (data->Index == 14 && (playerdata->Status & Status_Ground) != Status_Ground && PressedButtons[data->CharIndex] & Buttons_X) {
				data->field_A = 0;
				PlayHeroesSound(EspioSound_Attack);
				data->Action = 4;
			}
		}

		if (FrameCounterUnpaused % 20 == 0) data->field_A = 0;

		if (playerco2->IdleTime > 1000) {
			if (rand() % 2 == 0) {
				PlayHeroesSound(EspioSound_Idle1);
				playerco2->AnimationThing.Index = 4;
			}
			else {
				PlayHeroesSound(EspioSound_Idle2);
				playerco2->AnimationThing.Index = 4;
			}
			playerco2->IdleTime = 0;
		}

		{
			NJS_VECTOR anim = SpeedAnims(data, playerdata, playerco2); //id, speed, state
			PlayHeroesAnimation(obj, anim.x, EspioAnimData, anim.y, anim.z);
		}

		break;
	case 3:
		if (KickTrick(data, data2, playerco2, playerdata)) {
			ObjectMaster* obj = LoadObject(LoadObj_Data1, 3, NinjaObj);
			obj->Data1->Position = playerdata->Position;
			obj->Data1->Rotation = playerdata->Rotation;
			playerco2->Speed.x = 2;
		}

		PlayHeroesAnimation(obj, 53, EspioAnimData, 0, 0);
		break;
	case 4:
		TornadoTrick(data, data2, playerco2, playerdata);
		PlayHeroesAnimation(obj, 14, EspioAnimData, 0, 0);
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

void LoadEspioFiles() {
	ArchiveX arc(HelperFunctionsGlobal.GetReplaceablePath("system\\heroes-espio.arcx"));

	EspioMdls[0] = arc.GetModel("ESPIO_LOCATOR.sa1mdl");
	EspioMdls[1] = arc.GetModel("ESPIO_MABUTA.sa1mdl");
	EspioMdls[2] = arc.GetModel("SYURUKEN.sa1mdl");
	EspioMdls[3] = arc.GetModel("KUNAI.sa1mdl");

	HelperFunctionsGlobal.Weights->Init(EspioMdls[0]->getweightinfo(), EspioMdls[0]->getmodel());

	EspioAnms[0] = arc.GetAnimation("ES_WALK.saanim");
	EspioAnms[1] = arc.GetAnimation("ES_WALK_PULL.saanim");
	EspioAnms[2] = arc.GetAnimation("ES_WALK_PUSH.saanim");
	EspioAnms[3] = arc.GetAnimation("ES_TURN_L.saanim");
	EspioAnms[4] = arc.GetAnimation("ES_TURN_R.saanim");
	EspioAnms[5] = arc.GetAnimation("ES_SLOW_RUN.saanim");
	EspioAnms[6] = arc.GetAnimation("ES_MID_RUN.saanim");
	EspioAnms[7] = arc.GetAnimation("ES_TOP_RUN.saanim");
	EspioAnms[8] = arc.GetAnimation("ES_START.saanim");
	EspioAnms[9] = arc.GetAnimation("ES_JUMP_WALL.saanim");
	EspioAnms[10] = arc.GetAnimation("ES_JUMP_TRIC_A.saanim");
	EspioAnms[11] = arc.GetAnimation("ES_JUMP_TRIC_B.saanim");
	EspioAnms[12] = arc.GetAnimation("ES_JUMP_TRIC_C.saanim");
	EspioAnms[13] = arc.GetAnimation("ES_JUMP_A.saanim");
	EspioAnms[14] = arc.GetAnimation("ES_JUMP_B.saanim");
	EspioAnms[15] = arc.GetAnimation("ES_JUMP_C.saanim");
	EspioAnms[16] = arc.GetAnimation("ES_JUMP_D.saanim");
	EspioAnms[17] = arc.GetAnimation("ES_JUMP_E.saanim");
	EspioAnms[18] = arc.GetAnimation("ES_JUMP_F.saanim");
	EspioAnms[19] = arc.GetAnimation("ES_JUMP_GUM.saanim");
	EspioAnms[20] = arc.GetAnimation("ES_JUMP_GLIND.saanim");
	EspioAnms[21] = arc.GetAnimation("ES_GLIND.saanim");
	EspioAnms[22] = arc.GetAnimation("ES_GLIND_BK.saanim");
	EspioAnms[23] = arc.GetAnimation("ES_GLIND_BK_L.saanim");
	EspioAnms[24] = arc.GetAnimation("ES_GLIND_BK_R.saanim");
	EspioAnms[25] = arc.GetAnimation("ES_GLIND_FLIP_B.saanim");
	EspioAnms[26] = arc.GetAnimation("ES_GLIND_FLIP_FR.saanim");
	EspioAnms[27] = arc.GetAnimation("ES_GLIND_L.saanim");
	EspioAnms[28] = arc.GetAnimation("ES_GLIND_R.saanim");
	EspioAnms[29] = arc.GetAnimation("ES_FLY_IDLE.saanim");
	EspioAnms[30] = arc.GetAnimation("ES_FLY_SLOW.saanim");
	EspioAnms[31] = arc.GetAnimation("ES_FLY_GLIND.saanim");
	EspioAnms[32] = arc.GetAnimation("ES_HANG_OFF.saanim");
	EspioAnms[33] = arc.GetAnimation("ES_HANG_ON.saanim");
	EspioAnms[34] = arc.GetAnimation("ES_BREAK_A.saanim");
	EspioAnms[35] = arc.GetAnimation("ES_BREAK_B.saanim");
	EspioAnms[36] = arc.GetAnimation("ES_BREAK_C.saanim");
	EspioAnms[37] = arc.GetAnimation("ES_BREAK_TURN_L.saanim");
	EspioAnms[38] = arc.GetAnimation("ES_BREAK_TURN_R.saanim");
	EspioAnms[39] = arc.GetAnimation("ES_BRA_MID.saanim");
	EspioAnms[40] = arc.GetAnimation("ES_BRA_TOP.saanim");
	EspioAnms[41] = arc.GetAnimation("ES_FLORT.saanim");
	EspioAnms[42] = arc.GetAnimation("ES_DAM_MID_A.saanim");
	EspioAnms[43] = arc.GetAnimation("ES_DAM_MID_B.saanim");
	EspioAnms[44] = arc.GetAnimation("ES_DAM_MID_C.saanim");
	EspioAnms[45] = arc.GetAnimation("ES_EDDGE_OTTO_A.saanim");
	EspioAnms[46] = arc.GetAnimation("ES_EDDGE_OTTO_B.saanim");
	EspioAnms[47] = arc.GetAnimation("ES_EDDGE_OTTO_C.saanim");
	EspioAnms[48] = arc.GetAnimation("ES_EDDGE_OTTO_C.saanim");
	EspioAnms[49] = arc.GetAnimation("ES_FW_JUMP.saanim");
	EspioAnms[50] = arc.GetAnimation("ES_TRAP_JUMP.saanim");
	EspioAnms[51] = arc.GetAnimation("ES_POW_ROT.saanim");
	EspioAnms[52] = arc.GetAnimation("ES_WIN.saanim");
	EspioAnms[53] = arc.GetAnimation("ES_ATC_SHURI.saanim");
	EspioAnms[54] = arc.GetAnimation("ES_IDLE_HALF.saanim");
	EspioAnms[55] = arc.GetAnimation("ES_IDLE_B_HALF.saanim");
	EspioAnms[56] = arc.GetAnimation("ES_IDLE_C_HALF.saanim");
	EspioAnms[57] = arc.GetAnimation("CAO_ES.saanim");
	EspioAnms[58] = arc.GetAnimation("ES_KONOHA.saanim");

	for (uint8_t i = 0; i < LengthOfArray(EspioAnimData); ++i) {
		if (EspioAnms[i] == nullptr) continue;
		EspioAnimData[i].Animation = new NJS_ACTION;
		EspioAnimData[i].Animation->object = EspioMdls[0]->getmodel();
		EspioAnimData[i].Animation->motion = EspioAnms[i]->getmotion();
		EspioAnimData[i].NextAnim = i;
		EspioAnimData[i].AnimationSpeed = 0.5f;
	}

	EspioAnimData[13].NextAnim = 14;
	EspioAnimData[15].NextAnim = 16;
	EspioAnimData[18].NextAnim = 19;
	EspioAnimData[42].Property = 1;
	EspioAnimData[43].Property = 1;
	EspioAnimData[27].Property = 1;
	EspioAnimData[28].Property = 1;
	EspioAnimData[20].Property = 1;
	EspioAnimData[25].Property = 1;
	EspioAnimData[26].Property = 1;
	EspioAnimData[44].NextAnim = 17;
}

void UnloadEspioFiles() {
	HelperFunctionsGlobal.Weights->DeInit(EspioMdls[0]->getweightinfo(), EspioMdls[0]->getmodel());
	FreeMDLFiles(EspioMdls, LengthOfArray(EspioMdls));
	FreeANMFiles(EspioAnms, LengthOfArray(EspioAnms));
}