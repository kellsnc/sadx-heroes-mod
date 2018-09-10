#pragma once

DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, FramerateSetting, 0x0389D7DC);
DataArray(int, HeldButtons2, 0x3B0E3A8, 8);

extern unsigned int anim;
extern NJS_TEXLIST * CurrentLevelTexlist;
extern uint8_t DayTimeModifier;
extern uint8_t CurrentChunk;
extern bool EnableModels;
extern bool EnableSounds;
extern bool IsLoaded;
extern bool ChunkSwapped;

void Levels_Init(const char *path, const HelperFunctions &helperFunctions);
void Objects_Init(const char *path, const HelperFunctions &helperFunctions);

typedef struct {
	NJS_VECTOR		pathpos;
	Angle			pathrot[3];		//manual value for the angle the object should have, the value is absolute, if unset it will calculate it relatively
	float			pathdist;		//manual value for the speed to the next point, if unset it will calculate it itself on frame
	uint8_t			parameter;
} SH_PATH;

typedef struct {
	SH_PATH			*path;
	float			totaldist;
	uint8_t			parameters[3];	/*[0] > 1 to allow rotation
									[1] > 1 to remove the main character gravity for the length of the path
									[2] > 1 to remove the main character controls for the length of the path
									*/
	uint8_t			pathentrysize;
} SH_PATHS;

typedef struct {
	NJS_MODEL_SADX	*soiobject;
	NJS_VECTOR		soipos;
	Angle			soirot[3];
	NJS_VECTOR		soiscl;
	Float			soibias;
	uint8_t			soichunk;
	Float			soidrawdist;
	uint8_t			soidisplay; /* Where 0 is visible, 1 is skip (invisible), 2 is below directx 11 only, 3 is directx 11 only */
	uint8_t			parameter1;
	uint8_t			parameter2;
} SOI_LIST;

typedef struct {
	uint8_t			soicharid;
	uint16_t		soicount;
	SOI_LIST		*soientry;
} SOI_LISTS;

typedef struct {
	uint8_t			chunkid;
	NJS_VECTOR		chunkpos1;
	NJS_VECTOR		chunkpos2;
	uint8_t			chunkdisplay; /* Where 0 is visible, 1 is below directx 11 only, 2 is directx 11 only */
} CHUNK_LIST;

typedef struct {
	int			texid;
	int			count;
	int			duration[40];
} SH_ANIMTEXS;

typedef struct {
	NJS_TEX*		uvlist;
	int				uvsize;
	int8_t			uvshift[2];
	uint8_t			uvtime;
} SH_UVSHIFT;