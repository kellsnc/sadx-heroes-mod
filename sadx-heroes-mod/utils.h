#pragma once
#include "stdafx.h"

ModelInfo* LoadMDL(const char* type, const char* name);
ModelInfo* LoadCommonModel(const char* name);
ModelInfo* LoadObjectModel(ModelInfo* ptr, const char* name);
ModelInfo* LoadCharacterModel(const char* name);
ModelInfo* FreeMDL(ModelInfo* pointer);

AnimationFile* LoadANM(const char* type, const char* name);
AnimationFile* LoadObjectAnim(const char* name);
AnimationFile* LoadCharacterAnim(const char* name);

void FreeANM(AnimationFile* pointer);
void FreeMDLFiles(ModelInfo** Files, int size);
void FreeANMFiles(AnimationFile** Files, int size);

void DrawObjModel(ObjectMaster* a1, NJS_MODEL_SADX* m, bool scalable);
void displaySub_Global(ObjectMaster* a1);
void mainSub_Global(ObjectMaster* a1);
void DynCol_Add(ObjectMaster* a1, uint8_t col);
void DynCol_Update(EntityData1* data, uint8_t type);
void DynCol_Delete(ObjectMaster* a1);
bool DynColRadius(ObjectMaster* a1, float radius, uint8_t col);
bool DynColRadiusAuto(ObjectMaster* a1, uint8_t col);

bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
int IsPlayerInsideSphere_(NJS_VECTOR* center, float radius);
bool IsSpecificPlayerInSphere(NJS_VECTOR* center, float radius, uint8_t player);
bool IsPlayerInBox(NJS_VECTOR playerpos, NJS_VECTOR pos1, NJS_VECTOR pos2);

void AddUVList(SH_UVSHIFT* UVSHIFT, int id, NJS_MODEL_SADX* model, int mesh);
void AnimateUV(SH_UVSHIFT* UVSHIFT, int size);
bool CheckModelDisplay(SOI_LIST item);
bool CheckModelDisplay2(SOI_LIST2 item);

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state);
Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
void DrawChunkModel(NJS_CNK_MODEL* model);
void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags);
void njCnkAction(NJS_ACTION* action, float frame);
void SetupWorldMatrix();