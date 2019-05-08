#pragma once

CHUNK_LIST EggFleetChunks[]{
	{ 1,{ 0, 0, 0 },{ 0, 0, 2462 } },
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