#pragma once

extern int CurrentPlayer;
extern int CharFilesLoaded[];
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

enum {
	MTN_SPD_WALK,
	MTN_SPD_WALK_PULL,
	MTN_SPD_WALK_PUSH,
	MTN_SPD_TURN_L,
	MTN_SPD_TURN_R,
	MTN_SPD_SLOW_RUN,
	MTN_SPD_MID_RUN,
	MTN_SPD_TOP_RUN,
	MTN_SPD_START,
	MTN_SPD_JUMP_WALL,
	MTN_SPD_JUMP_TRIC_A,
	MTN_SPD_JUMP_TRIC_B,
	MTN_SPD_JUMP_TRIC_C,
	MTN_SPD_JUMP_A,
	MTN_SPD_JUMP_B,
	MTN_SPD_JUMP_C,
	MTN_SPD_JUMP_D,
	MTN_SPD_JUMP_E,
	MTN_SPD_JUMP_F,
	MTN_SPD_JUMP_TRNGL,
	MTN_SPD_JUMP_GLIND,
	MTN_SPD_GLIND,
	MTN_SPD_GLIND_BK,
	MTN_SPD_GLIND_BK_L,
	MTN_SPD_GLIND_BK_R,
	MTN_SPD_GLIND_FLIP_B,
	MTN_SPD_GLIND_FLIP_FR,
	MTN_SPD_GLIND_L,
	MTN_SPD_GLIND_R,
	MTN_SPD_FLY_IDLE,
	MTN_SPD_FLY_SLOW,
	MTN_SPD_FLY_GLIND,
	MTN_SPD_HANG_OFF,
	MTN_SPD_HANG_ON,
	MTN_SPD_BREAK_A,
	MTN_SPD_BREAK_B,
	MTN_SPD_BREAK_C,
	MTN_SPD_BREAK_TURN_L,
	MTN_SPD_BREAK_TURN_R,
	MTN_SPD_BRA_MID,
	MTN_SPD_BRA_TOP,
	MTN_SPD_FLORT,
	MTN_SPD_DAM_MID_A,
	MTN_SPD_DAM_MID_B,
	MTN_SPD_DAM_MID_C,
	MTN_SPD_EDDGE_OTTO_A,
	MTN_SPD_EDDGE_OTTO_B,
	MTN_SPD_EDDGE_OTTO_C,
	MTN_SPD_FW_JUMP,
	MTN_SPD_TRAP_JUMP,
	MTN_SPD_POW_ROT,
	MTN_SPD_WIN,
	MTN_SPD_ATC,
	MTN_SPD_IDLE,
	MTN_SPD_IDLE_B,
	MTN_SPD_IDLE_C,
	MTN_SPD_TEAM,
	MTN_SPD_ROLL, // added
	MTN_SPD_MAX,

	MTN_SH_TOP_SKATE = MTN_SPD_MAX,

	MTN_AM_IDLE_POW_HALF = MTN_SPD_MAX,
	MTN_AM_IDLE_POW_SHAKE,
	MTN_AM_START_IDLE,
	MTN_AM_ATC_TRNADE,

	MTN_ES_KONOHA = MTN_SPD_MAX,
};

enum {
	MTN_POW_WALK,
	MTN_POW_WALK_PULL,
	MTN_POW_WALK_PUSH,
	MTN_POW_TURN_L,
	MTN_POW_TURN_R,
	MTN_POW_SLOW_RUN,
	MTN_POW_MID_RUN,
	MTN_POW_TOP_RUN,
	MTN_POW_START,
	MTN_POW_ATC_A,
	MTN_POW_ATC_B,
	MTN_POW_ATC_C,
	MTN_POW_ATC_D,
	MTN_POW_JUMP_A,
	MTN_POW_JUMP_B,
	MTN_POW_JUMP_C,
	MTN_POW_JUMP_D,
	MTN_POW_JUMP_E,
	MTN_POW_JUMP_F,
	MTN_POW_JUMP_TRNGL,
	MTN_POW_JUMP_GLIND,
	MTN_POW_GLIND,
	MTN_POW_GLIND_BK,
	MTN_POW_GLIND_BK_L,
	MTN_POW_GLIND_BK_R,
	MTN_POW_GLIND_FLIP_B,
	MTN_POW_GLIND_FLIP_FR,
	MTN_POW_GLIND_L,
	MTN_POW_GLIND_R,
	MTN_POW_FLY_IDLE,
	MTN_POW_FLY_SLOW,
	MTN_POW_FLY_GLIND,
	MTN_POW_HANG_OFF,
	MTN_POW_HANG_ON,
	MTN_POW_BREAK_A,
	MTN_POW_BREAK_B,
	MTN_POW_BREAK_C,
	MTN_POW_BREAK_TURN_L,
	MTN_POW_BREAK_TURN_R,
	MTN_POW_BRA_MID,
	MTN_POW_BRA_TOP,
	MTN_POW_FLORT,
	MTN_POW_DAM_M_A,
	MTN_POW_DAM_M_B,
	MTN_POW_DAM_M_C,
	MTN_POW_EDGE_OTTO_A,
	MTN_POW_EDGE_OTTO_B,
	MTN_POW_EDGE_OTTO_C,
	MTN_POW_FW_JUMP,
	MTN_POW_TRAP_JUMP,
	MTN_POWRO_KN,
	MTN_POW_WIN_B,
	MTN_POW_ATC_RENDA,
	MTN_POW_IDLE,
	MTN_POW_IDLE_B,
	MTN_POW_IDLE_C,
	MTN_POW_BOB,
	MTN_POW_BOB_L,
	MTN_POW_BOB_R,
	MTN_POW_ROLL, // added
	MTN_POW_MAX,
};

struct playerwk_heroes
{
	mtnjvwk mm;
	mtnjvwk mm_sub;
	NJS_TEXLIST* texlist;
};

typedef void(__cdecl *PlaySoundFuncPtr)(int);
typedef void(__cdecl *VoidFunction)();

void HeroesChars_InitPlayer(task* tp, TEX_PVMTABLE pvm, int lifeicontex, PL_ACTION* pl_action);
void CharactersCommon_DrawBall(EntityData1* playerdata, EntityData1* data);

void SonicAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp);
void KnucklesAnimConverter(mtnjvwk* mtn, int heroes_plno, taskwk* pltwp, playerwk* pwp);

bool CanDoTricks(EntityData1* player);
void TornadoTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
bool KickTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
int PowerComboTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
int PowerLaunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);
bool FlightPunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata);

extern ObjectMaster* HeroesChars[];

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

void VectorHeroes_Display(ObjectMaster *obj);
void VectorHeroes_Main(ObjectMaster *obj);
void LoadVectorFiles();
void UnloadVectorFiles();
void PlaySound_Vector(int ID);
void PlayVoice_Vector(int ID);