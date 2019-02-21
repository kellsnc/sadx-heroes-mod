#include "stdafx.h"
#include "mod.h"

//Render a set of models in only one object
//I should remove this some day
void ModelHandler_Display(ObjectMaster * a1) {
	SOI_LISTS * List = (SOI_LISTS*)a1->Data1->LoopData;
	if (!DroppedFrames) {
		for (int i = 0; i < a1->Data1->Action; ++i) {
			if (List[i].soicharid == 0 || a1->Data1->Index == List[i].soicharid) {
				for (int j = 0; j < List[i].soicount; ++j) {
					if (List[i].soientry[j].soidisplay != 1) {

						if (List[i].soientry[j].soichunk == CurrentChunk || List[i].soientry[j].soichunk == 0) {
							njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
							njPushMatrix(0);
							njTranslate(nullptr, List[i].soientry[j].soipos.x, List[i].soientry[j].soipos.y, List[i].soientry[j].soipos.z); //Position of object
							njRotateXYZ(nullptr, List[i].soientry[j].soirot[0], List[i].soientry[j].soirot[1], List[i].soientry[j].soirot[2]); //Rotation of object
							njScale(nullptr, List[i].soientry[j].soiscl.x, List[i].soientry[j].soiscl.y, List[i].soientry[j].soiscl.z); //Scale of object
							DrawQueueDepthBias = List[i].soientry[j].soibias;
							if (List[i].soientry[j].soidrawdist == 0.0f) {
								njDrawModel_SADX(List[i].soientry[j].soiobject);
							}
							else {
								if (IsPlayerInsideSphere(&List[i].soientry[j].soipos, List[i].soientry[j].soidrawdist) == true)
									njDrawModel_SADX(List[i].soientry[j].soiobject);
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

void ModelHandler_Main(ObjectMaster * a1) {
	ModelHandler_Display(a1);
}

void ModelHandler_Init(ObjectMaster * a1) {
	a1->Data1->Index = GetCharacterID(0) + 1;

	SOI_LISTS * List = (SOI_LISTS*)a1->Data1->LoopData;
	while (a1->Data1->NextAction == 0) {
		a1->Data1->Action += 1;

		if (List[a1->Data1->Action].soicount == NULL) {
			a1->Data1->NextAction = 1;
		}
	}

	a1->Data1->Scale.x = 0;
	
	a1->DisplaySub = ModelHandler_Display;
	a1->MainSub = ModelHandler_Main;
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