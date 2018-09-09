#pragma once

StartPosition PowerPlant_StartPositions[]{
	{ 8, 0,{ 2000, 1554, 63 }, 0xBFFF },
};

CHUNK_LIST PowerPlantChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, -2081 } },
	{ 2,{ 0, 0, -2081 },{ 0, 0, -3449 } },
	{ 3,{ 0, 0, -3449 },{ 0, 0, -5897 } },
	{ 4,{ 11810, 0, -5897 },{ 0, 0, -8264 } },
	{ 5,{ 0, 0, -8264 },{ 11810, 0, -8919 } },
	{ 6,{ 13813, 0, -8919 },{ 0, 0, -11035 } },
	{ 7,{ 17370, 0, -9798 },{ 13813, 0, -12411 } },
	{ 8,{ 17470, 0, -12411 },{ 0, 0, -15437 } },
	{ 8,{ 18504, 0, -12814 },{ 18086, 0, -13454 } },
	{ 9,{ 19149, 0, 0 },{ 18505, 0, -12797 } },
	{ 10,{ 21501, 0, 0 },{ 19149, 0, 0 } },
	{ 11,{ 0, 0, 0 },{ 21501, 0, 0 } }
};

SH_ANIMTEXS PowerPlantAnimTexs[]{
	{ 128, 31,{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, } },
	{ 160, 31,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, } }
};

DeathZone PowerPlantDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ6 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ7 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &PP_DZ8 },
	{ 0 }
};