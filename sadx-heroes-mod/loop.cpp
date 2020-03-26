#include "stdafx.h"

enum PathStates {
	PathState_RunPath,
	PathState_AttractPlayer,
	PathState_Jumping,
	PathState_Delete
};

void LoopPath_CharStatus(EntityData1* PlayerEntity, CharObj2* co2) {

}

void LoopPath_Detach(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, DetachTypes type) {
	entity->Action = PathState_Delete;
	entity->Status |= Status_KillSound;
	PlayerEntity->Action = 1;

	int id = PlayerEntity->CharID;

	switch (type) {
	case DetachType_End:
		break;
	}
}

void LoopPath_PerformPhysics(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, LoopHead* loophead) {
	int16_t point = entity->Scale.x;
	Loop* LoopPoint = &loophead->LoopList[point];
	Loop* NextPoint = &loophead->LoopList[point + 1];

	float speed = fabs(entity->Scale.z); //current speed

	uint8_t backward = entity->CharID;
	int btn = HeldButtons2[PlayerEntity->CharIndex];

	PhysicsData* physics = &PhysicsArray[PlayerEntity->CharID];

	//Get the forward angle to adjust speed
	float angX;
	if (backward) angX = NextPoint->Position.y - LoopPoint->Position.y;
	else angX = LoopPoint->Position.y - NextPoint->Position.y;
	if (angX != 0) speed += angX * (physics->Gravity / 1000);
	
	//increase speed if holding the forward button
	if (btn & Buttons_Up && angX > -1 && speed < physics->HSpeedCap) speed += physics->field_14;

	entity->Scale.z = speed;
}

void LoopPath_PerformPath(EntityData1* entity, EntityData1* PlayerEntity, CharObj2* co2, LoopHead* loophead) {
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
		LoopPath_Detach(entity, PlayerEntity, co2, DetachType_End);
		return;
	}

	//Get the speed calculated in the Physics function
	float railspeed = entity->Scale.z;

	if (railspeed < 0.5f) {
		LoopPath_Detach(entity, PlayerEntity, co2, DetachType_End);
		return;
	}

	//get the movement for this frame
	if (backward) entity->Scale.y = entity->Scale.y - (loophead->TotalDist / distance) / loophead->TotalDist * railspeed;
	else entity->Scale.y = entity->Scale.y + (loophead->TotalDist / distance) / loophead->TotalDist * railspeed;

	float state = entity->Scale.y;

	//set the movement
	PlayerEntity->Position = GetPathPosition(
		&LoopPoint->Position,
		&NextPoint->Position,
		state);

	//set the rotation
	PlayerEntity->Rotation.x = LoopPoint->Ang_X;
	PlayerEntity->Rotation.y = fPositionToRotation(&LoopPoint->Position, &NextPoint->Position).y;
	PlayerEntity->Rotation.z = LoopPoint->Ang_Y;

	//go to the next point if it is reached
	if (backward) {
		if (state <= 0) { entity->Scale.y = 1; --entity->Scale.x; }
		PlayerEntity->Rotation.y += 0x8000;
	}
	else {
		if (state >= 1) { entity->Scale.y = 0; ++entity->Scale.x; }
	}
}

void LoopPath_Executor(ObjectMaster* obj) {
	EntityData1*	entity = obj->Data1;
	uint8_t			id = obj->Data1->CharIndex;
	EntityData1*	PlayerEntity = EntityData1Ptrs[id];
	CharObj2*		co2 = CharObj2Ptrs[id];
	LoopHead*		loophead = (LoopHead*)entity->LoopData;

	switch (entity->Action) {
	case PathState_RunPath:
		LoopPath_PerformPhysics(entity, PlayerEntity, co2, loophead);
		LoopPath_PerformPath(entity, PlayerEntity, co2, loophead);
		
		break;
	case PathState_Delete:
		if (++entity->Unknown == 30) {
			PlayerEntity->field_A = 0;
			DeleteObject_(obj);
		}

		break;
	}
}

void LoopPath_LoadLoop(EntityData1* PlayerEntity, LoopHead* loophead, uint16_t point, float state) {
	//The player is detected, load the executor for that player
	ObjectMaster* exec = LoadObject(LoadObj_Data1, 2, LoopPath_Executor);
	EntityData1* execentity = exec->Data1;
	execentity->LoopData = (Loop*)loophead;
	execentity->CharIndex = PlayerEntity->CharIndex;

	CharObj2* co2 = CharObj2Ptrs[PlayerEntity->CharIndex];

	//Get the direction and start speed
	execentity->Scale.z = co2->Speed.x;
	execentity->CharID = Path_GetDirection(PlayerEntity, loophead, point);

	//Set character status
	LoopPath_CharStatus(PlayerEntity, co2);

	//adjust things when backward
	if (execentity->CharID == 1) {
		execentity->Scale.y = 1;
		if (point < loophead->Count - 2) point += 1;
	}

	execentity->Scale.x = point; //give the executor our position in the spline
	execentity->Scale.y = state;

	PlayerEntity->field_A = PlayerState_InLoop;
}

void LoopPath(ObjectMaster* obj) {
	EntityData1* entity = obj->Data1;
	LoopHead* loophead = (LoopHead*)entity->LoopData; //Current Path Header

	//Check for every player
	for (uint8_t player = 0; player < MaxPlayers; ++player) {
		EntityData1* PlayerEntity = EntityData1Ptrs[player];

		//Check if player exists and if he is not already on the rail
		if (!PlayerEntity || PlayerEntity->field_A == PlayerState_InLoop
			|| PlayerEntity->CharID == Characters_Tikal) continue;

		if (PlayerEntity->Status & Status_Ground) {
			CharObj2* co2 = CharObj2Ptrs[PlayerEntity->CharIndex];
			if (co2->Speed.x < 0.5f);

			//Check if the player is near the loop center to reduce cpu usage
			NJS_VECTOR center = GetPathPosition(&loophead->LoopList[0].Position, &loophead->LoopList[loophead->Count - 1].Position, 0.5f);
			float dist = GetDistance(&loophead->LoopList[0].Position, &loophead->LoopList[loophead->Count - 1].Position) * 1.2 / 2;

			if (!IsPlayerInsideSphere_(&center, dist)) continue;

			//Go through every point of the spline
			for (int16_t point = 0; point < loophead->Count - 1; point += 1) {
				Loop* LoopPoint = &loophead->LoopList[point];
				Loop* NextPoint = &loophead->LoopList[point + 1];

				//Check if the player is between the two points
				for (float state = 0; state <= 1; state += 0.01f) {
					center = GetPathPosition(&LoopPoint->Position, &NextPoint->Position, state);
					if (!IsSpecificPlayerInSphere(&center, 10, player)) continue;

					LoopPath_LoadLoop(PlayerEntity, loophead, point, state);

					return;
				}
			}
		}
	}
}