#include "stdafx.h"
#include "mod.h"

#define _HIBYTE(x)   (*((Uint8*)&(x)+1))

ObjectMaster * TeamObj;

ObjectFuncPtr TeamSonic[]{
	Sonic_Main,
	Tails_Main,
	Knuckles_Main
};

enum AIStates {
	AIState_Ground,
	AIState_Jumping,
	AIState_AirAction,
	AIState_Falling,
	AIState_Stop
};

Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);

bool IsPointInsideSphere(NJS_VECTOR *center, NJS_VECTOR *pos, float radius) {
	return (powf(pos->x - center->x, 2) + pow(pos->y - center->y, 2) + pow(pos->z - center->z, 2)) <= pow(radius, 2);
}

bool IsPointInsideBox(NJS_VECTOR *p1, NJS_VECTOR *p2, NJS_VECTOR *pos) {
	return (pos->x > p1->x && pos->x < p2->x &&
		pos->y > p1->y && pos->y < p2->y &&
		pos->z > p1->z && pos->z < p2->z);
}

inline void AI_PerformJump(EntityData1* data, EntityData1* botdata, CharObj2* botco2, float height, float dist) {
	botco2->Speed.y = fmax(2, height / 5);
	botco2->Speed.x = fmax(1, dist / 20);
	data->NextAction = AIState_Jumping;
}

void AI_Movement(EntityData1* data, EntityData1* playerdata, EntityData1* botdata, CharObj2* botco2, float dist, float distplayer) {
	//the Team Handler gives each AI a point which it must try to reach

	//collision box
	NJS_VECTOR p1 = { data->Scale.x - 5, data->Scale.y - 50, data->Scale.z - 5 };
	NJS_VECTOR p2 = { data->Scale.x + 5, data->Scale.y + 50, data->Scale.z + 5 };

	if (IsPointInsideBox(&p1, &p2, &botdata->Position)) {
		if (data->NextAction == AIState_AirAction) {
			botdata->Rotation.y = fPositionToRotation(&botdata->Position, &playerdata->Position).y;
			
			if (botdata->CharID == Characters_Tails) {
				botco2->AnimationThing.Index = 42;
				botco2->Speed.x = 0;
			}
			else if (botdata->CharID == Characters_Knuckles) {
				botdata->Action = 9;
 				data->NextAction = AIState_Falling;
				botco2->AnimationThing.Index = 19;
				botco2->PhysicsData.Gravity = PhysicsArray[Characters_Knuckles].Gravity;
			}
			
			return;
		}

		if (data->NextAction == AIState_Falling) {
			botco2->AnimationThing.Index = 19;
		}

		data->field_A = 0;
		botco2->Speed = { 0, 0, 0 };
		botdata->Action = 1;
		botco2->AnimationThing.Index = 0;
	}
	else {
		//rotate player towards point to follow
		botdata->Rotation.y = fPositionToRotation(&botdata->Position, &data->Scale).y; 

		//calculate speed roughly
		float speed; float speedmax;
		switch (botdata->CharID) {
		case Characters_Sonic:
			speed = 8, speedmax = 14;
			break;
		case Characters_Tails:
			speed = 10, speedmax = 12;
			break;
		default:
			speed = 14, speedmax = 10;
			break;
		}

		botco2->Speed.x = dist / speed < speedmax ? dist / speed : speedmax;

		if (botco2->Speed.x >= speedmax) {
			if (botco2->Speed.x < botco2->PhysicsData.HSpeedCap) {
				botco2->TailsFlightTime += 0.01f;
				botco2->Speed.x += botco2->TailsFlightTime;
			}
		}
		else {
			botco2->TailsFlightTime = 0;
		}

		//running animations
		if (data->NextAction == AIState_AirAction) return;

		if (botco2->Speed.x > 0.2f) {
			//animation run 1
			botdata->Action = 2;
			switch (botdata->CharID) {
			case Characters_Sonic:
			case Characters_Tails:
				botco2->AnimationThing.Index = 12;
				break;
			case Characters_Knuckles:
				botco2->AnimationThing.Index = 10;
				break;
			}
		}
		else if (botco2->Speed.x > 1) {
			//animation run 2
			botdata->Action = 2;
			switch (botdata->CharID) {
			case Characters_Sonic:
			case Characters_Tails:
				botco2->AnimationThing.Index = 13;
				break;
			case Characters_Knuckles:
				botco2->AnimationThing.Index = 12;
				break;
			}
		}
	}
}

