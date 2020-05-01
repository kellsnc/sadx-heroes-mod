#include "stdafx.h"

HeroesLevelData* HeroesLevelList[] {
	&SeasideHillData,
	&SeaGateData,
	&OceanPalaceData,
	&RoadRockData,
	&GrandMetropolisData,
	&PowerPlantData,
	&CasinoParkData,
	&BingoHighwayData,
	&HangCastleData,
	&MysticMansionData,
	&EggFleetData,
	&SpecialStagesData
};

static bool EnabledLevels[42] = { false };

static bool EnableFog		= true;
static bool NoMysticMusic	= false;
static bool NoPinball		= false;

static NJS_TEXNAME HeroesTexnames[64];
static NJS_TEXLIST HeroesTexlist = { arrayptrandlength(HeroesTexnames) };

static std::vector<LandTableInfo*> CurrentChunks;
static LandTable HeroesLandTable = { 0, 0, 8, 0, nullptr, nullptr, NULL, &HeroesTexlist, 0, 0 };
uint8_t CurrentChunk = 0;

static int CurrentMusic = 0;

// global variables return functions
bool IsCurrentHeroesLevel() {
	return EnabledLevels[CurrentLevel];
}

bool IsNoPinballEnabled() {
	return NoPinball;
}

bool IsNoMysticMusicEnabled() {
	return NoMysticMusic;
}

void SetHeroesLeveltex() {
	njSetTexture(&HeroesTexlist);
}

/*** Level Load System ***/

void HeroesLevelDestructor(int heap);
Trampoline RunLevelDestructor_t((int)RunLevelDestructor, (int)RunLevelDestructor + 0x6, HeroesLevelDestructor);
void HeroesLevelDestructor(int heap) {
	if (CurrentChunks.size() > 0) {
		DeleteCustomEnemies();

		for (uint16_t i = 0; i < SETTable_Count; ++i) {
			if (CurrentSetFile[i].Properties.z == 1
				&& CurrentSetFile[i].Properties.x == 0
				&& CurrentSetFile[i].Properties.y == 0) {
				CurrentSetFile[i].Properties.z = 0;
			}
		}

		for (Uint8 c = 0; c < CurrentChunks.size(); ++c) {
			delete CurrentChunks[c];
		}

		CurrentChunks.clear();
		CurrentChunk = 0;

		delete[] HeroesLandTable.Col;
		HeroesLandTable.Col = nullptr;
	}

	FunctionPointer(void, RunLevelDestructor_orig, (int heap), RunLevelDestructor_t.Target());
	RunLevelDestructor_orig(heap);
}

bool ForceWhiteDiffuse(NJS_MATERIAL* material, Uint32 flags)
{
	if (IsCurrentHeroesLevel() && CurrentLevel == 1 && material->attr_texId == 41) set_shader_flags_ptr(ShaderFlags_Fog, false);
	set_diffuse_ptr(1, false);
	set_specular_ptr(7, false);
	return true;
}

LandTable* LoadLevelGeometry(std::string name, HeroesLevelIDs levelid, Uint8 act, Uint8 ChunkMax) {
	std::vector<COL*> cols;
	int colcount = 0;
	int geocount = 0;

	// Load every chunk
	for (Uint8 i = 1; i < ChunkMax + 1; ++i) {
		std::string num;

		if (i < 10) num = "0" + std::to_string(i);
		else num = std::to_string(i);

		std::string fullPath = "system\\levels\\" + name + num + ".sa1lvl";
		LandTableInfo* info = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

		LandTable* land = info->getlandtable();
		if (land) {
			CurrentChunks.push_back(info);
			colcount += land->COLCount;
			
			for (int j = 0; j < land->COLCount; ++j) {
				if (land->Col[j].Flags & ColFlags_Visible) {
					geocount++;
					land->Col[j].Padding[0] = i;

					for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
						NJS_MATERIAL* landmtl[1] = { &land->Col[j].Model->basicdxmodel->mats[k] };
						if (IsLantern) material_register_ptr(landmtl, 1, &ForceWhiteDiffuse);
					}
				}

				cols.push_back(&land->Col[j]);
			}
		}
	}

	// Merge everything into one landtable
	COL* newcol = new COL[colcount];

	for (int i = 0; i < colcount; ++i) {
		newcol[i] = *cols[i];
	}

	HeroesLandTable.COLCount = colcount;
	HeroesLandTable.Col = newcol;
	GeoLists[act + 8 * levelid] = &HeroesLandTable;
	InitLandTable(levelid, act);

	return &HeroesLandTable;
}

