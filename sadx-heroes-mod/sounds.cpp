#include "stdafx.h"

bool chrsounds = true;
bool jmpsounds = true;
bool flysounds = true;

// temporary stuff before using a custom sound system

void PlayHeroesSound(int ID) {
	PlayVoice(ID);
}

void DelayedHeroesSound(ObjectMaster* obj) {
	if (--obj->Data1->Scale.y <= 0) {
		PlayHeroesSound(obj->Data1->Scale.x);
		DeleteObject_(obj);
	}
}

void PlayDelayedHeroesSound(int ID, int time) {
	ObjectMaster* temp = LoadObject(LoadObj_Data1, 1, DelayedHeroesSound);
	temp->Data1->Scale.x = ID;
	temp->Data1->Scale.y = time;
}

void PlayHeroesSound3D(int ID, ObjectMaster* obj, float dist) {
	if (obj && IsPlayerInsideSphere(&obj->Data1->Position, dist) == 1) PlayHeroesSound(ID);
}

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