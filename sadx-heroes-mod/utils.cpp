#include "stdafx.h"

D3DMATRIX WorldMatrixBackup;

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
			PrintDebug("Failed.\n");
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

ModelInfo* LoadCommonModel(const char *name) {
	PrintDebug("[SHM] Loading common object model: %s... ", name);
	return LoadMDL("objects", name);
}

ModelInfo* LoadObjectModel(ModelInfo* ptr, const char *name) {
	if (ptr) return ptr;

	PrintDebug("[SHM] Loading object model: %s... ", name);
	return LoadMDL("objects", name);
}

ModelInfo* LoadCharacterModel(const char *name) {
	PrintDebug("[SHM] Loading character model: %s... ", name);
	return LoadMDL("characters", name);
}

//Free Object File
ModelInfo* FreeMDL(ModelInfo* pointer) {
	if (GameState == 9 || (GameState == 8 && Lives == 0)) {
		PrintDebug("[SHM] Freeing model: %s... \n", pointer->getdescription().c_str());
		delete(pointer);
		return nullptr;
	}
	else {
		return pointer;
	}
}

//Load Animation File
AnimationFile* LoadANM(const char *type, const char *name) {
	std::string fullPath = "system\\";
	fullPath = fullPath + type + "\\" + name + ".saanim";

	AnimationFile * temp = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (temp->getmodelcount()) {
		PrintDebug("Done.\n");
		return temp;
	}
	else {
		PrintDebug("Failed.\n");
		delete temp;
		return nullptr;
	}
}

AnimationFile* LoadObjectAnim(const char *name) {
	PrintDebug("[SHM] Loading object animation: %s... ", name);
	return LoadANM("objects", name);
}

AnimationFile* LoadCharacterAnim(const char *name) {
	PrintDebug("[SHM] Loading character animation: %s... ", name);
	return LoadANM("characters", name);
}

//Free Animation File
void FreeANM(AnimationFile* pointer) {
	if (pointer) {
		delete(pointer);
	}
}

//Free a list of files
void FreeMDLFiles(ModelInfo** Files, int size) {
	PrintDebug("[SHM] Freeing %s models... ", std::to_string(size).c_str());

	for (int i = 0; i < size; ++i) {
		FreeMDL(Files[i]);
	}

	PrintDebug("Done. \n");
}

void FreeANMFiles(AnimationFile** Files, int size) {
	PrintDebug("[SHM] Freeing %s animations... ", std::to_string(size).c_str());

	for (int i = 0; i < size; ++i) {
		FreeANM(Files[i]);
	}

	PrintDebug("Done. \n");
}

//Basic drawing call
void DrawObjModel(ObjectMaster* obj, NJS_MODEL_SADX *m, bool scalable) {
	if (!MissedFrames) {
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &obj->Data1->Position);
		njRotateXYZ(nullptr, obj->Data1->Rotation.x, obj->Data1->Rotation.y, obj->Data1->Rotation.z);
		if (scalable) njScale(nullptr, obj->Data1->Scale.x, obj->Data1->Scale.y, obj->Data1->Scale.z);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(m);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

//Global display sub calling a generic drawing call
void displaySub_Global(ObjectMaster* obj) {
	DrawObjModel(obj, obj->Data1->Object->basicdxmodel, false);
}

//Global mainsub for object that just display their model
void mainSub_Global(ObjectMaster* obj) {
	if (!ClipSetObject(obj)) {
		displaySub_Global(obj);
	}
}

/*	Add complex/dynamic collisions to an object, limited to 255 collisions at a time
	Delete itself if the global delete sub is used	*/
void DynCol_Add(ObjectMaster* obj, uint8_t col) {
	/*	0 is static
	1 is moving (refresh the colision every frame)
	2 is static, scalable
	3 is moving, scalable	*/

	EntityData1 * original = obj->Data1;
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

	colobject->basicdxmodel = obj->Data1->Object->basicdxmodel; //object it will use as a collision
	obj->Data1->LoopData = (Loop*)colobject; //pointer to the collision object into our original object

	if (col == 0 || col == 2) DynamicCOL_Add(ColFlags_Solid, obj, colobject); //Solid
	else if (col == 1 || col == 3) DynamicCOL_Add((ColFlags)0x8000000, obj, colobject); //Dynamic, solid
	else if (col == 4) DynamicCOL_Add((ColFlags)0x8000001, obj, colobject);
	else if (col == 5) DynamicCOL_Add(ColFlags_Water, obj, colobject);
	else if (col == 6) DynamicCOL_Add((ColFlags)(0x8000000 | ColFlags_Solid | ColFlags_Hurt), obj, colobject);
}

//Sphere check functions
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius) {
	return (powf(pos->x - center->x, 2) + pow(pos->y - center->y, 2) + pow(pos->z - center->z, 2)) <= pow(radius, 2);
}

