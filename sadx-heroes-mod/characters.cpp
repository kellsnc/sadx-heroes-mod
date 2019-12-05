#include "stdafx.h"

uint8_t FlyCharEnabled;
uint8_t SpeedCharEnabled;
ObjectMaster* HeroesChars[8];
bool CharTexsLoaded[5];
int CurrentPlayer;

bool JumpBallEnabled = true;

ModelInfo* CharMdls[2];
CollisionData Tornado_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, { 20, 0.0f, 0.0f }, 0, 0 };

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

Uint32 CharColours[]{
	0x96FBCA04,
	0x96FB04FB,
	0x96FBFB04,
	0x96FBFB04,
	0x960080FF,
};

//Store the current player id at the start of their function
//to get which character triggered a sound, as PlaySound doesn't keep track of the entity
void Tails_Main_r(ObjectMaster* a1);
Trampoline Tails_Main_t(0x00461700, 0x00461712, Tails_Main_r);
void Tails_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	ObjectFunc(original, Tails_Main_t.Target());
	original(obj);
}

//Character Animation like Sonic Heroes does
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

//Common player removal function
void CharactersCommon_Delete(ObjectMaster* obj) {
	HeroesChars[obj->Data1->CharIndex] = nullptr;

	int character = obj->Data1->CharID;
	bool ArethereOthers = false;

	for (uint8_t player = 0; player < 8; ++player) {
		if (player != obj->Data1->CharIndex && PlayerPtrs[player]) {
			if (HeroesChars[player]->Data1->CharID == character) ArethereOthers = true;
		}
	}

	if (!ArethereOthers) {
		njReleaseTexture((NJS_TEXLIST*)obj->Data1->LoopData);
		CharTexsLoaded[character - 9] = false;
	}

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

//Display the ball and dash effects
void BallObject(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	
	if (GameState != 16) {
		if (data->Index != 0) {
			DeleteObject_(obj);
			return;
		}
	}
	
	njPushMatrix(0);
	njSetTexture(&SHCommonTextures);
	njTranslateV(0, &obj->Data1->Position);
	njRotateZ(0, obj->Data1->Rotation.z);
	njRotateX(0, obj->Data1->Rotation.x);
	njRotateY(0, -obj->Data1->Rotation.y - 0x4000);
	

	if (data->Action == 0) {
		njTranslate(0, 0, 5.5f, -2);
		CharMdls[0]->getmodel()->basicdxmodel->mats[0].diffuse.color = CharColours[obj->Data1->CharID - 9];
		njDrawModel_SADX(CharMdls[0]->getmodel()->basicdxmodel);
	}
	else {
		njTranslate(0, 0, 4, -4);
		njScale(0, 0.9f, 0.9f, 0.9f);
		njRotateZ(0, 0x4000);
		njRotateX(0, 0x4000);
		njRotateY(0, 0x7000);
		CharMdls[0]->getmodel()->child->basicdxmodel->mats[0].diffuse.color = CharColours[obj->Data1->CharID - 9];
		njDrawModel_SADX(CharMdls[0]->getmodel()->child->basicdxmodel);
	}
	
	njPopMatrix(1);

	obj->Data1->Index = 1;
}

void CharactersCommon_DrawBall(EntityData1* playerdata, EntityData1* data) {
	if (!JumpBallEnabled) return;
	
	if (playerdata->CharID == Characters_Sonic) {
		if (data->Scale.z != 14) {
			data->Scale.z = playerdata->Action;
		}
		else {
			if (++data->Scale.y > 50 || playerdata->Status & Status_Ground) {
				data->Scale.z = 0;
				data->Scale.y = 0;
			}
		}
	}
	
	if ((playerdata->CharID == Characters_Sonic && data->Index == 14) ||
		(playerdata->CharID == Characters_Tails && data->Index == 19)) {

		ObjectMaster * ball = LoadObject(LoadObj_Data1, 5, BallObject);
		ball->DisplaySub = ball->MainSub;
		ball->Data1->Position = playerdata->Position;
		ball->Data1->Rotation = playerdata->Rotation;
		ball->Data1->CharID = data->CharID;
	}
	else if ((playerdata->CharID == Characters_Sonic && data->Index == 49) && data->Scale.z == 14) {
		ObjectMaster * ball = LoadObject(LoadObj_Data1, 5, BallObject);
		ball->DisplaySub = ball->MainSub;
		ball->Data1->Position = playerdata->Position;
		ball->Data1->Rotation = playerdata->Rotation;
		ball->Data1->CharID = data->CharID;
		ball->Data1->Action = 1;
	}
}

//Common player init function
bool CharactersCommon_Init(ObjectMaster* obj, const char* name, NJS_TEXLIST* tex) {
	EntityData1* data = obj->Data1;
	ObjectMaster* playerobj = PlayerPtrs[data->CharIndex];

	if (data->Action == 0) {
		obj->DeleteSub = CharactersCommon_Delete;
		data->Action = 1;

		if (!CharTexsLoaded[data->CharID - 9]) {
			CharTexsLoaded[data->CharID - 9] = true;
			LoadPVM(name, tex);
		}

		data->LoopData = (Loop*)tex;

		return false;
	}
	else if (data->Action == 1) {
		if (playerobj->Data1) {
			obj->Data1->Action = 2;
			return true;
		}
		
		return false;
	}

	if (!playerobj || (data->CharID >= Characters_HeroesSonic && playerobj->Data1->CharID != Characters_Sonic) 
		|| (data->CharID < Characters_HeroesSonic && playerobj->Data1->CharID != Characters_Tails)) { 
		DeleteObject_(obj); 
		return false; 
	}

	return true;
}

//Speed characters common anims
NJS_VECTOR SpeedAnims(EntityData1* data, EntityData1* playerdata, CharObj2* playerco2) {
	int anim = playerco2->AnimationThing.Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	switch (playerco2->AnimationThing.Index) {
	case 0: case 1:	case 2:	case 7: case 8:anim = 54; data->Status = 0; break; //stance
	case 3: case 4: case 5: case 6: anim = 55; if (++data->Status == 100) { playerco2->AnimationThing.Index = 0; data->Status = 0; } break; //idle
	case 9: data->Status = 0; anim = 0; if (playerco2->Speed.x < 0.02f) anim = 11; break;
	case 10: anim = 0; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 11: anim = 5; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 12: anim = 6; speed = 1.5f + playerco2->Speed.x * 0.1f; break;
	case 13: anim = 7; speed = 0.5f + playerco2->Speed.x * 0.1f; break;
	case 14: //jumping
		if (anim < 13 || anim > 18) { anim = 13; }
		else if (anim == 14) { if (data->Unknown > 2) anim = 15; }
		else if (anim == 16) { if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 17; } break;
	case 15: case 16: case 17: anim = 14; break; //roll
	case 18: //fall after spring jump
		if (playerco2->Speed.x > 1 || playerdata->Action == 14) anim = 49;
		else if (playerco2->Speed.y > 2 && playerco2->Speed.x < 1) anim = 50;
		else anim = 16; break;
	case 19: anim = 18; //falling
		if (playerco2->Speed.x > 8 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500) {
			anim = 49;
			playerco2->AnimationThing.Index = 150;
		}
		break;
	case 20: case 25: anim = 36; //break
		if (playerco2->Speed.x > 6) anim = 35;
		else if (playerco2->Speed.x > 3) anim = 34; break;
	case 21: case 22: anim = 48; break; //push
	case 24: anim = 42; break; //hurt
	case 26: case 46: anim = 41; break; //updraft
	case 27: case 28: anim = 42; break;
	case 29: anim = 18; break;
	case 30: anim = 37; break;
	case 31: //ball
	case 32: case 39: case 40: case 41: case 42: case 43: case 45: case 83: case 125: anim = 14; break;
	case 44: anim = 9; break;
	case 47: anim = 80; anim = 29; break;
	case 48: anim = 1; break; //pull
	case 49: //shake tree
	case 50: //pickup
	case 51: //shake
	case 53: case 54: case 55: case 56:
	case 65: //car
	case 77: //rocket
	case 130: case 131: anim = 33; break; //grab
	case 52: case 57: case 58: case 59: case 60: case 61: case 62: case 63: case 132: anim = 32; break; //put
	case 64:
	case 70:
	case 79: anim = 49; break; //rocket
	case 71: anim = 0; break; //ice
	case 75: case 76: //won
		anim = 52;
		if (data->Unknown > 1) {
			if (frame > 124) data->Unknown = 20;
			if (frame < 110) data->Unknown = 40;

			if (data->Unknown > 20 && data->Unknown < 40) {
				++data->Unknown;
				state = 124 - (data->Unknown - 20);
			}
			else if (data->Unknown > 40) {
				++data->Unknown;
				state = 110 + (data->Unknown - 40);
			}
		}
		break;
	case 82: anim = 42; speed = 0; break;
	case 84: anim = 45; break;
	case 85: case 86: case 87: anim = 42; break;
	case 88: anim = 53; break;
	case 102: case 105: case 109: case 110: case 111: case 112: case 113: case 114: case 115: anim = 21; break; //snowboard
	case 103: case 107: //snowboard right
		if (playerco2->Speed.z > 0.1f) anim = 28;
		else anim = 26;
		break;
	case 104: case 108: //snowboard left
		if (playerco2->Speed.z < 0.1f) anim = 27;
		else anim = 26;
		break;
	case 106: anim = 20; break;
	case 116: case 117: case 118: case 119: case 120: case 121: case 122: case 123: anim = 25; break;
	case 124: anim = 22; break;
	case 126: case 127: case 128: anim = 56; break;
	case 129: anim = 11;
	}

	return { (float)anim, speed, state };
}

//Speed characters tornado trick
void TornadoObj(ObjectMaster* obj) {
	if (GameState != 16) {
		obj->Data1->Rotation.y += 1000;
		obj->Data1->Rotation.x += 300;
		obj->Data1->Rotation.z += 500;

		if (++obj->Data1->InvulnerableTime >= 180) DeleteObject_(obj);
		if (obj->Data1->InvulnerableTime > 150) {
			obj->Data1->Scale.x = ((180 - obj->Data1->InvulnerableTime) / 30) * 250;

			obj->Data1->Scale.x = 180 - obj->Data1->InvulnerableTime;
			obj->Data1->Scale.x /= 30;
			obj->Data1->Scale.x *= 250;
		}

		AddToCollisionList(obj->Data1);
	}

	if (!MissedFrames) {
		njPushMatrix(0);
		njSetTexture(&SHCommonTextures);
		njTranslateV(0, &obj->Data1->Position);
		njScale(0, 0.8f, 0.7f, 0.8f);

		njRotateXYZ(0, 0x4000, obj->Data1->Rotation.y, 0);
		CharMdls[1]->getmodel()->basicdxmodel->mats[0].diffuse.color = CharColours[obj->Data1->CharID - 9];
		if (obj->Data1->Scale.x) CharMdls[1]->getmodel()->basicdxmodel->mats[0].diffuse.argb.a = obj->Data1->Scale.x;
		njDrawModel_SADX(CharMdls[1]->getmodel()->basicdxmodel);

		njRotateY(0, obj->Data1->Rotation.x);
		CharMdls[1]->getmodel()->child->basicdxmodel->mats[0].diffuse.color = CharColours[obj->Data1->CharID - 9];
		if (obj->Data1->Scale.x) CharMdls[1]->getmodel()->child->basicdxmodel->mats[0].diffuse.argb.a = obj->Data1->Scale.x;
		njDrawModel_SADX(CharMdls[1]->getmodel()->child->basicdxmodel);

		njRotateY(0, obj->Data1->Rotation.z);
		CharMdls[1]->getmodel()->child->child->basicdxmodel->mats[0].diffuse.color = CharColours[obj->Data1->CharID - 9];
		if (obj->Data1->Scale.x) CharMdls[1]->getmodel()->child->child ->basicdxmodel->mats[0].diffuse.argb.a = obj->Data1->Scale.x;
		njDrawModel_SADX(CharMdls[1]->getmodel()->child->child->basicdxmodel);

		njPopMatrix(1);
	}
}

void TornadoTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata) {
	if (++data->field_A == 130 || PressedButtons[data->CharIndex] & Buttons_X) {
		data->Action = 2;
		data->field_A = 0;

		playerco2->Speed.y = 2;
		playerco2->AnimationThing.Index = 129;
		playerco2->Powerups &= ~Powerups_Invincibility;
	}
	else {
		EntityData1* playerdata = EntityData1Ptrs[data->CharIndex];

		if (data->field_A == 1) {
			data2->SomeCollisionVector = playerdata->Position;
			data2->field_28 = 1;

			ObjectMaster * tornado = LoadObject(LoadObj_Data1, 5, TornadoObj);
			tornado->DisplaySub = tornado->MainSub;
			tornado->Data1->Position = playerdata->Position;

			if (playerco2->_struct_a3.DistanceMax - tornado->Data1->Position.y > -50)
				tornado->Data1->Position.y = playerco2->_struct_a3.DistanceMax;
			else
				tornado->Data1->Position.y -= 20;

			tornado->Data1->CharID = data->CharID;
			Collision_Init(tornado, &Tornado_Col, 1, 3u);

			PlayHeroesSoundQueue(CommonSound_Tornado, tornado, 500, false);
		}
		
		playerco2->Powerups |= Powerups_Invincibility;
		data->Rotation.z += 0x500;

		NJS_VECTOR dir = { 0, 0, 15 };
		njPushMatrix(_nj_unit_matrix_);
		njTranslateV(0, &data2->SomeCollisionVector);
		njRotateY(0, data->Rotation.z);
		njCalcPoint(0, &dir, &data2->VelocityDirection);
		njPopMatrix(1u);

		playerdata->Rotation.y = fPositionToRotation(&playerdata->Position, &data2->VelocityDirection).y;
		playerdata->Position = data2->VelocityDirection;
	}
}

