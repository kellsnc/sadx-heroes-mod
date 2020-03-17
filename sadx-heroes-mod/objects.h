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

void ElevatePlayer(uint8_t slot);
bool IsPointInCollision(NJS_VECTOR* pos, float radius);
void DoBall(uint8_t id);

void ObjectBreaker(ObjectMaster* obj);

uint8_t Fans_IsSpecificPlayerInCylinder(EntityData1* entity, NJS_VECTOR* center, float radius, float height);
uint8_t Fans_IsPlayerInCylinder(NJS_VECTOR* center, float radius, float height);
void Fans_SetSpeedAndAnim(uint8_t player);
void Fans_HandlePlayer(ObjectMaster* obj);

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