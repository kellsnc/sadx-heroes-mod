#include "stdafx.h"
#include "bass_vgmstream.h"

bool chrsounds = true;
bool jmpsounds = true;
bool flysounds = true;
bool envsounds = true;

typedef struct {
	int			id;
	float		frame;
	float		volume;
	float		pitch;
	HSTREAM		stream;
} NSS_SOUNDENTRY;

NSS_SOUNDENTRY SoundListEntries[24];

bool SoundDevice;

int GetFreeSoundEntry() {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream == NULL) return i;
	}

	return 0;
}

int GetSoundEntryByID(int id) {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].id == id) return i;
	}

	return -1;
}

int GetSoundEntryByStream(HCHANNEL channel) {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream == channel) return i;
	}

	return -1;
}

static void __stdcall FreeSoundStreamQueue(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	int entryID = GetSoundEntryByStream(channel);

	if (entryID > -1) {
		SoundListEntries[entryID].stream = NULL;
	}

	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

static void __stdcall FreeSoundStream(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

DWORD LoadSoundStream(int ID) {
	const char* filename = "system\\sounddata\\sounds\\";
	std::string str = filename;
	str = str + std::to_string(ID) + ".wav";
	filename = HelperFunctionsGlobal.GetReplaceablePath(str.c_str());
	
	DWORD channel = BASS_VGMSTREAM_StreamCreate(filename, 0);

	if (channel == 0)
	{
		channel = BASS_StreamCreateFile(false, filename, 0, 0, 0);
	}

	if (channel != 0) {
		return channel;
	}
	else {
		return 0;
	}
}

void PlaySoundChannelQueue(int ID, float volume) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
		HSTREAM channel = LoadSoundStream(ID);

		if (channel != 0)
		{
			BASS_ChannelPlay(channel, false);
			BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, (volume + 10000) / 30000.0f);
			BASS_ChannelSetSync(channel, BASS_SYNC_END, 0, FreeSoundStream, nullptr);
		}

		SoundListEntries[entryID].id = ID;
		SoundListEntries[entryID].volume = volume;
		SoundListEntries[entryID].stream = channel;
	}
}

void PlayHeroesSound(int ID) {
	PlaySoundChannelQueue(ID, 100);
	return;

	HSTREAM channel = LoadSoundStream(ID);

	if (channel != 0)
	{
		BASS_ChannelPlay(channel, false);
		BASS_ChannelSetSync(channel, BASS_SYNC_END, 0, FreeSoundStream, nullptr);
	}
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

void PlayHeroesSound3DPitch(int ID, ObjectMaster* obj, float dist, float pitch) {
	if (obj && IsPlayerInsideSphere(&obj->Data1->Position, dist) == 1) PlayHeroesSound(ID);
}

void Sounds_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	chrsounds = config->getBool("4- Sounds", "chrsounds", true);
	jmpsounds = config->getBool("4- Sounds", "jmpsounds", true);
	flysounds = config->getBool("4- Sounds", "flysounds", true);
	envsounds = config->getBool("4- Sounds", "envsounds", true);

	if (config->getBool("4- Sounds", "bgmsounds", true)) {
		MusicList[MusicIDs_invncibl].Name = "heroes_invncibl";
		MusicList[MusicIDs_rndclear].Name = "heroes_rndclear";
		MusicList[MusicIDs_speedup].Name = "heroes_speedup";
	}
}