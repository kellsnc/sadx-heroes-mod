#include "stdafx.h"
#include "mod.h"
#include "levels.h"
#include "mystic-mansion-paths.h"
#include "mystic-mansion-deathzones.h"
#include "mystic-mansion.h"

ObjectFunc(FinalEgg_SkyBox_Load, 0x5ADFE0);

void MysticMansionObjects_Init();

SH_UVSHIFT MysticMansion_UVShift[]{
	{ nullptr, 0,{ 5, -5 } },
	{ nullptr, 0,{ -5, 5 } }
};

NJS_MODEL_SADX * MMMODELLIST[2];

extern ModelInfo * MM_SPHERE1;
extern ModelInfo * MM_SPHERE2;
extern ModelInfo * MM_SKELFAN;
extern ModelInfo * MM_MYSTCAR;
extern ModelInfo * MM_MOVPLAT;
extern ModelInfo * MM_MYSDOOR;
extern ModelInfo * MM_MYSWALL;

extern SOI_LISTS hang_castle_objects[];

void MysticMansion_Delete(ObjectMaster * a1) {
	FreeMDL(MM_SPHERE1);
	FreeMDL(MM_SPHERE2);
	FreeMDL(MM_SKELFAN);
	FreeMDL(MM_MYSTCAR);
	FreeMDL(MM_MOVPLAT);
	FreeMDL(MM_MYSDOOR);
	FreeMDL(MM_MYSWALL);

	set_shader_flags(ShaderFlags_Blend, false);
	set_blend(-1, -1);
	LevelHandler_Delete(a1);
}

void MysticMansionHandler(ObjectMaster * a1) {
	auto entity = EntityData1Ptrs[0];
	CharObj2 * co2 = GetCharObj2(0);

	if (a1->Data1->Action == 0) {
		if (CurrentAct != 0) {
			CurrentLevelObject = LoadObject(LoadObj_Data1, 0, Obj_FinalEgg);
			LoadObject(LoadObj_Data1, 1, FinalEgg_SkyBox_Load);

			LevelObjTexlists[0] = &OBJ_FINALEGG_TEXLIST;
			LoadPVM("OBJ_FINALEGG", &OBJ_FINALEGG_TEXLIST);

			DeleteObjectMaster(a1);
		}
		else {
			MovePlayerToStartPoint(entity);
			camerahax_b();

			InitializeSoundManager();
			PlayMusic(MusicIDs_finaleg1);
			SoundManager_Delete2();

			LevelDrawDistance.Maximum = -999999.0f;
			Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);

			a1->Data1->Action = 1;
			a1->DeleteSub = MysticMansion_Delete;

			if (CurrentAct == 0) {
				CurrentLevelTexlist = (TexList*)0x1B98518;
				CurrentLandAddress = (LandTable**)0x97DB48;
				
				MM_SKELFAN = LoadMDL("MM_SKELFAN");
				MM_SPHERE1 = LoadMDL("MM_SPHERE1");
				MM_SPHERE2 = LoadMDL("MM_SPHERE2");
				MM_MYSTCAR = LoadMDL("MM_MYSTCAR");
				MM_MOVPLAT = LoadMDL("MM_MOVPLAT");
				MM_MYSDOOR = LoadMDL("MM_MYSDOOR");
				MM_MYSWALL = LoadMDL("MM_MYSWALL");

				MMMODELLIST[0] = MM_SPHERE1->getmodel()->basicdxmodel;
				MMMODELLIST[1] = MM_SPHERE1->getmodel()->basicdxmodel;
				MysticMansion_UVShift[0].uvlist = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].vertuv;
				MysticMansion_UVShift[1].uvlist = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].vertuv;
				MysticMansion_UVShift[0].uvsize = MM_SPHERE1->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;
				MysticMansion_UVShift[1].uvsize = MM_SPHERE2->getmodel()->basicdxmodel->meshsets[0].nbMesh * 3;

				set_shader_flags(ShaderFlags_Blend, true);

				ObjectMaster * modelhandler = LoadObject(LoadObj_Data1, 3, ModelHandler_Init);
				modelhandler->Data1->LoopData = (Loop*)&hang_castle_objects;
			}
		}
	}
	else {
		switch (CurrentAct) {
		case 0:
			ChunkHandler("MM", MysticMansionChunks, LengthOfArray(MysticMansionChunks), entity->Position);
			AnimateTextures(MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
			AnimateObjectsTextures(MMMODELLIST, 2, MysticMansionAnimTexs, LengthOfArray(MysticMansionAnimTexs));
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

	helperFunctions.RegisterStartPosition(Characters_Sonic, MysticMansion_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Tails, MysticMansion_StartPositions[0]);
	helperFunctions.RegisterStartPosition(Characters_Knuckles, MysticMansion_StartPositions[0]);
	helperFunctions.RegisterPathList(MysticMansionPaths);
	helperFunctions.RegisterTrialLevel(Characters_Tails, { 10, 0 });
	helperFunctions.RegisterTrialLevel(Characters_Knuckles, { 10, 0 });
	SaveFile.LevelClear[(Characters_Tails * 43) + LevelIDs_FinalEgg] = 1;
	SaveFile.LevelClear[(Characters_Knuckles * 43) + LevelIDs_FinalEgg] = 1;

	for (uint8_t i = 0; i < 3; i++) {
		DrawDist_FinalEgg1[i].Maximum = -999999.0f;
		FogData_FinalEgg1[i].Toggle = false;
	}
	
	//Prevent skybox from loading
	WriteData<4>((void*)0x0090C218, 0x00u);

	WriteData((DeathZone**)0x1A49218, MysticMansionDeathZones);

	//Load the level handler
	WriteData((ObjectFuncPtr*)0x90BF60, &MysticMansionHandler);

	MysticMansionObjects_Init();
}