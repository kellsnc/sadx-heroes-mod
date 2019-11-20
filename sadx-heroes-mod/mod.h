#pragma once

extern bool IsLantern;
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

extern std::string modpath;
extern HelperFunctions HelperFunctionsGlobal;

DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, FramerateSetting, 0x0389D7DC);
DataArray(int, HeldButtons2, 0x3B0E3A8, 8);

extern unsigned int anim;
extern uint8_t CurrentChunk;
extern NJS_TEXLIST * CurrentLevelTexlist;

extern bool EnableModels;
extern bool EnableSounds;
extern bool IsLoaded;
extern bool ChunkSwapped;

typedef struct {
	int			texid;
	int			count;
	int			duration[40];
} SH_ANIMTEXS;