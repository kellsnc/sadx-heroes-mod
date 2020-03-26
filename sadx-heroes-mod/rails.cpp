#include "stdafx.h"

enum RailStates {
	RailState_Running,
	RailState_Swap,
	RailState_Swapping,
	RailState_Deleting
};

enum RailSounds {
	RailSound_Start = 1374,
	RailSound_Grind,
	RailSound_Fast,
	RailSound_200,
	RailSound_500,
	RailSound_1000
};

enum EnumRailPhysics {
	RailPhysics_StartSpeed,
	RailPhysics_MaxNormalSpeed,
	RailPhysics_MaxAcceleration,
	RailPhysics_MaxSpeed,
	RailPhysics_SpeedIncrement,
	RailPhysics_GravityInfluence,
};

enum RailTypes {
	RailType_Normal = 0x1,
	RailType_ForceForward = 0x10,
	RailType_Hang = 0x100,
	RailType_Trick = 0x200
};

float RailValues[]{
	3,
	7,
	8,
	10,
	0.08f,
	0.0002f
};

void RailPath_CharStatus(EntityData1* PlayerEntity, CharObj2* co2) {
	PlayerEntity->Status = Status_Ground;
	PlayerEntity->Action = PlayerState_OnRail; //When the character action does not exists, the character is unmovable.
	PlayerEntity->field_A = PlayerState_OnRail;
	co2->AnimationThing.Index = 0;
	co2->Speed = { 0, 0, 0 }; //Prevents the character from moving if something touches it

	int btn = HeldButtons2[PlayerEntity->CharIndex];

	switch (PlayerEntity->CharID) {
	case Characters_Sonic:
		if (SuperSonicFlag == 0) {
			co2->AnimationThing.Index = 102; //snowboard animation

			if (btn & Buttons_Left) {
				co2->AnimationThing.Index = 103; //lean left
			}
			else if (btn & Buttons_Right) {
				co2->AnimationThing.Index = 104; //lean right
			}
			else if (btn & Buttons_X) {
				co2->AnimationThing.Index = 124;
			}
		}
		
		break;
	case Characters_Tails:
		PlayerEntity->Action = 48;
		
		co2->AnimationThing.Index = 107; //snowboard animation

		if (btn & Buttons_Left) {
			co2->AnimationThing.Index = 112; //lean left
		}
		else if (btn & Buttons_Right) {
			co2->AnimationThing.Index = 113; //lean right
		}
		else if (btn & Buttons_X) {
			co2->AnimationThing.Index = 108;
		}

		break;
	}
}

void RailPath_Detach(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, DetachTypes type) {
	entity->Action = RailState_Deleting;
	entity->Status |= Status_KillSound;
	PlayerEntity->Action = 1;

	int id = PlayerEntity->CharID;

	switch (type) {
	case DetachType_End:
	case DetachType_Fall:
		co2->Speed.x = entity->Scale.z / 2;
		co2->Speed.y = 1;

		switch (id) {
		case Characters_Sonic:
			if (SuperSonicFlag == 0) {
				co2->AnimationThing.Index = 120;
			}
			
			break;
		}
		break;
	case DetachType_Jump:
		PlayerEntity->Status = Status_Ball | Status_Attack;
		PlayerEntity->Action = 8;
		co2->AnimationThing.Index = 14;
		PlaySound(17, 0, 0, 0);

		co2->Speed.y = 2;
		co2->Speed.x = entity->Scale.z / 2;

		switch (id) {
		case Characters_Tails:
		case Characters_Knuckles:
			PlayerEntity->Action = 6;
			break;
		case Characters_Amy:
			PlayerEntity->Action = 5;
			break;
		case Characters_Gamma:
		case Characters_Big:
			PlayerEntity->Action = 4;
			break;
		}
		break;
	case DetachType_Death:
		switch (id) {
		case Characters_Sonic:
			if (SuperSonicFlag == 0) {
				co2->AnimationThing.Index = 86;
			}
			
			break;
		case Characters_Tails:
			co2->AnimationThing.Index = 28;
			break;
		case Characters_Knuckles:
			co2->AnimationThing.Index = 27;
			break;
		}
		break;
	}
}

