#include "stdafx.h"

bool IsE2000Initialized;

#define MDL_COUNT 3
#define ANM_COUNT 11

ModelInfo*		e2000Mdls[MDL_COUNT];
AnimationFile*	e2000Anms[ANM_COUNT];
AnimData		e2000AnimData[ANM_COUNT];

NJS_TEXNAME E2000_TEXNAMES[11];
NJS_TEXLIST E2000_TEXLIST = { arrayptrandlength(E2000_TEXNAMES) };
NJS_TEXNAME CURRENTE2000_TEXNAMES[3];
NJS_TEXLIST CURRENTE2000_TEXLIST = { arrayptrandlength(CURRENTE2000_TEXNAMES) };

NJS_MATRIX e2000Matrices[5];

CollisionData e2000CollisionData[]{
	{0, CollisionShape_Cylinder, 0x77, 0x21, 0x2400, { 0, -5, 0}, {8, 15, 0}},
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x802400, { 0, 10, 0}, {8, 6, 0}}
};

CollisionData LaserCollisionData[]{
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x2400, { 0, 0, -30}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -60}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -90}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -120}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -150}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -180}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -210}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -240}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -270}, {30, 30, 0}},
	{0, CollisionShape_Sphere, 0x77, 0xE2, 0x2400, { 0, 0, -300}, {30, 30, 0}},
};

enum e2000Actions {
	e2000Action_Init,
	e2000Action_Hover,
	e2000Action_Flying,
	e2000Action_Shield
};

enum e2000SubActions {
	e2000SubAction_Transform,
	e2000SubAction_Search,
	e2000SubAction_StartShoot,
	e2000SubAction_Shoot,
	e2000SubAction_EndShoot
};

enum class e2000Anim {
	E2K,
	GUARD_A,
	GUARD_C,
	HOVER,
	MOVEL,
	MOVER,
	SHOT0,
	SHOT2,
	SHOTLOOP,
	SIBIRE,
	TRANSFB
};

enum class e2000Type {
	Normal,
	King
};

struct E2KCustomData {
	e2000Actions startaction;
	e2000Anim anim;
	e2000Type type;
	float height;
	char damage;
	char switchid;
};

const char* e2000MdlNames[]{
	"EN_E2K",
	"EN_E2KJETS",
	"EN_E2KLASR"
};

const char* e2000AnmNames[]{
	"EN_E2K",
	"EN_E2K_GUARD_A",
	"EN_E2K_GUARD_C",
	"EN_E2K_HOVER",
	"EN_E2K_MOVEL",
	"EN_E2K_MOVER",
	"EN_E2K_SHOT0",
	"EN_E2K_SHOT2",
	"EN_E2K_SHOTLOOP",
	"EN_E2K_SIBIRE",
	"EN_E2K_TRANSF_B"
};

void e2000Laser_UV(NJS_OBJECT* obj, short uv) {
	for (uint8_t m = 0; m < obj->basicdxmodel->nbMeshset; ++m) {
		for (uint8_t uv = 0; uv < obj->basicdxmodel->nbPoint; ++uv) {
			obj->basicdxmodel->meshsets[m].vertuv[uv].v += uv;
		}
	}
}

void e2000Laser_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		njPushMatrix(0);
		njSetTexture(&E2000_TEXLIST);
		memcpy(_nj_current_matrix_ptr_, e2000Matrices[0], sizeof(NJS_MATRIX));
		njScale(0, data->Scale.x, data->Scale.x, data->Scale.x);
		DrawObject(data->Object);
		njPopMatrix(1);
	}
}

void e2000Laser_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (anim % 4 == 0) {
		++data->field_A;
	}

	if (++data->InvulnerableTime > 250) {
		data->Scale.x -= 0.05f;

		if (data->Scale.x <= 0) {
			DeleteObject_(obj);
		}
	}
	else {
		if (data->Scale.x < 1) {
			data->Scale.x += 0.05f;
		}
	}

	AddToCollisionList(data);

	e2000Laser_UV(data->Object, data->field_A);
	e2000Laser_UV(data->Object->child, data->field_A);
	e2000Laser_UV(data->Object->child->child, data->field_A);
	e2000Laser_UV(data->Object->child->child->child, data->field_A);
	obj->DisplaySub(obj);
}

