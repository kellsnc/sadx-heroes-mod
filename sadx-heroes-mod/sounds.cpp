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
	NJS_VECTOR* position;
	HSTREAM			stream;
	ObjectMaster* obj;
} CUSTOM_SOUNDENTRY;

CUSTOM_SOUNDENTRY SoundListEntries[24];

static bool SoundDevice;

Trampoline PauseSounds_t((int)PauseSound, (int)PauseSound + 0x6, PauseSounds);
void PauseSounds() {
	VoidFunc(original, PauseSounds_t.Target());
	original();

	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		BASS_ChannelPause(SoundListEntries[i].stream);
	}
}

Trampoline ResumeSounds_t((int)ResumeSound, (int)ResumeSound + 0x6, ResumeSounds);
void ResumeSounds() {
	VoidFunc(original, ResumeSounds_t.Target());
	original();

	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		BASS_ChannelPlay(SoundListEntries[i].stream, false);
	}
}

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

int GetSoundEntryByStream(HCHANNEL channel) {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream == channel) return i;
	}

	return -1;
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

void FreeAllCustomSounds() {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		FreeSoundStream(NULL, SoundListEntries[i].stream, NULL, NULL);
		SoundListEntries[i].stream = NULL;
		SoundListEntries[i].dist = 0;
		SoundListEntries[i].obj = nullptr;
		SoundListEntries[i].pitch = 0;
		SoundListEntries[i].volumeA = 0.4f;
		SoundListEntries[i].volumeB = 0;
		SoundListEntries[i].position = NULL;
	}
}

DWORD LoadSoundSteam_LoadFromFile(std::string path) {
	const char* filename = HelperFunctionsGlobal.GetReplaceablePath(path.c_str());

	DWORD channel = BASS_VGMSTREAM_StreamCreate(filename, 0);

	if (channel == 0)
	{
		channel = BASS_StreamCreateFile(false, filename, 0, 0, 0);
	}

	return channel;
}

DWORD LoadSoundStream(int ID) {
	const char* filename = "system\\sounddata\\sounds\\";
	std::string str1 = filename + std::to_string(ID) + ".wav";
	std::string str2 = filename + std::to_string(ID) + ".adx";

	DWORD channel = LoadSoundSteam_LoadFromFile(str1);

	if (channel == 0) {
		channel = LoadSoundSteam_LoadFromFile(str2);
	}

	return channel;
}

float GetVolumeRange(NJS_VECTOR* pos, float dist) {
	if (Camera_Data1) {
		float playerdist = GetDistance(&Camera_Data1->Position, pos);
		return 0.4 - (0.4 * playerdist / dist);
	}

	return 0;
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

		SoundListEntries[entryID].id = ID;
		SoundListEntries[entryID].stream = channel;
	}
}

void PlayCustomSound(int ID) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
		PlaySoundChannelQueue(ID, entryID, 0);
	}
}

void PlayCustomSoundVolume(int ID, float volume) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
		SoundListEntries[entryID].volumeB = volume;
		PlaySoundChannelQueue(ID, entryID, 0);
	}
}

void PlayCustomSoundQueue(int ID, ObjectMaster* obj, NJS_VECTOR* pos, float dist, bool loop, float volume, float pitch) {
	int entryID = GetFreeSoundEntry();

	if (entryID > -1) {
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

void PlayCustomSound_Entity(int ID, ObjectMaster* obj, float dist, bool loop) {
	dist *= 2;
	PlayCustomSoundQueue(ID, obj, NULL, dist, loop, NULL, NULL);
}

void PlayCustomSound_EntityAndVolume(int ID, ObjectMaster* obj, float dist, float volume, bool loop) {
	dist *= 2;
	PlayCustomSoundQueue(ID, obj, NULL, dist, loop, volume, NULL);
}

void PlayCustomSound_EntityAndPos(int ID, ObjectMaster* obj, NJS_VECTOR* pos, float dist, float volume, bool loop) {
	PlayCustomSoundQueue(ID, obj, pos, dist, loop, volume, NULL);
}

void PlayCustomSound_Pos(int ID, NJS_VECTOR* pos, float dist, float volume, bool loop) {
	PlayCustomSoundQueue(ID, nullptr, pos, dist, loop, volume, NULL);
}

void DelayedCustomSound(ObjectMaster* obj) {
	if (--obj->Data1->Scale.y <= 0) {
		if (obj->Data1->Scale.z != 0) {
			PlayCustomSoundVolume(obj->Data1->Scale.x, obj->Data1->Scale.z);
		}
		else {
			PlayCustomSound(obj->Data1->Scale.x);
		}

		DeleteObject_(obj);
	}
}

void PlayDelayedCustomSound(int ID, int time, float volumeoverride) {
	ObjectMaster* temp = LoadObject(LoadObj_Data1, 1, DelayedCustomSound);
	temp->Data1->Scale.x = ID;
	temp->Data1->Scale.y = time;
	temp->Data1->Scale.z = volumeoverride;
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

void RunCustomSounds() {
	for (int i = 0; i < LengthOfArray(SoundListEntries); ++i) {
		if (SoundListEntries[i].stream != NULL) {
			HSTREAM stream = SoundListEntries[i].stream;

			// Ignore if the sound is paused
			if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {
				continue;
			}

			if (SoundListEntries[i].obj && (!SoundListEntries[i].obj->Data1 || SoundListEntries[i].obj->Data1->Status & Status_KillSound)) {
				FreeSoundStreamQueue(NULL, stream, NULL, NULL);
				SoundListEntries[i].stream = NULL;
				continue;
			}

			// Get the volume of the sound based on the distance from the entity or the static position
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

				// If volume B is set (static volume), multiply Volume A (3d volume here) by it.
				if (SoundListEntries[i].volumeB) {
					SoundListEntries[i].volumeA *= SoundListEntries[i].volumeB;
				}

				// Set the new volume
				BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, SoundListEntries[i].volumeA);
			}
			else if (SoundListEntries[i].volumeB) {
				// If the sound is not 3d and Volume B is set (static volume), set it.
				BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, SoundListEntries[i].volumeB);
				SoundListEntries[i].volumeB = 0; //run only once
			}
		}
	}
}