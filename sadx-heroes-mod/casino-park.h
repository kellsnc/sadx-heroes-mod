#pragma once

StartPosition CasinoPark_StartPositions[]{
	{ 3, 0,{ -8000, 2188, 0 }, 0xBFFF },
};

CHUNK_LIST CasinoParkChunks[]{
	{ 1,{ 0, 0, 0 },{ -8696, 0, -1697 } },
	{ 2,{ 0, 0, -1697 },{ -8696, 0, -2697 } },
	{ 3,{ -8696, 0, -1397 },{ -10578, 0, -2697 } },
	{ 5,{ -10646, 0, -1397 },{ -14200, 0, -2697 } },
	{ 6,{ -14323, 0, -987 },{ 0, 0, -2697 } },
	{ 7,{ -14323, 0, -230 },{ 0, 0, -864 } },
	{ 8,{ -13926, 0, 1083 },{ 0, 0, -205 } },
	{ 9,{ -13926, 0, 2642 },{ 0, 0, 1089 } },
	{ 10,{ -13926, 0, 0 },{ 0, 1100, 2662 } },
	{ 11,{ -13926, 1090, 0 },{ 0, 0, 2662 } },
};

SH_ANIMTEXS CasinoParkAnimTexs[]{
	{ 60, 3,{ 5, 5, 5, 5 } },
	{ 64, 17,{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5 } },
	{ 82, 3,{ 8, 8, 8, 8 } },
	{ 86, 3,{ 8, 8, 8, 8 } },
	{ 90, 2,{ 58, 58, 58 } },
	{ 93, 3,{ 8, 8, 8, 8 } },
	{ 97, 3,{ 8, 8, 8, 8 } },
	{ 101, 3,{ 8, 8, 8, 8 } },
	{ 105, 3,{ 8, 8, 8, 8 } },
	{ 109, 3,{ 5, 5, 5, 5 } },
	{ 113, 7,{ 8, 8, 8, 8, 8, 8, 8, 8, } },
	{ 121, 3,{ 8, 8, 8, 8 } },
	{ 125, 3,{ 8, 8, 8, 8 } },
	{ 129, 3,{ 8, 8, 8, 8 } },
	{ 174, 8,{ 4, 4, 4, 4, 4, 4, 4, 4, 4 } },
	{ 183, 3,{ 5, 5, 5, 5 } },
	{ 187, 5,{ 8, 8, 8, 8, 8, 8 } },
	{ 193, 17,{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4 } },
	{ 212, 1,{ 38, 38 } },
	{ 214, 3,{ 10, 10, 10, 10 } },
	{ 218, 1,{ 60, 90 } },
	{ 220, 1,{ 60, 90 } },
	{ 222, 1,{ 60, 90 } },
	{ 224, 7,{ 9, 9, 9, 9, 9, 9, 9, 9, } },
	{ 232, 1,{ 40, 40 } },
	{ 234, 1,{ 45, 45 } },
	{ 236, 1,{ 40, 40 } },
	{ 238, 7,{ 7, 7, 7, 7, 7, 7, 7, 7, } },
	{ 246, 1,{ 50, 50 } },
	{ 248, 4,{ 10, 10, 10, 10, 10 } },
	{ 253, 4,{ 10, 10, 10, 10, 10 } },
	{ 258, 4,{ 10, 10, 10, 10, 10 } },
	{ 263, 2,{ 50, 50, 50 } },
	{ 266, 2,{ 5, 5, 5, 5 } },
	{ 270, 8,{ 4, 4, 4, 4, 4, 4, 4, 4, 4 } },
	{ 279, 7,{ 7, 7, 7, 7, 7, 7, 7, 7, } },
	{ 293, 1,{ 60, 60 } },
	{ 287, 3,{ 10, 10, 10, 10 } },
	{ 295, 1,{ 30, 30 } }
};

DeathZone CasinoParkDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &CP_DZ6 },
	{ 0 }
};