void e2000_LoadLaser(EntityData1* data) {
	ObjectMaster* obj = LoadChildObject(LoadObj_Data1, e2000Laser_Main, data->CollisionInfo->Object);
	obj->Data1->Position = data->Position;
	obj->Data1->Rotation = data->Rotation;
	obj->Data1->Object = e2000Mdls[2]->getmodel();
	obj->DisplaySub = e2000Laser_Display;

	PlayHeroesSound_Entity(CommonSound_Shoot2, obj, 500, false);

	Collision_Init(obj, arrayptrandlength(LaserCollisionData), 4);
	SetCollisionInfoRadius(obj->Data1->CollisionInfo, 1000);
}

void e2000_DeleteFiles() {
	if (IsE2000Initialized == true) {
		IsE2000Initialized = false;
		njReleaseTexture(&E2000_TEXLIST);
		FreeMDLFiles(arrayptrandlength(e2000Mdls));
		FreeANMFiles(arrayptrandlength(e2000Anms));
	}
}

inline void e2000_SetDefaultTextures() {
	CURRENTE2000_TEXLIST.textures[0] = E2000_TEXLIST.textures[0];
	CURRENTE2000_TEXLIST.textures[1] = E2000_TEXLIST.textures[1];
	CURRENTE2000_TEXLIST.textures[2] = E2000_TEXLIST.textures[2];
}

inline void e2000_SetVariantTextures() {
	CURRENTE2000_TEXLIST.textures[0] = E2000_TEXLIST.textures[8];
	CURRENTE2000_TEXLIST.textures[1] = E2000_TEXLIST.textures[9];
	CURRENTE2000_TEXLIST.textures[2] = E2000_TEXLIST.textures[10];
}

