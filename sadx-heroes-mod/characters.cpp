#include "stdafx.h"

uint8_t FlyCharEnabled;
bool CharTexsLoaded[4];

ObjectMaster* HeroesChars[8];
int CurrentPlayer;

using std::string;

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

//For flying characters
void Tails_Display_(ObjectMaster* obj) {
	if (!HeroesChars[CurrentPlayer]) {
		Tails_Display(obj);
		return;
	}

	switch (HeroesChars[CurrentPlayer]->Data1->CharID) {
	case Characters_Cream:
		CreamHeroes_Display(obj);
		break;
	case Characters_Rouge:
		RougeHeroes_Display(obj);
		break;
	case Characters_Charmy:
		CharmyHeroes_Display(obj);
		break;
	case Characters_HeroesTails:
		TailsHeroes_Display(obj);
		break;
	default:
		Tails_Display(obj);
		break;
	}
}

void Tails_Main_r(ObjectMaster* a1);
Trampoline Tails_Main_t(0x00461700, 0x00461712, Tails_Main_r);
void Tails_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	ObjectFunc(original, Tails_Main_t.Target());
	original(obj);
}

void PlayVoice_HeoresChar(int ID) {
	if (!HeroesChars[CurrentPlayer]) {
		PlayVoice(ID);
		return;
	}

	switch (ID) {
	case 1803:
		switch (HeroesChars[CurrentPlayer]->Data1->CharID) {
		case Characters_Cream:
			PlayVoice_Cream(ID);
			break;
		case Characters_Rouge:
			PlayVoice_Rouge(ID);
			break;
		case Characters_Charmy:
			PlayVoice_Charmy(ID);
			break;
		case Characters_HeroesTails:
			PlayVoice_Tails(ID);
			break;
		default:
			PlayVoice(ID);
			break;
		}
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
	
	switch (HeroesChars[CurrentPlayer]->Data1->CharID) {
	case Characters_Cream:
		return PlaySound_Cream(ID, a2, a3, a4);
		break;
	case Characters_Rouge:
		return PlaySound_Rouge(ID, a2, a3, a4);
		break;
	case Characters_Charmy:
		return PlaySound_Charmy(ID, a2, a3, a4);
		break;
	case Characters_HeroesTails:
		return PlaySound_Tails(ID, a2, a3, a4);
		break;
	default:
		return PlaySound(ID, a2, a3, a4);
		break;
	}
}

void Characters_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	const string FlyCharacter = config->getString("2- Characters", "FlyCharacter", "None");
	
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

	WriteCall((void*)0x462456, Tails_Display_);
	WriteCall((void*)0x45C037, PlaySound_HeroesChar); //jump
	WriteCall((void*)0x45BE01, PlaySound_HeroesChar); //fly
	WriteCall((void*)0x45BF8D, PlaySound_HeroesChar); //hurt
	WriteCall((void*)0x446A49, PlaySound_HeroesChar); //death
	WriteCall((void*)0x45BE57, PlayVoice_HeoresChar); //death
}

void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (!EntityData1Ptrs[player]) continue;
		if (EntityData1Ptrs[player]->CharID == Characters_Tails && !HeroesChars[player]) {
			if (FlyCharEnabled == Characters_Cream) {
				HeroesChars[player] = LoadObject(LoadObj_Data1, 1, CreamHeroes_Main);
				HeroesChars[player]->Data1->CharIndex = player;
			}
			else if (FlyCharEnabled == Characters_Rouge) {
				HeroesChars[player] = LoadObject(LoadObj_Data1, 1, RougeHeroes_Main);
				HeroesChars[player]->Data1->CharIndex = player;
			}
			else if (FlyCharEnabled == Characters_Charmy) {
				HeroesChars[player] = LoadObject(LoadObj_Data1, 1, CharmyHeroes_Main);
				HeroesChars[player]->Data1->CharIndex = player;
			}
			else if (FlyCharEnabled == Characters_HeroesTails) {
				HeroesChars[player] = LoadObject(LoadObj_Data1, 1, TailsHeroes_Main);
				HeroesChars[player]->Data1->CharIndex = player;
			}
		}
	}
}