#pragma once

void TransformSpline(EntityData1* entity, NJS_VECTOR orig, NJS_VECTOR dest, float state);
void PathHandler(EntityData1* entity, LoopHead* path, float speed);
bool Path_GetDirection(EntityData1* PlayerEntity, LoopHead* loophead, uint16_t point);

void AutoLoop(ObjectMaster* a1);
void RailPath(ObjectMaster* a1);
void SlopePath(ObjectMaster* a1);
void PropellerPath(ObjectMaster* obj);
void LoopPath(ObjectMaster* obj);

extern NJS_OBJECT* PropellerModel;

enum PlayerStates {
	PlayerState_OnRail = 125,
	PlayerState_SwapLeft,
	PlayerState_SwapRight,
	PlayerState_InLoop
};

enum DetachTypes {
	DetachType_End,
	DetachType_Fall,
	DetachType_Jump,
	DetachType_Death
};