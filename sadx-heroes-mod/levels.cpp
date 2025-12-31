#include "stdafx.h"
#include "lanternapi.h"
#include "mod.h"
#include "enemies.h"
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
static bool EnableEggFleet = true;
static bool EnableSpecialStages = true;

bool EnableFog = true;
bool IsHeroesLevel = false;
bool NoMysticMusic = false;
bool NoPinball = false;

LandTable** CurrentLandAddress = nullptr;

LandTable* pHeroesLandTable = nullptr;
std::vector<LandTableInfo*> landInfos;
int last_chunk[8];
static int32_t(*viewport_get_num)() = nullptr;

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

bool IsCurrentHeroesLevel() {
	return IsHeroesLevel;
}

bool IsNoPinballEnabled() {
	return NoPinball;
}

bool IsNoMysticMusicEnabled() {
	return NoMysticMusic;
}

//Chunk system
bool ForceWhiteDiffuse(NJS_MATERIAL* material, Uint32 flags)
{
	if (IsHeroesLevel && CurrentLevel == 1 && material->attr_texId == 41) set_shader_flags_ptr(ShaderFlags_Fog, false);
	set_diffuse_ptr(1, false);
	set_specular_ptr(7, false);
	return true;
}

void LoadLevelFile(const char* shortname, int chunknb) {
	std::string fullPath = "system\\levels\\";
	fullPath += shortname;
	if (chunknb < 10) fullPath += "0";
	fullPath += std::to_string(chunknb) + ".sa1lvl";

	LandTableInfo* info = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));
	if (info && info->getlandtable())
	{
		LandTable* land = info->getlandtable();

		for (int j = 0; j < land->COLCount; ++j)
		{
			if (land->Col[j].Flags & ColFlags_Visible)
			{
				for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
					NJS_MATERIAL* landmtl[1] = { &land->Col[j].Model->basicdxmodel->mats[k] };
					landmtl[0]->diffuse.color = 0xFFFFFFFF;
					if (IsLantern) material_register_ptr(landmtl, LengthOfArray(landmtl), &ForceWhiteDiffuse);
				}
				InitLandTableObject(land->Col[j].Model);
			}
		}

		pHeroesLandTable = land;
	}

	landInfos.push_back(info);
}

void ChunkManagerDisp(ObjectMaster* obj)
{
	if (viewport_get_num)
	{
		int pnum = viewport_get_num();

		auto data = obj->Data1;

		auto chunklist = (CHUNK_LIST*)data->LoopData;
		auto size = data->InvulnerableTime;

		NJS_VECTOR* pos = &Camera_Data1->Position;

		for (Int i = 0; i < size; ++i) {
			if (chunklist[i].Chunk != last_chunk[pnum]) {
				EntityData1* entity = EntityData1Ptrs[0];
				if (entity != nullptr) {
					if (((chunklist[i].Position1.x == 0 || pos->x < chunklist[i].Position1.x)) &&
						((chunklist[i].Position1.y == 0 || pos->y < chunklist[i].Position1.y)) &&
						((chunklist[i].Position1.z == 0 || pos->z < chunklist[i].Position1.z)) &&
						((chunklist[i].Position2.x == 0 || pos->x > chunklist[i].Position2.x)) &&
						((chunklist[i].Position2.y == 0 || pos->y > chunklist[i].Position2.y)) &&
						((chunklist[i].Position2.z == 0 || pos->z > chunklist[i].Position2.z))) {
						last_chunk[pnum] = chunklist[i].Chunk;
						break;
					}
				}
			}
		}

		*(int*)0x3B36D48 = (1 << last_chunk[pnum]);
	}
}

void ChunkManagerExec(ObjectMaster* obj)
{
	auto data = obj->Data1;
	
	auto chunklist = (CHUNK_LIST*)data->LoopData;
	auto size = data->InvulnerableTime;

	NJS_VECTOR* pos = &Camera_Data1->Position;

	for (int i = 0; i < size; ++i) {
		if (chunklist[i].Chunk != CurrentChunk) {
			EntityData1* entity = EntityData1Ptrs[0];
			if (entity != nullptr) {
				if (((chunklist[i].Position1.x == 0 || pos->x < chunklist[i].Position1.x)) &&
					((chunklist[i].Position1.y == 0 || pos->y < chunklist[i].Position1.y)) &&
					((chunklist[i].Position1.z == 0 || pos->z < chunklist[i].Position1.z)) &&
					((chunklist[i].Position2.x == 0 || pos->x > chunklist[i].Position2.x)) &&
					((chunklist[i].Position2.y == 0 || pos->y > chunklist[i].Position2.y)) &&
					((chunklist[i].Position2.z == 0 || pos->z > chunklist[i].Position2.z))) {
					CurrentChunk = chunklist[i].Chunk;
					break;
				}
			}
		}
	}

	*(int*)0x3B36D48 = (1 << CurrentChunk);
	CurrentLandTable = pHeroesLandTable;
}

