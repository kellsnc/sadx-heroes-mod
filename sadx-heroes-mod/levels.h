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

DataArray(Rotation3, LevelGlobalColors, 0x90BFE8, 42);

#define ReplacePVM(a, b) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" b ".pvm")
#define ReplaceBIN(a, b) helperFunctions.ReplaceFile("system\\" a ".BIN", "system\\" b ".bin")

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];
extern LandTable** CurrentLandAddress;

bool IsCurrentHeroesLevel();

void AnimateTexlist(SH_ANIMTEXS *list, Int listcount, NJS_TEXLIST* texlist);
void AnimateTextures(SH_ANIMTEXS *list, Int listcount);
void LoadLevelFile(const char *shortname, int chunknb);
void SwapChunk(const char* shortname, int chunknb);
void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos);
void SetFog(FogData * fog);
void DefaultLight(HeroesLevelIDs levelid);

void HeroesSkybox_Main(ObjectMaster *a1);
void LevelHandler_Delete(ObjectMaster * a1);
void SeasideHill_Init(const char *path, const HelperFunctions &helperFunctions);
void OceanPalace_Init(const char *path, const HelperFunctions &helperFunctions);
void GrandMetropolis_Init(const char *path, const HelperFunctions &helperFunctions);
void PowerPlant_Init(const char *path, const HelperFunctions &helperFunctions);
void CasinoPark_Init(const char *path, const HelperFunctions &helperFunctions);
void BingoHighway_Init(const char *path, const HelperFunctions &helperFunctions);
void HangCastle_Init(const char *path, const HelperFunctions &helperFunctions);
void MysticMansion_Init(const char *path, const HelperFunctions &helperFunctions);
void EggFleet_Init(const char *path, const HelperFunctions &helperFunctions);
void SpecialStages_Init(const char *path, const HelperFunctions &helperFunctions);