#include "stdafx.h"
#include "mod.h"
#include <math.h>

ObjectMaster * railcam;
extern LoopHead *MysticMansionPathList[70];

void TransformSpline(ObjectMaster * a1, NJS_VECTOR orig, NJS_VECTOR dest, float state) {
	EntityData1 * entity = a1->Data1;
	entity->Position.x = (dest.x - orig.x) * state + orig.x;
	entity->Position.y = ((dest.y - orig.y) * state + orig.y);
	entity->Position.z = (dest.z - orig.z) * state + orig.z;
}

Rotation3 fPositionToRotation(NJS_VECTOR orig, NJS_VECTOR point) {
	NJS_VECTOR dist;
	Rotation3 result;

	dist.x = orig.x - point.x;
	dist.y = orig.y - point.y;
	dist.z = orig.z - point.z;

	result.x = atan2(dist.y, dist.z) * 65536.0 * -0.1591549762031479;
	float len = dist.z * dist.z + dist.y * dist.y;
	result.y = atan2(dist.x, squareroot(len)) * 65536.0 * 0.1591549762031479;

	if (dist.x < 0 && dist.z < 0) { result.y = result.y; result.x = result.x; }
	if (dist.z < 0 && dist.x > 0) { result.y = result.y; result.x = result.x; }
	if (dist.x < 0 || dist.z < 0) { result.y += 0x8000; result.x += 0x8000; }
	if (dist.x < 0 && dist.z > 0) { result.y += 0x8000; result.x += 0x8000; }

	result.y = -result.y - 0x4000;
	return result;
}

//Rail Physics
float RailValues[]{ 5, 8, 0.2f, 0.03f };

enum EnumRailPhysics {
	RailPhysics_MaxSpeed,
	RailPhysics_MaxAcceleration,
	RailPhysics_SpeedIncrement,
	RailPhysics_GravityInfluence,
};

void RailPhysics(ObjectMaster * a1, EntityData1 * player, CharObj2 * co2, LoopHead * loopdata) {
	float railspeed = a1->Data1->Scale.y;
	bool backward = a1->Data1->CharIndex;
	float maxspeed;

	//determine maxspeed
	if (ControllerPointers[a1->Data1->NextAction]->HeldButtons & Buttons_X) {
		maxspeed = RailValues[RailPhysics_MaxAcceleration];
		if (GetCharacterID(a1->Data1->NextAction) == Characters_Sonic) co2->AnimationThing.Index = 124;
	}
	else {
		maxspeed = RailValues[RailPhysics_MaxSpeed];
		if (GetCharacterID(a1->Data1->NextAction) == Characters_Sonic) co2->AnimationThing.Index = 102;
	}

	//we get the Y position of the current and next point to get the gravity acceleration
	float cY;
	float nY;
	if (!backward) {
		cY = loopdata->LoopList[a1->Data1->InvulnerableTime].Position.y;
		nY = loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position.y;
	}
	else {
		cY = loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position.y;
		nY = loopdata->LoopList[a1->Data1->InvulnerableTime].Position.y;
	}

	//now for simple gravity calculations
	cY -= nY;
	if (cY >= 0) cY /= 1300;
	else cY /= 1800;
	if (cY >= RailValues[RailPhysics_SpeedIncrement]) cY = RailValues[RailPhysics_SpeedIncrement];
	else if (cY >= RailValues[RailPhysics_GravityInfluence]) cY = RailValues[RailPhysics_GravityInfluence];
	if (cY >= 0 && a1->Data1->Scale.y >= maxspeed) a1->Data1->Scale.y -= RailValues[RailPhysics_SpeedIncrement]; //if we're over maxspeed, go slower
	else a1->Data1->Scale.y += cY;

	//turning animations
	int btn = HeldButtons2[a1->Data1->NextAction];
	if (btn & Buttons_Left) {
		if (GetCharacterID(a1->Data1->NextAction) == Characters_Sonic) co2->AnimationThing.Index = 103;
	}
	else if (btn & Buttons_Right) {
		if (GetCharacterID(a1->Data1->NextAction) == Characters_Sonic) co2->AnimationThing.Index = 104;
	}

	//too slow, change direction or eject player if on a straigth rail.
	if (a1->Data1->Scale.y < 0) {
		if (cY < 0) {
			a1->Data1->Scale.y = 0;
			if (a1->Data1->CharIndex == 1) a1->Data1->CharIndex = 0;
			else a1->Data1->CharIndex = 1;
		}
		else {
			a1->Data1->Action = 1;
			co2->Speed.z += 1;
		}
	}
}

