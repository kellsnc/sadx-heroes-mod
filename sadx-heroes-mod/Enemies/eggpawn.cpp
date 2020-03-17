#include "stdafx.h"

bool IsEggPawnInitialized;

#define MDL_COUNT 4
#define ANM_COUNT 17

ModelInfo*		EggPawnMdls[MDL_COUNT];
AnimationFile*	EggPawnAnms[ANM_COUNT];
AnimData		EggPawnAnimData[ANM_COUNT];

NJS_TEXNAME EGGPAWN_TEXNAMES[15];
NJS_TEXLIST EGGPAWN_TEXLIST = { arrayptrandlength(EGGPAWN_TEXNAMES) };

NJS_MATRIX EggPawnMatrices[4];

CollisionData EggPawnCollisionData[]{
	{0, CollisionShape_Cylinder, 0, 0x21, 0x2400, { 0, 0, 0}, {8, 10, 0}},
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x802400, { 0, 10, 0}, {8, 6, 0}}
};

enum EggPawnActions {
	EggPawnAction_Stand,
	EggPawnAction_Sleep,
	EggPawnAction_Check,
	EggPawnAction_RunForward,
	EggPawnAction_RunToPlayer,
	EggPawnAction_Aim,
	EggPawnAction_Shoot
};

enum class EggPawnAnim {
	AT,
	ATRUN,
	CANNON,
	DAMAGE,
	DROP1,
	DROP2,
	FALL,
	PANIC,
	RUN,
	SEARCH,
	SHLDAWAY,
	STANDUP,
	WALK,
	EN_SUB_CANNON,
	EN_SUB_LANCE,
	EN_SUB_MGUN,
	EN_SUB_SHIELD
};

enum class EggPawnType {
	Normal,
	King,
	Yellow,
	Blue,
	Turquoise = 5,
	Purple,
	Pink = 9,
	Green = 11
};

enum class EggPawnWeapon {
	None,
	Lance,
	Pistol,
	Cannon
};

enum class EggPawnShield {
	None,
	Normal,
	Spike,
	Rock
};

struct PawnCustomData {
	EggPawnActions startaction;
	EggPawnAnim pawnanim;
	EggPawnType pawntype;
	EggPawnWeapon pawnweapon;
	EggPawnShield pawnshield;
	bool CanMove;
	Uint32 origtex1;
	Uint32 origtex2;
};

const char* EggPawnMdlNames[]{
	"EN_EGGPAWN",
	"EN_PWNKING",
	"EN_PWNOBJS",
	"EN_PWNVARS"
};

const char* EggPawnAnmNames[]{
	"EN_PAWN_AT",
	"EN_PAWN_ATRUN",
	"EN_PAWN_CANNON",
	"EN_PAWN_DAMAGE",
	"EN_PAWN_DROP1",
	"EN_PAWN_DROP2",
	"EN_PAWN_FALL",
	"EN_PAWN_PANIC",
	"EN_PAWN_RUN",
	"EN_PAWN_SEARCH",
	"EN_PAWN_SHLDAWAY",
	"EN_PAWN_STANDUP",
	"EN_PAWN_WALK",
	"EN_SUB_CANNON",
	"EN_SUB_LANCE",
	"EN_SUB_MGUN",
	"EN_SUB_SHIELD"
};

NJS_VECTOR EggPawn_GetPoint(NJS_VECTOR* orig, Rotation3* rot, float z, float y) {
	NJS_VECTOR point;

	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, orig);
	njRotateXYZ(0, rot->x, rot->y, rot->z);
	njTranslate(0, 0, y, -z);
	njGetTranslation(_nj_current_matrix_ptr_, &point);
	njPopMatrix(1u);

	return point;
}

void EggPawn_Delete(ObjectMaster* obj) {
	//Delete files if the level is exited, or the player has no life left.
	if ((GameState == GameState_ExitLevel || (GameState == GameState_Death && Lives == 0))
		&& IsEggPawnInitialized == true) {
		IsEggPawnInitialized = false;
		njReleaseTexture(&EGGPAWN_TEXLIST);
		FreeMDLFiles(arrayptrandlength(EggPawnMdls));
		FreeANMFiles(arrayptrandlength(EggPawnAnms));
	}
}

