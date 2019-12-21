#include "stdafx.h"

uint8_t FlyCharEnabled;
uint8_t SpeedCharEnabled;
uint8_t PowerCharEnabled;

bool CustomPhysics = true;
bool JumpBallEnabled = true;
bool P2SoundsEnabled = false;

ObjectMaster* HeroesChars[8];
bool CharTexsLoaded[9];
int CurrentPlayer;

ModelInfo* CharMdls[2];
CollisionData Tornado_Col = { 0, 0, 0, 0, 0, { 0.0f, 0.0f, 0.0f }, { 20, 0.0f, 0.0f }, 0, 0 };

DataPointer(NJS_VECTOR, bombpos, 0x3C5AB24);
float bombsize;

ObjectFuncPtr DisplayFuncs[]{
	CreamHeroes_Display,
	RougeHeroes_Display,
	CharmyHeroes_Display,
	TailsHeroes_Display,
	SonicHeroes_Display,
	ShadowHeroes_Display,
	AmyHeroes_Display,
	EspioHeroes_Display,
	KnucklesHeroes_Display
};

ObjectFuncPtr MainFuncs[]{
	CreamHeroes_Main,
	RougeHeroes_Main,
	CharmyHeroes_Main,
	TailsHeroes_Main,
	SonicHeroes_Main,
	ShadowHeroes_Main,
	AmyHeroes_Main,
	EspioHeroes_Main,
	KnucklesHeroes_Main
};

PlaySoundFuncPtr SoundFuncs[]{
	PlaySound_Cream,
	PlaySound_Rouge,
	PlaySound_Charmy,
	PlaySound_Tails,
	PlaySound_Sonic,
	PlaySound_Shadow,
	PlaySound_Amy,
	PlaySound_Espio,
	PlaySound_Knuckles
};

PlaySoundFuncPtr VoiceFuncs[]{
	PlayVoice_Cream,
	PlayVoice_Rouge,
	PlayVoice_Charmy,
	PlayVoice_Tails,
	PlayVoice_Sonic,
	PlayVoice_Shadow,
	PlayVoice_Amy,
	PlayVoice_Espio,
	PlayVoice_Knuckles
};

Uint32 CharColours[]{
	0x96FBCA04,
	0x96FB04FB,
	0x96FBFB04,
	0x96FBFB04,
	0x960080FF,
	0xBC660000,
	0x96FB7D88,
	0x96BD008A,
	0x96E90500
};

VoidFunction LoadFiles[]{
	LoadCreamFiles,
	LoadRougeFiles,
	LoadCharmyFiles,
	LoadTailsFiles,
	LoadSonicFiles,
	LoadShadowFiles,
	LoadAmyFiles,
	LoadEspioFiles,
	LoadKnuckFiles
};

//Store the current player id at the start of their function
//to get which character triggered a sound, as PlaySound doesn't keep track of the entity
void Knuckles_Main_r(ObjectMaster* obj);
Trampoline Knuckles_Main_t((int)Knuckles_Main, (int)Knuckles_Main + 0x8, Knuckles_Main_r);
void Knuckles_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	ObjectFunc(original, Knuckles_Main_t.Target());
	original(obj);
}

void Tails_Main_r(ObjectMaster* obj);
Trampoline Tails_Main_t((int)Tails_Main, (int)Tails_Main + 0x12, Tails_Main_r);
void Tails_Main_r(ObjectMaster* obj) {
	CurrentPlayer = obj->Data1->CharIndex;

	//Player 2 can play sounds
	if (P2SoundsEnabled && HeroesChars[obj->Data1->CharIndex]) {
		WriteData((char*)0x45C02C, (char)0x99);
		WriteData((char*)0x45BDF3, (char)0x99);
		WriteData((char*)0x45BF7F, (char)0x99);
	}

	ObjectFunc(original, Tails_Main_t.Target());
	original(obj);

	if (P2SoundsEnabled && HeroesChars[obj->Data1->CharIndex]) {
		WriteData((char*)0x45C02C, (char)0x01);
		WriteData((char*)0x45BDF3, (char)0x01);
		WriteData((char*)0x45BF7F, (char)0x01);
	}
}

