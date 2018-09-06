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

std::string modpath;
LandTableInfo *info = nullptr;
LandTableInfo *oldinfo = nullptr;

#pragma region Chunks
void LoadLevelFile(const char *shortname, int chunknb) {
	std::string fullPath = modpath + "\\system\\levels\\" + shortname;
	std::string numtos = std::to_string(chunknb);
	if (chunknb < 10) fullPath += "0";
	fullPath += numtos + ".sa1lvl";
	const char *foo = fullPath.c_str();

	PrintDebug("[SHM] Swapping for ");
	PrintDebug(foo);
	PrintDebug("\n");

	FreeLandTable(1, 0);

	if (info) {
		if (oldinfo) {
			delete oldinfo;
			oldinfo = nullptr;

		}
		oldinfo = info;
		info = nullptr;
	}

	info = new LandTableInfo(foo);
	LandTable *land = info->getlandtable();
	for (Int j = 0; j < land->COLCount; ++j) {
		land->TexList = &BEACH01_TEXLIST;
		if (land->Col[j].Flags == 0x80000000) {
			for (Int k = 0; k < land->Col[j].Model->basicdxmodel->nbMat; ++k) {
				land->Col[j].Model->basicdxmodel->mats[k].attrflags |= NJD_FLAG_IGNORE_LIGHT;
			}
		}
	}
	land->Flags = 0xC;
	CurrentChunk = chunknb;
	WriteData((LandTable**)0x97DA28, land);
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
	FreeLandTable(1, 0);
	delete oldinfo;
	oldinfo = nullptr;
	anim = 0;
}

void __cdecl FreeLandTable_h(int level, int act)
{
	LandTable *v2; // esi
	int v3; // ebp
	int v4; // ebx
	NJS_OBJECT **v5; // edi
	GeoAnimData *v6; // ebx
	signed int v7; // edi
	__int16 v8; // ax
	__int16 v9; // ax
	__int16 original_a10; // ax
	__int16 original_a11; // ax
	__int16 original_a12; // ax
	GeoAnimData *original_a13; // esi

	v2 = GeoLists[act + 8 * level];
	if (v2)
	{
		v3 = v2->COLCount;
		if (v3 > 0)
		{
			v5 = &v2->Col->Model;
			v4 = v2->COLCount;
			do
			{
				if (((unsigned int)v5[2] & (ColFlags_Visible | ColFlags_UvManipulation)) == ColFlags_Visible)
				{
					FreeLandTableObject(*v5);
				}
				v5 += 9;
				--v4;
			} while (v4);
		}
		if (ClipLevel < 2)
		{
			v6 = v2->AnimData;
			v7 = 0;
			if (v2->AnimCount > 0)
			{
				do
				{
					FreeLandTableObject(v6->Model);
					++v7;
				} while (v7 < v2->AnimCount);
			}
		}
		v8 = v2->Flags;
		if (v8 & LandTableFlags_TextureList)
		{
			v9 = HIWORD(v2->Flags);
			if (v9 & 3)
			{
				original_a10 = v9 - 1;
				v2->Flags = original_a10;
				if (!(original_a10 & 3))
				{
					njReleaseTexture(v2->TexList);
					if (!IsLoaded) SetQueueDrawingState_BlankScreen();
					return;
				}
			}
		}
		else if (v8 & LandTableFlags_TextureName)
		{
			if (v2->TexName)
			{
				original_a11 = HIWORD(v2->Flags);
				if (original_a11 & 3)
				{
					original_a12 = original_a11 - 1;
					v2->Flags = original_a12;
					if (!(original_a12 & 3))
					{
						njReleaseTexture(v2->TexList);
						if (!IsLoaded) SetQueueDrawingState_BlankScreen();
						return;
					}
				}
			}
		}
		else if (v8 & LandTableFlags_Animation)
		{
			original_a13 = v2->AnimData;
			if (v3 > 0)
			{
				do
				{
					if (original_a13->TexList)
					{
						njReleaseTexture(original_a13->TexList);
					}
					--v3;
				} while (v3);
			}
		}
	}
	if (!IsLoaded) SetQueueDrawingState_BlankScreen();
}
#pragma endregion

void Levels_Init(const char *path, const HelperFunctions &helperFunctions)
{
	modpath = std::string(path);

	//Get the config.ini information
	const IniFile *config = new IniFile(modpath + "\\config.ini");
	EnableSeasideHill = config->getBool("Levels", "EnableSeasideHill", true);
	EnableSeaGate = config->getBool("Levels", "EnableSeaGate", true);
	EnableOceanPalace = config->getBool("Levels", "EnableOceanPalace", true);
	EnableRoadRock = config->getBool("Levels", "EnableRoadRock", true);
	EnableGrandMetropolis = config->getBool("Levels", "EnableGrandMetropolis", true);
	EnablePowerPlant = config->getBool("Levels", "EnablePowerPlant", true);
	EnableCasinoPark = config->getBool("Levels", "EnableCasinoPark", true);
	EnableBingoHighway = config->getBool("Levels", "EnableBingoHighway", true);
	EnableHangCastle = config->getBool("Levels", "EnableHangCastle", true);
	delete config;

	//black screen fix
	WriteJump((void*)0x0043A350, FreeLandTable_h);

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

	Objects_Init(path, helperFunctions);
}