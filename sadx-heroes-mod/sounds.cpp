#include "stdafx.h"

bool chrsounds = true;
bool jmpsounds = true;
bool flysounds = true;

void Sounds_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	chrsounds = config->getBool("4- Sounds", "chrsounds", true);
	jmpsounds = config->getBool("4- Sounds", "jmpsounds", true);
	flysounds = config->getBool("4- Sounds", "flysounds", true);

	if (config->getBool("4- Sounds", "bgmsounds", true)) {
		MusicList[MusicIDs_invncibl].Name = "heroes_invncibl";
		MusicList[MusicIDs_rndclear].Name = "heroes_rndclear";
		MusicList[MusicIDs_speedup].Name = "heroes_speedup";
	}
}