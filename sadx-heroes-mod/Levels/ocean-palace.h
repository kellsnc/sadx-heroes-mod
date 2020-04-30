#pragma once

SH_ANIMTEXS OceanPalaceAnimTexs[]{
	{ 2, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4 } },
	{ 90, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } }
};

CHUNK_LIST OceanPalaceChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, -10637 } },
	{ 2,{ 0, 0, -10638 },{ 0, 0, -16182 } },
	{ 3,{ 0, 0, -16185 },{ 0, 0, -21276 } },
	{ 4,{ 0, 0, -21286 },{ 0, 0, -24604 } },
	{ 5,{ 0, 0, -25192 },{ 0, 0, -33666 } },
	{ 6,{ 0, 0, -33667 },{ 0, 0, -40250 } },
	{ 7,{ 0, 0, -40251 },{ -8443, 0, -48669 } },
	{ 8,{ -8442, 0, -45529 },{ 0, 0, 0 } }
};

SH_ANIMTEXS RoadRockAnimTexs[]{
	{ 0, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4 } },
	{ 47, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } }
};

DeathZone RoadRockDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma, &S69_DZ },
	{ CharacterFlags_Big, &S69_DZ_BIG },
	{ 0 }
};

void OceanPalaceObjects_Init();

void OPFins_Main(ObjectMaster *a1);
void OPFlowers(ObjectMaster *a1);
void OPWaterfalls(ObjectMaster *a1);
void OPBoulders(ObjectMaster *a1);

extern DeathZone SeasideHillDeathZones[];

SH_UVSHIFT OceanPalace_UVShift[]{
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
	{ nullptr,0,{ 0, -10 } },
};

extern ModelInfo * OP_WATERFS;
extern ModelInfo * OP_FLOWERS;
extern ModelInfo * OP_TURFINS;
extern ModelInfo * OP_BOULDER;
extern ModelInfo * OP_POLFLAG;
extern ModelInfo * OP_SKYMDLS;