#pragma once

void Enemies_Init(const char* path, const HelperFunctions& helperFunctions, const IniFile* config);
void Enemies_CheckEnemiesSwap();

void LoadExplosion(NJS_VECTOR* position);
void LoadMissile(NJS_VECTOR* pos, Rotation3* dir, float scale);

extern bool EnableEnemiesSAStages;
extern bool Enemies_CanSwap;

extern Trampoline* KikiTrampoline;
void EggPawn_Init(ObjectMaster* obj);