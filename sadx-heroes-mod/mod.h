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

FunctionPointer(void, DrawObject, (NJS_OBJECT*), 0x408530);
FunctionPointer(float, GetGroundYPosition, (float x, float y, float z, Rotation3* rotation), 0x49E920);
DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, FramerateSetting, 0x0389D7DC);
DataPointer(NJS_VECTOR, bombpos, 0x3C5AB24);
DataArray(int, HeldButtons2, 0x3B0E3A8, 8);
ObjectFunc(Bomb_Init, 0x4AC920);
FunctionPointer(bool, CheckBombPowerupRadius, (NJS_VECTOR* pos), 0x4C0550);

extern float bombsize;

extern unsigned int anim;
extern uint8_t CurrentChunk;
extern NJS_TEXLIST * CurrentLevelTexlist;

extern bool IsLoaded;
extern bool ChunkSwapped;

struct ObjectData
{
    int field_0;
    NJS_VECTOR field_4;
    NJS_VECTOR field_10;
    char gap1C[20];
    int field_30;
    float field_34;
    float field_38;
    float field_3C;
    char gap40[9];
    Uint8 ActionBackup;
    char field_4A;
    char field_4B;
    __int16 SomeIndex;
    __int16 SomeFlags;
    NJS_VECTOR StartPosition;
    NJS_VECTOR vector_b;
    NJS_VECTOR vector_c;
    float field_74;
    float field_78;
    float field_7C;
    float field_80;
    float field_84;
    float field_88;
    float field_8C;
    char gap_90[12];
    int field_9C;
    float field_A0;
    int field_A4;
    float field_A8;
    float field_AC;
    float field_B0;
    float field_B4;
    float field_B8;
    float field_BC;
    float field_C0;
    float field_C4;
    int field_C8;
    float field_CC;
    float field_D0;
    char gapD4[204];
    float field_1A0;
    float field_1A4;
    int field_1A8;
    int SomeAngleZ;
    int field_1B0;
    int SomeAngleY;
    char gap1B8[4];
    int field_1BC;
    int field_1C0;
    char gap1C4[12];
    int SomeAngleC;
    int SomeAngleX;
    float field_1D8;
    float field_1DC;
    float field_1E0;
    float field_1E4;
    float field_1E8;
    float field_1EC;
    float field_1F0;
    NJS_ACTION* AnimHead_ptr_A;
    NJS_ACTION* AnimHead_ptr_B;
    NJS_MATRIX matrix;
};

struct enemywk {
    int unk0;
    NJS_VECTOR velo;
    NJS_VECTOR acc;
    Angle ang_aim;
    Angle ang_spd;
    float radius;
    float height;
    float weight;
    ObjectMaster* lock_tp;
    void(__cdecl* dest_org)(ObjectMaster*);
    char pnum;
    char old_mode;
    char old_smode;
    __int16 aim_num;
    unsigned __int16 flag;
    NJS_VECTOR home;
    NJS_VECTOR aim;
    NJS_VECTOR pre;
    NJS_VECTOR force;
    NJS_VECTOR norm;
    NJS_ARGB argb;
    NJS_VECTOR colli_center;
    float colli_top;
    float colli_radius;
    float colli_bottom;
    float cliff_height;
    float bound_side;
    float bound_floor;
    float bound_friction;
    float bound_ceiling;
    int bound_add_angle;
    float shadow_scl;
    float shadow_scl_ratio;
    int shadow;
    float buyoscale;
    int angx_spd;
    int angy_spd;
    int angz_spd;
    int sub_angle[8];
    int aim_angle;
    int view_angle;
    float view_range;
    float hear_range;
    float nframe;
    float pframe;
    float lframe;
    float link_ratio;
    float link_spd;
    NJS_ACTION* actp;
    NJS_ACTION* linkactp;
    NJS_MATRIX matrix;
};

enum CollisionShapes {
    CollisionShape_Sphere,
    CollisionShape_Cylinder,
    CollisionShape_Cube,
    CollisionShape_Cone,
    CollisionShape_Capsule = 5
};

#define MaxPlayers 8