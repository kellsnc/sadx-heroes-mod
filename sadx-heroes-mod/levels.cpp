#include "stdafx.h"

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
static bool EnableEggFleet = true;
static bool EnableSpecialStages = true;

bool envsounds = true;
bool NoMysticMusic = false;
bool NoPinball = false;
bool EnableFog = true;
bool chunkswapped = false;
bool IsHeroesLevel = false;

LandTable** CurrentLandAddress;

LandTableInfo *info = nullptr;
LandTableInfo *oldinfo = nullptr;

VoidFunc(sub_40D3B0, 0x40D3B0);
FunctionPointer(void, DrawModelBlend_IsVisible, (NJS_MODEL_SADX *model, QueuedModelFlagsB blend, float radius_scale), 0x4094D0);
FunctionPointer(void, DrawSimpleModel_IsVisible, (NJS_MODEL_SADX *model, float scale), 0x407A00);

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
	{ HeroesLevelID_MysticMansion, 0,{ 0, 23, 777 }, 0xBFFF },
	{ HeroesLevelID_EggFleet, 0,{ 500, 4230, 5320 }, 0xBFFF },
	{ HeroesLevelID_SpecialStages, 0,{ 200, 0, 0 }, 0xBFFF }
};

//Chunk system
bool ForceWhiteDiffuse(NJS_MATERIAL* material, Uint32 flags)
{
	if (IsHeroesLevel && CurrentLevel == 1 && material->attr_texId == 41) set_shader_flags_ptr(ShaderFlags_Fog, false);
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
				NJS_MATERIAL* landmtl[1] = { &land->Col[j].Model->basicdxmodel->mats[k] };
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
	std::string numtos = std::to_string(chunknb);

	PrintDebug("[SHM] Loading "); PrintDebug(shortname); if (chunknb < 10) PrintDebug("0");
	PrintDebug(numtos.c_str()); PrintDebug("... ");
	
	std::string fullPath = "system\\levels\\";
	fullPath += shortname;
	if (chunknb < 10) fullPath += "0";
	fullPath += numtos + ".sa1lvl";
	const char *foo = fullPath.c_str();

	PrintDebug("Freeing chunk... ");

	FreeCurrentChunk(CurrentLevel, CurrentAct);

	if (info) {
		if (oldinfo) {
			delete oldinfo;
			oldinfo = nullptr;

		}
		oldinfo = info;
		info = nullptr;
	}

	info = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	PrintDebug("Done. Loaded '"); PrintDebug(foo); PrintDebug("'. Swapping landtable... ");

	CurrentChunk = chunknb;
	SwapCurrentLandTable();
	SetCurrentLandTable();

	PrintDebug("Done. \n");
}

