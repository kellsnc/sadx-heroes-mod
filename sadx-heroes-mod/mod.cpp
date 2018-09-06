#include "stdafx.h"
#include "mod.h"
#include "IniFile.hpp"

bool EnableModels = true;
bool EnableSounds = true;
bool IsLoaded = false;
bool ChunkSwapped = false;

unsigned int anim = 0;
uint8_t CurrentChunk = 0;
NJS_TEXLIST * CurrentLevelTexlist;

void CommonObjects_OnFrame();

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		//Get the config.ini information
		const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
		EnableSounds = config->getBool("General", "EnableSounds", true);
		EnableModels = config->getBool("General", "EnableModels", true);
		delete config;

		Levels_Init(path, helperFunctions);
		Objects_Init(path, helperFunctions);
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		++anim;

		if ((GameState == 15 || GameState == 4)) {
			if (EnableModels) CommonObjects_OnFrame();
			if (!IsLoaded) IsLoaded = true;
		}

		ChunkSwapped = false;
		if (GameState == 7 || GameState == 6) IsLoaded = false;
	}
	
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}
