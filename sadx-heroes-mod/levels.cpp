#include "stdafx.h"
#include "mod.h"

#include "levels.h"

static bool EnableSeasideHill = true;
static bool EnableSeaGate = true;
static bool EnableOceanPalace = true;
static bool EnableRoadRock = true;
static bool EnableGrandMetropolis = true;
static bool EnablePowerPlant = true;
static bool EnableCasinoPark = true;
static bool EnableBingoHighway = true;
static bool EnableHangCastle = true;
static bool EnableMysticMansion = true;

bool NoPinball = false;
bool chunkswapped = false;
bool IsHeroesLevel = false;

LandTable** CurrentLandAddress;

std::string modpath;
LandTableInfo *info = nullptr;
LandTableInfo *oldinfo = nullptr;

NJS_MATERIAL* landmtl[] = { nullptr };

StartPosition Heroes_StartPositions[]{
	{ HeroesLevelID_SeasideHill, 0,{ 0, 6.800581f, 5.217285f }, 0xBFFF },
	{ HeroesLevelID_SeasideHill, 1,{ 6090, 30, 1000 }, 0xBFFF },			//Sea Gate
	{ HeroesLevelID_OceanPalace, 0,{ -8771, 1303, -2819.688f }, 0xBFFF },
	{ HeroesLevelID_OceanPalace, 1,{ 0, 3020, 4462 }, 0xBFFF },				//Road Rock
	{ HeroesLevelID_GrandMetropolis, 0,{ 0.5f, 70, 4125.8f }, 0xBFFF },
	{ HeroesLevelID_PowerPlant, 0,{ 2000, 1564, 63 }, 0xBFFF },
	{ HeroesLevelID_CasinoPark, 0,{ -8000, 2188, 0 }, 0xBFFF },
	{ HeroesLevelID_BingoHighway, 0,{ 7999, 2277, 472 }, 0xBFFF },
	{ HeroesLevelID_HangCastle, 0,{ 3999, 4000, 109 }, 0xBFFF },
	{ HeroesLevelID_MysticMansion, 0,{ 0, 23, 777 }, 0xBFFF }
};

//Chunk system
bool ForceWhiteDiffuse(NJS_MATERIAL* material, Uint32 flags)
{
	set_diffuse_ptr(1, false);
	set_specular_ptr(7, false);
	return true;
}

void FreeCurrentChunk(int level, int act)
{
	LandTable *CurrentLand; // esi
	int COL_Length; // ebp
	int COL_LengthT; // ebx
	NJS_OBJECT **LandObject; // edi

	CurrentLand = GeoLists[act + 8 * level];
	if (CurrentLand)
	{
		COL_Length = CurrentLand->COLCount;
		if (COL_Length > 0)
		{
			LandObject = &CurrentLand->Col->Model;
			COL_LengthT = CurrentLand->COLCount;
			do
			{
				if (((unsigned int)LandObject[2] & (ColFlags_Visible | ColFlags_UvManipulation)) == ColFlags_Visible)
				{
					FreeLandTableObject(*LandObject);
				}
				LandObject += 9;
				--COL_LengthT;
			} while (COL_LengthT);
		}
	}
	if (!IsLoaded) SetQueueDrawingState_BlankScreen();
}

void SwapCurrentLandTable() {
	LandTable *land = info->getlandtable();
	for (Int j = 0; j < land->COLCount; ++j) {
		if (land->Col[j].Flags == 0x80000000) {
			for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
				landmtl[0] = &land->Col[j].Model->basicdxmodel->mats[k];
				if (IsLantern) material_register_ptr(landmtl, LengthOfArray(landmtl), &ForceWhiteDiffuse);
			}
		}
	}

	chunkswapped = true;
	land->TexList = CurrentLevelTexlist;
	land->AnimCount = 0;
	WriteData((LandTable**)CurrentLandAddress, land);
}

void LoadLevelFile(const char *shortname, int chunknb) {
	std::string fullPath = modpath + "\\system\\levels\\" + shortname;
	std::string numtos = std::to_string(chunknb);
	if (chunknb < 10) fullPath += "0";
	fullPath += numtos + ".sa1lvl";
	const char *foo = fullPath.c_str();

	PrintDebug("[SHM] Swapping for ");
	PrintDebug(foo);
	PrintDebug("\n");

	FreeCurrentChunk(CurrentLevel, CurrentAct);

	if (info) {
		if (oldinfo) {
			delete oldinfo;
			oldinfo = nullptr;

		}
		oldinfo = info;
		info = nullptr;
	}

	info = new LandTableInfo(foo);
	CurrentChunk = chunknb;
	SwapCurrentLandTable();
	SetCurrentLandTable();
}