int IsPlayerInsideSphere_(NJS_VECTOR* center, float radius) {
	for (uint8_t player = 0; player < MaxPlayers; ++player) {
		if (!EntityData1Ptrs[player]) continue;

		NJS_VECTOR* pos = &EntityData1Ptrs[player]->Position;
		if (IsPointInsideSphere(center, pos, radius)) {
			return player + 1;
		}
	}

	return 0;
}

bool IsSpecificPlayerInSphere(NJS_VECTOR* center, float radius, uint8_t player) {
	if (IsPlayerInsideSphere_(center, radius) == player + 1) return true;
	else return false;
}

//Update the dyncol data
void DynCol_Update(EntityData1* data, uint8_t type) {
	NJS_OBJECT* col = (NJS_OBJECT*)data->LoopData;
	
	if (col) {
		switch (type) {
		case 0:
posy: 
			col->pos[1] = data->Position.y;
			break;
		case 1:
posxyz:
			col->pos[0] = data->Position.x;
			col->pos[1] = data->Position.y;
			col->pos[2] = data->Position.z;
			break;
		case 2:
			col->ang[1] = data->Rotation.y;
			goto posy;
		case 3:
			col->ang[0] = data->Rotation.x;
			col->ang[1] = data->Rotation.y;
			col->ang[2] = data->Rotation.z;
			goto posy;
		case 4:
			col->ang[0] = data->Rotation.x;
			col->ang[1] = data->Rotation.y;
			col->ang[2] = data->Rotation.z;
			goto posxyz;
		}
	}
}

//Clean dynamic collisions
void DynCol_Delete(ObjectMaster* obj) {
	if (obj->Data1->LoopData)
	{
		DynamicCOL_Remove(obj, (NJS_OBJECT*)obj->Data1->LoopData);
		ObjectArray_Remove((NJS_OBJECT*)obj->Data1->LoopData);
		obj->Data1->LoopData = nullptr;
	}
}

//Only allocate dynamic collision within radius
bool DynColRadius(ObjectMaster* obj, float radius, uint8_t col) {
	if (IsPlayerInsideSphere_(&obj->Data1->Position, radius)) {
		if (!obj->Data1->LoopData) {
			DynCol_Add(obj, col);
			return 2;
 		}
		return true;
	}
	else {
		DynCol_Delete(obj);
	}

	return false;
}

bool DynColRadiusAuto(ObjectMaster* obj, uint8_t col) {
	//Use the model radius
	return DynColRadius(obj, obj->Data1->Object->basicdxmodel->r + 50, col);
}

//Shift uv of models, requires a SH_UVSHIFT struct
void AnimateUV(SH_UVSHIFT* UVSHIFT, int size) {
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

//Distance checks
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest) {
	return sqrtf(powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2));
}

//Position functions
NJS_VECTOR GetPathPosition(NJS_VECTOR* orig, NJS_VECTOR* dest, float state) {
	NJS_VECTOR result;
	result.x = (dest->x - orig->x) * state + orig->x;
	result.y = (dest->y - orig->y) * state + orig->y;
	result.z = (dest->z - orig->z) * state + orig->z;

	return result;
}

Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point) {
	NJS_VECTOR dist = *point;
	Rotation3 result;

	njSubVector(&dist, orig);

	result.x = atan2(dist.y, dist.z) * 65536.0 * -0.1591549762031479;
	result.y = -(atan2(dist.x, dist.z) * 65536.0 * 0.1591549762031479) + 0x4000;

	return result;
}

void DrawChunkModel(NJS_CNK_MODEL* model)
{
	DrawChunkModel_(model->vlist, model->plist);
}

void njCnkAction_Queue(NJS_ACTION* action, float frame, QueuedModelFlagsB flags)
{
	DisplayAnimationFrame(action, frame, flags, 0, (void(__cdecl*)(NJS_MODEL_SADX*, int, int))DrawChunkModel);
}

void njCnkAction(NJS_ACTION* action, float frame)
{
	DisplayAnimationFrame(action, frame, (QueuedModelFlagsB)0, 0, (void(__cdecl*)(NJS_MODEL_SADX*, int, int))DrawChunkModel);
}

void SetupWorldMatrix()
{
	ProjectToWorldSpace();
	WorldMatrixBackup = WorldMatrix;
	Direct3D_SetWorldTransform();
	memcpy(_nj_current_matrix_ptr_, &ViewMatrix, sizeof(NJS_MATRIX));
}