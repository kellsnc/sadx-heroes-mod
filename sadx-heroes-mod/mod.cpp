#include "stdafx.h"
#include "lanternapi.h"
#include "levels.h"
#include "objects.h"
#include "characters.h"
#include "sounds.h"
#include "enemies.h"

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

static const Uint8 FREECAM_FIX[] = { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ };

void Levels_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void Objects_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void Characters_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);
void Sounds_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config);

void CommonObjects_OnFrame();
void Sounds_OnFrame();

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		if (helperFunctions.Version < 17)
		{
			MessageBox(WindowHandle, TEXT("Heroes Conversion failed to load because the Mod Loader is outdated. Please update it through SA Mod Manager."), TEXT("Heroes Conversion"), MB_OK | MB_ICONWARNING);
			return;
		}

		//Set up function pointers for Lantern API (borrowed from PkR)
		auto lantern = helperFunctions.Mods->find_by_name("Lantern Engine");
		if (lantern)
		{
			IsLantern = true;
			set_shader_flags_ptr = lantern->GetDllExport<decltype(set_shader_flags_ptr)>("set_shader_flags");
			material_register_ptr = lantern->GetDllExport<decltype(material_register_ptr)>("material_register");
			set_diffuse_ptr = lantern->GetDllExport<decltype(set_diffuse_ptr)>("set_diffuse");
			set_specular_ptr = lantern->GetDllExport<decltype(set_specular_ptr)>("set_specular");
			set_blend_factor_ptr = lantern->GetDllExport<decltype(set_blend_factor_ptr)>("set_blend_factor");
			set_diffuse_blend_ptr = lantern->GetDllExport<decltype(set_diffuse_blend_ptr)>("set_diffuse_blend");
			set_specular_blend_ptr = lantern->GetDllExport<decltype(set_specular_blend_ptr)>("set_specular_blend");
			set_diffuse_blend_factor_ptr = lantern->GetDllExport<decltype(set_diffuse_blend_factor_ptr)>("set_diffuse_blend_factor");
			set_specular_blend_factor_ptr = lantern->GetDllExport<decltype(set_specular_blend_factor_ptr)>("set_specular_blend_factor");
			set_blend_ptr = lantern->GetDllExport<decltype(set_blend_ptr)>("set_blend");
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

		Sounds_OnFrame();

		if (GameState == 15 || GameState == 4 || GameMode == 12) {
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
