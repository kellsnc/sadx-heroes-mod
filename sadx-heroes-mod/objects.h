#pragma once

extern uint8_t killcount;

void DynCol_Add(ObjectMaster *a1, uint8_t col);
void DrawObjModel(ObjectMaster *a1, NJS_MODEL_SADX *m, bool scalable);
void DynCol_Delete(ObjectMaster *a1);
void displaySub_Global(ObjectMaster *a1);
void mainSub_Global(ObjectMaster *a1);
void AnimateUV(SH_UVSHIFT *uvshift, int size);
void AnimateObjectsTextures(NJS_MODEL_SADX * *objlist, int size, SH_ANIMTEXS *list, Int listcount);
bool IsPlayerInBox(NJS_VECTOR playerpos, NJS_VECTOR pos1, NJS_VECTOR pos2);
void ElevatePlayer(uint8_t slot);
bool CheckModelDisplay(SOI_LIST item);
bool CheckModelDisplay2(SOI_LIST2 item);
bool DynColRadius(ObjectMaster *a1, float radius, uint8_t col);
bool DynColRadiusAuto(ObjectMaster *a1, uint8_t col);
Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);
NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state);
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions);
void CommonObjects_OnFrame();

void ObjReel(ObjectMaster *a1);
void ObjFan(ObjectMaster *a1);
void SHCameraSwitch(ObjectMaster *a1);
void ObjBalloon(ObjectMaster *a1);
void ObjCannon(ObjectMaster *a1);
void ObjBoxW(ObjectMaster *a1);
void OBJCASE(ObjectMaster *a1);
void WaterfallObject(ObjectMaster *a1);

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];