//Speed characters Accel trick
void KickTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata) {
	if (data->field_A == 0) {
		data->field_A = 1;
		playerco2->Speed.x = 5;
	}
	else if (data->field_A < 20) {
		++data->field_A;
	}
	else {
		data->field_A = 0;
		playerdata->Action = 2;
		data->Action = 2;
	}
}

//Add new weaknesses for enemies: cheese, tails' trap rings and tornados
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2);
Trampoline OhNoImDead2_t(0x004CE030, 0x004CE036, OhNoImDead2);
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2) {
	if (a1->CollisionInfo->CollidingObject) {
		if (a1->CollisionInfo->CollidingObject->Object->MainSub == Cheese_Main
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == TrapRing_Main
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == TornadoObj) return 1;
	}

	FunctionPointer(bool, original, (EntityData1 *a1, ObjectData2 *a2), OhNoImDead2_t.Target());
	return original(a1, a2);
}

//Redirect the player's display sub if a Heroes character is loaded on top of it.
void Heroes_Display(ObjectMaster* obj) {
	if (!HeroesChars[obj->Data1->CharIndex]) {
		switch (EntityData1Ptrs[obj->Data1->CharIndex]->CharID) {
		case Characters_Sonic:
			Sonic_Display(obj);
			break;
		case Characters_Tails:
			Tails_Display(obj);
			break;
		}
		return;
	}

	DisplayFuncs[HeroesChars[obj->Data1->CharIndex]->Data1->CharID - 9](obj);
}