void LoadHeroesSetFiles(std::string name, Uint8 act) {
	ReleaseCamFile();
	ReleaseSetFile();
	
	std::string file = name + "-set";

	switch (CurrentCharacter) {
	case Characters_Tails:
		file += "-tails";
		break;
	case Characters_Knuckles:
		file += "-knucks";
		break;
	case Characters_Amy:
		file += "-amy";
		break;
	case Characters_Big:
		file += "-big";
		break;
	case Characters_Gamma:
		file += "-gamma";
		break;
	}

	file += ".bin";

	LoadFileWithMalloc(file.c_str(), (LPVOID*)&SetFiles[act]);

	if (!SetFiles[act]) {
		file = name + "-set.bin";
		LoadFileWithMalloc(file.c_str(), (LPVOID*)&SetFiles[act]);
	}

	file = name + "-cam.bin";
	LoadFileWithMalloc(file.c_str(), (LPVOID*)&CamData[act]);

	if (!CamData[act]) {
		file = "heroes-cam.bin";
		LoadFileWithMalloc(file.c_str(), (LPVOID*)&CamData[act]);
	}

	SetCurrentCamData(CurrentLevel);
	SetCurrentSetData(CurrentLevel);
}

void __cdecl LoadHeroesLevelFiles();
Trampoline LoadLevel_t(0x415210, 0x415216, LoadHeroesLevelFiles);
void __cdecl LoadHeroesLevelFiles() {
	VoidFunc(original, LoadLevel_t.Target());
	original();
	
	if (IsCurrentHeroesLevel()) {
		for (uint8_t i = 0; i < LengthOfArray(HeroesLevelList); ++i) {
			HeroesLevelData* level = HeroesLevelList[i];

			if (CurrentLevel == level->LevelID && CurrentAct == level->Act) {
				PrintDebug("[SHM] Loading %s files... ", level->name.c_str());

				if (level->ChunkAmount) {
					CurrentLandTable = LoadLevelGeometry(level->shortname, level->LevelID, level->Act, level->ChunkAmount);
					CurrentLandTable->TexName = level->name.c_str();
				}
				
				level->loadfunc();

				LoadHeroesSetFiles(level->name, level->Act);

				CurrentMusic = level->musicid;

				PrintDebug("Done. \n");
				return;
			}
		}
	}
}

/*** Chunk System ***/

void SwapChunk(Uint8 chunk) {
	for (int j = 0; j < CurrentLandTable->COLCount; ++j) {
		COL* col = &CurrentLandTable->Col[j];
		if (col->Padding[0] == 0) continue;

		if (col->Padding[0] == chunk) {
			col->Flags |= ColFlags_Visible;
		}
		else {
			col->Flags &= ~ColFlags_Visible;
		}
	}

	CurrentChunk = chunk;
	ChunkSwapped = true;
}

void ChunkHandler(const char * level, CHUNK_LIST * chunklist, uint8_t size, NJS_VECTOR pos) {
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

					SwapChunk(chunklist[i].Chunk);

					break;
				}
			}
		}
	}
}

// Load a single chunk
void LoadLevelFile(std::string name, std::string texname) {
	std::string fullPath = "system\\levels\\" + name + ".sa1lvl";
	LandTableInfo* info = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	CurrentChunks.push_back(info);

	LandTable* land = info->getlandtable();
	for (Int j = 0; j < land->COLCount; ++j) {
		if (land->Col[j].Flags == 0x80000000) {
			for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
				NJS_MATERIAL* landmtl[1] = { &land->Col[j].Model->basicdxmodel->mats[k] };
				if (IsLantern) material_register_ptr(landmtl, LengthOfArray(landmtl), &ForceWhiteDiffuse);
			}
		}
	}

	LoadPVM(texname.c_str(), &HeroesTexlist);
	land->TexList = &HeroesTexlist;
	land->AnimCount = 0;
	
	GeoLists[CurrentAct + 8 * CurrentLevel] = land;
	SetCurrentLandTable();
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
	AnimateTexlist(list, listcount, &HeroesTexlist);
}

