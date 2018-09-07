#pragma once

#define ReplacePVM(a, b) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" b ".pvmx")
#define ReplaceBIN(a, b) helperFunctions.ReplaceFile("system\\" a ".BIN", "system\\" b ".bin")
#define ReplaceDAT(a, b) helperFunctions.ReplaceFile("system\\sounddata\\se\\" a ".DAT", "system\\" b ".dat")
#define ReplaceADX(a, b) helperFunctions.ReplaceFile("system\\sounddata\\bgm\\wma\\" a ".wma", "system\\" b ".adx")

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];

extern LandTable** CurrentLandAddress;

void AutoLoop(ObjectMaster * a1);
void RailPath(ObjectMaster * a1);

void ModelHandler_Init(ObjectMaster * a1);
void LevelHandler_Delete(ObjectMaster * a1);
void AnimateTextures(SH_ANIMTEXS *list, Int listcount);
void LoadLevelFile(const char *shortname, int chunknb);
void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos);
void SeasideHill_Init(const char *path, const HelperFunctions &helperFunctions);
void OceanPalace_Init(const char *path, const HelperFunctions &helperFunctions);