void RailPath_PerformPhysics(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, LoopHead* loophead) {
	int16_t point = entity->Scale.x;
	Loop* LoopPoint = &loophead->LoopList[point];
	Loop* NextPoint = &loophead->LoopList[point + 1];

	float speed = fabs(entity->Scale.z); //current speed

	uint8_t backward = entity->CharID;
	int btn = HeldButtons2[PlayerEntity->CharIndex];

	//Get the forward angle to adjust speed
	float angX;
	if (backward) angX = NextPoint->Position.y - LoopPoint->Position.y;
	else angX = LoopPoint->Position.y - NextPoint->Position.y;
	if (angX != 0) speed += angX * RailValues[RailPhysics_GravityInfluence];

	//increase speed if holding the action button
	if (btn & Buttons_X && angX > -1 && speed < RailValues[RailPhysics_MaxAcceleration]) speed += RailValues[RailPhysics_SpeedIncrement];

	//if the player is too slow and the angle is high enough, we need to change direction
	//otherwise we detach the player
	if (speed <= 0) {
		if (loophead->Unknown_0 & RailType_ForceForward) {
			RailPath_Detach(entity, PlayerEntity, co2, DetachType_Fall);
		}
		else {
			if (angX < 5) {
				if (backward) entity->CharID = 0;
				else entity->CharID = 1;
				if (entity->Scale.x < loophead->Count - 2) entity->Scale.x += 1;
			}
			else {
				RailPath_Detach(entity, PlayerEntity, co2, DetachType_Fall);
			}
		}
	}

	entity->Scale.z = speed;
}

void RailPath_PerformJumpAndDamage(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2) {
	//Jump or swap rail
	if (PressedButtons[PlayerEntity->CharIndex] & Buttons_A) {
		if (HeldButtons2[PlayerEntity->CharIndex] & Buttons_Left) {
			entity->Action = RailState_Swap;
			PlayerEntity->Action = PlayerState_SwapLeft;
			PlayerEntity->field_A = 0;
		}
		else if (HeldButtons2[PlayerEntity->CharIndex] & Buttons_Right) {
			entity->Action = RailState_Swap;
			PlayerEntity->Action = PlayerState_SwapRight;
			PlayerEntity->field_A = 0;
		}
		else {
			LoopHead* loophead = (LoopHead*)entity->LoopData;
			if (loophead->Unknown_0 & RailType_Hang)
				RailPath_Detach(entity, PlayerEntity, co2, DetachType_Fall);
			else
				RailPath_Detach(entity, PlayerEntity, co2, DetachType_Jump);
		}

		return;
	}

	//Damage
	if (PlayerEntity->Status & Status_Hurt) {
		entity->Scale.z = 1; //speed
	}

	//Death
	if (co2->Powerups & Powerups_Dead) {
		RailPath_Detach(entity, PlayerEntity, co2, DetachType_Death);
	}
}

void RailPath_PerformPath(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, LoopHead* loophead) {
	int16_t point = entity->Scale.x;
	Loop* LoopPoint = &loophead->LoopList[point];
	Loop* NextPoint = &loophead->LoopList[point + 1];

	uint16_t end = loophead->Count - 1;
	uint8_t backward = entity->CharID;

	//if we are backward, adjust the variables
	if (backward) {
		end = 0;
		LoopPoint = &loophead->LoopList[point - 1];
		NextPoint = &loophead->LoopList[point];
	}

	float distance = LoopPoint->Dist;

	//Detach from rail if at the end
	if (point == end) {
		RailPath_Detach(entity, PlayerEntity, co2, DetachType_End);
		return;
	}

	//Get the speed calculated in the Physics function
	float railspeed = entity->Scale.z;

	//get the movement for this frame
	if (backward) entity->Scale.y = entity->Scale.y - (loophead->TotalDist / distance) / loophead->TotalDist * railspeed;
	else entity->Scale.y = entity->Scale.y + (loophead->TotalDist / distance) / loophead->TotalDist * railspeed;

	float state = entity->Scale.y;

	//set the movement
	if (!entity->Action == RailState_Swap) {
		PlayerEntity->Position = GetPathPosition(
			&LoopPoint->Position,
			&NextPoint->Position,
			state);
	}

	//set the rotation
	PlayerEntity->Rotation.x = LoopPoint->Ang_X;
	PlayerEntity->Rotation.y = fPositionToRotation(&LoopPoint->Position, &NextPoint->Position).y;
	PlayerEntity->Rotation.z = LoopPoint->Ang_Y; //Ang_Y is actually the Z axis

	//visual changes for hang rails
	if (loophead->Unknown_0 & RailType_Hang) {
		PlayerEntity->Rotation.z = 0;
		PlayerEntity->Position.z -= 2;
		PlayerEntity->Position.y -= co2->PhysicsData.CollisionSize;

		switch (PlayerEntity->CharID) {
		case Characters_Sonic:
			co2->AnimationThing.Index = 47;
			break;
		case Characters_Knuckles:
			co2->AnimationThing.Index = 84;
			break;
		case Characters_Amy:
			co2->AnimationThing.Index = 32;
			break;
		}
	}

	//go to the next point if it is reached
	if (backward) {
		if (state <= 0) { entity->Scale.y = 1; --entity->Scale.x; }
		PlayerEntity->Rotation.y += 0x8000;
	}
	else {
		if (state >= 1) { entity->Scale.y = 0; ++entity->Scale.x; }
	}
}

