#include "stdafx.h"

bool EnableEnemiesSAStages = false;
bool EnableEggpawn = true;

bool Enemies_CanSwap = false;

Trampoline* KikiTrampoline = nullptr;

void Enemies_Init(const char* path, const HelperFunctions& helperFunctions, const IniFile* config) {
	EnableEnemiesSAStages = config->getBool("5- Enemies", "EnableEnemiesSAStages", false);
	EnableEggpawn = config->getBool("5- Enemies", "EnableEggpawn", true);

	if (EnableEggpawn == true) {
		KikiTrampoline = new Trampoline((int)Kiki_Load, (int)Kiki_Load + 0x7, EggPawn_Init);
	}
}

void Enemies_CheckEnemiesSwap() {
	if (EnableEnemiesSAStages == true) {
		Enemies_CanSwap = true;
	}
	else if (IsHeroesLevel == true) {
		Enemies_CanSwap = true;
	}
	else {
		Enemies_CanSwap = false;
	}
}