// In the old chunk system, each chunk was a separate landtable. That was ineficient and not compatible with multiplayer.
// This merges every chunk into a single landtable, the chunk ID is preserved as a visibility flag ("blockbit" system)
void LoadChunkManager(const char* level, CHUNK_LIST* chunklist, int size)
{
	if (!pHeroesLandTable)
	{
		std::vector<COL*> visible;
		std::vector<COL*> colli;
		std::vector<int> prev_ids;

		for (int i = 0; i < size; ++i)
		{
			int id = chunklist[i].Chunk;

			if (std::find(prev_ids.begin(), prev_ids.end(), id) != prev_ids.end())
			{
				continue;
			}

			prev_ids.push_back(id);

			std::string fullPath = "system\\levels\\";
			fullPath += level;
			if (id < 10) fullPath += "0";
			fullPath += std::to_string(id) + ".sa1lvl";

			LandTableInfo* info = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));
			if (info && info->getlandtable())
			{
				LandTable* land = info->getlandtable();

				for (int j = 0; j < land->COLCount; ++j)
				{
					if (land->Col[j].Flags & ColFlags_Visible)
					{
						for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
							NJS_MATERIAL* landmtl[1] = { &land->Col[j].Model->basicdxmodel->mats[k] };
							landmtl[0]->diffuse.color = 0xFFFFFFFF;
							if (IsLantern) material_register_ptr(landmtl, LengthOfArray(landmtl), &ForceWhiteDiffuse);
						}
						land->Col[j].anonymous_6 = (1 << id);
						visible.push_back(&land->Col[j]);
						InitLandTableObject(land->Col[j].Model);
					}
					else
					{
						colli.push_back(&land->Col[j]);
					}
				}
			}
			landInfos.push_back(info);
		}

		int count = colli.size() + visible.size();

		if (count == 0)
		{
			return;
		}

		COL* cols = (COL*)calloc(count, sizeof(COL));
		int cc = 0;

		for (auto& col : visible)
		{
			cols[cc] = *col;
			++cc;
		}

		for (auto& col : colli)
		{
			cols[cc] = *col;
			++cc;
		}

		pHeroesLandTable = (LandTable*)calloc(1, sizeof(LandTable));
		pHeroesLandTable->Flags = 0xC;
		pHeroesLandTable->TexList = CurrentLevelTexlist;
		pHeroesLandTable->TexName = "seaside-hill";
		pHeroesLandTable->COLCount = count;
		pHeroesLandTable->Col = cols;
	}

	ObjectMaster* obj = LoadObject(LoadObj_Data1, 1, ChunkManagerExec);
	if (obj)
	{
		obj->DisplaySub = ChunkManagerDisp;
		obj->Data1->LoopData = (Loop*)chunklist;
		obj->Data1->InvulnerableTime = size;
	}

	CurrentLandTable = pHeroesLandTable;
}

void LevelHandler_Delete(ObjectMaster * a1) {
	CurrentChunk = 0;
	anim = 0;

	DeleteCustomEnemies();

	if (pHeroesLandTable)
	{
		for (int i = 0; i < pHeroesLandTable->COLCount; ++i)
		{
			FreeLandTableObject(pHeroesLandTable->Col[i].Model);
		}
		free(pHeroesLandTable->Col);
		free(pHeroesLandTable);
		pHeroesLandTable = NULL;
		landInfos.clear();
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
	for (uint8_t character = 0; character < MaxPlayers; ++character) {
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
		if (EnableEggFleet)
			SetStartLevelData(helperFunctions, character, 10, HeroesLevelID_EggFleet, 0);
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

	IsHeroesLevel = false;

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
	}

	Enemies_CheckEnemiesSwap();
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
		if (IsHeroesLevel == true && a1->mats[0].attrflags & NJD_FLAG_IGNORE_LIGHT) {
			DisableFog();
			DrawSimpleModel_IsVisible(a1, 1.0);
			ToggleStageFog();
		}
		else {
			DrawSimpleModel_IsVisible(a1, 1.0);
		}
	}
}

//Default light for stages
void DefaultLight(HeroesLevelIDs levelid) {
	for (int i = 0; i < StageLightList_Length; ++i) {
		if (StageLightList[i].level == levelid)
			StageLightList[i] = *GetStageLight(LevelIDs_Casinopolis, 0, 0);
	}
}

//Initialize levels
void Levels_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config)
{
	HMODULE hmod_multi = GetModuleHandle(L"sadx-multiplayer");
	if (hmod_multi != INVALID_HANDLE_VALUE)
	{
		viewport_get_num = (decltype(viewport_get_num))GetProcAddress(hmod_multi, "viewport_get_num");
	}

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