void RailPath_SparkleEffect(NJS_VECTOR* pos) {
	for (uint8_t l = 0; l < 5; ++l) {
		ObjectMaster* sparkle = LoadObject(LoadObj_Data1, 6, (ObjectFuncPtr)0x4CE830); //sparkle effect of kiki bombs
		if (sparkle)
		{
			NJS_VECTOR dir; //direction of the sparkles, random
			dir.x = (0.5 - (double)rand() * 0.000030517578) * 1.2;
			dir.y = ((double)rand() * 0.000030517578 + 0.2) * 0.89999998;
			dir.z = (0.5 - (double)rand() * 0.000030517578) * 1.2;

			EntityData1* entity = sparkle->Data1;
			entity->InvulnerableTime = 0;
			entity->Position = *pos;
			entity->Scale = dir;
		}
	}
}

void RailPath_SwapTransition(EntityData1* entity, EntityData1* PlayerEntity, LoopHead* loophead) {
	if (entity->Position.x == 0)
		entity->Position.x = 20;

	int16_t point = entity->Scale.x;
	Loop* LoopPoint = &loophead->LoopList[point];
	Loop* NextPoint = &loophead->LoopList[point + 1];

	++entity->Position.x;
	float state = (float)entity->Position.x / 80;

	if (state >= 0.3f) {
		entity->Action = RailState_Running;
		return;
	}

	NJS_VECTOR dest = GetPathPosition(&LoopPoint->Position,
		&NextPoint->Position, entity->Scale.y);

	PlayerEntity->Position = GetPathPosition(
		&PlayerEntity->Position,
		&dest,
		state);
}

float RailPath_GetSpeed(CharObj2* co2) {
	//get the highest speed between forward and vertical, and use it as startspeed
	//except if it exceeds the maximum acceleration speed
	float speed = max(co2->Speed.x, fabs(co2->Speed.y)); //store horizontal speed
	if (speed == 0) return RailValues[RailPhysics_StartSpeed];
	else if (speed > RailValues[RailPhysics_MaxSpeed]) return RailValues[RailPhysics_MaxSpeed];
	else return speed;
}

void __cdecl RailPath_Exec(ObjectMaster* obj) {
	EntityData1* entity = obj->Data1;
	uint8_t id = obj->Data1->CharIndex;
	EntityData1* PlayerEntity = EntityData1Ptrs[id];
	CharObj2* co2 = CharObj2Ptrs[id];
	LoopHead* loophead = (LoopHead*)entity->LoopData;

	switch (entity->Action) {
	case RailState_Running:		//Check and perform rail actions
		RailPath_CharStatus(PlayerEntity, co2);
		RailPath_PerformJumpAndDamage(entity, PlayerEntity, co2);
		if (entity->Action != RailState_Running) return;

		RailPath_PerformPhysics(entity, PlayerEntity, co2, loophead);
		RailPath_PerformPath(entity, PlayerEntity, co2, loophead);

		if ((loophead->Unknown_0 & RailType_Hang) != RailType_Hang)
			RailPath_SparkleEffect(&PlayerEntity->Position);

		break;
	case RailState_Swap: //if the player requested swapping, 
						//we check if it has worked, otherwise we jump
		if (PlayerEntity->Action == PlayerState_OnRail)
			DeleteObject_(obj);
		else {
			PlayerEntity->field_A = PlayerState_OnRail;
			RailPath_Detach(entity, PlayerEntity, co2, DetachType_Jump);
		}

		break;
	case RailState_Swapping: //if the player swapped from a rail, we slide it sideways
		RailPath_PerformPhysics(entity, PlayerEntity, co2, loophead);
		RailPath_PerformPath(entity, PlayerEntity, co2, loophead);
		RailPath_SparkleEffect(&PlayerEntity->Position);
		RailPath_SwapTransition(entity, PlayerEntity, loophead);

		break;
	case RailState_Deleting:	//wait before allowing another rail
		if (++entity->Unknown == 30) {
			PlayerEntity->field_A = 0;
			DeleteObject_(obj);
		}

		break;
	}
}