void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos) {
	if (!DroppedFrames && anim % 4 == 0) {
		for (Int i = 0; i < size; ++i) {
			if (chunklist[i].Chunk != CurrentChunk) {
				EntityData1 *entity = EntityData1Ptrs[0];
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

	for (uint16_t i = 0; i < SETTable_Count; ++i) {
		if (CurrentSetFile[i].Properties.z == 1
			&& CurrentSetFile[i].Properties.x == 0
			&& CurrentSetFile[i].Properties.y == 0) {
			CurrentSetFile[i].Properties.z = 0;
		}
	}
}

//Animate textures of the current landtable in a similar way to Sonic Heroes
void AnimateTexlist(SH_ANIMTEXS *list, Int listcount, NJS_TEXLIST* texlist) {
	if (!DroppedFrames && texlist) {
		for (uint8_t count = 0; count < listcount; ++count) {
			SH_ANIMTEXS * clist = &list[count];
			int base = clist->texid;
			NJS_TEXNAME * texname = &texlist->textures[base];
			
			Int CurrentTex = base + clist->cache;

			if (anim % clist->duration[clist->cache] == 0) {
				if (CurrentTex == base + clist->count) {
					clist->cache = 0;
					texname->texaddr = clist->address;
				}
				else {
					if (clist->cache == 0) clist->address = texname->texaddr;
					texname->texaddr = texlist->textures[CurrentTex + 1].texaddr;
					clist->cache += 1;
				}
			}
		}
	}
}

void AnimateTextures(SH_ANIMTEXS *list, Int listcount) {
	AnimateTexlist(list, listcount, CurrentLevelTexlist);
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
			SetStartLevelData(helperFunctions, character, 5, HeroesLevelID_PowerPlant, 0);
		if (EnableCasinoPark)
			SetStartLevelData(helperFunctions, character, 6, HeroesLevelID_CasinoPark, 0);
		if (EnableBingoHighway)
			SetStartLevelData(helperFunctions, character, 7, HeroesLevelID_BingoHighway, 0);
		if (EnableHangCastle)
			SetStartLevelData(helperFunctions, character, 8, HeroesLevelID_HangCastle, 0);
		if (EnableMysticMansion)
			SetStartLevelData(helperFunctions, character, 9, HeroesLevelID_MysticMansion, 0);
	}

	helperFunctions.RegisterTrialLevel(Characters_Amy, { 1, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Amy, { 2, 0 });
}

//We prevent the skybox objects from loading in heroes levels
void HeroesSkybox_Main(ObjectMaster *a1)
{
	LevelDrawDistance.Maximum = -999999.0f;
	SkyboxDrawDistance.Maximum = -36000.0f;
	Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
}

//We force acts here
void __cdecl ForceAct()
{
	njReleaseTextureAll();
	QueueDrawingState = 2;
	sub_40D3B0();

	if (CurrentLevel == 5 && EnableMysticMansion) CurrentLevel = HeroesLevelID_MysticMansion;

	if (CurrentLevel) {
		if ((CurrentLevel == HeroesLevelID_SeasideHill && EnableSeasideHill) ||
			(CurrentLevel == HeroesLevelID_OceanPalace && EnableOceanPalace) ||
			(CurrentLevel == HeroesLevelID_GrandMetropolis && EnableGrandMetropolis) ||
			(CurrentLevel == HeroesLevelID_PowerPlant && EnablePowerPlant) ||
			(CurrentLevel == HeroesLevelID_CasinoPark && EnableCasinoPark) ||
			(CurrentLevel == HeroesLevelID_BingoHighway && EnableBingoHighway) ||
			(CurrentLevel == HeroesLevelID_HangCastle && EnableHangCastle) ||
			(CurrentLevel == HeroesLevelID_MysticMansion && EnableMysticMansion) ||
			(CurrentLevel == HeroesLevelID_EggFleet && EnableEggFleet) ||
			(CurrentLevel == HeroesLevelID_SpecialStages && EnableSpecialStages)) {

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

//Fog
void SetFog(FogData * fog) {
	if (EnableFog) {
		LevelFogData.Layer = fog->Layer;
		LevelFogData.Distance = fog->Distance;
		LevelFogData.Color = fog->Color;
		LevelFogData.Toggle = fog->Toggle;
		ToggleStageFog();
	}
}

void __cdecl DrawLandTableFog(NJS_MODEL_SADX *a1)
{
	if (a1->r < 900.0)
	{
		DrawModelBlend_IsVisible(a1, (QueuedModelFlagsB)0, 1.0);
	}
	else
	{
		if (IsHeroesLevel && a1->mats[0].attrflags & NJD_FLAG_IGNORE_LIGHT) {
			DisableFog();
			DrawSimpleModel_IsVisible(a1, 1.0);
			ToggleStageFog();
		}
		else {
			DrawSimpleModel_IsVisible(a1, 1.0);
		}
	}
}

//Initialize levels
void Levels_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config)
{
	EnableSeasideHill = config->getBool("1- Levels", "EnableSeasideHill", true);
	EnableOceanPalace = config->getBool("1- Levels", "EnableOceanPalace", true);
	EnableGrandMetropolis = config->getBool("1- Levels", "EnableGrandMetropolis", true);
	EnablePowerPlant = config->getBool("1- Levels", "EnablePowerPlant", true);
	EnableCasinoPark = config->getBool("1- Levels", "EnableCasinoPark", true);
	EnableBingoHighway = config->getBool("1- Levels", "EnableBingoHighway", true);
	EnableHangCastle = config->getBool("1- Levels", "EnableHangCastle", true);
	EnableMysticMansion = config->getBool("1- Levels", "EnableMysticMansion", true);
	EnableEggFleet = config->getBool("1- Levels", "EnableEggFleet", true);
	EnableSpecialStages = config->getBool("1- Levels", "EnableSpecialStages", true);
	NoMysticMusic = config->getBool("0- General", "NoMysticMusic", false);
	NoPinball = config->getBool("0- General", "NoPinball", false);
	EnableFog = config->getBool("0- General", "EnableFog", true);
	envsounds = config->getBool("4- Sounds", "envsounds", true);
	
	WriteJump((void*)0x406F00, ForceAct); //njReleaseTextureAll_
	WriteJump((void*)0x40A140, DrawLandTableFog); //DrawLandTableObject_SimpleModel

	if (EnableSeasideHill) SeasideHill_Init(path, helperFunctions);
	if (EnableOceanPalace) OceanPalace_Init(path, helperFunctions);
	if (EnableGrandMetropolis) GrandMetropolis_Init(path, helperFunctions);
	if (EnablePowerPlant) PowerPlant_Init(path, helperFunctions);
	if (EnableCasinoPark) CasinoPark_Init(path, helperFunctions);
	if (EnableBingoHighway) BingoHighway_Init(path, helperFunctions);
	if (EnableHangCastle) HangCastle_Init(path, helperFunctions);
	if (EnableMysticMansion) MysticMansion_Init(path, helperFunctions);
	if (EnableEggFleet) EggFleet_Init(path, helperFunctions);
	if (EnableSpecialStages) SpecialStages_Init(path, helperFunctions);

	SetCharactersLevelData(helperFunctions);
}