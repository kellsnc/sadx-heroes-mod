#include "stdafx.h"
#include "mod.h"

float IsLantern = false;
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

bool EnableModels = true;
bool EnableSounds = true;
bool IsLoaded = false;
bool ChunkSwapped = false;

unsigned int anim = 0;
uint8_t CurrentChunk = 0;
NJS_TEXLIST * CurrentLevelTexlist;

void Levels_Init(const char *path, const HelperFunctions &helperFunctions);
void Objects_Init(const char *path, const HelperFunctions &helperFunctions);
void CommonObjects_OnFrame();

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		//Get the config.ini information
		const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
		EnableSounds = config->getBool("General", "EnableSounds", true);
		EnableModels = config->getBool("General", "EnableModels", true);
		delete config;

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
		
		Levels_Init(path, helperFunctions);
		Objects_Init(path, helperFunctions);
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		++anim;

		if ((GameState == 15 || GameState == 4)) {
			if (EnableModels) CommonObjects_OnFrame();
			if (!IsLoaded) IsLoaded = true;
		}

		ChunkSwapped = false;
		if (GameState == 7 || GameState == 6) IsLoaded = false;
	}
	
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}
