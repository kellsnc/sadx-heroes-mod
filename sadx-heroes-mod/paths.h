#pragma once

void TransformSpline(EntityData1* entity, NJS_VECTOR orig, NJS_VECTOR dest, float state);
void PathHandler(EntityData1* entity, LoopHead* path, float speed);

void AutoLoop(ObjectMaster* a1);
void RailPath(ObjectMaster* a1);
void SlopePath(ObjectMaster* a1);
void PropellerPath(ObjectMaster* obj);