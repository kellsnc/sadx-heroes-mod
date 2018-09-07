#include "stdafx.h"
#include "mod.h"
#include "IniFile.hpp"
#include "LandTableInfo.h"

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

LandTable** CurrentLandAddress;

std::string modpath;
LandTableInfo *info = nullptr;
LandTableInfo *oldinfo = nullptr;

#pragma region Chunks
void __cdecl FreeCurrentChunk(int level, int act)
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
		land->TexList = &BEACH01_TEXLIST;
		if (land->Col[j].Flags == 0x80000000) {
			for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
				land->Col[j].Model->basicdxmodel->mats[k].attrflags |= NJD_FLAG_IGNORE_LIGHT;
			}
		}
	}

	WriteData((LandTable**)CurrentLandAddress, land);
	land->TexList = CurrentLevelTexlist;
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
			if (chunklist[i].chunkid != CurrentChunk) {
				auto entity = EntityData1Ptrs[0];
				if (entity != nullptr) {
					if (((chunklist[i].chunkpos1.x == 0 || pos.x < chunklist[i].chunkpos1.x)) &&
						((chunklist[i].chunkpos1.y == 0 || pos.y < chunklist[i].chunkpos1.y)) &&
						((chunklist[i].chunkpos1.z == 0 || pos.z < chunklist[i].chunkpos1.z)) &&
						((chunklist[i].chunkpos2.x == 0 || pos.x > chunklist[i].chunkpos2.x)) &&
						((chunklist[i].chunkpos2.y == 0 || pos.y > chunklist[i].chunkpos2.y)) &&
						((chunklist[i].chunkpos2.z == 0 || pos.z > chunklist[i].chunkpos2.z))) {

						LoadLevelFile(level, chunklist[i].chunkid);
						
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
#pragma endregion

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

	Objects_Init(path, helperFunctions);
}