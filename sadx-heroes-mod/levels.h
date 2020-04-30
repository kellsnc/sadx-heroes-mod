#pragma once

enum HeroesLevelIDs {
	HeroesLevelID_SeasideHill = 1,
	HeroesLevelID_OceanPalace,
	HeroesLevelID_CasinoPark,
	HeroesLevelID_BingoHighway,
	HeroesLevelID_EggFleet = 6,
	HeroesLevelID_HangCastle,
	HeroesLevelID_PowerPlant,
	HeroesLevelID_GrandMetropolis,
	HeroesLevelID_MysticMansion,
	HeroesLevelID_SpecialStages = 35
};

struct HeroesLevelData {
	HeroesLevelIDs	LevelID;
	Uint8			Act;
	Uint8			ChunkAmount;
	std::string		name;
	std::string		shortname;
	VoidFuncPtr		loadfunc;
	VoidFuncPtr		unloadfunc;
	HelperFuncPtr	initfunc;
	NJS_POINT3		startpos;
	int				musicid;
};

typedef struct {
	uint8_t			Chunk;
	NJS_VECTOR		Position1;
	NJS_VECTOR		Position2;
} CHUNK_LIST;

typedef struct {
	int			texid;
	int			count;
	int			duration[32];
	int			cache;
	Uint32		address;
} SH_ANIMTEXS;

extern HeroesLevelData SeasideHillData;
extern HeroesLevelData SeaGateData;
extern HeroesLevelData OceanPalaceData;
extern HeroesLevelData RoadRockData;
extern HeroesLevelData GrandMetropolisData;
extern HeroesLevelData PowerPlantData;
extern HeroesLevelData CasinoParkData;
extern HeroesLevelData BingoHighwayData;
extern HeroesLevelData HangCastleData;
extern HeroesLevelData MysticMansionData;
extern HeroesLevelData EggFleetData;
extern HeroesLevelData SpecialStagesData;

DataArray(Rotation3, LevelGlobalColors, 0x90BFE8, 42);

#define ReplacePVM(a, b) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" b ".pvm")
#define ReplaceBIN(a, b) helperFunctions.ReplaceFile("system\\" a ".BIN", "system\\" b ".bin")

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];
extern uint8_t CurrentChunk;

bool IsCurrentHeroesLevel();
bool IsNoPinballEnabled();
bool IsNoMysticMusicEnabled();
void SetHeroesLeveltex();

void AnimateTexlist(SH_ANIMTEXS *list, Int listcount, NJS_TEXLIST* texlist);
void AnimateTextures(SH_ANIMTEXS *list, Int listcount);
void LoadLevelFile(std::string name);
void SwapChunk(Uint8 chunk);
void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos);
void SetFog(FogData * fog);

void HeroesSkybox_Main(ObjectMaster *obj);