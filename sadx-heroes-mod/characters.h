#pragma once

extern int CurrentPlayer;
extern bool CharTexsLoaded[];

enum CharactersHeroes
{
	Characters_Cream = 9,
};

void DrawChunkModel(NJS_CNK_MODEL* model);
void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags);
void njCnkAction(NJS_ACTION* action, float frame);
void SetupWorldMatrix();
void PlayHeroesAnimation(ObjectMaster *obj, uint8_t ID, AnimData* animdata, float forcespeed, float forcestate);

extern ObjectMaster* Creams[];
void CreamHeroes_Display(ObjectMaster *obj);
void CreamHeroes_Main(ObjectMaster *obj);
void Cheese_Main(ObjectMaster *obj);
void LoadCreamFiles(const char *path, const HelperFunctions &helperFunctions);
int PlaySound_Tails(int ID, void *a2, int a3, void *a4);
void PlayVoice_Tails(int ID);

static void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;