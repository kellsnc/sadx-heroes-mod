#pragma once

extern NJS_MATERIAL matlist_col[1];
extern NJS_MATERIAL matlist_waterfall[1];
extern uint8_t killcount;

void AddToCollision(ObjectMaster *a1, uint8_t col);
void DrawObjModel(ObjectMaster *a1, NJS_MODEL_SADX *m, bool scalable);
void deleteSub_Global(ObjectMaster *a1);
void displaySub_Global(ObjectMaster *a1);
void mainSub_Global(ObjectMaster *a1);
void AnimateUV(SH_UVSHIFT *uvshift, int size);
void AnimateObjectsTextures(NJS_MODEL_SADX * *objlist, int size, SH_ANIMTEXS *list, Int listcount);

void CommonObjects_Init(const char *path, const HelperFunctions &helperFunctions);
void CommonObjects_OnFrame();

void __cdecl ObjReel(ObjectMaster *a1);
void __cdecl ObjFan(ObjectMaster *a1);
void __cdecl SHCameraSwitch(ObjectMaster *a1);
void __cdecl ObjBalloon(ObjectMaster *a1);
void __cdecl ObjCannon(ObjectMaster *a1);
void __cdecl ObjBob(ObjectMaster *a1);
void __cdecl ObjBoxW(ObjectMaster *a1);
void __cdecl OBJCASE(ObjectMaster *a1);