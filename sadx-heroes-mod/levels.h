#pragma once

#define ReplacePVM(a, b) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" b ".pvm")
#define ReplaceBIN(a, b) helperFunctions.ReplaceFile("system\\" a ".BIN", "system\\" b ".bin")
#define ReplaceDAT(a, b) helperFunctions.ReplaceFile("system\\sounddata\\se\\" a ".DAT", "system\\" b ".dat")
#define ReplaceADX(a, b) helperFunctions.ReplaceFile("system\\sounddata\\bgm\\wma\\" a ".wma", "system\\" b ".adx")
#define MoveADX(a) helperFunctions.ReplaceFile("system\\sounddata\\bgm\\wma\\" a ".wma", "system\\" a ".adx")

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];

extern LandTable** CurrentLandAddress;

extern bool chunkswapped;

void AutoLoop(ObjectMaster * a1);
void RailPath(ObjectMaster * a1);
void SlopePath(ObjectMaster * a1);

void HeroesSkybox_Main(ObjectMaster *a1);
void AnimateObjectsTextures(NJS_MODEL_SADX * *objlist, int size, SH_ANIMTEXS *list, Int listcount);
void AnimateUV(SH_UVSHIFT *UVSHIFT, int size);

void LevelHandler_Delete(ObjectMaster * a1);
void AnimateTextures(SH_ANIMTEXS *list, Int listcount);
void LoadLevelFile(const char *shortname, int chunknb);
void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos);
void SetFog(FogData * fog);

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

StartPosition Heroes_StartPositions[]{
	{ HeroesLevelID_SeasideHill, 0,{ 0, 6.800581f, 5.217285f }, 0xBFFF },
	{ HeroesLevelID_SeasideHill, 1,{ 6090, 30, 1000 }, 0xBFFF },			//Sea Gate
	{ HeroesLevelID_OceanPalace, 0,{ -8771, 1303, -2819.688f }, 0xBFFF },
	{ HeroesLevelID_OceanPalace, 1,{ 0, 3020, 4462 }, 0xBFFF },				//Road Rock
	{ HeroesLevelID_GrandMetropolis, 0,{ 0.5f, 70, 4125.8f }, 0xBFFF },
	{ HeroesLevelID_PowerPlant, 0,{ 2000, 1564, 63 }, 0xBFFF },
	{ HeroesLevelID_CasinoPark, 0,{ -8000, 2188, 0 }, 0xBFFF },
	{ HeroesLevelID_BingoHighway, 0,{ 7999, 2277, 472 }, 0xBFFF },
	{ HeroesLevelID_HangCastle, 0,{ 3999, 4000, 109 }, 0xBFFF },
	{ HeroesLevelID_MysticMansion, 0,{ 0, 23, 777 }, 0xBFFF },
	{ HeroesLevelID_EggFleet, 0,{ 500, 4230, 5320 }, 0xBFFF },
	{ HeroesLevelID_SpecialStages, 0,{ 200, 0, 0 }, 0xBFFF }
};