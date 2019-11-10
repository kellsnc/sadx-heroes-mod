#include "stdafx.h"
#include "mod.h"

ModelInfo* CreamMdls[4];
AnimationFile* CreamAnms[63];
AnimData CreamAnimData[63];
bool CreamEnabled = false;
bool CreamLoaded;
bool Creams[8];

enum CharactersHeroes
{
	Characters_Cream = 9,
};

NJS_TEXNAME CREAM_TEXNAMES[4];
NJS_TEXLIST CREAM_TEXLIST = { arrayptrandlength(CREAM_TEXNAMES) };

#pragma region Chunk stuff
FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
D3DMATRIX WorldMatrixBackup;

void DrawChunkModel(NJS_CNK_MODEL* model)
{
	DrawChunkModel_(model->vlist, model->plist);
}

void DrawTransformedChunkObject(void* a1)
{
	D3DMATRIX tmp = WorldMatrix;
	WorldMatrix = WorldMatrixBackup;
	Direct3D_SetWorldTransform();
	DrawChunkObject((NJS_CNK_OBJECT*)a1);
	WorldMatrix = tmp;
}

void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags)
{
	DisplayAnimationFrame(action, frame, flags, 0, (void(__cdecl*)(NJS_MODEL_SADX*, int, int))DrawChunkModel);
}

void njCnkAction(NJS_ACTION* action, float frame)
{
	DisplayAnimationFrame(action, frame, (QueuedModelFlagsB)0, 0, (void(__cdecl*)(NJS_MODEL_SADX*, int, int))DrawChunkModel);
}

void SetupWorldMatrix()
{
	ProjectToWorldSpace();
	WorldMatrixBackup = WorldMatrix;
	Direct3D_SetWorldTransform();
	memcpy(_nj_current_matrix_ptr_, &ViewMatrix, sizeof(NJS_MATRIX));
}
#pragma endregion

#define LOWORD(l) ((WORD)(l))

void CreamHeroes_Delete(ObjectMaster *obj) {
	njReleaseTexture(&CREAM_TEXLIST);
	obj->Child = nullptr;
}

void CreamHeroes_Display(ObjectMaster *obj) {
	if (!obj->Parent) return;
	
	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* co2 = entity2->CharacterData;

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -LOWORD(entity1->Rotation.y) - 0x4000);

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	njCnkAction(CreamAnimData[obj->Parent->Data1->Index].Animation, obj->Parent->Data1->Scale.x);

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

void Tails_Display_(ObjectMaster* obj) {
	if (Creams[obj->Data1->CharIndex]) {
		CreamHeroes_Display(obj);
	}
	else {
		Tails_Display(obj);
	}
}

void CreamHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		obj->DeleteSub = CreamHeroes_Delete;
		LoadPVM("cream", &CREAM_TEXLIST);
		data->Action = 1;
		return;
	}
	else if (data->Action == 1) {
		data->Action = 2;

		PlayerPtrs[data->CharIndex]->Parent = obj;
		obj->Child = PlayerPtrs[data->CharIndex];
	}

	if (!obj->Child) DeleteObject_(obj);

	EntityData1* playerdata = obj->Child->Data1;
	EntityData2* playerdata2 = (EntityData2*)obj->Child->Data2;
	CharObj2* playerco2 = playerdata2->CharacterData;
	
	switch (data->Action) {
	case 2:
		PlayerPtrs[data->CharIndex]->DisplaySub = CreamHeroes_Display;
		EntityData1Ptrs[1]->Position = { -8771, 1303, -2819.688f };
		EntityData1Ptrs[1]->Rotation = { 0, 0, 0 };
		data->Index = 1;
		data->Scale.x += 0.5f;

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
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions) {
	const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
	CreamEnabled = config->getBool("Characters", "EnableCream", false);
	delete config;

	CreamEnabled = true;

	if (CreamEnabled) {
		LoadCreamFiles(path, helperFunctions);
		WriteCall((void*)0x462456, Tails_Display_);
	}
}

void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (GetCharacterID(player) == Characters_Tails && CreamEnabled) {
			if (Creams[player] == false) {
				LoadObject(LoadObj_Data1, 1, CreamHeroes_Main)->Data1->CharIndex = player;
				Creams[player] = true;
			}
		}
	}
}
