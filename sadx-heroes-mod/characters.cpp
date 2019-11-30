#include "stdafx.h"

uint8_t FlyCharEnabled;
uint8_t SpeedCharEnabled;
ObjectMaster* HeroesChars[8];
bool CharTexsLoaded[5];
int CurrentPlayer;

ObjectFuncPtr DisplayFuncs[]{
	CreamHeroes_Display,
	RougeHeroes_Display,
	CharmyHeroes_Display,
	TailsHeroes_Display,
	SonicHeroes_Display
};

ObjectFuncPtr MainFuncs[]{
	CreamHeroes_Main,
	RougeHeroes_Main,
	CharmyHeroes_Main,
	TailsHeroes_Main,
	SonicHeroes_Main
};

PlaySoundFuncPtr SoundFuncs[]{
	PlaySound_Cream,
	PlaySound_Rouge,
	PlaySound_Charmy,
	PlaySound_Tails,
	PlaySound_Sonic
};

PlaySoundFuncPtr VoiceFuncs[]{
	PlayVoice_Cream,
	PlayVoice_Rouge,
	PlayVoice_Charmy,
	PlayVoice_Tails,
	PlayVoice_Sonic
};

bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2);
Trampoline OhNoImDead2_t(0x004CE030, 0x004CE036, OhNoImDead2);
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2) {
	if (a1->CollisionInfo->CollidingObject) {
		if (a1->CollisionInfo->CollidingObject->Object->MainSub == Cheese_Main 
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == TrapRing_Main) return 1;
	}

	FunctionPointer(bool, original, (EntityData1 *a1, ObjectData2 *a2), OhNoImDead2_t.Target());
	return original(a1, a2);
}

void PlayHeroesAnimation(ObjectMaster* obj, uint8_t ID, AnimData* animdata, float forcespeed, float forcestate) {
	EntityData1* data = obj->Data1;
	float frame = data->Scale.x;

	data->Index = ID;
	if (forcestate) {
		frame = forcestate - 1;
	}
	else {
		if (forcespeed) {
			frame += forcespeed;
		}
		else {
			frame += animdata[ID].AnimationSpeed;
		}
	}
	
	if (frame == animdata[ID].Animation->motion->nbFrame - 1) {
		frame = 0;

		if (animdata[ID].Property == 1) {
			frame = animdata[ID].Animation->motion->nbFrame - 2;
			if (data->Unknown == 0) data->Unknown = 1;
		}
		
		if (animdata[ID].NextAnim != ID) {
			data->Index = animdata[ID].NextAnim;
		}
		else {
			if (data->Unknown < 256) {
				data->Unknown += 1;
			}
		}
	}

	data->Scale.x = frame;
}

void CharactersCommon_Delete(ObjectMaster* obj) {
	HeroesChars[obj->Data1->CharIndex] = nullptr;

	ObjectMaster* playerobj = PlayerPtrs[obj->Data1->CharIndex];
	if (playerobj) {
		EntityData2* playerdata2 = (EntityData2*)playerobj->Data2;
		CharObj2* playerco2 = playerdata2->CharacterData;

		playerobj->DisplaySub = Tails_Display;

		playerco2->PhysicsData.MaxAccel = PhysicsArray[2].MaxAccel;
		playerco2->PhysicsData.field_14 = PhysicsArray[2].field_14;
		playerco2->PhysicsData.AirAccel = PhysicsArray[2].AirAccel;
	}
}

void Heroes_Display(ObjectMaster* obj) {
	if (!HeroesChars[CurrentPlayer]) {
		switch (EntityData1Ptrs[CurrentPlayer]->CharID) {
		case Characters_Sonic:
			Sonic_Display(obj);
			break;
		case Characters_Tails:
			Tails_Display(obj);
			break;
		}
		return;
	}

	DisplayFuncs[HeroesChars[CurrentPlayer]->Data1->CharID - 9](obj);
}

void Tails_Main_r(ObjectMaster* a1);
Trampoline Tails_Main_t(0x00461700, 0x00461712, Tails_Main_r);
void Tails_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	ObjectFunc(original, Tails_Main_t.Target());
	original(obj);
}

void PlayVoice_HeroesChar(int ID) {
	if (!HeroesChars[CurrentPlayer]) {
		PlayVoice(ID);
		return;
	}

	switch (ID) {
	case 1803:
		VoiceFuncs[HeroesChars[CurrentPlayer]->Data1->CharID - 9](ID);
		break;
	default:
		PlayVoice(ID);
		break;
	}
}

int PlaySound_HeroesChar(int ID, void *a2, int a3, void *a4) {
	if (!HeroesChars[CurrentPlayer]) {
		return PlaySound(ID, a2, a3, a4);
	}
	
	if (ID == 17 && !jmpsounds) {
		PlayHeroesSound(CommonSound_Jumping);
		return 1;
	}
	else if (!chrsounds || (!flysounds && ID == 1243)) {
		return 0;
	}
	
	SoundFuncs[HeroesChars[CurrentPlayer]->Data1->CharID - 9](ID);
	return 1;
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	const std::string SpeedCharacter = config->getString("2- Characters", "SpeedCharacter", "None");
	const std::string FlyCharacter = config->getString("2- Characters", "FlyCharacter", "None");
	
	if (!SpeedCharacter.compare("Sonic")) {
		LoadSonicFiles(path, helperFunctions);
		SpeedCharEnabled = Characters_HeroesSonic;
	}
	
	if (SpeedCharEnabled) {
		WriteCall((void*)0x49BF04, Heroes_Display);
	}

	if (!FlyCharacter.compare("Cream")) {
		LoadCreamFiles(path, helperFunctions);
		FlyCharEnabled = Characters_Cream;
	}
	else if (!FlyCharacter.compare("Rouge")) {
		LoadRougeFiles(path, helperFunctions);
		FlyCharEnabled = Characters_Rouge;
	}
	else if (!FlyCharacter.compare("Charmy")) {
		LoadCharmyFiles(path, helperFunctions);
		FlyCharEnabled = Characters_Charmy;
	}
	else if (!FlyCharacter.compare("Tails")) {
		LoadTailsFiles(path, helperFunctions);
		FlyCharEnabled = Characters_HeroesTails;
	}

	if (FlyCharEnabled) {
		WriteCall((void*)0x462456, Heroes_Display);
		WriteCall((void*)0x45C037, PlaySound_HeroesChar); //jump
		WriteCall((void*)0x45BE01, PlaySound_HeroesChar); //fly
		WriteCall((void*)0x45BF8D, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x446A49, PlaySound_HeroesChar); //death
		WriteCall((void*)0x45BE57, PlayVoice_HeroesChar); //win
	}
}

void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (!EntityData1Ptrs[player] || HeroesChars[player]) continue;

		if (SpeedCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Sonic) {
			HeroesChars[player] = LoadObject(LoadObj_Data1, 1, MainFuncs[SpeedCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
		}
		else if (FlyCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Tails) {
			HeroesChars[player] = LoadObject(LoadObj_Data1, 1, MainFuncs[FlyCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
		}
	}
}