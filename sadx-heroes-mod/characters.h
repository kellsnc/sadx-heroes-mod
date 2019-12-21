#pragma once

typedef void(__cdecl *PlaySoundFuncPtr)(int);

extern int CurrentPlayer;
extern bool CharTexsLoaded[];
extern bool CustomPhysics;

enum CharactersHeroes
{
	Characters_Cream = 9,
	Characters_Rouge,
	Characters_Charmy,
	Characters_HeroesTails,
	Characters_HeroesSonic,
	Characters_Shadow,
	Characters_HeroesAmy,
	Characters_Espio,
	Characters_HeroesKnuckles,
	Characters_Omega,
	Characters_HeroesBig,
	Characters_Vector
};

void DrawChunkModel(NJS_CNK_MODEL* model);
void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags);
void njCnkAction(NJS_ACTION* action, float frame);
void SetupWorldMatrix();
void PlayHeroesAnimation(ObjectMaster *obj, uint8_t ID, AnimData* animdata, float forcespeed, float forcestate);
bool CharactersCommon_Init(ObjectMaster* obj, const char* name, NJS_TEXLIST* tex);
void CharactersCommon_DrawBall(EntityData1* playerdata, EntityData1* data);

NJS_VECTOR SpeedAnims(EntityData1* data, EntityData1* playerdata, CharObj2* playerco2);
void TornadoTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
bool KickTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);

NJS_VECTOR PowerAnims(EntityData1* data, EntityData1* playerdata, CharObj2* playerco2);
int PowerComboTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
bool FlightPunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);

extern ObjectMaster* HeroesChars[];

void ExploseEnemies(NJS_VECTOR* pos, float size);

void CreamHeroes_Display(ObjectMaster *obj);
void CreamHeroes_Main(ObjectMaster *obj);
void Cheese_Main(ObjectMaster *obj);
void LoadCreamFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Cream(int ID);
void PlayVoice_Cream(int ID);

void RougeHeroes_Display(ObjectMaster *obj);
void RougeHeroes_Main(ObjectMaster *obj);
void LoadRougeFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Rouge(int ID);
void PlayVoice_Rouge(int ID);

void CharmyHeroes_Display(ObjectMaster *obj);
void CharmyHeroes_Main(ObjectMaster *obj);
void LoadCharmyFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Charmy(int ID);
void PlayVoice_Charmy(int ID);

void TailsHeroes_Display(ObjectMaster *obj);
void TailsHeroes_Main(ObjectMaster *obj);
void TrapRing_Main(ObjectMaster* obj);
void LoadTailsFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Tails(int ID);
void PlayVoice_Tails(int ID);

void SonicHeroes_Display(ObjectMaster *obj);
void SonicHeroes_Main(ObjectMaster *obj);
void LoadSonicFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Sonic(int ID);
void PlayVoice_Sonic(int ID);

void ShadowHeroes_Display(ObjectMaster *obj);
void ShadowHeroes_Main(ObjectMaster *obj);
void LoadShadowFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Shadow(int ID);
void PlayVoice_Shadow(int ID);

void AmyHeroes_Display(ObjectMaster *obj);
void AmyHeroes_Main(ObjectMaster *obj);
void LoadAmyFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Amy(int ID);
void PlayVoice_Amy(int ID);

void EspioHeroes_Display(ObjectMaster *obj);
void EspioHeroes_Main(ObjectMaster *obj);
void NinjaObj(ObjectMaster* obj);
void LoadEspioFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Espio(int ID);
void PlayVoice_Espio(int ID);

void KnucklesHeroes_Display(ObjectMaster *obj);
void KnucklesHeroes_Main(ObjectMaster *obj);
void LoadKnuckFiles(const char *path, const HelperFunctions &helperFunctions);
void PlaySound_Knuckles(int ID);
void PlayVoice_Knuckles(int ID);

static void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;