void AI_Obstacles(EntityData1* data, EntityData1* playerdata, EntityData1* botdata, CharObj2* botco2, float dist, float distplayer) {
	//check the conditions to jump, go around objects
	float height = botdata->Position.y - data->Scale.y;
	float test = fPositionToRotation(&botdata->Position, &data->Scale).y;
	if (dist > 20 && dist < 40 && height < -8 && height > - 20 && 
		test == botdata->Rotation.y && playerdata->Rotation.x == 0 && playerdata->Action < 5) {
		AI_PerformJump(data, botdata, botco2, height, dist);
	}

	if (dist > 200) {
		AI_PerformJump(data, botdata, botco2, height, dist);
	}

	if (FrameCounterUnpaused % 100 == 0 && dist > 30) {
 		if (IsPointInsideSphere(&data->Position, &botdata->Position, 5)) {
			AI_PerformJump(data, botdata, botco2, 10, 30);
			data->field_A += 1;
		}
		
		if (data->field_A > 3) {
			data->field_A = 0;
			botdata->Position = data->Scale;
			botdata->Position.y += 50;
		}

		data->Position = botdata->Position;
	}

	if (dist > 600) {
		botdata->Position = data->Scale;
		botdata->Position.y += 50;
	}
}

void AI_InAir(EntityData1* data, EntityData1* playerdata, EntityData1* botdata, CharObj2* botco2, float dist, float distplayer) {
	if (data->NextAction == AIState_Jumping) {
		botco2->AnimationThing.Index = 14;
		botdata->Status |= Status_Ball;
		botdata->Status |= Status_Attack;
		botdata->Rotation.y = fPositionToRotation(&botdata->Position, &data->Scale).y;

		switch (botdata->CharID) {
		case Characters_Sonic:
			botdata->Action = 8;
			break;
		case Characters_Tails:
		case Characters_Knuckles:
			botdata->Action = 6;
			break;
		}

		float test = botco2->_struct_a3.DistanceMax - botdata->Position.y;
		if (dist > 200 && botco2->_struct_a3.DistanceMax - botdata->Position.y < -10) {
			data->NextAction = AIState_AirAction;

			if (botdata->CharID == Characters_Tails) {
				botdata->Status &= ~Status_Attack;
			}

			botdata->Status &= ~Status_Ball;
			botdata->Action = 15;
		}
	}
	else if (data->NextAction == AIState_AirAction) {
		

		//Fly
		if (botdata->CharID == Characters_Tails) {
			botco2->PhysicsData.Gravity = 0.02;
			botco2->AnimationThing.Index = 37;
		}
		//Glide
		else if (botdata->CharID == Characters_Knuckles) {
			botco2->Speed.y *= 0.9f;
			botco2->PhysicsData.Gravity = 0.02;
			botco2->AnimationThing.Index = 51;
		}

		AI_Movement(data, playerdata, botdata, botco2, dist, distplayer);
	}
}

bool Team_SwapCharacter(ObjectMaster* obj, ObjectMaster* player) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData2* objdata2 = (EntityData2*)player->Data2;
	EntityData1* objdata = player->Data1;

	if (!data2->field_30) {
		if (PressedButtons[0] & Buttons_C) {
			data->Index -= 1;
			data->Unknown += 1;
			if (data->Index < 0) data->Index = 2;
		}

		if (PressedButtons[0] & Buttons_Z) {
			data->Index += 1;
			data->Unknown += 1;
			if (data->Index > 2) data->Index = 0;
		}

		if (data->Unknown % 3 != 0 && (EntityData1Ptrs[0]->Status & Status_Ground) && EntityData1Ptrs[0]->Action < 5) {
			data->Unknown = 0;
			EntityData1Ptrs[0]->Status = Status_Ground;
			data2->field_30 = objdata->Action;
			data2->VelocityDirection = objdata2->CharacterData->Speed;
			player->MainSub = TeamSonic[data->Index];
			player->Data1->Action = 0;
			return true;
		}
	}
	else {
		if (objdata->CollisionInfo) {
			for (short i = 0; i < objdata->CollisionInfo->Count; i++) {
				_HIBYTE(objdata->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;
				objdata->CollisionInfo->CollisionArray[i].field_3 &= ~0x20u;
			}

			objdata->Action = 2;
			objdata2->CharacterData->Speed = data2->VelocityDirection;
			data2->field_30 = 0;
		}
	}

	return false;
}

