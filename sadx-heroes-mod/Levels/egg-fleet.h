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
};

SH_ANIMTEXS EggFleetAnimTexs[]{
	{ 41, 7,{ 14, 10, 8, 10, 8, 10, 8, 5 } }
};

DeathZone EggFleetDeathZones[] = {
	{ 0 }
};

LoopHead * EggFleetPathList[] = {
	0
};

PathDataPtr EggFleetPaths = { LevelAndActIDs_SkyDeck1, EggFleetPathList };

extern ModelInfo * EF_CANNON1;
extern ModelInfo * EF_BULLETS;