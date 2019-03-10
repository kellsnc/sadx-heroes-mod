#pragma once

extern float IsLantern;
extern set_shader_flags* set_shader_flags_ptr;
extern material_register* material_register_ptr;
extern set_diffuse* set_diffuse_ptr;
extern set_specular* set_specular_ptr;
extern set_blend_factor* set_blend_factor_ptr;
extern set_diffuse_blend* set_diffuse_blend_ptr;
extern set_specular_blend* set_specular_blend_ptr;
extern set_diffuse_blend_factor* set_diffuse_blend_factor_ptr;
extern set_specular_blend_factor* set_specular_blend_factor_ptr;
extern set_blend* set_blend_ptr;

VoidFunc(sub_40D3B0, 0x40D3B0);
FunctionPointer(void, DrawModelBlend_IsVisible, (NJS_MODEL_SADX *model, QueuedModelFlagsB blend, float radius_scale), 0x4094D0);
FunctionPointer(void, DrawSimpleModel_IsVisible, (NJS_MODEL_SADX *model, float scale), 0x407A00);
DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, FramerateSetting, 0x0389D7DC);
DataArray(int, HeldButtons2, 0x3B0E3A8, 8);
DataArray(Rotation3, LevelGlobalColors, 0x90BFE8, 42);
DataArray(ObjectFuncPtr, SkyboxObjects, 0x90C1F0, 42);

ModelInfo* LoadMDL(const char *shortname);
void FreeMDL(ModelInfo * pointer);

extern unsigned int anim;
extern NJS_TEXLIST * CurrentLevelTexlist;
extern uint8_t DayTimeModifier;
extern uint8_t CurrentChunk;
extern bool EnableModels;
extern bool EnableSounds;
extern bool IsLoaded;
extern bool ChunkSwapped;

class ModelInfo;

enum HeroesLevelIDs {
	HeroesLevelID_SeasideHill = 1,
	HeroesLevelID_OceanPalace,
	HeroesLevelID_CasinoPark,
	HeroesLevelID_BingoHighway,
	HeroesLevelID_HangCastle = 7,
	HeroesLevelID_PowerPlant,
	HeroesLevelID_GrandMetropolis,
	HeroesLevelID_MysticMansion
};

typedef struct {
	NJS_VECTOR		Position;
	Angle			Rotation[3];		//manual value for the angle the object should have, the value is absolute, if unset it will calculate it relatively
	float			Distance;			//manual value for the speed to the next point, if unset it will calculate it itself on frame
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
	uint8_t			Chunk;
	NJS_VECTOR		Position1;
	NJS_VECTOR		Position2;
} CHUNK_LIST;

typedef struct {
	int			texid;
	int			count;
	int			duration[40];
} SH_ANIMTEXS;

typedef struct {
	NJS_TEX*		List;
	int				Size;
	int8_t			uvshift[2];
	uint8_t			uvtime;
} SH_UVSHIFT;