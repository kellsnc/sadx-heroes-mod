#pragma once

CHUNK_LIST HangCastleChunks[]{
	{ 1,{ 8000, 0, 0 },{ 0, 0, -3111 } },
	{ 2,{ 0, 0, 0 },{ 8000, 0, -7700 } },
	{ 3,{ 8000, 0, -3111 },{ 0, 0, -7079 } },
	{ 4,{ 8000, 0, -7079 },{ 0, 0, -8850 } },
	{ 5,{ 8000, 0, -8850 },{ 0, 0, -12640 } },
	{ 6,{ 0, 0, -7700 },{ 8000, 0, -12000 } },
	{ 7,{ 8000, 0, -12640 },{ 0, 0, -15495 } },
	{ 8,{ 0, 0, -12000 },{ 8000, 0, -14837 } },
	{ 9,{ 8000, 0, -15495 },{ 0, 0, -18924 } },
	{ 10,{ 0, 0, -14837 },{ 8000, 0, -19000 } },
	{ 11,{ 0, 0, -19000 },{ 8000, 0, 0 } },
	{ 12,{ 8000, 0, -18924 },{ 0, 0, 0} },
};

SH_ANIMTEXS HangCastleAnimTexs[]{
	{ 47, 7,{ 2,2,2,2,2,2,2,2, } },
	{ 55, 15,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, } },
	{ 71, 15,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, } },
	{ 108, 24,{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 } },
	{ 133, 13,{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1 } },
};

DeathZone HangCastleDeathZones[] = {
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC1 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC2 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC3 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC4 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC5 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC6 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC7 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC8 },
	{ CharacterFlags_Sonic | CharacterFlags_Tails | CharacterFlags_Knuckles | CharacterFlags_Amy | CharacterFlags_Gamma | CharacterFlags_Big, &DZ_HC9 },
{ 0 }
};

void HangCastleObjects_Init(const char *path);
void HCSpiders(ObjectMaster *a1);
void HCFlags_Reset();
void HCFlags_Animate();

extern ModelInfo * HC_HCBLADE;
extern ModelInfo * HC_HFLAMES;
extern ModelInfo * HC_HPLANTA;
extern ModelInfo * HC_HPLANTB;
extern ModelInfo * HC_POLFLAG;
extern ModelInfo * HC_SPDSIGN;
extern ModelInfo * HC_SPKDOOR;
extern ModelInfo * HC_SPKTREE;
extern ModelInfo * HC_SPKWARP;

NJS_MODEL_SADX * HCMODELLIST[2];