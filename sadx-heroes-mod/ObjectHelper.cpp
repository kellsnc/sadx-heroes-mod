#include "stdafx.h"
#include "mod.h"

//Load Object File
ModelInfo* LoadMDL(const char *type, const char *name) {
	std::string fullPath = "system\\";
	fullPath = fullPath + type + "\\" + name + ".sa1mdl";

	ModelInfo * temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (temp->getformat() == ModelFormat_Invalid) {
		delete temp;
		fullPath = "system\\";
		fullPath = fullPath + type + "\\" + name + ".cnkmdl";

		temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

		if (temp->getformat() == ModelFormat_Invalid) {
			delete temp;
			return nullptr;
		}
		else {
			PrintDebug("Done.\n");
			return temp;
		}
	}
	else {
		PrintDebug("Done.\n");
		return temp;
	}
}

ModelInfo* LoadObjectModel(const char *name) {
	PrintDebug("[SHM] Loading object model %s...", name);
	return LoadMDL("objects", name);
}

ModelInfo* LoadCharacterModel(const char *name) {
	PrintDebug("[SHM] Loading character model %s...", name);
	return LoadMDL("characters", name);
}

//Free Object File
void FreeMDL(ModelInfo * pointer) {
	if (pointer) delete(pointer);
}

//Basic drawing call
void DrawObjModel(ObjectMaster *a1, NJS_MODEL_SADX *m, bool scalable) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		if (scalable) njScale(nullptr, a1->Data1->Scale.x, a1->Data1->Scale.y, a1->Data1->Scale.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(m);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

//Global display sub calling a generic drawing call
void displaySub_Global(ObjectMaster *a1) {
	DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);
}

//Global mainsub for object that just display their model
void mainSub_Global(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		displaySub_Global(a1);
	}
}

/*	Add complex/dynamic collisions to an object, limited to 255 collisions at a time
	Delete itself if the global delete sub is used	*/
void DynCol_Add(ObjectMaster *a1, uint8_t col) {
	/*	0 is static
	1 is moving (refresh the colision every frame)
	2 is static, scalable
	3 is moving, scalable	*/
	
	EntityData1 * original = a1->Data1;
	NJS_OBJECT *colobject;

	colobject = ObjectArray_GetFreeObject(); //The collision is a separate object, we add it to the list of object

	//if scalable
	if (col == 2 || col == 3) {
		colobject->scl[0] = original->Scale.x;
		colobject->scl[1] = original->Scale.y;
		colobject->scl[2] = original->Scale.z;
	}
	else {
		colobject->scl[0] = 1.0;
		colobject->scl[1] = 1.0;
		colobject->scl[2] = 1.0;
	}

	//add the rest
	colobject->ang[0] = original->Rotation.x;
	colobject->ang[1] = original->Rotation.y;
	colobject->ang[2] = original->Rotation.z;
	colobject->pos[0] = original->Position.x;
	colobject->pos[1] = original->Position.y;
	colobject->pos[2] = original->Position.z;

	colobject->basicdxmodel = a1->Data1->Object->basicdxmodel; //object it will use as a collision
	a1->Data1->LoopData = (Loop*)colobject; //pointer to the collision object into our original object

	if (col == 0 || col == 2) DynamicCOL_Add(ColFlags_Solid, a1, colobject); //Solid
	else if (col == 1 || col == 3) DynamicCOL_Add((ColFlags)0x8000000, a1, colobject); //Dynamic, solid
	else if (col == 4) DynamicCOL_Add((ColFlags)0x8000001, a1, colobject);
	else if (col == 5) DynamicCOL_Add(ColFlags_Water, a1, colobject);
	else if (col == 6) DynamicCOL_Add((ColFlags)(0x8000000 | ColFlags_Solid | ColFlags_Hurt), a1, colobject);
}

//Clean dynamic collisions
void DynCol_Delete(ObjectMaster *a1) {
	if (a1->Data1->LoopData)
	{
		DynamicCOL_Remove(a1, (NJS_OBJECT*)a1->Data1->LoopData);
		ObjectArray_Remove((NJS_OBJECT*)a1->Data1->LoopData);
	}
}

