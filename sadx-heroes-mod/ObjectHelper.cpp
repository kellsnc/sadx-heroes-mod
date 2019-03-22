#include "stdafx.h"
#include "mod.h"

//Load Object File
ModelInfo* LoadMDL(const char *name) {
	PrintDebug("Loading model "); PrintDebug(name); PrintDebug("... ");
	std::string fullPath = modpath + "\\system\\objects\\" + name + ".sa1mdl";
	const char *foo = fullPath.c_str();

	ModelInfo * temp = new ModelInfo(foo);

	if (temp->getformat() == ModelFormat_Basic) PrintDebug("OK.\n");
	else PrintDebug("ERROR.\n");

	return temp;
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

//Global delete sub, deletes the object itself and any dynamic collision
void deleteSub_Global(ObjectMaster *a1) {
	if (a1->Data1->Object)
	{
		DynamicCOL_Remove(a1, a1->Data1->Object);
		ObjectArray_Remove(a1->Data1->Object);
	}
	DeleteObject_(a1);
}

//Global display sub calling a generic drawing call
void displaySub_Global(ObjectMaster *a1) {
	if (!MissedFrames) DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);
}

//Global mainsub for object that just display their model
void mainSub_Global(ObjectMaster *a1) {
	if (IsPlayerInsideSphere(&a1->Data1->Position, 2500.0f)) {
		displaySub_Global(a1);
	}
	else {
		deleteSub_Global(a1);
	}
}

/*	Add complex/dynamic collisions to an object, limited to 255 collisions at a time
Delete itself if the global delete sub is used	*/
void AddToCollision(ObjectMaster *a1, uint8_t col) {
	/*	0 is static
	1 is moving (refresh the colision every frame)
	2 is static, scalable
	3 is moving, scalable	*/

	EntityData1 * original = a1->Data1;
	NJS_OBJECT *colobject;

	colobject = ObjectArray_GetFreeObject(); //The collision is a separate object, we add it to the list of object

	//if scalable
	if (col == 2 || col == 3) {
		colobject->evalflags = NJD_EVAL_BREAK | NJD_EVAL_SKIP | NJD_EVAL_HIDE;
		colobject->scl[0] = original->Scale.x;
		colobject->scl[1] = original->Scale.y;
		colobject->scl[2] = original->Scale.z;
	}
	else {
		colobject->evalflags = NJD_EVAL_UNIT_SCL | NJD_EVAL_BREAK | NJD_EVAL_SKIP | NJD_EVAL_HIDE; //ignore scale
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
	a1->Data1->Object = colobject; //pointer to the collision object into our original object

	if (col == 0 || col == 2) DynamicCOL_Add((ColFlags)1, a1, colobject); //Solid
	else if (col == 1 || col == 3) DynamicCOL_Add((ColFlags)0x8000000, a1, colobject); //Dynamic, solid
	else if (col == 4) DynamicCOL_Add((ColFlags)0x8000001, a1, colobject);
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