void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos) {
	if (!DroppedFrames && anim % 4 == 0) {
		for (Int i = 0; i < size; ++i) {
			if (chunklist[i].Chunk != CurrentChunk) {
				auto entity = EntityData1Ptrs[0];
				if (entity != nullptr) {
					if (((chunklist[i].Position1.x == 0 || pos.x < chunklist[i].Position1.x)) &&
						((chunklist[i].Position1.y == 0 || pos.y < chunklist[i].Position1.y)) &&
						((chunklist[i].Position1.z == 0 || pos.z < chunklist[i].Position1.z)) &&
						((chunklist[i].Position2.x == 0 || pos.x > chunklist[i].Position2.x)) &&
						((chunklist[i].Position2.y == 0 || pos.y > chunklist[i].Position2.y)) &&
						((chunklist[i].Position2.z == 0 || pos.z > chunklist[i].Position2.z))) {

						LoadLevelFile(level, chunklist[i].Chunk);
						
						ChunkSwapped = true;
						break;
					}
				}
			}
		}
	}
}

void LevelHandler_Delete(ObjectMaster * a1) {
	FreeCurrentChunk(CurrentLevel, CurrentAct);
	delete oldinfo;
	oldinfo = nullptr;
	anim = 0;
}

//Set start positions and trial menu entries for every character
void SetStartLevelData(const HelperFunctions &helperFunctions, uint8_t character, int pos, int level, int act) {
	helperFunctions.RegisterStartPosition(character, Heroes_StartPositions[pos]);
	//helperFunctions.RegisterTrialLevel(character, { level, act });
	//SaveFile.LevelClear[(character * 43) + level] = 1;
}

void SetCharactersLevelData(const HelperFunctions &helperFunctions) {
	for (uint8_t character = 0; character < 8; ++character) {
		if (EnableSeasideHill) {
			SetStartLevelData(helperFunctions, character, 0, HeroesLevelID_SeasideHill, 0);
			SetStartLevelData(helperFunctions, character, 1, HeroesLevelID_SeasideHill, 1);
		}
		if (EnableOceanPalace) {
			SetStartLevelData(helperFunctions, character, 2, HeroesLevelID_OceanPalace, 0);
			SetStartLevelData(helperFunctions, character, 3, HeroesLevelID_OceanPalace, 1);
		}
		if (EnableGrandMetropolis) 
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_GrandMetropolis, 0);
		if (EnablePowerPlant)
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_PowerPlant, 0);
		if (EnableCasinoPark)
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_CasinoPark, 0);
		if (EnableBingoHighway)
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_BingoHighway, 0);
		if (EnableHangCastle)
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_HangCastle, 0);
		if (EnableMysticMansion)
			SetStartLevelData(helperFunctions, character, 4, HeroesLevelID_MysticMansion, 0);
	}

	helperFunctions.RegisterTrialLevel(Characters_Amy, { 1, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Amy, { 2, 0 });
}

//We prevent the skybox objects from loading in heroes levels
void __cdecl LoadSkyboxObject_r();
Trampoline LoadSkyboxObject_t(0x414420, 0x414469, LoadSkyboxObject_r);
void __cdecl LoadSkyboxObject_r()
{
	SetGlobalPoint2Col_Colors(LevelGlobalColors[CurrentLevel].x, LevelGlobalColors[CurrentLevel].y, LevelGlobalColors[CurrentLevel].z);
	
	if (IsHeroesLevel) {
		LevelDrawDistance.Maximum = -999999.0f;
		Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
	}
	else {
		if (SkyboxObjects[CurrentLevel])
		{
			LoadObject(LoadObj_Data1, 1, SkyboxObjects[CurrentLevel]);
		}
	}
}

