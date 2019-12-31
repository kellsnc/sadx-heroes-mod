#pragma once

extern int CurrentPlayer;
extern bool CharFilesLoaded[];
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

typedef void(__cdecl *PlaySoundFuncPtr)(int);
typedef void(__cdecl *VoidFunction)();

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
int PowerLaunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
bool FlightPunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);

extern ObjectMaster* HeroesChars[];

static void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;
void ExploseEnemies(NJS_VECTOR* pos, float size);

void CreamHeroes_Display(ObjectMaster *obj);
void CreamHeroes_Main(ObjectMaster *obj);
void Cheese_Main(ObjectMaster *obj);
void LoadCreamFiles();
void UnloadCreamFiles();
void PlaySound_Cream(int ID);
void PlayVoice_Cream(int ID);

void RougeHeroes_Display(ObjectMaster *obj);
void RougeHeroes_Main(ObjectMaster *obj);
void LoadRougeFiles();
void UnloadRougeFiles();
void PlaySound_Rouge(int ID);
void PlayVoice_Rouge(int ID);

void CharmyHeroes_Display(ObjectMaster *obj);
void CharmyHeroes_Main(ObjectMaster *obj);
void LoadCharmyFiles();
void UnloadCharmyFiles();
void PlaySound_Charmy(int ID);
void PlayVoice_Charmy(int ID);

void TailsHeroes_Display(ObjectMaster *obj);
void TailsHeroes_Main(ObjectMaster *obj);
void TrapRing_Main(ObjectMaster* obj);
void LoadTailsFiles();
void UnloadTailsFiles();
void PlaySound_Tails(int ID);
void PlayVoice_Tails(int ID);

void SonicHeroes_Display(ObjectMaster *obj);
void SonicHeroes_Main(ObjectMaster *obj);
void LoadSonicFiles();
void UnloadSonicFiles();
void PlaySound_Sonic(int ID);
void PlayVoice_Sonic(int ID);

void ShadowHeroes_Display(ObjectMaster *obj);
void ShadowHeroes_Main(ObjectMaster *obj);
void LoadShadowFiles();
void UnloadShadowFiles();
void PlaySound_Shadow(int ID);
void PlayVoice_Shadow(int ID);

void AmyHeroes_Display(ObjectMaster *obj);
void AmyHeroes_Main(ObjectMaster *obj);
void LoadAmyFiles();
void UnloadAmyFiles();
void PlaySound_Amy(int ID);
void PlayVoice_Amy(int ID);

void EspioHeroes_Display(ObjectMaster *obj);
void EspioHeroes_Main(ObjectMaster *obj);
void NinjaObj(ObjectMaster* obj);
void LoadEspioFiles();
void UnloadEspioFiles();
void PlaySound_Espio(int ID);
void PlayVoice_Espio(int ID);

void KnucklesHeroes_Display(ObjectMaster *obj);
void KnucklesHeroes_Main(ObjectMaster *obj);
void LoadKnuckFiles();
void UnloadKnuckFiles();
void PlaySound_Knuckles(int ID);
void PlayVoice_Knuckles(int ID);

void OmegaHeroes_Display(ObjectMaster *obj);
void OmegaHeroes_Main(ObjectMaster *obj);
void LoadOmegaFiles();
void UnloadOmegaFiles();
void PlaySound_Omega(int ID);
void PlayVoice_Omega(int ID);

void BigHeroes_Display(ObjectMaster *obj);
void BigHeroes_Main(ObjectMaster *obj);
void LureObj_Main(ObjectMaster *obj);
void LoadBigFiles();
void UnloadBigFiles();
void PlaySound_Big(int ID);
void PlayVoice_Big(int ID);