void e2000_DrawCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)e2000Mdls[0]->getdata("Dummy018")) {
		memcpy(e2000Matrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //cannon
	}
	else if (object == (NJS_OBJECT*)e2000Mdls[0]->getdata("Dummy010")) {
		memcpy(e2000Matrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //air 1
	}
	else if (object == (NJS_OBJECT*)e2000Mdls[0]->getdata("Dummy011")) {
		memcpy(e2000Matrices[2], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //air 2
	}
	else if (object == (NJS_OBJECT*)e2000Mdls[0]->getdata("Dummy021")) {
		memcpy(e2000Matrices[3], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //air 3
	}
	else if (object == (NJS_OBJECT*)e2000Mdls[0]->getdata("Dummy024")) {
		memcpy(e2000Matrices[4], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //air 4
	}
}

void e2000_DrawAir(EntityData1* data) {
	if (MissedFrames) return;

	Direct3D_PerformLighting(6);
	njPushMatrix(0);
	njSetTexture(&E2000_TEXLIST);
	memcpy(_nj_current_matrix_ptr_, e2000Matrices[1], sizeof(NJS_MATRIX));
	njDrawModel_SADX(e2000Mdls[1]->getmodel()->basicdxmodel);
	memcpy(_nj_current_matrix_ptr_, e2000Matrices[2], sizeof(NJS_MATRIX));
	njDrawModel_SADX(e2000Mdls[1]->getmodel()->basicdxmodel);
	memcpy(_nj_current_matrix_ptr_, e2000Matrices[3], sizeof(NJS_MATRIX));
	njDrawModel_SADX(e2000Mdls[1]->getmodel()->child->basicdxmodel);
	memcpy(_nj_current_matrix_ptr_, e2000Matrices[4], sizeof(NJS_MATRIX));
	njDrawModel_SADX(e2000Mdls[1]->getmodel()->child->basicdxmodel);
	njPopMatrix(1);
	Direct3D_PerformLighting(0);
}

void e2000_Display(ObjectMaster* obj) {
	if (MissedFrames && obj->Data1->Action != e2000Action_Init) return;

	EntityData1* data = obj->Data1;
	E2KCustomData* e2kdata = (E2KCustomData*)obj->Data2;
	int currentanim = (int)e2kdata->anim;

	if (e2kdata->type == e2000Type::King) {
		e2000_SetVariantTextures();
	}

	Direct3D_PerformLighting(6);
	njSetTexture(&CURRENTE2000_TEXLIST);
	njPushMatrix(0);
	njTranslate(0, data->Position.x, e2kdata->height, data->Position.z);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);
	njScale(0, 2, 2, 2);

	*NodeCallbackFuncPtr = e2000_DrawCallback;
	njAction(e2000AnimData[currentanim].Animation, data->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	njPopMatrix(1);
	Direct3D_PerformLighting(0);

	e2000_DrawAir(data);
	DrawShadow(data, 5);

	e2000_SetDefaultTextures();
}

void e2000_UpAndDown(EntityData1* data, E2KCustomData* e2kdata) {
	if (data->Action == e2000Action_Flying && data->NextAction != e2000SubAction_Transform) {
		
		int temp = IsPlayerInsideSphere_(&data->Position, 300);

		if (temp) {
			EntityData1* player = EntityData1Ptrs[temp - 1];

			if (player->Position.y > data->Position.y - 20 &&
				player->Position.y < data->Position.y + 20) {
				data->Position.y = player->Position.y;
			}
		}
		
		if (data->CharID == 0) {
			if (e2kdata->height > data->Position.y - 1) {
				e2kdata->height -= 0.05f;
			}
			else {
				data->CharID = 1;
			}
		}
		else {
			if (e2kdata->height < data->Position.y + 1) {
				e2kdata->height += 0.05f;
			}
			else {
				data->CharID = 0;
			}
		}
	}
	else {
		e2kdata->height = data->Position.y;
		
		if (data->Action != e2000Action_Init) {
			ObjectData* objdata = (ObjectData*)e2kdata;

			if (data->Position.y != objdata->StartPosition.y) {
				data->Scale.y += 0.01f;
				TransformSpline(data, data->Position, objdata->StartPosition, data->Scale.y);

				if (data->Scale.y >= 0.2) {
					data->Position = objdata->StartPosition;
					data->Scale.y = 0;
				}
			}
		}
	}
	
}

bool e2000_CheckDeath(EntityData1* data, E2KCustomData* e2kdata) {
	if (OhNoImDead(data, (ObjectData2*)e2kdata)) {
		if (data->Action == e2000Action_Shield) {
			EntityData1* temp = GetCollidingEntityA(data);

			if (temp) {
				EnemyBounceThing(temp->CharIndex, 0, 1, -2);
			}

			return false;
		}

		if (e2kdata->damage < 16) {
			PlayHeroesSound_Pos(CommonSound_Explosion, &data->Position, 200, 1, false);
			++e2kdata->damage;
			
			EntityData1* temp = GetCollidingEntityA(data);

			if (temp) {
				EnemyBounceAndRumble(temp->CharIndex);
			}

			return false;
		}

		if (e2kdata->switchid > 0) SwitchPressedStates[e2kdata->switchid] = true;

		SpawnAnimal(2, PosToVector(data->Position));
		SpawnAnimal(2, PosToVector(data->Position));

		Score += 5000;
		PlayHeroesSound_Pos(CommonSound_Explosion, &data->Position, 300, 5, false);
 		LoadObjectBreaker(&data->Position, &data->Rotation, data->Object->child, &E2000_TEXLIST);
		return true;
	}

	return false;
}

void e2000_Flying(EntityData1* data, E2KCustomData* e2kdata) {
	if (data->NextAction != e2000SubAction_Transform) {
		e2kdata->anim = e2000Anim::E2K;
		data->Scale.x = 0;
	}

	switch (data->NextAction) {
	case e2000SubAction_Transform:
		e2kdata->anim = e2000Anim::TRANSFB;

		if (data->Unknown > 0) {
			data->Unknown = 0;
			data->CollisionInfo->CollisionArray[0].origin.y = 10;
			data->CollisionInfo->CollisionArray[0].scale.y = 5;
			data->NextAction = e2000SubAction_Search;
		}

		break;
	case e2000SubAction_Search:
		data->CharIndex = IsPlayerInsideSphere_(&data->Position, 300);

		if (data->CharIndex) {
			data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[data->CharIndex - 1]->Position).y - 0x4000;

			if (++data->InvulnerableTime > 250) {
				data->NextAction = e2000SubAction_Shoot;
				e2000_LoadLaser(data);
				data->InvulnerableTime = 0;
				data->CharIndex -= 1;
				data->Unknown = 0;
			}
		}

		break;
	case e2000SubAction_Shoot:
		if (++data->InvulnerableTime > 250) {
			data->NextAction = e2000SubAction_EndShoot;
			data->InvulnerableTime = 0;
		}

		break;
	case e2000SubAction_EndShoot:
		if (rand() % 2 == 0) {
			data->Action = e2000Action_Hover;
			data->NextAction = e2000SubAction_Transform;
			data->Unknown = 0;
		}
		else {
			data->NextAction = e2000SubAction_Search;
		}
		
		break;
	}
}

void e2000_Shield(EntityData1* data, E2KCustomData* e2kdata) {
	if (data->NextAction == 0) {
		e2kdata->anim = e2000Anim::GUARD_A;

		if (data->Unknown > 0) {
			data->Unknown = 0;
			data->NextAction = 1;
			goto NEXT;
		}
		
	}
	else if (data->NextAction == 1) {
	NEXT:
		e2kdata->anim = e2000Anim::GUARD_C;
		data->Scale.x = 0;

		if (IsPlayerInsideSphere_(&data->Position, 100) == 0) {
			data->NextAction = 2;
		}
	}
	else {
		e2kdata->anim = e2000Anim::GUARD_C;

		if (data->Unknown > 0) {
			data->Unknown = 0;
			data->NextAction = e2000SubAction_Search;
			data->Action = e2000Action_Hover;
		}
	}
}

void e2000_Hover(EntityData1* data, E2KCustomData* e2kdata) {
	switch (data->NextAction) {
	case e2000SubAction_Transform:

		e2kdata->anim = e2000Anim::E2K;

		if (data->Unknown > 0) {
			data->Unknown = 0;
			data->CollisionInfo->CollisionArray[0].origin.y = -5;
			data->CollisionInfo->CollisionArray[0].scale.y = 15;
			data->NextAction = e2000SubAction_Search;
			goto NEXT;
		}

		break;
	case e2000SubAction_Search:
	NEXT:
		e2kdata->anim = e2000Anim::HOVER;

		if (IsPlayerInsideSphere_(&data->Position, 100)) {
			if (rand() % 300 == 0) {
				data->Action = e2000Action_Shield;
				data->Unknown = 0;
				data->NextAction = 0;
				return;
			}
		}

		data->CharIndex = IsPlayerInsideSphere_(&data->Position, 300);
		if (data->CharIndex) {
			data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[data->CharIndex - 1]->Position).y - 0x4000;

			if (++data->InvulnerableTime > 200) {

				if (rand() % 5 == 0) {
					data->Action = e2000Action_Flying;
					data->NextAction = e2000SubAction_Transform;
					data->Unknown = 0;
					return;
				}
				else {
					data->NextAction = e2000SubAction_StartShoot;
					data->InvulnerableTime = 0;
					data->CharIndex -= 1;
					data->Unknown = 0;
				}
			}
		}

		break;
	case e2000SubAction_StartShoot:
		data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[data->CharIndex]->Position).y - 0x4000;
		e2kdata->anim = e2000Anim::SHOT0;

		if (data->Unknown > 0) {
			e2000_LoadLaser(data);
			data->NextAction = e2000SubAction_Shoot;
			data->Unknown = 0;
		}

		break;
	case e2000SubAction_Shoot:
		e2kdata->anim = e2000Anim::SHOTLOOP;

		if (++data->InvulnerableTime > 250) {
			data->NextAction = e2000SubAction_EndShoot;
			data->InvulnerableTime = 0;
		}

		break;
	case e2000SubAction_EndShoot:
		e2kdata->anim = e2000Anim::SHOT2;

		if (data->Unknown > 0) {
			data->NextAction = e2000SubAction_Search;
			data->Unknown = 0;
		}

		break;
	}
}

void e2000_Appear(EntityData1* data, E2KCustomData* e2kdata) {
	ObjectData* objdata = (ObjectData*)e2kdata;

	if (data->NextAction == 0) {
		e2kdata->anim = e2000Anim::E2K;
		data->Scale.x = 0;

		if (IsPlayerInsideSphere_(&objdata->StartPosition, 400)) {
			data->NextAction = e2000SubAction_Search;
		}

		return;
	}
	else {
		if (data->Unknown > 0) {
			e2kdata->anim = e2000Anim::HOVER;
		}

		data->Scale.y += 0.001f;
		TransformSpline(data, data->Position, objdata->StartPosition, data->Scale.y);

		if (data->Scale.y >= 0.2) {
			data->Position = objdata->StartPosition;
			data->Scale.y = 0;
			data->Action = e2000Action_Hover;
			data->NextAction = 0;
		}
	}
}

void e2000_Main(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return; //	Delete the object if too far away

	EntityData1* data = obj->Data1;
	E2KCustomData* e2kdata = (E2KCustomData*)obj->Data2;
	
	if (e2000_CheckDeath(data, e2kdata)) {
		UpdateSetDataAndDelete(obj);
		return;
	}

	e2000_UpAndDown(data, e2kdata);

	switch (data->Action) {
	case e2000Action_Init:
		e2000_Appear(data, e2kdata);
		break;
	case e2000Action_Hover:
		e2000_Hover(data, e2kdata);
		break;
	case e2000Action_Flying:
		e2000_Flying(data, e2kdata);
		break;
	case e2000Action_Shield:
		e2000_Shield(data, e2kdata);
		break;
	}

	PlayHeroesAnimation(obj, (uint8_t)e2kdata->anim, e2000AnimData, 0, 0);
	AddToCollisionList(data);
	obj->DisplaySub(obj);
	RunObjectChildren(obj);
}

inline void e2000_LoadFiles() {
	LoadPVM("e2k", &E2000_TEXLIST);
	LoadModelListFuncPtr(arrayptrandlength(e2000MdlNames), e2000Mdls, LoadEnemyModel);
	LoadAnimListFuncPtr(arrayptrandlength(e2000AnmNames), e2000Anms, LoadEnemyAnim);
	FillAnimDataTable(e2000Anms, arrayptrandlength(e2000AnimData), e2000Mdls[0]->getmodel());

	e2000AnimData[(int)e2000Anim::TRANSFB].TransitionSpeed = 2;
}

void e2000_Init(ObjectMaster* obj) {
	if (IsE2000Initialized == false) {
		IsE2000Initialized = true;
		e2000_LoadFiles();
	}

	EntityData1* data = obj->Data1;

	//	Init the enemy handler (used for Gamma's missile, Sonic's attacks)
	E2KCustomData* e2kdata = (E2KCustomData*)AllocateObjectData2(obj, data);
	ObjectData2_SetStartPosition(data, (ObjectData2*)e2kdata);
	
	//	Init the collision (with the "hurt player if not attacking" and "target" flags)
	Collision_Init(obj, e2000CollisionData, 2, 3);

	//	Populate the ObjectData unused stuff with our own struct
	//	using the data in the setfile
	e2kdata->startaction = (e2000Actions)(int)data->Scale.x;
	e2kdata->type = (e2000Type)(int)data->Scale.y;
	e2kdata->switchid = data->Scale.z;
	e2kdata->height = data->Position.y;

	data->Action = e2kdata->startaction;
	data->Object = e2000Mdls[0]->getmodel();

	e2000_SetDefaultTextures();

	//	Reset the object properties
	data->Position.y += 100;
	data->Rotation = { 0, data->Rotation.y + 0x8000 , 0 };
	data->Scale = { 0, 0, 0 };
	
	//	Object functions
	obj->MainSub = e2000_Main;
	obj->DisplaySub = e2000_Display;
}