//We force acts here
void __cdecl njReleaseTextureAll__r();
Trampoline njReleaseTextureAll__t(0x406F00, 0x406F63, njReleaseTextureAll__r);
void __cdecl njReleaseTextureAll__r()
{
	njReleaseTextureAll();
	QueueDrawingState = 2;
	sub_40D3B0();

	if (CurrentLevel) {
		if ((CurrentLevel == HeroesLevelID_SeasideHill && EnableSeasideHill) ||
			(CurrentLevel == HeroesLevelID_OceanPalace && EnableOceanPalace) ||
			(CurrentLevel == HeroesLevelID_GrandMetropolis && EnableGrandMetropolis) ||
			(CurrentLevel == HeroesLevelID_PowerPlant && EnablePowerPlant) ||
			(CurrentLevel == HeroesLevelID_CasinoPark && EnableCasinoPark) ||
			(CurrentLevel == HeroesLevelID_BingoHighway && EnableBingoHighway) ||
			(CurrentLevel == HeroesLevelID_HangCastle && EnableHangCastle) ||
			(CurrentLevel == HeroesLevelID_MysticMansion && EnableMysticMansion)) {

			IsHeroesLevel = true;

			uint8_t CurrentAct_dupe = 0;
			if (EnableSeaGate && CurrentLevel == HeroesLevelID_SeasideHill && (CurrentCharacter == Characters_Amy || CurrentCharacter == Characters_Gamma)) CurrentAct_dupe = 1;
			if (EnableRoadRock && CurrentLevel == HeroesLevelID_OceanPalace && (CurrentCharacter == Characters_Amy || CurrentCharacter == Characters_Gamma)) CurrentAct_dupe = 1;

			if (CurrentAct != CurrentAct_dupe) {
				UnloadLevelTextures(GetLevelAndAct());
				CurrentAct = CurrentAct_dupe;
				LoadLevelTextures(GetLevelAndAct());
			}
		}
		else {
			IsHeroesLevel = false;
		}
	}
	else {
		IsHeroesLevel = false;
	}
}

void Levels_Init(const char *path, const HelperFunctions &helperFunctions)
{
	modpath = std::string(path);

	//Get the config.ini information
	const IniFile *config = new IniFile(modpath + "\\config.ini");
	EnableSeasideHill = config->getBool("Levels", "EnableSeasideHill", true);
	EnableOceanPalace = config->getBool("Levels", "EnableOceanPalace", true);
	EnableGrandMetropolis = config->getBool("Levels", "EnableGrandMetropolis", true);
	EnablePowerPlant = config->getBool("Levels", "EnablePowerPlant", true);
	EnableCasinoPark = config->getBool("Levels", "EnableCasinoPark", true);
	EnableBingoHighway = config->getBool("Levels", "EnableBingoHighway", true);
	EnableHangCastle = config->getBool("Levels", "EnableHangCastle", true);
	EnableMysticMansion = config->getBool("Levels", "EnableHangCastle", true);
	NoPinball = config->getBool("General", "NoPinball", false);
	delete config;

	//Init sound effects
	if (EnableSounds >= 1) {
		ReplaceADX("invncibl", "invncibl");
		ReplaceADX("rndclear", "rndclear");
		ReplaceADX("speedup", "speedup");
		ReplaceDAT("check_sheet_bank02", "SH_CHECK");
		ReplaceDAT("COMMON_BANK00", "SH_COMMON", );
		ReplaceDAT("P_METALTAILS_BANK03", "SH_METALTAILS");
		ReplaceDAT("P_SONICTAILS_BANK03", "SH_SONICTAILS");
		ReplaceDAT("V_SONICTAILS_E_BANK06", "SH_V_SONICTAILS");
	}

	if (EnableSeasideHill) SeasideHill_Init(path, helperFunctions);
	if (EnableOceanPalace) OceanPalace_Init(path, helperFunctions);
	if (EnableGrandMetropolis) GrandMetropolis_Init(path, helperFunctions);
	if (EnablePowerPlant) PowerPlant_Init(path, helperFunctions);
	if (EnableCasinoPark) CasinoPark_Init(path, helperFunctions);
	if (EnableBingoHighway) BingoHighway_Init(path, helperFunctions);
	if (EnableHangCastle) HangCastle_Init(path, helperFunctions);
	if (EnableMysticMansion) MysticMansion_Init(path, helperFunctions);

	SetCharactersLevelData(helperFunctions);
}