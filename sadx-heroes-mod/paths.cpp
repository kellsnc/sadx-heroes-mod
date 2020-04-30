#include "stdafx.h"

ObjectMaster * railcam;
extern LoopHead *MysticMansionPathList[70];

void TransformSpline(EntityData1* entity, NJS_VECTOR orig, NJS_VECTOR dest, float state) {
	entity->Position.x = (dest.x - orig.x) * state + orig.x;
	entity->Position.y = ((dest.y - orig.y) * state + orig.y);
	entity->Position.z = (dest.z - orig.z) * state + orig.z;
}

void PathHandler(EntityData1* entity, LoopHead* path, float speed) {
	if (entity->Scale.x > 1) { entity->Scale.x = 0; entity->InvulnerableTime++; };
	entity->Scale.x = entity->Scale.x + (path->TotalDist / path->LoopList[ entity->InvulnerableTime].Dist) / path->TotalDist * speed;
	TransformSpline(entity, path->LoopList[ entity->InvulnerableTime].Position, path->LoopList[ entity->InvulnerableTime + 1].Position, entity->Scale.x);
	entity->Rotation.y = fPositionToRotation(&path->LoopList[ entity->InvulnerableTime].Position, &path->LoopList[ entity->InvulnerableTime + 1].Position).y;
}

bool Path_GetDirection(EntityData1* PlayerEntity, LoopHead* loophead, uint16_t point) {
	//the rail is 2-directional, we need to check wheter we're going forward or backward
	//we get a point in front of sonic, one behind, and check which one is closer to the forward point
	Loop* LoopPoint = &loophead->LoopList[point];
	Loop* NextPoint = &loophead->LoopList[point + 1];

	NJS_VECTOR dir = { 20, 0, 0 };

	//we extend the next point pos for precision
	NJS_VECTOR NextPosExt;
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &NextPoint->Position);
	NJS_VECTOR pos = { NextPoint->Position.x, LoopPoint->Position.y, NextPoint->Position.z };
	njRotateXYZ(0, -LoopPoint->Ang_X, -fPositionToRotation(&LoopPoint->Position, &pos).y, -LoopPoint->Ang_Y);
	njCalcPoint(0, &dir, &NextPosExt);
	njPopMatrix(1u);

	//we get our points
	NJS_VECTOR front;
	NJS_VECTOR back;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &PlayerEntity->Position);
	njRotateY(0, -PlayerEntity->Rotation.y);

	dir.x = 5;
	njCalcPoint(0, &dir, &front);
	float frontdist = GetDistance(&front, &NextPosExt);

	dir.x = -5;
	njCalcPoint(0, &dir, &back);
	float backdist = GetDistance(&back, &NextPosExt);

	njPopMatrix(1u);

	//we compare them to the extended point
	if (frontdist < backdist) return false;
	else return true;
}

