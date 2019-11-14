#include "stdafx.h"
#include "mod.h"
#include "characters.h"

bool CreamEnabled = false;
bool CharTexsLoaded[1];

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);

D3DMATRIX WorldMatrixBackup;

int CurrentPlayer;

bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2);
Trampoline OhNoImDead2_t(0x004CE030, 0x004CE036, OhNoImDead2);
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2) {
	if (a1->CollisionInfo->CollidingObject) {
		if (a1->CollisionInfo->CollidingObject->Object->MainSub == Cheese_Main) return 1;
	}

	FunctionPointer(bool, original, (EntityData1 *a1, ObjectData2 *a2), OhNoImDead2_t.Target());
	return original(a1, a2);
}

void DrawChunkModel(NJS_CNK_MODEL* model)
{
	DrawChunkModel_(model->vlist, model->plist);
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

void PlayHeroesAnimation(ObjectMaster *obj, uint8_t ID, AnimData* animdata, float forcespeed, float forcestate) {
	EntityData1* data = obj->Data1;
	float frame = data->Scale.x;

	data->Index = ID;
	if (forcestate) {
		frame = forcestate - 1;
	}
	else {
		if (forcespeed) {
			frame += forcespeed;
		}
		else {
			frame += animdata[ID].AnimationSpeed;
		}
	}
	
	if (frame == animdata[ID].Animation->motion->nbFrame - 1) {
		frame = 0;

		if (animdata[ID].Property == 1) {
			frame = animdata[ID].Animation->motion->nbFrame - 2;
			if (data->Unknown == 0) data->Unknown = 1;
		}
		
		if (animdata[ID].NextAnim != ID) {
			data->Index = animdata[ID].NextAnim;
		}
		else {
			if (data->Unknown < 256) {
				data->Unknown += 1;
			}
		}
	}

	data->Scale.x = frame;
}

//For flying characters
void Tails_Display_(ObjectMaster* obj) {
	if (Creams[obj->Data1->CharIndex]) {
		CreamHeroes_Display(obj);
	}
	else {
		Tails_Display(obj);
	}
}

void Tails_Main_r(ObjectMaster* a1);
Trampoline Tails_Main_t(0x00461700, 0x00461712, Tails_Main_r);
void Tails_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	ObjectFunc(original, Tails_Main_t.Target());
	original(obj);
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions) {
	const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
	CreamEnabled = config->getBool("2- Characters", "EnableCream", false);
	delete config;

	if (CreamEnabled) {
		LoadCreamFiles(path, helperFunctions);
		WriteCall((void*)0x462456, Tails_Display_);
		WriteCall((void*)0x45C037, PlaySound_Tails); //jump
		WriteCall((void*)0x45BE01, PlaySound_Tails); //fly
		WriteCall((void*)0x45BF8D, PlaySound_Tails); //hurt
		WriteCall((void*)0x446A49, PlaySound_Tails); //death
		WriteCall((void*)0x45BE57, PlayVoice_Tails); //death
	}
}

void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (!EntityData1Ptrs[player]) continue;
		if (EntityData1Ptrs[player]->CharID == Characters_Tails && CreamEnabled) {
			if (!Creams[player]) {
				Creams[player] = LoadObject(LoadObj_Data1, 1, CreamHeroes_Main);
				Creams[player]->Data1->CharIndex = player;
			}
		}
	}
}