//We prevent the skybox objects from loading in heroes levels
void HeroesSkybox_Main(ObjectMaster *obj) {
	if (obj->Data1->Action == 0) {
		LevelDrawDistance.Maximum = -999999.0f;
		SkyboxDrawDistance.Maximum = -36000.0f;
		Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
		obj->Data1->Action = 1;

		InitializeSoundManager();
		PlayMusic((MusicIDs)CurrentMusic);
	}
}

int CountRings() {
	int rings = 0;
	int itemrings = 0;
	int enemies = 0;
	int falcons = 0;
	int lives = 0;

	if (SETTable && SETTable_Count > 0) {
		for (int count = 0; count < SETTable_Count; ++count) {
			SETEntry* setentry = &CurrentSetFile[count];
			if (!setentry) continue;

			ObjectFuncPtr current = CurrentObjectList->List[setentry->ObjectType].LoadSub;

			if (current == Ring_Main) {
				rings += 1;
			}

			if (current == RingGroup_Main) {
				rings += setentry->Properties.x + 1;
			}

			if (current == ItemBox_Main || current == ItemBoxAir_Main) {
				switch ((int)setentry->Properties.x)
				{
				case 2:
					itemrings += 5;
					break;
				case 3:
					itemrings += 10;
					break;
				case 4:
					itemrings += 40;
					break;
				case 6:
					lives += 1;
				}
			}

			if (current == Kiki_Load || current == RhinoTank_Main || current == Sweep_Load
				|| current == SpinnerA_Main || current == SpinnerB_Main || current == SpinnerC_Main
				|| current == UnidusA_Main || current == UnidusB_Main || current == UnidusC_Main
				|| current == Leon_Load || current == BoaBoa_Main || current == ESman
				|| current == e2000_Init) {
				enemies += 1;
			}

			if (current == Flyer_Init) {
				falcons += 1;
			}
		}
	}

	lives;
	enemies;
	falcons;
	itemrings += rings;
	return rings;
}

void __cdecl LoadSETObjs_r() {
	if (*(bool*)0x3C52464 == 0) {
		CountRings();
	}
	
	if (!EntityData1Ptrs[1] || EntityData1Ptrs[1]->CharID == Characters_Tails)
	{
		LoadSETObjs_NoP2OrDebugOrP2Tails();
		*(bool*)0x3C52464 = 1;
	}
	else
	{
		LoadSETObjs_P2NotTailsAndNotDebug();
		*(bool*)0x3C52464 = 1;
	}
}