//Hijack the sound functions of Sonic, Tails and Knuckles to redirect those
//to play custom character sounds, as they do not keep track of the entity
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

//Load the files of selected characters
void Characters_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	const std::string SpeedCharacter = config->getString("2- Characters", "SpeedCharacter", "None");
	const std::string FlyCharacter = config->getString("2- Characters", "FlyCharacter", "None");
	JumpBallEnabled = config->getBool("2- Characters", "JumpBallEnabled", true);

	if (!SpeedCharacter.compare("Sonic")) {
		LoadSonicFiles(path, helperFunctions);
		SpeedCharEnabled = Characters_HeroesSonic;
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

	if (SpeedCharEnabled) {
		WriteCall((void*)0x49BF04, Heroes_Display);
	}

	if (FlyCharEnabled) {
		WriteCall((void*)0x462456, Heroes_Display);
		WriteCall((void*)0x45C037, PlaySound_HeroesChar); //jump
		WriteCall((void*)0x45BE01, PlaySound_HeroesChar); //fly
		WriteCall((void*)0x45BF8D, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x446A49, PlaySound_HeroesChar); //death
		WriteCall((void*)0x45BE57, PlayVoice_HeroesChar); //win
	}

	if (SpeedCharEnabled || FlyCharEnabled) {
		CharMdls[0] = LoadObjectModel("effect_ball");
		CharMdls[1] = LoadObjectModel("effect_tornado");
	}
}

//Replace characters if they are loaded
void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (!EntityData1Ptrs[player] || HeroesChars[player]) continue;

		if (SpeedCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Sonic) {
			HeroesChars[player] = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, MainFuncs[SpeedCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
			HeroesChars[player]->Data1->CharID = SpeedCharEnabled;
		}
		else if (FlyCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Tails) {
			HeroesChars[player] = LoadObject(LoadObj_Data1, 1, MainFuncs[FlyCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
			HeroesChars[player]->Data1->CharID = FlyCharEnabled;
		}
	}
}