void SideKickAI(ObjectMaster* obj) {
	//Handle the AI
	EntityData1* data = obj->Data1;
	
	if (data->Action == 0) {
		data->Action = 1;
		
		data->Index = data->CharIndex + 1;
		obj->field_30 = (int)LoadObject((LoadObj)(LoadObj_UnknownA | LoadObj_Data1 | LoadObj_Data2), 3, TeamSonic[data->Index]);
		ObjectMaster* childobj = (ObjectMaster*)obj->field_30;
		EntityData1* childdata = childobj->Data1;
		
		childdata->CharIndex = 6 + data->CharIndex;
	}
	else {
		ObjectMaster* childobj = (ObjectMaster*)obj->field_30;
		EntityData1* childdata = childobj->Data1;

		switch (data->Action) {
		case 1: //Collision flag to avoid getting hurt by player
			if (childdata->CollisionInfo) {
				for (short i = 0; i < childdata->CollisionInfo->Count; i++) {
					_HIBYTE(childdata->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;
					childdata->CollisionInfo->CollisionArray[i].field_3 &= ~0x20u;
				}

				data->Action = 2;
			}
			break;
		case 2: //run the AI
			if (data->Status == 0) {
				childdata->Position = data->Scale;
				childdata->Rotation = EntityData1Ptrs[0]->Rotation;
				data->Status = 1;
			}

			if (Team_SwapCharacter(obj, GetCharacterObject(childdata->CharIndex))) {
				if (IsPlayerInsideSphere(&childdata->Position, 50)) {
					childdata->Position = data->Scale;
					childdata->Rotation = EntityData1Ptrs[0]->Rotation;
				}
			}
			else {
				EntityData1* playerdata = EntityData1Ptrs[0];
				EntityData2* botdata2 = (EntityData2*)childobj->Data2;
				CharObj2* botco2 = botdata2->CharacterData;

				float distplayer = GetDistance(&childdata->Position, &playerdata->Position);
				float dist = GetDistance(&childdata->Position, &data->Scale);

				if (childdata->Status & Status_Ground) {
					if (data->NextAction != AIState_Ground) {
						childdata->Status &= ~Status_Ball;
						childdata->Status &= ~Status_Attack;
						data->NextAction = AIState_Ground;

						if (data->NextAction == AIState_AirAction) {
							botco2->PhysicsData.Gravity = PhysicsArray[Characters_Knuckles].Gravity;
						}
					}

					AI_Movement(data, playerdata, childdata, botco2, dist, distplayer);
					AI_Obstacles(data, playerdata, childdata, botco2, dist, distplayer);
				}
				else {
					AI_InAir(data, playerdata, childdata, botco2, dist, distplayer);
				}
			}
			
			break;
		}
	}
}

void Team_GetPoints(ObjectMaster* obj) {
	//get the points that the ai must try to reach
	//all sorts of crazy calculations here sorry
	EntityData1* childdata = obj->Child->Data1;
	EntityData1* child2data = obj->Child->Child->Data1;

	EntityData1* data = obj->Data1;
	EntityData1* playerdata = EntityData1Ptrs[0];
	uint8_t formation = data->Index;
	NJS_VECTOR point;
	NJS_VECTOR point2;

	NJS_VECTOR dir = { -10, 0, 0 };
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &playerdata->Position);
	njRotateY(0, -playerdata->Rotation.y);
	njCalcPoint(0, &dir, &point);

	if (formation == 0) {
		childdata->Scale = point;
		dir.x -= CharObj2Ptrs[0]->Speed.x;
		dir.x = -20;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	if (formation == 1) {
		dir.x = -8; dir.z = -2.5f;
		njCalcPoint(0, &dir, &point);
		childdata->Scale = point;
		dir.x -= CharObj2Ptrs[0]->Speed.x;
		dir.x = -16; dir.z = 2.5f;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	

	if (formation == 2) {
		dir.x = -5; dir.z = -8;
		njCalcPoint(0, &dir, &point);
		childdata->Scale = point;
		dir.z = 8;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	njPopMatrix(1u);
}

void TeamHandler_Delete(ObjectMaster* obj) {
	TeamObj = nullptr;
}

void TeamHandler_Display(ObjectMaster* obj) {
	//Display the HUD
}

void TeamHandler(ObjectMaster* obj) {
	//Handle the character change, the hud, teamblast, etc
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		data->Action = 1;
		obj->DeleteSub = TeamHandler_Delete;
		obj->DisplaySub = TeamHandler_Display;

		//Load our two AI
		ObjectMaster* child = LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, obj);
		LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, child)->Data1->CharIndex = 1;
	}
	else {
		if (GetCharacterObject(1)) DeleteObject_(GetCharacterObject(1));
		EntityData1* MainPlayer = EntityData1Ptrs[0];
		
		switch (data->Action) {
		case 1: //we make sonic's collision not offensive to our AIs
			if (MainPlayer->CollisionInfo) {
				for (short i = 0; i < MainPlayer->CollisionInfo->Count; i++)
					_HIBYTE(MainPlayer->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;

				data->Action = 2;
			}
			break;
		case 2: //check for playerswap, and swap
			Team_SwapCharacter(obj, GetCharacterObject(0));
			Team_GetPoints(obj);

			break;
		}

		RunObjectChildren(obj);
		RunObjectChildren(obj->Child);
	}
}

void GamePlay_OnFrame() {
	if (!TeamObj && GetCharacterID(0) == Characters_Sonic) 
		TeamObj = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, TeamHandler);
}