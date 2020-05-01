#pragma once

void Enemies_Init(const IniFile* config);
void Enemies_CheckEnemiesSwap();
void DeleteCustomEnemies();

void LoadExplosion(NJS_VECTOR* position);
void LoadMissile(NJS_VECTOR* pos, Rotation3* dir, float scale);

extern bool EnableEnemiesSAStages;
extern bool Enemies_CanSwap;

extern Trampoline* KikiTrampoline;
void EggPawn_Init(ObjectMaster* obj);
void EggPawn_DeleteFiles();

void e2000_Init(ObjectMaster* obj);
void e2000_DeleteFiles();

void Flyer_Init(ObjectMaster* obj);
void Flyer_Trigger(ObjectMaster* obj);
void Flyer_DeleteFiles();