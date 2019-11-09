#include "stdafx.h"
#include "mod.h"

#include "cream.h"

bool CreamEnabled = false;

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

FunctionPointer(void*, sub_44BB60, (EntityData1*, EntityData2*, CharObj2*), 0x44BB60);
FunctionPointer(void*, sub_44C270, (EntityData1*, EntityData2*, CharObj2*), 0x44C270);
FunctionPointer(void*, PlayerFunc_AnalogToDirection, (EntityData1*, EntityData2*, CharObj2*), 0x443F50);
FunctionPointer(void*, PlayerFunc_DetectCollisionAndSetFlags, (EntityData1*, EntityData2*, CharObj2*), 0x44CDF0);
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
	njRotateXYZ(0, entity1->Rotation.x, -LOWORD(entity1->Rotation.y) - 0x4000, entity1->Rotation.z);

	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	CharObj2* co2 = CharObj2Ptrs[obj->Data1->CharIndex];
	DrawChunkObject(&Dummy002);
	
	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);

	Direct3D_PerformLighting(0);
	ClampGlobalColorThing_Thing();
	Direct3D_ResetZFunc();
}

void CreamHeroes_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;
	CharObj2* co2 = CharObj2Ptrs[data->CharIndex];
	
	EntityData1* entity1 = obj->Data1;
	EntityData2* entity2 = (EntityData2*)obj->Data2;
	CharObj2* charobj2 = entity2->CharacterData;
	Current_CharObj1 = entity1;
	Current_CharObj2 = charobj2;
	
	switch (entity1->Action) {
	case 0:
		obj->DeleteSub = CreamHeroes_Delete;

		entity2->CharacterData = new CharObj2;
		charobj2 = entity2->CharacterData;

		EntityData1Ptrs[entity1->CharIndex] = obj->Data1;
		EntityData2Ptrs[entity1->CharIndex] = (EntityData2*)obj->Data2;
		InitCharacterVars(entity1->CharIndex, obj);
		memcpy(&charobj2->PhysicsData, &PhysicsArray[2], sizeof(charobj2->PhysicsData));
		entity1->CharID = Characters_Cream;

		LoadPVM("cream", &CREAM_TEXLIST);

		Collision_Init(obj, (CollisionData*)0x91A9E8, 5, 0);
		entity1->CollisionInfo->CollisionArray->scale.x = charobj2->PhysicsData.RippleSize;
		
		entity1->Position = { 0, 20, -120 };
		entity1->Action = 1;
		break;
	case 1:
		sub_44BB60(entity1, entity2, charobj2);
		sub_44C270(entity1, entity2, charobj2);
		PlayerFunc_AnalogToDirection(entity1, entity2, charobj2);
		PlayerFunc_DetectCollisionAndSetFlags(entity1, entity2, charobj2);
		break;
	}

	RunEntityIntersections(entity1, &charobj2->_struct_a3);
	AddToCollisionList(entity1);
	RunUnderwaterStuff(entity1, entity2, charobj2);

	obj->DisplaySub(obj);
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions) {
	const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
	CreamEnabled = config->getBool("Characters", "EnableCream", false);
	delete config;
}

void Characters_OnFrame() {
	if (GetCharacterID(1) == Characters_Tails) {
		DeleteObject_(GetCharacterObject(1));
	}

	if (FrameCounterUnpaused == 30) LoadObject((LoadObj)(LoadObj_UnknownA | LoadObj_Data1 | LoadObj_Data2), 1, CreamHeroes_Main)->Data1->CharIndex = 1;
}