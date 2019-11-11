#include "stdafx.h"
#include "mod.h"
#include "characters.h"

ModelInfo* CreamMdls[4];
AnimationFile* CreamAnms[63];
AnimData CreamAnimData[63];

ObjectMaster* Creams[8];

NJS_TEXNAME CREAM_TEXNAMES[4];
NJS_TEXLIST CREAM_TEXLIST = { arrayptrandlength(CREAM_TEXNAMES) };

void CreamHeroes_Delete(ObjectMaster *obj) {
	bool IsThereCream = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (Creams[player]) IsThereCream = true;
		}
	}

	if (!IsThereCream) {
		njReleaseTexture(&CREAM_TEXLIST);
		CharTexsLoaded[9 - Characters_Cream] = false;
	}

	Creams[obj->Data1->CharIndex] = nullptr;
}

void CreamHeroes_Display(ObjectMaster *obj) {
	ObjectMaster* creamobj = Creams[obj->Data1->CharIndex];
	if (!creamobj) return;

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	if (entity1->Action == 13 || entity1->Action == 60) {
		if (FrameCounterUnpaused % 5 == 0) {
			if (creamobj->Data1->NextAction == 0) creamobj->Data1->NextAction = 1;
			else creamobj->Data1->NextAction = 0;
		}
	}

	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -entity1->Rotation.y - 0x4000 + creamobj->Data1->Rotation.y);

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	if (!creamobj->Data1->NextAction) {
		njCnkAction(CreamAnimData[creamobj->Data1->Index].Animation, creamobj->Data1->Scale.x);
	}

	njTranslate(0, 3, 8, 2);
	DrawChunkObject(CreamMdls[2]->getmodel());
	njTranslate(0, 0, 5, 0);
	DrawChunkObject(CreamMdls[3]->getmodel());

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CreamHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DeleteSub = CreamHeroes_Delete;
		data->CharID = Characters_Cream;
		data->Action = 1;

		if (!CharTexsLoaded[9 - Characters_Cream]) {
			CharTexsLoaded[9 - Characters_Cream] = true;
			LoadPVM("cream", &CREAM_TEXLIST);
		}

		return;
	}
	else if (data->Action == 1) {
		data->Action = 2;
		return;
	}

	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];
	if (!playerobj || playerobj->Data1->CharID != Characters_Tails) DeleteObject_(obj);

	EntityData1* playerdata = playerobj->Data1;
	EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;

	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = CreamHeroes_Display;

		int anim = data->Index;
		float speed = 0;
		float state = 0;
		float frame = data->Scale.x;

		if (anim == 19 && playerco2->AnimationThing.Index < 6) {
			anim = 20; //just stopped rolling
		}

		if (anim != 20) {
			switch (playerco2->AnimationThing.Index) {
			case 0: anim = 11; break;
			case 2: anim = 8; break;
			case 4:
			case 5:
			case 6:
				if (data->Index > 8 && frame == 0.5f && !data->Status) {
					data->Status = rand() % 2;
				}
				anim = 9 + data->Status;

				if (data->Index > 8 && frame > 58) {
					playerco2->AnimationThing.Index = 0;
					data->Status = 0;
				}
				break;
			case 9:
				anim = 0;
				if (playerco2->Speed.x < 0.02f) anim = 11;
				break;
			case 10:
				anim = 0;
				speed = 0.9f;
				break;
			case 11:
				anim = 0;
				speed = 1.5f;
				break;
			case 12:
				anim = 5;
				break;
			case 13:
				anim = 6;
				speed = 0.5f + playerco2->Speed.x * 0.1f;
				break;
			case 14: //jumping
				if (anim < 18 || anim > 23) {
					anim = 18;
				}
				else if (anim == 19) {
					if (data->Unknown > 2) {
						anim = 20;
					}
				}
				else if (anim == 21) {
					if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 22;
				}
				break;
			case 15: //rolling
			case 16:
				anim = 19;
				break;
			case 17: //spring jump
				anim = 19;
				break;
			case 18: //fall after spring jump
				anim = 21;
				break;
			case 19: //falling
				anim = 21;
				if (playerco2->Speed.x > 8 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500) {
					anim = 16;
					playerco2->AnimationThing.Index = 150;
				}
				break;
			case 20:
				anim = 23;
				break;
			case 21: //break
				anim = 47;
				if (playerco2->Speed.x > 6) {
					anim = 46;
				}
				else if (playerco2->Speed.x > 3) {
					anim = 45;
				}
				break;
			case 22:
				anim = 48;
				break;
			case 24:
				anim = 2;
				speed = 0;
				break;
			case 25:
				anim = 2;
				break;
			case 26:
				anim = 54;
				break;
			case 27:
				anim = 56;
				break;
			case 28: //hurt
			case 29:
			case 30:
			case 31:
				anim = 55;
				break;
			case 32:
				anim = 45;
				break;
			case 33: //thumbling
			case 34:
				anim = 53;
				break;
			case 35:
				anim = 55;
				break;
			case 36:
				anim = 11;
				break;
			case 37: //flying
			case 40:
			case 41:
				anim = 34; //52 IF HOLDING PLAYER
				if (HeldButtons2[data->CharIndex] & Buttons_A) speed = 0.8;
				speed += playerco2->Speed.x * 0.5f;
				break;
			case 38: //flying falling
			case 39:
			case 42:
				anim = 35;
				break;
			case 43:
				anim = 39;
				break;
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
				anim = 39;
				data->Rotation.y += 0x100;
				break;
			case 54: //won
			case 55:
				anim = 60;
				if (data->Unknown > 1) {
					if (frame > 124) data->Unknown = 20;
					if (frame < 110) data->Unknown = 40;

					if (data->Unknown > 20 && data->Unknown < 40) {
						++data->Unknown;
						state = 124 - (data->Unknown - 20);
					}
					else if (data->Unknown > 40) {
						++data->Unknown;
						state = 110 + (data->Unknown - 40);
					}
				}
				break;
			case 56: //lost
			case 57:
				anim = 10;
				break;
			case 58:
				anim = 44;
				break;
			case 72: //holding hook
				anim = 51;
				break;
			case 107: //snowboard
			case 108:
			case 109:
				anim = 26;
				break;
			case 110:
			case 111:
			case 116:
				anim = 26;
				break;
			case 112:
				if (playerco2->Speed.z > 0.1f) anim = 33;
				break;
			case 113:
				if (playerco2->Speed.z < 0.1f) anim = 32;
				break;
			case 114:
			case 115:
			case 117:
			case 118:
			case 119:
				anim = 30;
				break;
			case 120:
			case 121:
			case 122:
			case 123:
			case 124:
			case 125:
			case 126:
				anim = 31;
				break;
			case 127:
				anim = 27;
				break;
			case 150:
				anim = 16;
				break;
			}
		}

		if (anim != 39) {
			data->Rotation.y = 0;
		}
		if (anim != data->Index) {
			data->Scale.x = 0;
			data->Unknown = 0;
		}
		PlayHeroesAnimation(obj, anim, CreamAnimData, speed, state);

		break;
	}
}

