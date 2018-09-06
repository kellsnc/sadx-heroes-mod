#include "stdafx.h"
#include "mod.h"
#include <math.h>

float RailValues[]{ 5, 8, 0.2f, 0.03f };

enum EnumRailPhysics {
	RailPhysics_MaxSpeed,
	RailPhysics_MaxAcceleration,
	RailPhysics_SpeedIncrement,
	RailPhysics_GravityInfluence,
};

void TransformPlayer(char player, NJS_VECTOR orig, NJS_VECTOR dest, float state) {
	auto entity = EntityData1Ptrs[player];
	entity->Position.x = (dest.x - orig.x) * state + orig.x;
	entity->Position.y = ((dest.y - orig.y) * state + orig.y) + 5;
	entity->Position.z = (dest.z - orig.z) * state + orig.z;
}

float fPositionToRotation(CharObj2 * co2, NJS_VECTOR orig, NJS_VECTOR point) {
	co2->SoManyVectors[8].x = orig.x - point.x;
	co2->SoManyVectors[8].y = orig.y - point.y;
	co2->SoManyVectors[8].z = orig.z - point.z;
	float len = co2->SoManyVectors[8].z * co2->SoManyVectors[8].z + co2->SoManyVectors[8].y * co2->SoManyVectors[8].y;
	return (atan2(co2->SoManyVectors[8].x, squareroot(len)) * 65536.0 * 0.1591549762031479) + 0x4000;
}

void LookAt(char player, NJS_VECTOR orig, NJS_VECTOR point) {
	auto entity = EntityData1Ptrs[player];
	CharObj2 * co2 = GetCharObj2(player);

	entity->Rotation.y = fPositionToRotation(co2, orig, point);
}

#pragma region Rails

