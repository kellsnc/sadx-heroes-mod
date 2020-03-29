#include "stdafx.h"
#include "bass_vgmstream.h"

bool chrsounds = true;
bool jmpsounds = true;
bool flysounds = true;
bool envsounds = true;

typedef struct {
	int				id;
	float			dist;
	float			volumeA;
	float			volumeB;
	float			pitch;
	NJS_VECTOR*		position;
	HSTREAM			stream;
	ObjectMaster*	obj;
} NSS_SOUNDENTRY;

NSS_SOUNDENTRY SoundListEntries[24];

bool SoundDevice;

int GetFreeSoundEntry() {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream == NULL) {
			SoundListEntries[i].dist = 0;
			SoundListEntries[i].obj = nullptr;
			SoundListEntries[i].pitch = 0;
			SoundListEntries[i].volumeA = 0.4f;
			SoundListEntries[i].volumeB = 0;
			SoundListEntries[i].position = NULL;
			return i;
		}
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

float GetVolumeRange(NJS_VECTOR* pos, float dist) {
	if (Camera_Data1) {
		float playerdist = GetDistance(&Camera_Data1->Position, pos);
		return 0.4 - (0.4 * playerdist / dist);
	}
	
	return 0;
}

static void __stdcall FreeSoundStream(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

static void __stdcall FreeSoundStreamQueue(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	int entryID = GetSoundEntryByStream(channel);

	if (entryID > -1) {
		SoundListEntries[entryID].stream = NULL;
	}

	FreeSoundStream(handle, channel, data, user);
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

void PlaySoundChannelQueue(int ID, int entryID, bool loop) {
	HSTREAM channel = LoadSoundStream(ID);

	if (channel != 0)
	{
		BASS_ChannelPlay(channel, false);
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, SoundListEntries[entryID].volumeA);
		if (loop) {
			BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
		}
		else {
			BASS_ChannelSetSync(channel, BASS_SYNC_END, 0, FreeSoundStreamQueue, nullptr);
		}
		SoundListEntries[entryID].stream = channel;
	}
}

void PlayHeroesSound(int ID) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
		SoundListEntries[entryID].id = ID;
		PlaySoundChannelQueue(ID, entryID, 0);
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

void PlayHeroesSoundQueue(int ID, ObjectMaster* obj, NJS_VECTOR* pos, float dist, bool loop, float volume, float pitch) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
		SoundListEntries[entryID].id = ID;
		SoundListEntries[entryID].obj = obj;
		SoundListEntries[entryID].dist = dist;

		if (obj) SoundListEntries[entryID].volumeA = GetVolumeRange(&obj->Data1->Position, dist);
		else if (pos) SoundListEntries[entryID].volumeA = GetVolumeRange(pos, dist);
		
		if (volume) SoundListEntries[entryID].volumeB = volume;
		if (pitch) SoundListEntries[entryID].pitch = pitch;
		if (pos) SoundListEntries[entryID].position = pos;

		PlaySoundChannelQueue(ID, entryID, loop);
	}
}

void PlayHeroesSound_Entity(int ID, ObjectMaster* obj, float dist, bool loop) {
	dist *= 2;
	PlayHeroesSoundQueue(ID, obj, NULL, dist, loop, NULL, NULL);
}

void PlayHeroesSound_EntityAndVolume(int ID, ObjectMaster* obj, float dist, float volume, bool loop) {
	dist *= 2;
	PlayHeroesSoundQueue(ID, obj, NULL, dist, loop, volume, NULL);
}

void PlayHeroesSound_EntityAndPos(int ID, ObjectMaster* obj, NJS_VECTOR* pos, float dist, float volume, bool loop) {
	PlayHeroesSoundQueue(ID, obj, pos, dist, loop, volume, NULL);
}

void PlayHeroesSound_Pos(int ID, NJS_VECTOR* pos, float dist, float volume, bool loop) {
	PlayHeroesSoundQueue(ID, nullptr, pos, dist, loop, volume, NULL);
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

void Sounds_OnFrame() {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream != NULL) {
			HSTREAM stream = SoundListEntries[i].stream;
			
			HWND ActiveWindow = GetActiveWindow();

  			if ((GameState == 16 /*|| ActiveWindow != WindowHandle*/) && BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {
				BASS_ChannelPause(stream);
			}
			
			if ((GameState != 16 /*&& ActiveWindow == WindowHandle*/) && BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {
				BASS_ChannelPlay(stream, false);
			}

			if (SoundListEntries[i].obj && (!SoundListEntries[i].obj->Data1 || SoundListEntries[i].obj->Data1->Status & Status_KillSound)) {
            	FreeSoundStreamQueue(NULL, stream, NULL, NULL);
				SoundListEntries[i].stream = NULL;
				continue;
			}

			if (SoundListEntries[i].dist > 0) {
				if (SoundListEntries[i].position) {
					if (!IsPlayerInsideSphere_(SoundListEntries[i].position, SoundListEntries[i].dist)) {
						SoundListEntries[i].volumeA = 0;
					}
					else {
						SoundListEntries[i].volumeA = GetVolumeRange(SoundListEntries[i].position, SoundListEntries[i].dist);
					}

					
				}
				else {
					if (!IsPlayerInsideSphere_(&SoundListEntries[i].obj->Data1->Position, SoundListEntries[i].dist)) {
						SoundListEntries[i].volumeA = 0;
					}
					else {
						SoundListEntries[i].volumeA = GetVolumeRange(&SoundListEntries[i].obj->Data1->Position, SoundListEntries[i].dist);
					}
					
				}

				if (SoundListEntries[i].volumeB) {
					SoundListEntries[i].volumeA *= SoundListEntries[i].volumeB;
				}
				
				BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, SoundListEntries[i].volumeA);
			}
		}
	}
}