//Only allocate dynamic collision within radius
bool DynColRadius(ObjectMaster *a1, float radius, uint8_t col) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, radius)) {
		if (!a1->Data1->LoopData) {
			DynCol_Add(a1, col);
			return 2;
		}
		return true;
	} else if (a1->Data1->LoopData) {
		DynamicCOL_Remove(a1, (NJS_OBJECT*)a1->Data1->LoopData);
		ObjectArray_Remove((NJS_OBJECT*)a1->Data1->LoopData);
		a1->Data1->LoopData = nullptr;
	}

	return false;
}

bool DynColRadiusAuto(ObjectMaster *a1, uint8_t col) {
	//Use the model radius
	return DynColRadius(a1, a1->Data1->Object->basicdxmodel->r + 50, col);
}

//Shift uv of models, requires a SH_UVSHIFT struct
void AnimateUV(SH_UVSHIFT *UVSHIFT, int size) {
	if (GameState != 16) {
		for (int j = 0; j < size; ++j) {
			if (UVSHIFT[j].List == nullptr) continue;

			if (!UVSHIFT[j].uvtime || anim % UVSHIFT[j].uvtime == 0)
				for (int i = 0; i < UVSHIFT[j].Size; ++i) {
					if (UVSHIFT[j].uvshift[0] != 0) {
						if (UVSHIFT[j].List[i].u > 20000) UVSHIFT[j].List[i].u - 32000;
						if (UVSHIFT[j].List[i].u < -20000) UVSHIFT[j].List[i].u + 32000;
						UVSHIFT[j].List[i].u += UVSHIFT[j].uvshift[0];
					}
					if (UVSHIFT[j].uvshift[1] != 0) {
						if (UVSHIFT[j].List[i].v > 20000) UVSHIFT[j].List[i].v - 32000;
						if (UVSHIFT[j].List[i].v < -20000) UVSHIFT[j].List[i].v + 32000;
						UVSHIFT[j].List[i].v += UVSHIFT[j].uvshift[1];
					}
				}
		}
	}
}

//Animate textures of models, requires a SH_ANIMTEXS struct
void AnimateObjectsTextures(NJS_MODEL_SADX * *objlist, int size, SH_ANIMTEXS *list, Int listcount) {
	for (Int j = 0; j < size; ++j) {
		if (objlist[j] == nullptr) continue;

		for (Int k = 0; k < objlist[j]->nbMat; ++k) {
			for (int l = 0; l < listcount; ++l) {
				Int last = list[l].texid + list[l].count;

				if (objlist[j]->mats[k].attr_texId >= list[l].texid &&
					objlist[j]->mats[k].attr_texId <= last) {

					Int Currenttex = objlist[j]->mats[k].attr_texId - list[l].texid;
					if (anim % (list[l].duration[Currenttex]) == 0) {
						if (Currenttex == last - list[l].texid) {
							objlist[j]->mats[k].attr_texId = list[l].texid;
						}
						else {
							objlist[j]->mats[k].attr_texId += 1;
						}
					}
				}
			}
		}
	}
}

//is player in non-rotated box
bool IsPlayerInBox(NJS_VECTOR playerpos, NJS_VECTOR pos1, NJS_VECTOR pos2) {
	if ((playerpos.x > pos1.x) && (playerpos.x < pos2.x)
		&& (playerpos.y > pos1.y) && (playerpos.y < pos2.y)
		&& (playerpos.z > pos1.z) && (playerpos.z < pos2.z)
		) return true;
	else return false;
}

//check if we can display a SOI_LIST2 model
bool CheckModelDisplay(SOI_LIST item) {
	if (item.Chunk == CurrentChunk || item.Chunk == 0) {
		if (item.DrawDistance == 0.0f) {
			return true;
		}
		else {
			if (IsPlayerInsideSphere(&item.Position, item.DrawDistance) == true)
				return true;
		}
	}
	return false;
}

bool CheckModelDisplay2(SOI_LIST2 item) {
	if (item.Chunk == CurrentChunk || item.Chunk == 0) {
		if (item.DrawDistance == 0.0f) {
			return true;
		}
		else {
			if (IsPlayerInsideSphere(&item.Position, item.DrawDistance) == true)
				return true;
		}
	}
	return false;
}

NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state) {
	NJS_VECTOR result;
	result.x = (dest->x - orig->x) * state + orig->x;
	result.y = (dest->y - orig->y) * state + orig->y;
	result.z = (dest->z - orig->z) * state + orig->z;

	return result;
}

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest) {
	return sqrtf(powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2));
}