//Perform path functions
void RailPath_Main(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData; //current rail
	EntityData1 * entity = a1->Data1; //we can store data in this

	if (a1->Data1->Action == 0) {
		EntityData1 * player = EntityData1Ptrs[a1->Data1->NextAction];

		if (!player) {
			a1->Data1->Action = 1;
			return;
		}

		CharObj2 * co2 = CharObj2Ptrs[a1->Data1->NextAction];
		
		bool web = false;
		if (loopdata->Unknown_0 != 0) web = true;

		bool backward = a1->Data1->CharIndex;
		if (web) backward = false;

		//detach from rail if it's the last point / if the player press jump
		if ((!backward && loopdata->LoopList[a1->Data1->InvulnerableTime].Dist == 0) //if no distance, we're at the end
			|| (backward && a1->Data1->InvulnerableTime == 0) //if position is 0, we're at the start
			|| ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {

			if (web) {
				if (ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A ||
					loopdata->Unknown_0 == 0xFF) {
					a1->Data1->Action = 1;

					if (ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {
						player->Status = Status_Ball;
						if (GetCharacterID(a1->Data1->NextAction) == Characters_Tails) player->Action = 8;
						co2->AnimationThing.Index = 14;
						co2->Speed.y = 2;
					}
					else {
						co2->Speed.x = 1;
						co2->Speed.y = 0;
						player->Action = 12;
						co2->AnimationThing.Index = 18;
					}
				}
				else {
					a1->Data1->LoopData = (Loop*)MysticMansionPathList[loopdata->Unknown_0];
					a1->Data1->InvulnerableTime = 0;
					a1->Data1->Scale = { 0, 0, 0 };
				}
			}
			else {
				a1->Data1->Action = 1;

				//Animations
				if (ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {
					player->Status = Status_Ball;
					if (GetCharacterID(a1->Data1->NextAction) == Characters_Tails) player->Action = 8;
					co2->AnimationThing.Index = 14;
					co2->Speed.y = 2;
				}
				else {
					co2->Speed.y = 1;
					co2->Speed.x = 1;
					player->Action = 12;
					co2->AnimationThing.Index = 18;
				}

				co2->Speed.x = 2;
			}

			return;
		}
		else {
			float railspeed = 4;

			if (!web) {
				RailPhysics(a1, player, co2, loopdata);
				railspeed = a1->Data1->Scale.y;

				if (++a1->field_30 > 125) {
					a1->field_30 = 0;
					PlaySound(49, 0, 0, 0);
				}
			}
			
			//next position in spline
			if (!backward) a1->Data1->Scale.x = a1->Data1->Scale.x + (loopdata->TotalDist / loopdata->LoopList[a1->Data1->InvulnerableTime].Dist) / loopdata->TotalDist * railspeed;
			else a1->Data1->Scale.x = a1->Data1->Scale.x - (loopdata->TotalDist / loopdata->LoopList[a1->Data1->InvulnerableTime].Dist) / loopdata->TotalDist * railspeed;

			//animations
			player->Status = 0;

			if (web) co2->AnimationThing.Index = 102;

			if (GetCharacterID(a1->Data1->NextAction) == Characters_Tails) {
				player->Action = 48;
				co2->AnimationThing.Index = 0;
			}
			if (GetCharacterID(a1->Data1->NextAction) == Characters_Knuckles) co2->AnimationThing.Index = 0;

			TransformSpline(GetCharacterObject(a1->Data1->NextAction), loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
			player->Position.y += 5;
			if (CurrentLevel == 7) player->Position.y += 5;
			
			if (web) player->Rotation.y = fPositionToRotation(loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position).y;
			else player->Rotation = fPositionToRotation(loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position);
			player->Rotation.z = 0;

			if (loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_X != 0) player->Rotation.x = loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_X;
			if (backward) player->Rotation.y += 0x8000;

			//go to next point
			if (!backward && a1->Data1->Scale.x > 1) { a1->Data1->Scale.x = 0; a1->Data1->InvulnerableTime++; }
			if (backward && a1->Data1->Scale.x < 0) { a1->Data1->Scale.x = 1; a1->Data1->InvulnerableTime--; }
		}
	}

	if (a1->Data1->Action == 1) {
		if (a1->Data1->CharIndex < 20) a1->Data1->CharIndex += 1;
		else {
			EntityData1 *entity = EntityData1Ptrs[a1->Data1->NextAction];

			entity->NextAction = 0;
			if (railcam) {
				DeleteObjectMaster(railcam);
				railcam = nullptr;
			}

			DeleteObjectMaster(a1);
		}
	}
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
					player->Rotation.y = loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_Y;
					if (player->Rotation.y = loopdata->LoopList[a1->Data1->InvulnerableTime].Ang_X == 0) {
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

			TransformSpline(GetCharacterObject(a1->Data1->NextAction), loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position, a1->Data1->Scale.x);
			player->Rotation.y = fPositionToRotation(loopdata->LoopList[a1->Data1->InvulnerableTime].Position, loopdata->LoopList[a1->Data1->InvulnerableTime + 1].Position).y;

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

//Detects if a player interacts with a path
void Path_Main(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData;
	EntityData1 * entity = a1->Data1;

	char type = a1->Data1->field_A;

	if (!a1->Data1->Action) {
		EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
		for (uint8_t slot = 0; slot < 8; ++slot) {
			if (players[slot]) {

				if (players[slot]->NextAction == 1) return;
				if (CurrentLevel == HeroesLevelID_SeasideHill && players[slot]->Position.z > -900) return;
				if (a1->Data1->field_A == 2 && CharObj2Ptrs[slot]->Upgrades & Upgrades_SuperSonic) return;

				for (uint8_t point = 0; point < loopdata->Count; ++point) {
					for (float l = 0; l <= 1; l += 0.01f) {
						a1->Data1->Position.x = (loopdata->LoopList[point + 1].Position.x - loopdata->LoopList[point].Position.x) * l + loopdata->LoopList[point].Position.x;
						a1->Data1->Position.y = (loopdata->LoopList[point + 1].Position.y - loopdata->LoopList[point].Position.y) * l + loopdata->LoopList[point].Position.y;
						a1->Data1->Position.z = (loopdata->LoopList[point + 1].Position.z - loopdata->LoopList[point].Position.z) * l + loopdata->LoopList[point].Position.z;

						int range = 10;
						if (a1->Data1->field_A == 2) range = 5;

						if ((a1->Data1->Position.x > (players[slot]->Position.x - range) && a1->Data1->Position.x < (players[slot]->Position.x + range))
							&& (a1->Data1->Position.y > (players[slot]->Position.y - range) && a1->Data1->Position.y < (players[slot]->Position.y + range))
							&& (a1->Data1->Position.z > (players[slot]->Position.z - range) && a1->Data1->Position.z < (players[slot]->Position.z + range))) {

							a1->Data1->Scale = { -3, 8, 4 }; //fix weird bug
							if (IsPlayerInsideSphere(&a1->Data1->Scale, 10)) return;

							players[slot]->Position = a1->Data1->Position;
							a1->Data1->Action = 1;

							//we create a new object to support 8 players (one object per character)
							ObjectMaster * tempobj;
							if (type == 2) tempobj = LoadObject(LoadObj_Data1, 0, RailPath_Main);
							else tempobj = LoadObject(LoadObj_Data1, 0, AutoLoop_Main);

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

							//rail stuff
							if (type == 2) {
								if (co2) tempobj->Data1->Scale.y = co2->Speed.x * 2; //store horizontal speed
								if (tempobj->Data1->Scale.y == 0) RailValues[RailPhysics_MaxSpeed];

								float ang = fPositionToRotation(loopdata->LoopList[point].Position, loopdata->LoopList[point + 1].Position).y;
								float min = ang - 0x4000;
								if (min < 0) min += 0x10000;
								float max = ang + 0x4000;
								if (max > 65536) max -= 0x10000;

								if (min > max) {
									ang = max;
									max = min;
									min = ang;

									if (players[slot]->Rotation.y > min && players[slot]->Rotation.y < max) tempobj->Data1->CharIndex = 1; //backward
									else tempobj->Data1->CharIndex = 0; //forward
								}
								else {
									if (players[slot]->Rotation.y > min && players[slot]->Rotation.y < max) tempobj->Data1->CharIndex = 0; //forward
									else tempobj->Data1->CharIndex = 1; //backward
								}
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

void RailPath(ObjectMaster * a1) {
	a1->Data1->field_A = 2;
	a1->MainSub = Path_Main;
}

void SlopePath(ObjectMaster * a1) {
	a1->Data1->field_A = 1;
	a1->MainSub = Path_Main;
}