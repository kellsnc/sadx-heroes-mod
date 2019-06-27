#pragma once

extern uint8_t killcount;

void AddToCollision(ObjectMaster *a1, uint8_t col);
void DrawObjModel(ObjectMaster *a1, NJS_MODEL_SADX *m, bool scalable);
void deleteSub_Global(ObjectMaster *a1);
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

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions);
void CommonObjects_OnFrame();

void __cdecl ObjReel(ObjectMaster *a1);
void __cdecl ObjFan(ObjectMaster *a1);
void __cdecl SHCameraSwitch(ObjectMaster *a1);
void __cdecl ObjBalloon(ObjectMaster *a1);
void __cdecl ObjCannon(ObjectMaster *a1);
void __cdecl ObjBoxW(ObjectMaster *a1);
void __cdecl OBJCASE(ObjectMaster *a1);

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];