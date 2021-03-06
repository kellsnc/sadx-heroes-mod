#pragma once

CHUNK_LIST MysticMansionChunks[]{
	{ 1,{ 320, 0, 1000 },{ 0, 0, -1400 } },
	{ 2,{ 1300, 0, -750 },{ 680, 0, -2690 } },
	{ 3,{ 2160, 0, -1787 },{ 1840, 0, -3950 } },
	{ 4,{ 1802, 0, -3950 },{ 603, 0, -16219 } },
	{ 5,{ 2470, 0, -16219 },{ 980, 0, -20270 } },
	{ 7,{ 3470, 0, -18730 },{ 2705, 0, -21000 } },
	{ 8,{ 4740, 0, -20120 },{ 3740, 0, -21120 } },
	{ 9,{ 5300, 0, -21120 },{ 2650, 0, -22100 } },
	{ 10,{ 8270, 0, -21650 },{ 5496, 0, -22250 } },
	{ 11,{ 5900, 0, -23700 },{ 3900, 0, -26900 } },
	{ 11,{ 12000, 0, -21450 },{ 8270, 0, -35850 } },
	{ 12,{ 14850, 0, -34850 },{ 12000, 0, -38759 } },
	{ 13,{ 17650, 0, -38759 },{ 14850, -9145, -42500 } },
	{ 14,{ 17600, -9145, -42100 },{ 17200, 0, -42500 } },
	{ 14,{ 18230, 0, -43000 },{ 16620, 0, -45400 } }, //15
	{ 14,{ 24120, 0, -43000 },{ 22630, 0, -45400 } }, //17
	{ 16,{ 21200, 0, -43000 },{ 19650, 0, -45400 } },
	{ 16,{ 30300, 0, -43000 },{ 28470, 0, -45400 } }, //19
	{ 18,{ 27100, 0, -43000 },{ 25640, 0, -45400 } },
	{ 18,{ 32800, 0, -43000 },{ 32000, 0, -45400 } }, //20
};

MusicInfo MysticMansionMusics[6]{
	{ "mystic-mansion-a", 1 },
	{ "mystic-mansion-b", 1 },
	{ "mystic-mansion-c", 1 },
	{ "mystic-mansion-d", 1 },
	{ "mystic-mansion-e", 1 },
	{ "mystic-mansion-f", 1 },
};

SH_ANIMTEXS MysticMansionAnimTexs[]{
	{ 72, 18,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 } },
	{ 106, 13,{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 } },
	{ 122, 13,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2 } },
	{ 138, 13,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2 } },
};

DeathZone MysticMansionDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ6 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ7 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ8 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ9 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &MM_DZ10 },
	{ CharacterFlags_Knuckles, &MM_DZK },
	{ 0 }
};

void MysticMansionObjects_Init();
void __cdecl MysticTorches(ObjectMaster *a1);

SH_UVSHIFT MysticMansion_UVShift[]{
	{ nullptr, 0,{ 3, -3 } },
	{ nullptr, 0,{ -2, 2 } }
};

NJS_MODEL_SADX * MMMODELLIST[4];

extern ModelInfo * MM_SPHERE1;
extern ModelInfo * MM_SPHERE2;
extern ModelInfo * MM_SKELFAN;
extern ModelInfo * MM_MYSTCAR;
extern ModelInfo * MM_MOVPLAT;
extern ModelInfo * MM_MYSDOOR;
extern ModelInfo * MM_MYSWALL;
extern ModelInfo * MM_TORCHES;
extern ModelInfo * HC_SPKWARP;
extern ModelInfo * HC_HFLAMES;

extern int flamecount;