//Physics mess
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
		cY = loopdata->LoopList[a1->Data1->Index].Position.y;
		nY = loopdata->LoopList[a1->Data1->Index + 1].Position.y;
	}
	else {
		cY = loopdata->LoopList[a1->Data1->Index + 1].Position.y;
		nY = loopdata->LoopList[a1->Data1->Index].Position.y;
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

//Player X is on a rail
void RailMain(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData; //current rail
	EntityData1 * entity = a1->Data1; //we can store data in this

	if (a1->Data1->Action == 0) {
		EntityData1 * player = EntityData1Ptrs[a1->Data1->NextAction];

		if (!player) {
			a1->Data1->Action = 1;
			return;
		}

		CharObj2 * co2 = GetCharObj2(a1->Data1->NextAction);
		bool backward = a1->Data1->CharIndex;

		//detach from rail if it's the last point / if the player press jump
		if ((!backward && loopdata->LoopList[a1->Data1->Index].Dist == 0) //if no distance, we're at the end
			|| (backward && a1->Data1->Index == 0 && a1->Data1->Scale.x <= 0.08f) //if first point and position close to 0, we're at the start
			|| ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) { 
			a1->Data1->Action = 1;
			float railspeed = a1->Data1->Scale.y;

			//Animations
			if (ControllerPointers[a1->Data1->NextAction]->PressedButtons & Buttons_A) {
				player->Status = Status_Ball;
				player->Action = 8;
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

			return;
		}
		else {
			RailPhysics(a1, player, co2, loopdata);
			float railspeed = a1->Data1->Scale.y;

			//next position in spline
			if (!backward) a1->Data1->Scale.x = a1->Data1->Scale.x + (loopdata->TotalDist / loopdata->LoopList[a1->Data1->Index].Dist) / loopdata->TotalDist * railspeed;
			else a1->Data1->Scale.x = a1->Data1->Scale.x - (loopdata->TotalDist / loopdata->LoopList[a1->Data1->Index].Dist) / loopdata->TotalDist * railspeed;

			//animations
			player->Status = 0;
			if (GetCharacterID(a1->Data1->NextAction) == Characters_Tails) {
				player->Action = 48;
				co2->AnimationThing.Index = 0;
			}
			if (GetCharacterID(a1->Data1->NextAction) == Characters_Knuckles) co2->AnimationThing.Index = 0;

			TransformPlayer(a1->Data1->NextAction, loopdata->LoopList[a1->Data1->Index].Position, loopdata->LoopList[a1->Data1->Index + 1].Position, a1->Data1->Scale.x);
			LookAt(a1->Data1->NextAction, loopdata->LoopList[a1->Data1->Index].Position, loopdata->LoopList[a1->Data1->Index + 1].Position);
			
			
			if (backward) {
				player->Rotation.y += 0x8000;
			}

			//go to next point
			if (!backward && a1->Data1->Scale.x > 1) { a1->Data1->Scale.x = 0; a1->Data1->Index++; } 
			if (backward && a1->Data1->Scale.x < 0 ) { a1->Data1->Scale.x = 1; a1->Data1->Index--; }
		}
	}

	//hold before removing the rail object so that the character
	//don't immediately go back on the rail
	if (a1->Data1->Action == 1) {
		if (a1->Data1->CharIndex < 20) a1->Data1->CharIndex += 1;
		else {
			auto entity = EntityData1Ptrs[a1->Data1->NextAction];
			entity->NextAction = 0;
			DeleteObjectMaster(a1);
		}
	}
}

//Initialize rails
void Railpath(ObjectMaster * a1) {
	LoopHead * loopdata = (LoopHead*)a1->Data1->LoopData;
	EntityData1 * entity = a1->Data1;
	
	if (!a1->Data1->Action) {
		EntityData1 ** players = EntityData1Ptrs; //suport for 8 players, let's get all the pointers
		for (uint8_t slot = 0; slot < 8; ++slot) {
			if (players[slot]) {
				//we check if the player gets on the rail
				//to do that we get lots of points throughout the rail and check if the player is near one

				if (players[slot]->NextAction == 1) return;

				for (uint8_t point = 0; point < loopdata->Count; ++point) {
					for (float l = 0; l <= 1; l += 0.01f) {
						a1->Data1->Position.x = (loopdata->LoopList[point + 1].Position.x - loopdata->LoopList[point].Position.x) * l + loopdata->LoopList[point].Position.x;
						a1->Data1->Position.y = (loopdata->LoopList[point + 1].Position.y - loopdata->LoopList[point].Position.y) * l + loopdata->LoopList[point].Position.y;
						a1->Data1->Position.z = (loopdata->LoopList[point + 1].Position.z - loopdata->LoopList[point].Position.z) * l + loopdata->LoopList[point].Position.z;
					
						if ((a1->Data1->Position.x >(players[slot]->Position.x - 2.8f) && a1->Data1->Position.x < (players[slot]->Position.x + 2.8f))
							&& (a1->Data1->Position.y >(players[slot]->Position.y - 2.8f) && a1->Data1->Position.y < (players[slot]->Position.y + 2.8f))
							&& (a1->Data1->Position.z >(players[slot]->Position.z - 2.8f) && a1->Data1->Position.z < (players[slot]->Position.z + 2.8f))) {
							
							players[slot]->Position = a1->Data1->Position;
							a1->Data1->Action = 1;

							//we create a new object to support 8 players (one object per character)
							ObjectMaster * tempobj = LoadObject(LoadObj_Data1, 0, RailMain);
							CharObj2 * co2 = GetCharObj2(slot);
							tempobj->Data1->Index = point; //store current spline point
							tempobj->Data1->Scale.x = l; //store position in spline
							tempobj->Data1->NextAction = slot; //store character it applies to
							tempobj->Data1->LoopData = a1->Data1->LoopData;
							if (co2) tempobj->Data1->Scale.y = co2->Speed.x * 2; //store horizontal speed
							if (tempobj->Data1->Scale.y == 0) RailValues[RailPhysics_MaxSpeed];
							players[slot]->NextAction = 1;
							
							//get direction
							float ang = fPositionToRotation(co2, loopdata->LoopList[point].Position, loopdata->LoopList[point + 1].Position);
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
#pragma endregion