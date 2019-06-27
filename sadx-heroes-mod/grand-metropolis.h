#pragma once

CHUNK_LIST GrandMetropolisChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, -2119 } },
	{ 2,{ -640, 0, -2119 },{ 0, 0, -5862 } },
	{ 3,{ 0, 0, -5862 },{ -571, 0, -15692 } },
	{ 4,{ 0, 0, -15692 },{ 0, 0, -21825 } },
	{ 5,{ 0, 0, -21825 },{ 0, 0, -28910 } },
	{ 6,{ 0, 0, -28910 },{ 0, 0, -35934 } },
	{ 8,{ 0, 0, -35934 },{ 0, 0, -40843 } },
	{ 9,{ 4316, 0, -40843 },{ 0, 0, 0 } },
	{ 10,{ 9559, 0, -40843 },{ 4316, 0, 0 } },
	{ 11,{ 0, 0, -40843 },{ 9559, 0, 0 } }
};

SH_ANIMTEXS GrandMetropolisAnimTexs[]{
	{ 192, 31,{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, } },
	{ 224, 31,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, } }
};

DeathZone GrandMetropolisDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ6 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ7 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ8 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ9 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ10 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ11 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &GM_DZ12 },
	{ 0 }
};

void GrandMetropolisObjects_Init(const char *path);
void AutoPathsMovs();
void GMPistons(ObjectMaster *a1);
void GMCars(ObjectMaster *a1);
void GMAds(ObjectMaster *a1);
void GMSky(ObjectMaster *a1);

extern ModelInfo * GM_ADVERTS;
extern ModelInfo * GM_FLYCARS;
extern ModelInfo * GM_GPISTON;
extern ModelInfo * GM_GRFLUID;
extern ModelInfo * GM_GRPLANE;
extern ModelInfo * GM_MCLOUDS;
extern ModelInfo * GM_ZEPPLIN;

SH_UVSHIFT GrandMetropolis_UVSHIFT[]{
	{ nullptr,0,{ -2, -2 }, 3 },
	{ nullptr,0,{ 1, 1 }, 3 },
};

NJS_MODEL_SADX * GMOBJLIST[3];
extern NJS_MODEL_SADX s03_door_energy;