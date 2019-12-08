#pragma once

enum IndexObj : Sint8
{
	ObjIndex_NoDisplay = 0,
	ObjIndex_1 = 0x1,
	ObjIndex_Common = 0x2,
	ObjIndex_Stage = 0x3,
	ObjIndex_4 = 0x4,
	ObjIndex_5 = 0x5,
	ObjIndex_6 = 0x6,
	ObjIndex_RunFirst = 0x7
};

enum DistObj : Sint16
{
	DistObj_Default = 0,
	DistObj_UseDist = 0x1,
	DistObj_NoDistCheck = 0x2,
	DistObj_Unknown3 = 0x3,
	DistObj_Unknown4 = 0x4,
	DistObj_Unknown5 = 0x5
};

typedef struct {
	NJS_VECTOR		Position;
	Angle			Rotation[3];
	uint8_t			Chunk;
	Float			DrawDistance;
} SOI_LIST;

typedef struct {
	uint8_t			Model;
	NJS_VECTOR		Position;
	Angle			Rotation[3];
	NJS_VECTOR		Scale;
	Float			Bias;
	uint8_t			Chunk;
	Float			DrawDistance;
} SOI_LIST2;

typedef struct {
	NJS_TEX*		List;
	int				Size;
	int8_t			uvshift[2];
	uint8_t			uvtime;
} SH_UVSHIFT;

extern uint8_t killcount;

ModelInfo* LoadObjectModel(const char *name);
ModelInfo* LoadCharacterModel(const char *name);
AnimationFile* LoadObjectAnim(const char *name);
AnimationFile* LoadCharacterAnim(const char *name);
void FreeMDL(ModelInfo * pointer);
void FreeANM(AnimationFile * pointer);

void DynCol_Add(ObjectMaster *a1, uint8_t col);
void DrawObjModel(ObjectMaster *a1, NJS_MODEL_SADX *m, bool scalable);
void DynCol_Delete(ObjectMaster *a1);
void displaySub_Global(ObjectMaster *a1);
void mainSub_Global(ObjectMaster *a1);
void AnimateUV(SH_UVSHIFT *uvshift, int size);
bool IsPlayerInBox(NJS_VECTOR playerpos, NJS_VECTOR pos1, NJS_VECTOR pos2);
void ElevatePlayer(uint8_t slot);
bool CheckModelDisplay(SOI_LIST item);
bool CheckModelDisplay2(SOI_LIST2 item);
bool IsPointInsideSphere(NJS_VECTOR *center, NJS_VECTOR *pos, float radius);
int IsPlayerInsideSphere_(NJS_VECTOR *center, float radius);
bool DynColRadius(ObjectMaster *a1, float radius, uint8_t col);
bool DynColRadiusAuto(ObjectMaster *a1, uint8_t col);
bool IsPointInCollision(NJS_VECTOR* pos, float radius);
void TransformSpline(ObjectMaster * a1, NJS_VECTOR orig, NJS_VECTOR dest, float state);
void DoBall(uint8_t id);

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
void DrawChunkModel(NJS_CNK_MODEL* model);
void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags);
void njCnkAction(NJS_ACTION* action, float frame);
void SetupWorldMatrix();

Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);
NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state);
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);

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
extern NJS_TEXLIST SHCommonTextures;
extern bool EnableModels;