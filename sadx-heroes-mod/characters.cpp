#include "stdafx.h"
#include "mod.h"

#include "cream.h"

bool CreamEnabled = false;
bool CreamLoaded;
bool Creams[8];

enum CharactersHeroes
{
	Characters_Cream = 9,
};

NJS_TEXNAME CREAM_TEXNAMES[4];
NJS_TEXLIST CREAM_TEXLIST = { arrayptrandlength(CREAM_TEXNAMES) };

AnimData CreamAnimData[]{
	{ nullptr, 68, 3, 0, 0.0625, 0.5 },
};

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
}

void CreamHeroes_Display(ObjectMaster *obj) {
	Direct3D_SetZFunc(1u);
	Direct3D_PerformLighting(2);

	njSetTexture(&CREAM_TEXLIST);

	njPushMatrix(0);

	EntityData1* entity1 = obj->Data1;

	njTranslateV(0, &entity1->Position);
	njRotateZ(0, entity1->Rotation.z);
	njRotateX(0, entity1->Rotation.x);
	njRotateY(0, -LOWORD(entity1->Rotation.y) - 0x4000);

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	CharObj2* co2 = CharObj2Ptrs[obj->Data1->CharIndex];
	DrawChunkObject(&Cream002);

	njTranslate(0, 3, 8, 2);
	DrawChunkObject(&Cheese002);
	njTranslate(0, 0, 5, 0);
	DrawChunkObject(&ChaoBall016);

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
	CharObj2* co2 = CharObj2Ptrs[data->CharIndex];

	switch (data->Action) {
	case 0:
		obj->DeleteSub = CreamHeroes_Delete;
		LoadPVM("cream", &CREAM_TEXLIST);
		data->Action = 1;

		break;
	case 1:
		PlayerPtrs[data->CharIndex]->DisplaySub = CreamHeroes_Display;

		break;
	}
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions) {
	const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
	CreamEnabled = config->getBool("Characters", "EnableCream", false);
	delete config;

	CreamEnabled = true;

	if (CreamEnabled) {
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
