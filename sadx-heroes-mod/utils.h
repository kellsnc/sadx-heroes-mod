#pragma once

#include "objects.h"

#define CWK_FLAG_HIT    0x1          /* Currently intersecting another collision */
#define CWK_FLAG_2      0x2          /* ? */
#define CWK_FLAG_4      0x4          /* ? */
#define CWK_FLAG_8      0x8          /* ? */
#define CWK_FLAG_PUSH   0x10         /* Currently being pushed by another collision */
#define CWK_FLAG_DAMAGE 0x20         /* Currently receiving damage */
#define CWK_FLAG_HOMING 0x40         /* Can be added to the target lists */
#define CWK_FLAG_MEM    0x8000       /* The info array was allocated */

#define CI_ATTR_NO_SEND        0x4      /* Do not send information to other collisions */
#define CI_ATTR_NO_RECEIVE     0x8      /* Do not receive information from other collisions */
#define CI_ATTR_IGNORE         0x10     /* Do not run physical collision check */
#define CI_ATTR_POS_ABSOLUTE   0x20     /* The position is no longer relative to the entity */
#define CI_ATTR_ANG_YZX        0x200    /* Use YZX rotation order instead of ZXY */
#define CI_ATTR_DAMAGE         0x400    /* Allow damage */
#define CI_ATTR_2000           0x2000   /* ? */
#define CI_ATTR_NO_PUSH        0x4000   /* Do not get pushed by other collisions */
#define CI_ATTR_ANG_ABSOLUTE   0x8000   /* The angle is no longer relative to the entity */
#define CI_ATTR_IGNORE_PLAYER  0x40000  /* Do not interact with player collisions */
#define CI_ATTR_IGNORE_ENEMY   0x80000  /* Do not interact with enemy collisions */
#define CI_ATTR_IGNORE_OBJECT  0x100000 /* Do not interact with object collisions */
#define CI_ATTR_IGNORE_ITEM    0x200000 /* Do not interact with item collisions */
#define CI_ATTR_IGNORE_CHAO    0x400000 /* Do not interact with chao collisions */
#define CI_ATTR_800000         0x800000 /* ? */

#define CI_DMG_SET(att, def) (att | (def << 2)) /* Set the attack and defense priorities (0-3) */
#define CI_DMG_GET_ATT(dmg) (dmg & 0x3)
#define CI_DMG_GET_DEF(dmg) ((dmg >> 2) & 0x3)
#define CI_DMG_GET_LISTS(dmg) (dmg & 0xF0)
#define CI_DMG_HIT    0x10 /* Collision has been hurt */
#define CI_DMG_PLAYER 0x20 /* Can hurt collisions in PLAYER and CHAO list */
#define CI_DMG_ENEMY  0x40 /* Can hurt collisions in ENEMY and ENEMY2 lists */
#define CI_DMG_OBJECT 0x80 /* Can hurt collisions in OBJECT and OBJECT2 lists */
#define CI_DMG_ALL (CI_DMG_PLAYER | CI_DMG_ENEMY | CI_DMG_OBJECT) /* Can hurt all collisions except BULLET/ITEM ones */

#define CI_PUSH_PO_PLAYER 0x1 /* Collision can push a PLAYER or CHAO collision */
#define CI_PUSH_PO_ENEMY  0x2 /* Collision can push a ENEMY or ENEMY2 collision */
#define CI_PUSH_PO_OBJECT 0x4 /* Collision can push a OBJECT or OBJECT2 collision */
#define CI_PUSH_PO_ITEM   0x8 /* Collision can push a ITEM or ITEM2 collision */
#define CI_PUSH_PO_CMN    (CI_PUSH_PO_PLAYER | CI_PUSH_PO_ENEMY | CI_PUSH_PO_OBJECT) /* Collision can push all collisions except ITEM, ITEM2 and BULLET */
#define CI_PUSH_PO_ALL    (CI_PUSH_PO_CMN | CI_PUSH_PO_ITEM) /* Collision can push all collisions except BULLET */

#define CI_PUSH_TH_PLAYER 0x10 /* Collision can't be pushed by a CID_PLAYER or CID_CHAO collision */
#define CI_PUSH_TH_ENEMY  0x20 /* Collision can't be pushed by a CID_ENEMY or CID_ENEMY2 collision */
#define CI_PUSH_TH_OBJECT 0x40 /* Collision can't be pushed by a CID_OBJECT or CID_OBJECT2 collision */
#define CI_PUSH_TH_ITEM   0x80 /* Collision can't be pushed by a CID_ITEM or CID_ITEM2 collision */
#define CI_PUSH_TH_CMN    (CI_PUSH_TH_PLAYER | CI_PUSH_TH_ENEMY | CI_PUSH_TH_OBJECT) /* Collision can't be pushed by all collisions except ITEM, ITEM2 and BULLET */
#define CI_PUSH_TH_ALL    (CI_PUSH_TH_CMN | CI_PUSH_TH_ITEM) /* Collision can't pushed by all collisions except ITEM, ITEM2 and BULLET */