void LoadCreamFiles(const char *path, const HelperFunctions &helperFunctions) {
	CreamMdls[0] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cream_main.sa2mdl"));
	CreamMdls[1] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cream_eyelashes.sa2mdl"));
	CreamMdls[2] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cheese_main.sa2mdl"));
	CreamMdls[3] = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\cheese_ball.sa2mdl"));

	CreamAnms[0] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK.saanim"));
	CreamAnms[1] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK_PULL.saanim"));
	CreamAnms[2] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WALK_PUSH.saanim"));
	CreamAnms[3] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_TURN_L.saanim"));
	CreamAnms[4] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_TURN_R.saanim"));
	CreamAnms[5] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_SLOW_RUN.saanim"));
	CreamAnms[6] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_MID_RUN.saanim"));
	CreamAnms[7] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_START.saanim"));
	CreamAnms[8] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_START_IDLE.saanim"));
	CreamAnms[9] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_B_HALF.saanim"));
	CreamAnms[10] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_C_HALF.saanim"));
	CreamAnms[11] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_HALF.saanim"));
	CreamAnms[12] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_POW_HALF.saanim"));
	CreamAnms[13] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_IDLE_POW_SHAKE.saanim"));
	CreamAnms[14] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_POW_ROT.saanim"));
	CreamAnms[15] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_POW_JUMP.saanim"));
	CreamAnms[16] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FW_JUMP.saanim"));
	CreamAnms[17] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FW_POW_JUMP.saanim"));
	CreamAnms[18] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_A.saanim"));
	CreamAnms[19] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_B.saanim"));
	CreamAnms[20] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_C.saanim"));
	CreamAnms[21] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_D.saanim"));
	CreamAnms[22] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_E.saanim"));
	CreamAnms[23] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_F.saanim"));
	CreamAnms[24] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_UMBRELLA.saanim"));
	CreamAnms[25] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_JUMP_GLIND.saanim"));
	CreamAnms[26] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND.saanim"));
	CreamAnms[27] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK.saanim"));
	CreamAnms[28] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK_L.saanim"));
	CreamAnms[29] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_BK_R.saanim"));
	CreamAnms[30] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_FLIP_BK.saanim"));
	CreamAnms[31] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_FLIP_FR.saanim"));
	CreamAnms[32] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_L.saanim"));
	CreamAnms[33] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_GLIND_R.saanim"));
	CreamAnms[34] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_IDLE.saanim"));
	CreamAnms[35] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_SLOW.saanim"));
	CreamAnms[36] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_UP.saanim"));
	CreamAnms[37] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_PULL.saanim"));
	CreamAnms[38] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_PUSH.saanim"));
	CreamAnms[39] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_KICK.saanim"));
	CreamAnms[40] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_IDLE.saanim"));
	CreamAnms[41] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_OFF.saanim"));
	CreamAnms[42] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLY_HANG_ON.saanim"));
	CreamAnms[43] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_HANG_OFF.saanim"));
	CreamAnms[44] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_HANG_ON.saanim"));
	CreamAnms[45] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_A.saanim"));
	CreamAnms[46] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_B.saanim"));
	CreamAnms[47] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_C.saanim"));
	CreamAnms[48] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_TURN_L.saanim"));
	CreamAnms[49] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BREAK_TURN_R.saanim"));
	CreamAnms[50] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BRA_MID.saanim"));
	CreamAnms[51] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BRA_TOP.saanim"));
	CreamAnms[52] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_BOB.saanim"));
	CreamAnms[53] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_FLORT.saanim"));
	CreamAnms[54] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_A.saanim"));
	CreamAnms[55] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_B.saanim"));
	CreamAnms[56] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_DAM_MID_C.saanim"));
	CreamAnms[57] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_A.saanim"));
	CreamAnms[58] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_B.saanim"));
	CreamAnms[59] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_EDGE_OTTO_C.saanim"));
	CreamAnms[60] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_WIN.saanim"));
	CreamAnms[61] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\CR_ATC_CHAO.saanim"));
	CreamAnms[62] = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath("system\\characters\\ROSE_CR.saanim"));

	for (uint8_t i = 0; i < LengthOfArray(CreamAnimData); ++i) {
		CreamAnimData[i].Animation = new NJS_ACTION;
		CreamAnimData[i].Animation->object = CreamMdls[0]->getmodel();
		CreamAnimData[i].Animation->motion = CreamAnms[i]->getmotion();
		CreamAnimData[i].Instance = 68;
		CreamAnimData[i].NextAnim = i;
		CreamAnimData[i].Property = 3;
		CreamAnimData[i].TransitionSpeed = 0.25f;
		CreamAnimData[i].AnimationSpeed = 0.5f;
	}

	CreamAnimData[9].NextAnim = 8;
	CreamAnimData[10].NextAnim = 8;
	CreamAnimData[18].NextAnim = 19;
	CreamAnimData[20].NextAnim = 21;
	CreamAnimData[22].NextAnim = 23;
	CreamAnimData[54].Property = 1;
	CreamAnimData[55].Property = 1;
	CreamAnimData[60].Property = 1;
	CreamAnimData[32].Property = 1;
	CreamAnimData[25].Property = 1;
	CreamAnimData[30].Property = 1;
	CreamAnimData[31].Property = 1;
	CreamAnimData[33].Property = 1;
	CreamAnimData[56].NextAnim = 11;
	CreamAnimData[23].AnimationSpeed = 0.25f;
}