EntityData1* RailPath_LoadRail(EntityData1* PlayerEntity, LoopHead* loophead, uint16_t point, float state) {
	//The player is detected, load the executor for that player
	ObjectMaster* exec = LoadObject(LoadObj_Data1, 2, RailPath_Exec);
	EntityData1* execentity = exec->Data1;
	execentity->LoopData = (Loop*)loophead;
	execentity->CharIndex = PlayerEntity->CharIndex;

	CharObj2* co2 = CharObj2Ptrs[PlayerEntity->CharIndex];

	//Get the direction and start speed
	execentity->Scale.z = RailPath_GetSpeed(co2);
	if ((loophead->Unknown_0 & RailType_ForceForward) != RailType_ForceForward)
		execentity->CharID = Path_GetDirection(PlayerEntity, loophead, point);

	//Set character status
	RailPath_CharStatus(PlayerEntity, co2);

	//adjust things when backward
	if (execentity->CharID == 1) {
		execentity->Scale.y = 1;
		if (point < loophead->Count - 2) point += 1;
	}

	execentity->Scale.x = point; //give the executor our position in the spline
	execentity->Scale.y = state;

	PlayHeroesSound_EntityAndPos(CommonSound_RailHit, exec, &PlayerEntity->Position, 500, 2, false);
	PlayHeroesSound_EntityAndPos(CommonSound_Rail, exec, &PlayerEntity->Position, 500, 2, true);

	return execentity;
}

void RailPath_SwapRail(EntityData1* entity, EntityData1* PlayerEntity, LoopHead* loophead) {
	NJS_VECTOR dir = { 0, 0, 21 };
	if (PlayerEntity->Action == PlayerState_SwapLeft) dir.z = -21;

	//we extend the next point pos for precision
	NJS_VECTOR CheckPos;
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &PlayerEntity->Position);
	njRotateY(0, -PlayerEntity->Rotation.y);
	njCalcPoint(0, &dir, &CheckPos);
	njPopMatrix(1u);

	for (int16_t point = 0; point < loophead->Count - 1; point += 1) {
		Loop* LoopPoint = &loophead->LoopList[point];
		Loop* NextPoint = &loophead->LoopList[point + 1];

		for (float state = 0; state <= 1; state += 0.01f) {
			NJS_VECTOR center = GetPathPosition(&LoopPoint->Position, &NextPoint->Position, state);
			if (!IsPointInsideSphere(&center, &CheckPos, 20)) continue;

			RailPath_LoadRail(PlayerEntity, loophead, point, state)->Action = RailState_Swapping;
			return;
		}
	}
}

void RailPath(ObjectMaster* obj) {
	EntityData1* entity = obj->Data1;
	LoopHead* loophead = (LoopHead*)entity->LoopData; //Current Rail Header

	//Check for every player
	for (uint8_t player = 0; player < MaxPlayers; ++player) {
		EntityData1* PlayerEntity = EntityData1Ptrs[player];

		//Check if player exists and if he is not already on the rail
		if (!PlayerEntity || PlayerEntity->field_A == PlayerState_OnRail
			|| PlayerEntity->CharID == Characters_Tikal) continue;

		//Check if the player is near the rail center to reduce cpu usage
		NJS_VECTOR center = GetPathPosition(&loophead->LoopList[0].Position, &loophead->LoopList[loophead->Count - 1].Position, 0.5f);
		float dist = GetDistance(&loophead->LoopList[0].Position, &loophead->LoopList[loophead->Count - 1].Position) * 1.2 / 2;

		if (!IsPlayerInsideSphere_(&center, dist)) continue;

		//Check if the player wants to swap rail
		if (PlayerEntity->Action == PlayerState_SwapLeft || PlayerEntity->Action == PlayerState_SwapRight) {
			RailPath_SwapRail(entity, PlayerEntity, loophead);
			return;
		}

		//Go through every point of the spline
		for (int16_t point = 0; point < loophead->Count - 1; point += 1) {
			Loop* LoopPoint = &loophead->LoopList[point];
			Loop* NextPoint = &loophead->LoopList[point + 1];

			//Check if the player is between the two points
			for (float state = 0; state <= 1; state += 0.01f) {
				center = GetPathPosition(&LoopPoint->Position, &NextPoint->Position, state);
				if (!IsSpecificPlayerInSphere(&center, 10, player)) continue;

				RailPath_LoadRail(PlayerEntity, loophead, point, state);

				return;
			}
		}
	}
}