#include "stdafx.h"
#include "levels.h"
#include "enemies.h"

bool EnableEnemiesSAStages = false;
bool EnableEggpawn = true;

bool Enemies_CanSwap = false;

void Enemies_Init(const char* path, const HelperFunctions& helperFunctions, const IniFile* config) {
	EnableEnemiesSAStages = config->getBool("5- Enemies", "EnableEnemiesSAStages", false);
	EnableEggpawn = config->getBool("5- Enemies", "EnableEggpawn", true);
}

void Enemies_CheckEnemiesSwap() {
	if (EnableEnemiesSAStages == true) {
		Enemies_CanSwap = true;
	}
	else if (IsCurrentHeroesLevel() == true) {
		Enemies_CanSwap = true;
	}
	else {
		Enemies_CanSwap = false;
	}
}

void DeleteCustomEnemies() {
	EggPawn_DeleteFiles();
	e2000_DeleteFiles();
	Flyer_DeleteFiles();
}