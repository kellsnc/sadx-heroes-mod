#pragma once

void Enemies_Init(const char* path, const HelperFunctions& helperFunctions, const IniFile* config);
void Enemies_CheckEnemiesSwap();

extern bool EnableEnemiesSAStages;
extern bool Enemies_CanSwap;

extern Trampoline* KikiTrampoline;
void EggPawn_Init(ObjectMaster* obj);