enum {
	MD_MTN_INIT,
	MD_MTN_SET,
	MD_MTN_CHNG,
	MD_MTN_LOOP,
	MD_MTN_NEXT,
	MD_MTN_STOP,
	MD_MTN_TXEN,
	MD_MTN_POTS,
	MD_MTN_COMB,
	MD_MTN_XSPD,
	MD_MTN_WORK,
	MD_MTN_WKNX,
	MD_MTN_MANU,
	MD_MTN_TRNS,
	MD_MTN_END
};

FunctionPointer(void, SetCameraMode_, (int a1), 0x4348F0);
FunctionPointer(Uint32, GetCameraMode_, (), 0x434930);

DataPointer(NJS_VECTOR, explosion_p, 0x3C5AB24);
DataPointer(Float, explosion_r, 0x3C5A9D0);

ModelInfo* LoadMDL(const char* type, const char* name);
ModelInfo* LoadCommonModel(const char* name);
ModelInfo* LoadObjectModel(ModelInfo* ptr, const char* name);
ModelInfo* LoadCharacterModel(const char* name);
ModelInfo* LoadEnemyModel(const char* name);
ModelInfo* FreeMDL(ModelInfo* pointer);

AnimationFile* LoadANM(const char* type, const char* name);
AnimationFile* LoadObjectAnim(const char* name);
AnimationFile* LoadCharacterAnim(const char* name);
AnimationFile* LoadEnemyAnim(const char* name);
void FreeANM(AnimationFile* pointer);

void LoadModelListFuncPtr(const char** names, int count, ModelInfo** anms, ModelInfo* (*func)(const char*));
void LoadAnimListFuncPtr(const char** names, int count, AnimationFile** anms, AnimationFile* (*func)(const char*));
void FillAnimDataTable(AnimationFile** animfiles, AnimData* animdata, int count, NJS_OBJECT* obj);
void FreeMDLFiles(ModelInfo** Files, int size);
void FreeANMFiles(AnimationFile** Files, int size);

void DrawObjModel(ObjectMaster* a1, NJS_MODEL_SADX* m, bool scalable);
void displaySub_Global(ObjectMaster* a1);
void mainSub_Global(ObjectMaster* a1);
void mainSub_DyncolGlobal(ObjectMaster* obj);
void mainSub_Dyncol(ObjectMaster* obj);
ObjectMaster* LoadSubDynCol(ObjectMaster* obj, NJS_OBJECT* model, uint8_t col, float radius);
void DynCol_Add(ObjectMaster* a1, uint8_t col);
void DynCol_Update(EntityData1* data, uint8_t type);
void DynCol_Delete(ObjectMaster* a1);
bool DynColRadius(ObjectMaster* a1, float radius, uint8_t col);
bool DynColRadiusAuto(ObjectMaster* a1, uint8_t col);

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
int IsPlayerInsideSphere_(NJS_VECTOR* center, float radius);
bool IsSpecificPlayerInSphere(NJS_VECTOR* center, float radius, uint8_t player);
bool AreSpheresColliding(NJS_VECTOR* sphereA, float radiusA, NJS_VECTOR* sphereB, float radiusB);
bool IsPlayerInBox(NJS_VECTOR playerpos, NJS_VECTOR pos1, NJS_VECTOR pos2);

bool CheckObjectDamage(EntityData1* data);

void AddUVList(SH_UVSHIFT* UVSHIFT, int id, NJS_MODEL_SADX* model, int mesh);
void AnimateUV(SH_UVSHIFT* UVSHIFT, int size);
bool CheckModelDisplay(SOI_LIST item);
bool CheckModelDisplay2(SOI_LIST2 item);

NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state);
Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);
float GetGroundPositionEntity(EntityData1* data, bool rot);
NJS_VECTOR UnitMatrix_GetPoint(NJS_VECTOR* orig, Rotation3* rot, float x, float y, float z);

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
void DrawChunkModel(NJS_CNK_MODEL* model);
void SetupWorldMatrix();

void njTranslateX(float f);
void njTranslateY(float f);
void njTranslateZ(float f);
void njScaleX(float f);
void njScaleY(float f);
void njScaleZ(float f);

void njActionWeight(NJS_ACTION* action, Float frame, WeightInfo* weight);