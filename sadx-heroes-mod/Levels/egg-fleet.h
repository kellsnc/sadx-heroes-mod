#pragma once

FogData EggFleet_Fog = { -3.0, -20000, 0x00FFFFFF, 1 };

CHUNK_LIST EggFleetChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, -2300 } },
	{ 2,{ 0, 0, -2331 },{ -625, 0, -4390 } },
	{ 2,{ 65, 0, -4390 },{ -65, 0, -4631 } },
	{ 3,{ -65, 0, -4390 },{ -625, 0, -4631 } },
	{ 3,{ -625, 0, -3700 },{ -3005, 0, -5409 } },
	{ 4,{ -3005, 1201, -5109 },{ 0, 0, -8133 } },
	{ 5,{ 0, 1201, -8133 },{ 0, 0, -9919 } },
	{ 6,{ 0, 0, -8133 },{ 0, 2400, -12944 } },
	{ 7,{ 0, 1350, -15650 },{ 0, 0, -17588 } },
	{ 8,{ 0, 1350, -17588 },{ 0, 0, -21650 } },
	{ 9,{ 0, 1350, -21650 },{ 0, 0, -28800 } },
	{ 10,{ 0, 0, -28800  },{ 0, 0, -37965 } },
	{ 11,{ -9370, 0, -37965  },{ 0, -4220, 0 } }
};

SH_ANIMTEXS EggFleetAnimTexs[]{
	{ 41, 7,{ 14, 10, 8, 10, 8, 10, 8, 5 } }
};

SH_UVSHIFT EggFleet_UVSHIFT[]{
	{ nullptr ,0,{ 0, 6 }, 1 },
	{ nullptr ,0,{ 0, 6 }, 1 }
};

void EFRailends(ObjectMaster* a1);

extern ModelInfo * EF_CANNON1;
extern ModelInfo * EF_BULLETS;
extern ModelInfo * EF_PROPPLR;
extern ModelInfo * EF_BGSHIPS;
extern ModelInfo * EF_PLTFRMS;
extern ModelInfo * EF_PIPLINE;
extern ModelInfo * EF_ENDRAIL;
extern ModelInfo * EF_OBJSHIP;