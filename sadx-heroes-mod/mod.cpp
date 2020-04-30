#include "stdafx.h"

bool IsLantern = false;
set_shader_flags* set_shader_flags_ptr;
material_register* material_register_ptr;
set_diffuse* set_diffuse_ptr;
set_specular* set_specular_ptr;
set_blend_factor* set_blend_factor_ptr;
set_diffuse_blend* set_diffuse_blend_ptr;
set_specular_blend* set_specular_blend_ptr;
set_diffuse_blend_factor* set_diffuse_blend_factor_ptr;
set_specular_blend_factor* set_specular_blend_factor_ptr;
set_blend* set_blend_ptr;

std::string modpath;
HelperFunctions HelperFunctionsGlobal;

bool IsLoaded = false;
bool ChunkSwapped = false;

unsigned int anim = 0;
uint8_t CurrentChunk = 0;
NJS_TEXLIST * CurrentLevelTexlist;

static const Uint8 FREECAM_FIX[] = { 0x81, 0x0D };

void Levels_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void Objects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void Characters_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);

void CommonObjects_OnFrame();
void Characters_OnFrame();

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		//Set up function pointers for Lantern API (borrowed from PkR)
		HMODULE LanternDLL = GetModuleHandle(L"sadx-dc-lighting");

		if (LanternDLL) {
			IsLantern = true;
			set_shader_flags_ptr = (void(*)(uint32_t, bool))GetProcAddress(LanternDLL, "set_shader_flags");
			material_register_ptr = (void(*)(const NJS_MATERIAL *const *materials, size_t length, lantern_material_cb callback))GetProcAddress(LanternDLL, "material_register");
			set_diffuse_ptr = (void(*)(int32_t, bool))GetProcAddress(LanternDLL, "set_diffuse");
			set_specular_ptr = (void(*)(int32_t, bool))GetProcAddress(LanternDLL, "set_specular");
			set_blend_factor_ptr = (void(*)(float))GetProcAddress(LanternDLL, "set_blend_factor");
			set_diffuse_blend_ptr = (void(*)(int32_t, int32_t))GetProcAddress(LanternDLL, "set_diffuse_blend");
			set_specular_blend_ptr = (void(*)(int32_t, int32_t))GetProcAddress(LanternDLL, "set_specular_blend");
			set_diffuse_blend_factor_ptr = (void(*)(float))GetProcAddress(LanternDLL, "set_diffuse_blend_factor");
			set_specular_blend_factor_ptr = (void(*)(float))GetProcAddress(LanternDLL, "set_specular_blend_factor");
			set_blend_ptr = (void(*)(int32_t, int32_t))GetProcAddress(LanternDLL, "set_blend");
		}

		modpath = std::string(path);
		HelperFunctionsGlobal = helperFunctions;

		WriteData((Uint8*)0x438330, FREECAM_FIX); //freecam fix by SonicFreak94
		
		const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
		Levels_Init(path, helperFunctions, config);
		Objects_Init(path, helperFunctions, config);
		Characters_Init(path, helperFunctions, config);
		Sounds_Init(path, helperFunctions, config);
		Enemies_Init(path, helperFunctions, config);
		delete config;
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (GameState != 16) ++anim;

		RunCustomSounds();

		if (GameState == 15 || GameState == 4 || GameMode == 12) {
			Characters_OnFrame();
			
			if (GameMode != 12) {
				if (EnableModels) {
					CommonObjects_OnFrame();
				}
				if (!IsLoaded) IsLoaded = true;
			}
		}

		ChunkSwapped = false;
		if (GameState == 7 || GameState == 6) IsLoaded = false;
	}
	
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}
