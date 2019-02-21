#pragma once

CHUNK_LIST SeasideHillChunks[]{
	{ 16,{ 0, 0, 0 },{ 0, 0, -6264 } },
	{ 1,{ 0, 0, -6309 },{ -253, 0, -12098 } },
	{ 2,{ -430, 0, -6609 },{ -4107, 0, -12098 } },
	{ 3,{ -4137, 0, -8500 },{ 0, 0, -17200 } },
	{ 4,{ -1096, 0, -17506 },{ 0, 0, -22077 } },
	{ 5,{ 0, 0, -18242 },{ -458, 0, -19295 } },
	{ 6,{ 0, 0, -19378 },{ 48, 0, -20770 } },
	{ 15,{ 0, 0, -20866 },{ 0, 0, -23050 } },
	{ 7,{ 0, 0, -23299 },{ 0, 0, -32632 } },
	{ 8,{ 0, 0, -32723 },{ 0, 0, -34094 } },
	{ 9,{ 0, 0, -35416 },{ 0, 800, -43039 } },
	{ 9,{ 0, 0, -43039 },{ 1554, 0, -44091 } },
	{ 10,{ 1261, 0, -43039 },{ 0, 0, -44644 } },
	{ 10,{ 0, 0, -44644 },{ 0, 0, 0 } }
};

SH_ANIMTEXS SeasideHillAnimTexs[]{
	{ 21, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4 } },
	{ 99, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } }
};

DeathZone SeasideHillDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma, &S01_DZ1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma, &S01_DZ2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma, &S01_DZ3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &S01_DZ4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &S01_DZ5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &S01_DZ6 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &S01_DZ7 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &S01_DZ8 },
	{ 0 }
};

CHUNK_LIST SeaGateChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, -133 } },
	{ 2,{ 0, 0, -268 },{ 0, 0, -2049 } },
	{ 3,{ 0, 0, -2049 },{ 0, 0, -3955 } },
	{ 4,{ 0, 0, -4148 },{ 0, 0, -6501 } },
	{ 5,{ 0, 0, -6708 },{ 0, 0, -8159 } },
	{ 6,{ 0, 0, -8282 },{ 0, 0, -9392 } },
	{ 7,{ 0, 0, -9534 },{ 0, 0, 0 } }
};

SH_ANIMTEXS SeaGateAnimTexs[]{
	{ 0, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4 } },
	{ 84, 19,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } }
};

DeathZone SeaGateDeathZones[] = {
	{ 0 }
};