//Get the matrice of some parts of the robots
void EggPawn_DrawCallback(NJS_OBJECT* object) {
	if (object == (NJS_OBJECT*)EggPawnMdls[0]->getdata("handR")) {
		memcpy(EggPawnMatrices[0], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //weapon
	}
	else if (object == (NJS_OBJECT*)EggPawnMdls[0]->getdata("handL")) {
		memcpy(EggPawnMatrices[1], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //weapon
	}
	else if (object == (NJS_OBJECT*)EggPawnMdls[0]->getdata("head")) {
		memcpy(EggPawnMatrices[2], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //weapon
	}
	else if (object == (NJS_OBJECT*)EggPawnMdls[0]->getdata("bottom")) {
		memcpy(EggPawnMatrices[3], _nj_current_matrix_ptr_, sizeof(NJS_MATRIX)); //weapon
	}
}

//Set the correct texture variant
void EggPawn_SetVariant(EntityData1* data, PawnCustomData* pawndata) {
	switch (pawndata->pawntype) {
	case EggPawnType::Purple:
		EGGPAWN_TEXLIST.textures[1].texaddr = EGGPAWN_TEXLIST.textures[4].texaddr;
		break;
	case EggPawnType::Turquoise:
	case EggPawnType::Blue:
		EGGPAWN_TEXLIST.textures[1].texaddr = EGGPAWN_TEXLIST.textures[10].texaddr;
		break;
	}
	
	EGGPAWN_TEXLIST.textures[0].texaddr = EGGPAWN_TEXLIST.textures[(int)pawndata->pawntype].texaddr;
}

//Draw the extra models some variants have
void EggPawn_DrawVariantModels(EntityData1* data, PawnCustomData* pawndata) {
	switch (pawndata->pawntype) {
	case EggPawnType::Green:
		njPushMatrix(0);
		njSetTexture(&EGGPAWN_TEXLIST);
		memcpy(_nj_current_matrix_ptr_, EggPawnMatrices[2], sizeof(NJS_MATRIX));
		njDrawModel_SADX(EggPawnMdls[3]->getmodel()->child->child->basicdxmodel);
		njPopMatrix(1);

		break;
	case EggPawnType::Pink:
		njPushMatrix(0);
		njSetTexture(&EGGPAWN_TEXLIST);
		memcpy(_nj_current_matrix_ptr_, EggPawnMatrices[2], sizeof(NJS_MATRIX));
		njDrawModel_SADX(EggPawnMdls[3]->getmodel()->child->basicdxmodel);
		memcpy(_nj_current_matrix_ptr_, EggPawnMatrices[3], sizeof(NJS_MATRIX));
		njDrawModel_SADX(EggPawnMdls[3]->getmodel()->child->child->child->basicdxmodel);
		njPopMatrix(1);

		break;
	}
}

void EggPawn_Display(ObjectMaster* obj) {
	if (MissedFrames || obj->Data1->Status == 1) return;

	EntityData1* data = obj->Data1;
	PawnCustomData* pawndata = (PawnCustomData*)obj->Data2;
	int currentanim = (int)pawndata->pawnanim;
	
	if (pawndata->pawntype > EggPawnType::Normal) {
		EggPawn_SetVariant(data, pawndata);
	}
	
	NJS_MODEL_SADX* body = nullptr;

	if (pawndata->pawntype == EggPawnType::Turquoise) {
		body = data->Object->child->child->child->basicdxmodel;
		data->Object->child->child->child->basicdxmodel = EggPawnMdls[3]->getmodel()->basicdxmodel;
	}

	Direct3D_PerformLighting(6);
	njSetTexture(&EGGPAWN_TEXLIST);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);
	if (data->Action == EggPawnAction_Sleep) njTranslate(0, 0, -2, 0);

	*NodeCallbackFuncPtr = EggPawn_DrawCallback;
	EggPawnAnimData[currentanim].Animation->object = data->Object;
	njAction(EggPawnAnimData[currentanim].Animation, data->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	EggPawn_DrawVariantModels(data, pawndata);

	njPopMatrix(1);
	Direct3D_PerformLighting(0);
	DrawShadow(data, 2);

	EGGPAWN_TEXLIST.textures[0].texaddr = pawndata->origtex1;
	EGGPAWN_TEXLIST.textures[1].texaddr = pawndata->origtex2;

	if (body) data->Object->child->child->child->basicdxmodel = body;
}

//Get the height of the and make it fall if needed.
bool EggPawn_CheckRobotFalling(EntityData1* data, PawnCustomData* pawndata) {
	//store the ground height
	float y = GetGroundPositionEntity(data, false);
	y -= data->Position.y;

	if (y < -100000) {
		data->Position.y - 100;
	}

	// make the robot fall
	if (y < -3) {
		pawndata->pawnanim = EggPawnAnim::DROP2; //anim
		data->Position.y -= 2;
	}
	else if (y < -2) {
		pawndata->pawnanim = EggPawnAnim::DROP1; //anim
	}
	else {
		return true;
	}

	return false;
}

//	Make the egg pawn walk around, and turn
void EggPawn_WalkArround(EntityData1* data, PawnCustomData* pawndata) {
	if (pawndata->CanMove) {
		data->Position = EggPawn_GetPoint(&data->Position, &data->Rotation, 0.5f, 0);
		pawndata->pawnanim = EggPawnAnim::WALK;

		if (data->NextAction == 0) {
			if (rand() % 200 == 0) {
				data->NextAction = rand() % 3 + 1;
			}
		}
		else {
			if (data->NextAction == 1) {
				data->Rotation.y += 100;
			}
			else {
				data->Rotation.y -= 100;
			}

			if (++data->InvulnerableTime > 60) {
				data->InvulnerableTime = 0;
				data->NextAction = 0;
			}
		}
	}
	else {
		data->Action = EggPawnAction_Stand;
	}
}

//	Make the egg pawn run toward the player, then normal behaviour
void EggPawn_RunToPlayer(EntityData1* data, PawnCustomData* pawndata) {
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex - 1];

	if (data->CollisionInfo->CollidingObject == nullptr && playerdata && IsSpecificPlayerInSphere(&data->Position, 300, data->CharIndex - 1)) {
		float rotdest = -fPositionToRotation(&data->Position, &playerdata->Position).y - 0x4000;

		if (data->NextAction < 120) {
			++data->NextAction;
			
			pawndata->pawnanim = EggPawnAnim::WALK;

			if (data->Rotation.y < rotdest + 101 && data->Rotation.y > rotdest - 101) {
				data->Rotation.y = rotdest;
			}
			else if (data->Rotation.y < rotdest) data->Rotation.y += 150;
			else if (data->Rotation.y > rotdest) data->Rotation.y -= 150;
		}
		else if (data->CollisionInfo->CollidingObject == nullptr) {
			data->Rotation.y = rotdest;

			if (pawndata->CanMove) {
				data->Position = EggPawn_GetPoint(&data->Position, &data->Rotation, 1, 0);
				pawndata->pawnanim = EggPawnAnim::RUN;
			}
			else {
				data->Action = EggPawnAction_Check;
			}
		}
	}
	else {
		data->Action = EggPawnAction_Stand;
	}
}

//	Make the egg pawn run forward as soon as player in range, then normal behaviour
void EggPawn_RunForward(EntityData1* data, PawnCustomData* pawndata) {
	if (IsPlayerInsideSphere(&data->Position, 300)) {
		if (++data->InvulnerableTime > 300) {
			data->Action = EggPawnAction_Check;
			data->InvulnerableTime = 0;
			return;
		}

		if (pawndata->CanMove) {
			data->Position = EggPawn_GetPoint(&data->Position, &data->Rotation, 1, 0);
			pawndata->pawnanim = EggPawnAnim::RUN;
		}
		else {
			data->Action = EggPawnAction_Stand;
			return;
		}
	}
	else {
		pawndata->pawnanim = EggPawnAnim::SEARCH;
	}
}

//	If the egg pawn intersect with an entity, it should be pushed from its center
//	If there is no intersection and front point is not in the air, allow walking
void EggPawn_CanMoveAndRunPushed(EntityData1* data, PawnCustomData* pawndata) {
	if (data->Rotation.z < 0x7000 && data->Rotation.z < -0x7000) {
		int i = 0;
	}
	
	if (data->CollisionInfo->CollidingObject == nullptr) {
		if (anim % 4 == 0 && (data->Action == EggPawnAction_Stand ||
			data->Action == EggPawnAction_RunForward ||
			data->Action == EggPawnAction_RunToPlayer)) {

			NJS_VECTOR front = { 0, 0, 0 };
			Rotation3 temp = { 0, 0, 0 };
			float y = data->Position.y;

			front = EggPawn_GetPoint(&data->Position, &data->Rotation, 20, 10);
			y -= GetGroundYPosition(front.x, front.y, front.z, &temp);

			if (y > -30 && y < 30) {
				pawndata->CanMove = true;
			}
			else {
				pawndata->CanMove = false;
				pawndata->pawnanim = EggPawnAnim::SEARCH;
			}
		}
	}
	else {
		pawndata->CanMove = false;

		if (data->field_A == 0) {
			if (rand() % 200 == 0) {
				data->field_A = rand() % 3 + 1;
			}
		}
		else {
			if (data->field_A == 1) {
				data->Rotation.y += 100;

			}
			else {
				data->Rotation.y -= 100;
			}

			pawndata->pawnanim = EggPawnAnim::WALK;
		}

		//push code
		ObjectMaster* colobj = data->CollisionInfo->CollidingObject->Object;
		if (colobj && colobj->DisplaySub == EggPawn_Display) {
			Rotation3 temp = { 0, 0, 0 };
			temp.y = fPositionToRotation(&colobj->Data1->Position, &data->Position).y;
			data->Position = EggPawn_GetPoint(&data->Position, &temp, 1, 0);
			pawndata->pawnanim = EggPawnAnim::WALK;
		}
	}
}

void EggPawn_Main(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return; //	Delete the object if too far away

	EntityData1* data = obj->Data1;
	PawnCustomData* pawndata = (PawnCustomData*)obj->Data2;
	float frame = 0;
	
	//Run the logic if close enough
	if (IsPlayerInsideSphere_(&data->Position, 500)) {

		//	Check if the enemy is destroyed by the player, or a player's object (missiles, hammer...)
		if (OhNoImDead(data, (ObjectData2*)pawndata)) {
			obj->DisplaySub = nullptr;
			UpdateSetDataAndDelete(obj);
			Score += 100;

			// 3D Sound

			ObjectMaster* temp = LoadObject(LoadObj_Data1, 5, ObjectBreaker);
			temp->Data1->Position = data->Position;
			temp->Data1->Rotation = data->Rotation;
			temp->Data1->Object = data->Object->child->child;

			return;
		}

		EggPawn_CanMoveAndRunPushed(data, pawndata);

		switch (data->Action) {
		case EggPawnAction_Stand:
			if (EggPawn_CheckRobotFalling(data, pawndata) == true) { //if the robot isn't falling:
				
				data->Position.y = GetGroundPositionEntity(data, true);

				EggPawn_WalkArround(data, pawndata);

				//	After a while, stop and check for player
				if (++data->InvulnerableTime > 300) {
					data->Action = EggPawnAction_Check;
					data->InvulnerableTime = 0;
				}

				//	Still check for player while walking around
				goto CHECKFORPLAYER;
			}

			break;
		case EggPawnAction_Sleep:
			pawndata->pawnanim = EggPawnAnim::DROP1;

			if (IsPlayerInsideSphere_(&data->Position, 100)) {

				// Load icon "!"
				// 3D sound

				pawndata->pawnanim = EggPawnAnim::PANIC;

				if (data->Unknown == 1) {
					data->Action = EggPawnAction_Check;
				}
			}
			else {
				frame = 1;
			}

			break;
		case EggPawnAction_Check:
			pawndata->pawnanim = EggPawnAnim::SEARCH;

			//	If the pawn can't find a player, make it walk after a while
			if (++data->InvulnerableTime > 50) {
				data->Rotation.y = rand();
				data->Action = EggPawnAction_Stand;
				data->InvulnerableTime = 0;
				break;
			}

		CHECKFORPLAYER:
			data->CharIndex = IsPlayerInsideSphere_(&data->Position, 200);
			if (data->CharIndex) {
				data->NextAction = 0;

				// Load Icon "!"
				// 3D sound

				if (pawndata->pawnweapon >= EggPawnWeapon::Pistol) {
					data->Action = EggPawnAction_Aim;
				}
				else {
					data->Action = EggPawnAction_RunToPlayer;
				}
			}

			break;
		case EggPawnAction_RunForward:
			EggPawn_RunForward(data, pawndata);
			break;
		case EggPawnAction_RunToPlayer:
			EggPawn_RunToPlayer(data, pawndata);
			break;
		}

		PlayHeroesAnimation(obj, data->Index, EggPawnAnimData, 0, frame);
		AddToCollisionList(data);
	}
	else {
		if (data->Action == EggPawnAction_Sleep) pawndata->pawnanim = EggPawnAnim::DROP1;
		else pawndata->pawnanim = EggPawnAnim::SEARCH;
	}
	
	obj->DisplaySub(obj);
}

void EggPawn_LoadFiles() {
	LoadPVM("eggpawn", &EGGPAWN_TEXLIST);
	LoadModelListFuncPtr(arrayptrandlength(EggPawnMdlNames), EggPawnMdls, LoadEnemyModel);
	LoadAnimListFuncPtr(arrayptrandlength(EggPawnAnmNames), EggPawnAnms, LoadEnemyAnim);
	FillAnimDataTable(EggPawnAnms, arrayptrandlength(EggPawnAnimData), nullptr);
}

void EggPawn_Init(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	//	If the configs are ok, load an Egg Pawn
	if (Enemies_CanSwap) {

		//	Load the files only once
		if (IsEggPawnInitialized == false) {
			IsEggPawnInitialized = true;
			EggPawn_LoadFiles();
		}

		//	Init the enemy handler (used for Gamma's missile, Sonic's attacks)
		PawnCustomData* pawndata = (PawnCustomData*)AllocateObjectData2(obj, data);
		ObjectData2_SetStartPosition(data, (ObjectData2*)pawndata);

		//	Init the collision (with the "hurt player if not attacking" and "target" flags)
		Collision_Init(obj, EggPawnCollisionData, 2, 3);

		//	Populate the ObjectData unused stuff with our own struct
		//	using the data in the setfile
		pawndata->startaction =	(EggPawnActions)(int)data->Scale.x;
		pawndata->pawnweapon =	(EggPawnWeapon)(int)data->Scale.y;
		pawndata->pawnshield =	(EggPawnShield)(int)data->Scale.z;
		pawndata->pawntype =	(EggPawnType)data->Rotation.x;
		
		data->Action = pawndata->startaction;
		
		//	The King type is another model, swap
		if (pawndata->pawntype == EggPawnType::King) {
			data->Object = EggPawnMdls[1]->getmodel();
		}
		else {
			data->Object = EggPawnMdls[0]->getmodel();
		}

		pawndata->origtex1 = EGGPAWN_TEXLIST.textures[0].texaddr;
		pawndata->origtex2 = EGGPAWN_TEXLIST.textures[1].texaddr;

		//	Reset the object properties
		data->Position.y -= 4;
		data->Rotation = { 0, data->Rotation.y + 0x8000 , 0 };
		data->Scale = { 0, 0, 0 };

		//	Object functions
		obj->MainSub = EggPawn_Main;
		obj->DisplaySub = EggPawn_Display;
		obj->DeleteSub = EggPawn_Delete;
	}
	else {
		ObjectFunc(original, KikiTrampoline->Target()); // Otherwise, load a kiki.
		original(obj);
	}
}