void AutoLoop_Main(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData; //current rail
	EntityData1 * entity = a1->Data1; //we can store data in this

	char slope = a1->Data1->field_A;

	if (a1->Data1->Action == 0) {
		EntityData1 * player = EntityData1Ptrs[a1->Data1->NextAction];

		if (!player) {
			a1->Data1->Action = 1;
			return;
		}

		CharObj2 * co2 = CharObj2Ptrs[a1->Data1->NextAction];

		if (loopdata->LoopList[a1->Data1->InvulnerableTime].Dist == 0 || ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {
			if (slope == 0) {
				a1->Data1->Action = 1;
				//Animations
				if (ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {
					player->Status = Status_Ball;
					if ((co2->Upgrades & Upgrades_SuperSonic) == 0) co2->AnimationThing.Index = 14;
					else co2->AnimationThing.Index = 145;
					co2->Speed.y = 2;
				}
				else {
					co2->Speed.x = 4;
					player->Status = 0;
					if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
						player->Action = 2;
						co2->AnimationThing.Index = 13;
					}
					else {
						co2->AnimationThing.Index = 137;
					}
				}

				if (player = EntityData1Ptrs[0]) SetCameraMode_(1);
				return;
			}
			else {
				if (loopdata->LoopList[a1->Data1->InvulnerableTime].Dist == 0) {
					if (player->Rotation.y = loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_Y != 0) {
						co2->Speed.x = 4;
						co2->Speed.y = 1;
					}
					a1->Data1->Action = 1;
					return;
				}
			}
		}
		else {
			//next position in spline
			a1->Data1->Scale.x = a1->Data1->Scale.x + (loopdata->TotalDist / loopdata->LoopList[a1->Data1->InvulnerableTime].Dist) / loopdata->TotalDist * 8;
			
			//animations
			player->Status = 0;
			switch (GetCharacterID(a1->Data1->NextAction)) {
			case Characters_Sonic:
			case Characters_Tails: 
				if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
					player->Action = 4;
					co2->AnimationThing.Index = 15;
				}
				else {
					co2->AnimationThing.Index = 145;
				}
				break;
			case Characters_Knuckles:
				if ((co2->Upgrades & Upgrades_SuperSonic) == 0) {
					player->Action = 4;
					co2->AnimationThing.Index = 35;
				}
				break;
			}

			TransformSpline(GetCharacterObject(a1->Data1->NextAction)->Data1, loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
			player->Rotation.y = fPositionToRotation(&loopdata->LoopList[a1->Data1->InvulnerableTime].Position, &loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position).y;
			a1->Data1->Position = player->Position;

			//go to next point
			if (a1->Data1->Scale.x > 1) { a1->Data1->Scale.x = 0; a1->Data1->InvulnerableTime++; }
		}
	}

	if (a1->Data1->Action == 1) {
		if (a1->Data1->CharIndex < 20) a1->Data1->CharIndex += 1;
		else {
			EntityData1 *entity = EntityData1Ptrs[a1->Data1->NextAction];
			entity->NextAction = 0;
			DeleteObjectMaster(a1);
		}
	}
}

void BoulderPath(ObjectMaster *a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData;

	if (a1->Data1->Action == 0) {
		a1->DisplaySub = a1->MainSub;
		a1->Data1->Action = 1;
	}
	else {
		if (a1->Data1->InvulnerableTime < loopdata->Count) {
			if (GameState != 16) {
				a1->Data1->Scale.x = a1->Data1->Scale.x + (loopdata->TotalDist / loopdata->LoopList[a1->Data1->InvulnerableTime].Dist) / loopdata->TotalDist * 8;
				TransformSpline(a1->Data1, loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
				if (loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_Y != 0) a1->Data1->Rotation.y = loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_Y;
				if (a1->Data1->Scale.x > 1) { a1->Data1->Scale.x = 0; a1->Data1->InvulnerableTime++; }
				a1->Data1->Rotation.x += 1500;

				a1->Data1->CharIndex = IsPlayerInsideSphere(&a1->Data1->Position, 130.0f);
				for (uint8_t player = 0; player < MaxPlayers; ++player) {
					if (player == a1->Data1->CharIndex - 1) {
						EntityData1 *ed1 = EntityData1Ptrs[player];
						CharObj2 *co2 = CharObj2Ptrs[player];

						ed1->Rotation.y = a1->Data1->Rotation.y + 0x4000;
						co2->Speed.x = 22;
						co2->Speed.y = 2;
						HurtCharacter(player);
					}
				}
			}
			
			SetHeroesLeveltex();
			njPushMatrix(0);
			DrawQueueDepthBias = -6000.0f;

			njTranslateV(0, &a1->Data1->Position);
			njRotateY(nullptr, a1->Data1->Rotation.y);
			njRotateX(nullptr, a1->Data1->Rotation.x);

			njDrawModel_SADX(a1->Parent->Data1->Object->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
		else {
			SetCameraMode_(1);
			DeleteObject(a1);
		}
	}
}

//Detects if a player interacts with a path
void Path_Main(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData;
	EntityData1 * entity = a1->Data1;

	char type = a1->Data1->field_A;

	if (!a1->Data1->Action) {
		EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
		for (uint8_t slot = 0; slot < MaxPlayers; ++slot) {
			if (players[slot]) {

				if (players[slot]->NextAction == 1) return;
				if (CurrentLevel == HeroesLevelID_SeasideHill && players[slot]->Position.z > -900) return;
				if (a1->Data1->field_A == 2 && CharObj2Ptrs[slot]->Upgrades & Upgrades_SuperSonic) return;

				for (uint8_t point = 0; point < loopdata->Count - 1; ++point) {
					for (float l = 0; l <= 1; l += 0.01f) {
						a1->Data1->Position.x = (loopdata->LoopList[point + 1].Position.x - loopdata->LoopList[point].Position.x) * l + loopdata->LoopList[point].Position.x;
						a1->Data1->Position.y = (loopdata->LoopList[point + 1].Position.y - loopdata->LoopList[point].Position.y) * l + loopdata->LoopList[point].Position.y;
						a1->Data1->Position.z = (loopdata->LoopList[point + 1].Position.z - loopdata->LoopList[point].Position.z) * l + loopdata->LoopList[point].Position.z;

						int range = 10;
						if (a1->Data1->field_A == 2) range = 5;

						if ((a1->Data1->Position.x > (players[slot]->Position.x - range) && a1->Data1->Position.x < (players[slot]->Position.x + range))
							&& (a1->Data1->Position.y > (players[slot]->Position.y - range) && a1->Data1->Position.y < (players[slot]->Position.y + range))
							&& (a1->Data1->Position.z > (players[slot]->Position.z - range) && a1->Data1->Position.z < (players[slot]->Position.z + range))) {

							players[slot]->Position = a1->Data1->Position;
							a1->Data1->Action = 1;

							//we create a new object to support 8 players (one object per character)
							ObjectMaster * tempobj = LoadObject(LoadObj_Data1, 0, AutoLoop_Main);

							CharObj2 * co2 = CharObj2Ptrs[slot];
							tempobj->Data1->InvulnerableTime = point; //store current spline point
							tempobj->Data1->Scale.x = l; //store position in spline
							tempobj->Data1->NextAction = slot; //store character it applies to
							tempobj->Data1->LoopData = a1->Data1->LoopData;
							tempobj->Data1->field_A = type;
							players[slot]->NextAction = 1;

							if (type == 1) {
								//Load Camera
								railcam = LoadObject(LoadObj_Data1, 2, (ObjectFuncPtr)0x613460);
								railcam->Data1->LoopData = (Loop*)a1->Data1->LoopData;
								railcam->Data1->Object = a1->Data1->Object;
							}

							if (type == 0 && slot == 0) camera_flags = 6;

							break;
						}
					}
					if (a1->Data1->Action == 1) break;
				}
			}
		}
	}
	else a1->Data1->Action = 0;
}

//Path Initializing
void AutoLoop(ObjectMaster * a1) {
	a1->MainSub = Path_Main;
}

void SlopePath(ObjectMaster * a1) {
	a1->Data1->field_A = 1;
	a1->MainSub = Path_Main;
}