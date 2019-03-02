#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "mystic-mansion-paths.h"
#include "mystic-mansion-deathzones.h"
#include "mystic-mansion.h"

void MysticMansionObjects_Init();

#pragma region Objects Data
SH_UVSHIFT MysticMansion_UVShift[]{
	{ nullptr, 0,{ 5, -5 } },
	{ nullptr, 0,{ -5, 5 } }
};

NJS_MODEL_SADX * MMMODELLIST[4];

extern ModelInfo * MM_SPHERE1;
extern ModelInfo * MM_SPHERE2;
extern ModelInfo * MM_SKELFAN;
extern ModelInfo * MM_MYSTCAR;
extern ModelInfo * MM_MOVPLAT;
extern ModelInfo * MM_MYSDOOR;
extern ModelInfo * MM_MYSWALL;
extern ModelInfo * HC_SPKWARP;
extern ModelInfo * HC_HFLAMES;
#pragma endregion

void MysticMansion_InitObjects() {
	MM_SKELFAN = LoadMDL("MM_SKELFAN");
	MM_SPHERE1 = LoadMDL("MM_SPHERE1");
	MM_SPHERE2 = LoadMDL("MM_SPHERE2");
	MM_MYSTCAR = LoadMDL("MM_MYSTCAR");
	MM_MOVPLAT = LoadMDL("MM_MOVPLAT");
	MM_MYSDOOR = LoadMDL("MM_MYSDOOR");
	MM_MYSWALL = LoadMDL("MM_MYSWALL");
	HC_SPKWARP = LoadMDL("HC_SPKWARP");
	HC_HFLAMES = LoadMDL("HC_HFLAMES");

	MMMODELLIST[0] = MM_SPHERE1->getmodel()->basicdxmodel;
	MMMODELLIST[1] = MM_SPHERE1->getmodel()->basicdxmodel;
	MMMODELLIST[2] = HC_HFLAMES->getmodel()->basicdxmodel;
	MMMODELLIST[3] = HC_HFLAMES->getmodel()->child->basicdxmodel;
	MysticMansion_UVShift[0].List = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[1].List = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].vertuv;
	MysticMansion_UVShift[0].Size = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
	MysticMansion_UVShift[1].Size = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
}

void MysticMansion_Delete(ObjectMaster * a1) {
	FreeMDL(MM_SPHERE1);
	FreeMDL(MM_SPHERE2);
	FreeMDL(MM_SKELFAN);
	FreeMDL(MM_MYSTCAR);
	FreeMDL(MM_MOVPLAT);
	FreeMDL(MM_MYSDOOR);
	FreeMDL(MM_MYSWALL);
	FreeMDL(HC_SPKWARP);
	FreeMDL(HC_HFLAMES);

	if (IsLantern) {
		set_shader_flags_ptr(ShaderFlags_Blend, false);
		set_blend_ptr(-1, -1);
	}

	LevelHandler_Delete(a1);
}

void MysticMansionHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		InitializeSoundManager();
		PlayMusic(MusicIDs_finaleg1);
		SoundManager_Delete2();

		a1->Data1->Action = 1;
		a1->DeleteSub = MysticMansion_Delete;

		if (CurrentAct == 0) {
			CurrentLevelTexlist = (TexList*)0x1B98518;
			CurrentLandAddress = (LandTable**)0x97DB48;
				
			MysticMansion_InitObjects();

			if (IsLantern) set_shader_flags_ptr(ShaderFlags_Blend, true);
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("MM", MysticMansionChunks, LengthOfArray(MysticMansionChunks), entity->Position);
			AnimateTextures(MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateObjectsTextures(MMMODELLIST, 4, MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateUV(MysticMansion_UVShift, LengthOfArray(MysticMansion_UVShift));

			chunkswapped = false;

			break;
		}
	}
}

void MysticMansion_Init(const char *path, const HelperFunctions &helperFunctions) {
	ReplacePVM("FINALEGG1", "mystic-mansion");
	ReplaceBIN("SET1000S", "mystic-mansion-set");
	ReplaceBIN("CAM1000S", "mystic-mansion-cam");
	ReplaceDAT("FINAL_EGG_BANK01", "MYSTIC_BANK");
	ReplaceADX("finaleg1", "mystic-mansion");
	ReplaceBIN("PL_A0B", "mystic-mansion-shaders");

	helperFunctions.RegisterPathList(MysticMansionPaths);

	for (uint8_t i = 0; i < 3; i++) {
		FogData_FinalEgg1[i].Toggle = false;
	}

	WriteData((DeathZone**)0x1A49218, MysticMansionDeathZones);

	WriteData((ObjectFuncPtr*)0x90BF60, &MysticMansionHandler);

	MysticMansionObjects_Init();
}