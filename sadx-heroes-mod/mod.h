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

extern HelperFunctions HelperFunctionsGlobal;

VoidFunc(sub_40D3B0, 0x40D3B0);

FunctionPointer(void, DrawModelBlend_IsVisible, (NJS_MODEL_SADX *model, QueuedModelFlagsB blend, float radius_scale), 0x4094D0);
FunctionPointer(void, DrawSimpleModel_IsVisible, (NJS_MODEL_SADX *model, float scale), 0x407A00);

DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, FramerateSetting, 0x0389D7DC);

DataArray(int, HeldButtons2, 0x3B0E3A8, 8);
DataArray(Rotation3, LevelGlobalColors, 0x90BFE8, 42);
DataArray(DeathZone*, EmeraldCoastDeathZones, 0x102F8E8, 4);
DataArray(DeathZone*, WindyValleyDeathZones, 0xBFD820, 3);
DataArray(DeathZone*, TwinkleParkDeathZones, 0x26B3C58, 4);
DataArray(DeathZone*, IceCapDeathZones, 0xE2FE4C, 4);
DataArray(DeathZone*, SkyDeckDeathZones, 0x223082C, 3);
DataArray(DeathZone*, CasinopolisDeathZones, 0x1D7E374, 4);
DataArray(DeathZone*, LostWorldDeathZones, 0x2032108, 3);
DataArray(DeathZone*, FinalEggDeathZones, 0x1A49218, 4);

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
extern std::string modpath;

class ModelInfo;

enum HeroesLevelIDs {
	HeroesLevelID_SeasideHill = 1,
	HeroesLevelID_OceanPalace,
	HeroesLevelID_CasinoPark,
	HeroesLevelID_BingoHighway,
	HeroesLevelID_EggFleet = 6,
	HeroesLevelID_HangCastle,
	HeroesLevelID_PowerPlant,
	HeroesLevelID_GrandMetropolis,
	HeroesLevelID_MysticMansion,
	HeroesLevelID_SpecialStages = 35
};

typedef struct {
	NJS_VECTOR		Position;
	Angle			Rotation[3];
	float			Distance;
	uint8_t			parameter;
} SH_PATH;

typedef struct {
	SH_PATH			*path;
	float			totaldist;
	uint8_t			parameters[3];
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