//We force acts here
void __cdecl ForceAct()
{
	njReleaseTextureAll();
	QueueDrawingState = 2;
	sub_40D3B0();

	if (IsCurrentHeroesLevel() && (CurrentCharacter == Characters_Amy || CurrentCharacter == Characters_Gamma || CurrentCharacter == Characters_Big)) {
		if (CurrentLevel == 1 || CurrentLevel == 2) {
			UnloadLevelTextures(GetLevelAndAct());
			CurrentAct = 1;
			LoadLevelTextures(GetLevelAndAct());
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
		if (IsCurrentHeroesLevel() == true && a1->mats[0].attrflags & NJD_FLAG_IGNORE_LIGHT) {
			DisableFog();
			DrawSimpleModel_IsVisible(a1, 1.0);
			ToggleStageFog();
		}
		else {
			DrawSimpleModel_IsVisible(a1, 1.0);
		}
	}
}

/*** Initialize levels ***/

inline int8_t GetLevelListID(HeroesLevelIDs id, Uint8 act) {
	for (Uint8 i = 0; i < LengthOfArray(HeroesLevelList); ++i) {
		if (HeroesLevelList[i]->LevelID == id && HeroesLevelList[i]->Act == act) {
			return i;
		}
	}

	return -1;
}

int SetLevelMusic(const char* name, const HelperFunctions& helperFunctions) {
	MusicInfo MusicFile = { name, 1 };
	return helperFunctions.RegisterMusicFile(MusicFile);
}

void DefaultHeroesLight(char level, char act) {
	for (int i = 0; i < StageLightList_Length; ++i) {
		if (StageLightList[i].level == level && StageLightList[i].act == act) {
			StageLightList[i] = *GetStageLight(LevelIDs_Casinopolis, 0, 0);
			return;
		}
	}
}

void AddTrialEntry(unsigned char character, char level, char act, const HelperFunctions& helperFunctions) {
	for (uint8_t i = 0; i < TrialLevels[character].Count; ++i) {
		if (TrialLevels[character].Levels[i].Level == level && TrialLevels[character].Levels[i].Act == act) {
			return;
		}
	}

	helperFunctions.RegisterTrialLevel(character, { level, act });
}

void SetStartPositions(Uint8 id, const HelperFunctions& helperFunctions) {
	HeroesLevelData* level = HeroesLevelList[id];
	int levelid = level->LevelID;
	int levelact = level->Act;

	StartPosition start = { levelid, levelact, level->startpos, 0xBFFF };

	for (unsigned char i = 0; i < 8; ++i) {
		helperFunctions.RegisterStartPosition(i, start);
		DefaultHeroesLight(levelid, levelact);
		AddTrialEntry(i, levelid, levelact, helperFunctions);
	}
}

const char* RegisterLevelShader(int32_t level, int32_t act) {
	HeroesLevelData* leveldata = HeroesLevelList[GetLevelListID((HeroesLevelIDs)level, act)];

	if (leveldata >= 0) {
		std::string path = modpath + "\\system\\" + leveldata->name + "-shaders.bin";
		return path.c_str();
	}

	return nullptr;
}

const char* RegisterLevelLight(int32_t level, int32_t act) {
	HeroesLevelData* leveldata = HeroesLevelList[GetLevelListID((HeroesLevelIDs)level, act)];

	if (leveldata >= 0) {
		std::string path = modpath + "\\system\\heroes-light.bin";
		return path.c_str();
	}
	
	return nullptr;
}

inline void InitLevelListData(Uint8 id, const HelperFunctions& helperFunctions) {
	HeroesLevelList[id]->initfunc(helperFunctions);
	HeroesLevelList[id]->musicid = SetLevelMusic(HeroesLevelList[id]->name.c_str(), helperFunctions);
	SetStartPositions(id, helperFunctions);

	if (IsLantern == true) {
		pl_load_register_ptr(RegisterLevelShader);
		//sl_load_register_ptr(RegisterLevelLight);
	}
}

inline void InitLevelData(HeroesLevelIDs id, Uint8 act, const HelperFunctions& helperFunctions) {
	EnabledLevels[id] = true;
	InitLevelListData(GetLevelListID(id, act), helperFunctions);
}

void Levels_Init(const char *path, const HelperFunctions &helperFunctions, const IniFile *config) {
	if (config->getBool("Levels", "EnableSeasideHill", true)) { InitLevelData(HeroesLevelID_SeasideHill, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableSeaGate", true)) { InitLevelData(HeroesLevelID_SeasideHill, 1, helperFunctions); }
	if (config->getBool("Levels", "EnableOceanPalace", true)) { InitLevelData(HeroesLevelID_OceanPalace, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableRoadRock", true)) { InitLevelData(HeroesLevelID_OceanPalace, 1, helperFunctions); }
	if (config->getBool("Levels", "EnableGrandMetropolis", true)) { InitLevelData(HeroesLevelID_GrandMetropolis, 0, helperFunctions); }
	if (config->getBool("Levels", "EnablePowerPlant", true)) { InitLevelData(HeroesLevelID_PowerPlant, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableCasinoPark", true)) { InitLevelData(HeroesLevelID_CasinoPark, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableBingoHighway", true)) { InitLevelData(HeroesLevelID_BingoHighway, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableHangCastle", true)) { InitLevelData(HeroesLevelID_HangCastle, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableMysticMansion", true)) { InitLevelData(HeroesLevelID_MysticMansion, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableEggFleet", true)) { InitLevelData(HeroesLevelID_EggFleet, 0, helperFunctions); }
	if (config->getBool("Levels", "EnableSpecialStages", true)) { InitLevelData(HeroesLevelID_SpecialStages, 0, helperFunctions); }

	NoMysticMusic = config->getBool("0- General", "NoMysticMusic", false);
	NoPinball = config->getBool("0- General", "NoPinball", false);
	EnableFog = config->getBool("0- General", "EnableFog", true);
	
	WriteJump((void*)0x406F00, ForceAct); //njReleaseTextureAll_
	WriteJump((void*)0x40A140, DrawLandTableFog); //DrawLandTableObject_SimpleModel
	//WriteJump(LoadSETObjs, LoadSETObjs_r);
}