#include "stdafx.h"
#include "mod.h"

//Render a set of models in only one object
//I should remove this some day
void ModelHandler_Display(ObjectMaster * a1) {
	SOI_LISTS * List = (SOI_LISTS*)a1->Data1->LoopData;
	if (!DroppedFrames) {
		for (int i = 0; i < a1->Data1->Action; ++i) {
			if (List[i].CharacterID == 0 || a1->Data1->Index == List[i].CharacterID) {
				for (int j = 0; j < List[i].Count; ++j) {
					if (List[i].Entry[j].DisplayParameter != 1) {

						if (List[i].Entry[j].Chunk == CurrentChunk || List[i].Entry[j].Chunk == 0) {
							njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
							njPushMatrix(0);
							njTranslate(nullptr, List[i].Entry[j].Position.x, List[i].Entry[j].Position.y, List[i].Entry[j].Position.z); //Position of object
							njRotateXYZ(nullptr, List[i].Entry[j].Rotation[0], List[i].Entry[j].Rotation[1], List[i].Entry[j].Rotation[2]); //Rotation of object
							njScale(nullptr, List[i].Entry[j].Scale.x, List[i].Entry[j].Scale.y, List[i].Entry[j].Scale.z); //Scale of object
							DrawQueueDepthBias = List[i].Entry[j].Bias;
							if (List[i].Entry[j].DrawDistance == 0.0f) {
								njDrawModel_SADX(List[i].Entry[j].Model);
							}
							else {
								if (IsPlayerInsideSphere(&List[i].Entry[j].Position, List[i].Entry[j].DrawDistance) == true)
									njDrawModel_SADX(List[i].Entry[j].Model);
							}
							DrawQueueDepthBias = 0;
							njPopMatrix(1u);
						}
					}

				}
			}
		}
	}
}

void ModelHandler_Init(ObjectMaster * a1) {
	a1->Data1->Index = GetCharacterID(0) + 1;

	SOI_LISTS * List = (SOI_LISTS*)a1->Data1->LoopData;
	while (a1->Data1->NextAction == 0) {
		a1->Data1->Action += 1;

		if (List[a1->Data1->Action].Count == NULL) {
			a1->Data1->NextAction = 1;
		}
	}

	a1->Data1->Scale.x = 0;
	
	a1->DisplaySub = ModelHandler_Display;
	a1->MainSub = ModelHandler_Display;
}

//Animate textures of the current landtable in a similar way to Sonic Heroes
void AnimateTextures(SH_ANIMTEXS *list, Int listcount) {
	if (!DroppedFrames) {
		for (Int j = 0; j < CurrentLandTable->COLCount; ++j) {
			if (CurrentLandTable->Col[j].Flags == 0x80000000 || CurrentLandTable->Col[j].Flags == 0x80000001 || CurrentLandTable->Col[j].Flags == 0x80000002) {
				for (Int k = 0; k < CurrentLandTable->Col[j].Model->basicdxmodel->nbMat; ++k) {

					for (int l = 0; l < listcount; ++l) {
						Int last = list[l].texid + list[l].count;

						if (CurrentLandTable->Col[j].Model->basicdxmodel->mats[k].attr_texId >= list[l].texid &&
							CurrentLandTable->Col[j].Model->basicdxmodel->mats[k].attr_texId <= last) {

							Int Currenttex = CurrentLandTable->Col[j].Model->basicdxmodel->mats[k].attr_texId - list[l].texid;
							if (anim % (list[l].duration[Currenttex]) == 0) {
								if (Currenttex == last - list[l].texid) {
									CurrentLandTable->Col[j].Model->basicdxmodel->mats[k].attr_texId = list[l].texid;
								}
								else {
									CurrentLandTable->Col[j].Model->basicdxmodel->mats[k].attr_texId += 1;
								}
							}
						}
					}
				}
			}
		}
	}
}