void Sonic_Act1_r(EntityData1 *entity1, EntityData2 *entity2, CharObj2 *obj2);
Trampoline Sonic_Act1_t((int)Sonic_Act1, (int)Sonic_Act1 + 0x8, Sonic_Act1_r);
void Sonic_Act1_r(EntityData1 *entity1, EntityData2 *entity2, CharObj2 *obj2) {
	CurrentPlayer = entity1->CharIndex;

	FunctionPointer(void, original, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *obj2), Sonic_Act1_t.Target());
	original(entity1, entity2, obj2);
}

//Character Animation like Sonic Heroes does
void PlayHeroesAnimation(ObjectMaster* obj, uint8_t ID, AnimData* animdata, float forcespeed, float forcestate) {
	EntityData1* data = obj->Data1;
	
	if (data->Index != ID) {
		data->Index = ID;
		data->Scale.x = 0;
		data->Unknown = 0;
	}

	float frame = data->Scale.x;

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

	if (GameState == 9 || (GameState == 8 && Lives == 0)) {
		int character = obj->Data1->CharID;
		bool ArethereOthers = false;

		for (uint8_t player = 0; player < 8; ++player) {
			if (player != obj->Data1->CharIndex && HeroesChars[player]) {
				if (HeroesChars[player]->Data1->CharID == character) ArethereOthers = true;
			}
		}

		if (!ArethereOthers) {
			njReleaseTexture((NJS_TEXLIST*)obj->Data1->LoopData);
			CharTexsLoaded[character - 9] = false;
		}
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
		(playerdata->CharID == Characters_Knuckles && data->Index == 14) ||
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
			LoadFiles[data->CharID - 9]();
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

	char charid = playerobj->Data1->CharID;
	if ((charid == Characters_Sonic && (data->CharID < Characters_HeroesSonic || data->CharID > Characters_Espio)) ||
		(charid == Characters_Tails && data->CharID > Characters_HeroesTails) ||
		(charid == Characters_Knuckles && data->CharID < Characters_HeroesKnuckles) || !playerobj) {
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
	case 9: data->Status = 0; anim = 0; if (playerco2->Speed.x < 0.02f) anim = 5; break;
	case 10: anim = 0; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 11: anim = 5; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 12: anim = 6; speed = 1.5f + playerco2->Speed.x * 0.1f; 
		if (data->CharID == Characters_Shadow && playerco2->Speed.x > 3) anim = 7; break;
	case 13: anim = 7; speed = 0.5f + playerco2->Speed.x * 0.1f; 
		if (data->CharID == Characters_Shadow && playerco2->Speed.x > 3) anim = 57; break;
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
	case 47: anim = 40; break;
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
		if (data->CharID == Characters_Espio) anim = 55;
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

NJS_VECTOR PowerAnims(EntityData1* data, EntityData1* playerdata, CharObj2* playerco2) {
	int anim = playerco2->AnimationThing.Index;
	float speed = 0;
	float state = 0;
	float frame = data->Scale.x;

	switch (playerco2->AnimationThing.Index) {
	case 0: case 1:	case 2:	case 7: case 8: anim = 54; data->Status = 0; 
		if (playerco2->AnimationThing.Index == 0 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500 && playerdata->Action > 5) anim = 49; break; //stance
	case 3: case 4: case 5: case 6: anim = 55; if (++data->Status == 100) { playerco2->AnimationThing.Index = 0; data->Status = 0; } break; //idle
	case 9: data->Status = 0; anim = 0; if (playerco2->Speed.x < 0.02f) anim = 5; break;
	case 10: anim = 0; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 11: anim = 5; speed = 0.9f + playerco2->Speed.x * 0.2f; break;
	case 12: anim = 6; speed = 1.5f + playerco2->Speed.x * 0.1f;
		if (data->CharID == Characters_Shadow && playerco2->Speed.x > 3) anim = 7; break;
	case 13: anim = 7; speed = 0.5f + playerco2->Speed.x * 0.1f;
		if (data->CharID == Characters_Shadow && playerco2->Speed.x > 3) anim = 57; break;
	case 14: //jumping
		if (anim < 13 || anim > 18) { anim = 13; }
		else if (anim == 14) { if (data->Unknown > 2) anim = 15; }
		else if (anim == 16) { if (playerdata->Position.y - playerco2->_struct_a3.DistanceMax < 10) anim = 17; } break;
	case 15: case 16: case 17: case 34: anim = 14; break; //roll
	case 18: //fall after spring jump
		if (playerco2->Speed.x > 1 || playerdata->Action == 14) anim = 49;
		else if (playerco2->Speed.y > 2 && playerco2->Speed.x < 1) anim = 50;
		else anim = 16; break;
	case 19: anim = 16; //falling
		if (playerco2->Speed.x > 8 && playerdata->Position.y - playerco2->_struct_a3.DistanceMax > 500) {
			anim = 49;
		}
		break;
	case 20: anim = 18; break;
	case 21: anim = 36; //break
		if (playerco2->Speed.x > 6) anim = 35;
		else if (playerco2->Speed.x > 3) anim = 34; break;
	case 22: anim = 38; break;
	case 23: case 24: anim = 2; break; //push
	case 25: case 27: case 28: case 38: anim = 42; break; //hurt, drowning
	case 30: anim = 41; break;
	case 33: anim = 18; break;
	case 39: case 40: anim = 52; break;
	case 41: case 42: case 43: case 44: anim = 10; break;
	case 45: anim = 45; break;
	case 51: anim = 19; 
		if (data->field_A == 0) {
			data->field_A = 1;
			PlayHeroesSound(CommonSound_GlideBegin);
		} break; //gliding
	case 46: case 47: case 48: anim = 1; break;
	case 49: case 52: anim = 33; break;
	case 50: anim = 47; break;
	case 54: anim = 11; break;
	case 55: anim = 10; break;
	case 56: anim = 12; break;
	case 70: case 71: case 72: case 73: case 74: case 75: case 85: case 86: case 104: case 107: anim = 1; break;
	case 84: case 101: case 102: case 103: case 108: case 109: case 110: anim = 29; break;
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
	if (++data->field_A == 130 || PressedButtons[data->CharIndex] & Buttons_X || IsPointInCollision(&playerdata->Position, 5)) {
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
bool KickTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata) {
	if (data->field_A == 0) {
		data->field_A = 1;
		playerco2->Powerups |= Powerups_Invincibility;
		playerco2->Speed.x = 5;
	}
	else if (data->field_A < 20) {
		if (++data->field_A == 10) return true;
	}
	else {
		data->field_A = 0;
		playerco2->Powerups &= ~Powerups_Invincibility;
		playerdata->Action = 2;
		data->Action = 2;
	}

	return false;
}

void ExploseEnemies(NJS_VECTOR* pos, float size) {
	bombpos = *pos;
	bombsize = size * 10;
}

//Power combo trick, return the attacks' number as it launches, 4 is custom character attack.
int PowerComboTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata) {
	int combo = data2->field_30;
	float frame = data->Scale.x;
	playerdata->Action = 2;

	switch (combo) {
	case 0:
		data2->field_30 = 1;
		playerco2->Speed.x = 2;
		playerco2->Powerups |= Powerups_Invincibility;
		return 1;
	case 1:
		if (frame > 30) {
			data2->field_30 = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
			data->Action = 2;
			return 0;
		}
		else if (frame > 15) {
			if (PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				data2->field_30 = 2;
				playerco2->Speed.x = 3;
				data->Scale.x = 30;
				return 2;
			}
		}
		else if (frame > 5) {
			if (PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				data2->field_30 = 1;
				playerco2->Speed.x = 1;
				data->Scale.x = 0;
				return 1;
			}
		}
		break;
	case 2:
		if (frame > 60) {
			data2->field_30 = 0;
			playerco2->Powerups &= ~Powerups_Invincibility;
			data->Action = 2;
			return 0;
		}
		else if (frame > 45) {
			if (PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				data2->field_30 = 3;
				data->Scale.x = 60;
				return 3;
			}
		}
		else if (frame > 35) {
			if (PressedButtons[data->CharIndex] & Buttons_X && playerdata->Status & Status_Ground) {
				data2->field_30 = 1;
				playerco2->Speed.x = 1;
				data->Scale.x = 0;
				return 1;
			}
		}
		break;
	case 3:
		return 4;
	}
	
	return 0;
}

//Power flight punch, return when the attack launches
bool FlightPunchTrick(EntityData1* data, EntityData2* data2, CharObj2* playerco2, EntityData1* playerdata) {
	playerdata->Action = 2;
	
	if (data->field_A == 0) {
		data->field_A = 1;
		PlayHeroesSound(CommonSound_FlyPunchBegin);
		playerco2->Powerups |= Powerups_Invincibility;
		playerco2->Speed.y = 1;
		playerco2->Speed.x = 1;
		return true;
	}
	else if (data->field_A == 1) {
		playerco2->Speed.y -= 0.01f;
		playerco2->Speed.x = 0;

		if (playerdata->Status & Status_Ground) {
			playerco2->Powerups &= ~Powerups_Invincibility;
			PlayHeroesSound(CommonSound_FlyPunchHit);
			ExploseEnemies(&playerdata->Position, 3);
			data->field_A = 2;
		}
		
		if (data->Scale.x > 29) {
			playerco2->Powerups &= ~Powerups_Invincibility;
			data->field_A = 0;
			data->Action = 2;
		}
	}
	else {
		if (data->Scale.x > 29) {
			data->field_A = 0;
			data->Action = 2;
		}
	}

	return false;
}

//Add new weaknesses for enemies: cheese, tails' trap rings, tornadoes, etc.
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2);
Trampoline OhNoImDead2_t(0x004CE030, 0x004CE036, OhNoImDead2);
bool OhNoImDead2(EntityData1 *a1, ObjectData2 *a2) {
	if (a1->CollisionInfo->CollidingObject) {
		if (a1->CollisionInfo->CollidingObject->Object->MainSub == Cheese_Main
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == TrapRing_Main
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == TornadoObj
			|| a1->CollisionInfo->CollidingObject->Object->MainSub == NinjaObj) return 1;
	}

	float i = GetDistance(&bombpos, &a1->Position);

	if (GetDistance(&bombpos, &a1->Position) < bombsize) 
		return 1;

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
		case Characters_Knuckles:
			Knuckles_Display(obj);
			break;
		}

		return;
	}

	if (obj->Data1->CharID == Characters_Sonic && SuperSonicFlag && HeroesChars[obj->Data1->CharIndex]) {
		Sonic_Display(obj);
		obj->DisplaySub = Sonic_Display;
		DeleteObject_(HeroesChars[obj->Data1->CharIndex]);
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
	const std::string PowerCharacter = config->getString("2- Characters", "PowerCharacter", "None");

	CustomPhysics = config->getBool("2- Characters", "CustomPhysics", true);
	JumpBallEnabled = config->getBool("2- Characters", "JumpBallEnabled", true);
	P2SoundsEnabled = config->getBool("2- Characters", "P2SoundsEnabled", false);

	if (!SpeedCharacter.compare("Sonic")) {
		SpeedCharEnabled = Characters_HeroesSonic;
	}
	else if (!SpeedCharacter.compare("Shadow")) {
		SpeedCharEnabled = Characters_Shadow;
	}
	else if (!SpeedCharacter.compare("Amy")) {
		SpeedCharEnabled = Characters_HeroesAmy;
	}
	else if (!SpeedCharacter.compare("Espio")) {
		SpeedCharEnabled = Characters_Espio;
	}
	
	if (!FlyCharacter.compare("Cream")) {
		FlyCharEnabled = Characters_Cream;
	}
	else if (!FlyCharacter.compare("Rouge")) {
		FlyCharEnabled = Characters_Rouge;
	}
	else if (!FlyCharacter.compare("Charmy")) {
		FlyCharEnabled = Characters_Charmy;
	}
	else if (!FlyCharacter.compare("Tails")) {
		FlyCharEnabled = Characters_HeroesTails;
	}

	if (!PowerCharacter.compare("Knuckles")) {
		PowerCharEnabled = Characters_HeroesKnuckles;
	}
	else if (!PowerCharacter.compare("Omega")) {
		PowerCharEnabled = Characters_Omega;
	}
	else if (!PowerCharacter.compare("Big")) {
		PowerCharEnabled = Characters_HeroesBig;
	}
	else if (!PowerCharacter.compare("Vector")) {
		PowerCharEnabled = Characters_Vector;
	}

	if (SpeedCharEnabled) {
		WriteCall((void*)0x49BF04, Heroes_Display);
		WriteCall((void*)0x495EAA, PlaySound_HeroesChar); //jump
		WriteCall((void*)0x494BE7, PlaySound_HeroesChar); //dash
		WriteCall((void*)0x492E96, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x492F08, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x492DD4, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x492DD4, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x446A26, PlaySound_HeroesChar); //death

		//Common special effects
		CharMdls[1] = LoadObjectModel(CharMdls[1], "effect_tornado");

		//Remove the dash trail
		WriteData<5>((void*)0x49B238, 0x90);
		WriteData<5>((void*)0x49AF36, 0x90);
		WriteData<5>((void*)0x49AFD7, 0x90);
		WriteData<5>((void*)0x49BB46, 0x90);
		WriteData<5>((void*)0x49BC1A, 0x90);
		WriteData<5>((void*)0x49BC29, 0x90);
		WriteData<5>((void*)0x49AFE6, 0x90);
		WriteData<5>((void*)0x49BB55, 0x90);
		WriteData<5>((void*)0x49BC29, 0x90);
	}

	if (FlyCharEnabled) {
		WriteCall((void*)0x462456, Heroes_Display);
		WriteCall((void*)0x45C037, PlaySound_HeroesChar); //jump
		WriteCall((void*)0x45BE01, PlaySound_HeroesChar); //fly
		WriteCall((void*)0x45BF8D, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x446A49, PlaySound_HeroesChar); //death
	}

	if (PowerCharEnabled) {
		WriteCall((void*)0x47B489, Heroes_Display);
		WriteCall((void*)0x473766, PlaySound_HeroesChar); //jump
		WriteCall((void*)0x4507AC, PlaySound_HeroesChar); //hurt
		WriteCall((void*)0x446A49, PlaySound_HeroesChar); //death
	}

	if (SpeedCharEnabled || FlyCharEnabled || PowerCharEnabled) {
		CharMdls[0] = LoadCommonModel("effect_ball");
		WriteCall((void*)0x45BE57, PlayVoice_HeroesChar); //win
	}
}

//Replace characters if they are loaded
void Characters_OnFrame() {
	for (uint8_t player = 0; player < 8; ++player) {
		if (!EntityData1Ptrs[player] || HeroesChars[player]) continue;

		if (SpeedCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Sonic && !SuperSonicFlag) {
			HeroesChars[player] = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, MainFuncs[SpeedCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
			HeroesChars[player]->Data1->CharID = SpeedCharEnabled;
		}
		else if (FlyCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Tails) {
			HeroesChars[player] = LoadObject(LoadObj_Data1, 1, MainFuncs[FlyCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
			HeroesChars[player]->Data1->CharID = FlyCharEnabled;
		}
		else if (PowerCharEnabled && EntityData1Ptrs[player]->CharID == Characters_Knuckles) {
			HeroesChars[player] = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, MainFuncs[PowerCharEnabled - 9]);
			HeroesChars[player]->Data1->CharIndex = player;
			HeroesChars[player]->Data1->CharID = PowerCharEnabled;
		}
	}
}