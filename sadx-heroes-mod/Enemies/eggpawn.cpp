#include "stdafx.h"

bool IsEggPawnInitialized;

#define MDL_COUNT 4
#define ANM_COUNT 17

ModelInfo*		EggPawnMdls[MDL_COUNT];
AnimationFile*	EggPawnAnms[ANM_COUNT];
AnimData		EggPawnAnimData[ANM_COUNT];

NJS_TEXNAME EGGPAWN_TEXNAMES[15];
NJS_TEXLIST EGGPAWN_TEXLIST = { arrayptrandlength(EGGPAWN_TEXNAMES) };
NJS_TEXNAME CURRENTEGGPAWN_TEXNAMES[2];
NJS_TEXLIST CURRENTEGGPAWN_TEXLIST = { arrayptrandlength(CURRENTEGGPAWN_TEXNAMES) };

NJS_MATRIX EggPawnMatrices[4];

CollisionData EggPawnCollisionData[]{
	{0, CollisionShape_Cylinder, 0, 0x21, 0x2400, { 0, 0, 0}, {8, 10, 0}},
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x802400, { 0, 10, 0}, {8, 6, 0}}
};

CollisionData PawnWeaponCollisionData[]{
	{0, CollisionShape_Sphere, 0, 0x21, 0, { 0, 0, 0}, {3, 3, 0}},
	{0, CollisionShape_Cone, 0, 0x21, 0, { 0, 0, 0}, {3, 6, 0}}
};

CollisionData PawnShieldCollisionData[]{
	{0, CollisionShape_Sphere, 0x77, 0, 0x2400, { 0, 0, 0}, {3, 3, 0}},
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x2400, { 0, 0, 0}, {3.1f, 3.1f, 0}}
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

enum EggPawnWeapon_Actions {
	PawnWeaponAction_Init,
	PawnWeaponAction_Run,
	PawnWeaponAction_Shoot
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
	float miny;
	bool CanMove;
	char damage;
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

void EggPawnWeapon_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		Direct3D_PerformLighting(6);
		njPushMatrix(0);
		njSetTexture(&EGGPAWN_TEXLIST);
		memcpy(_nj_current_matrix_ptr_, EggPawnMatrices[0], sizeof(NJS_MATRIX));
		njDrawModel_SADX(data->Object->basicdxmodel);
		njPopMatrix(1);
		Direct3D_PerformLighting(0);
	}
}

void EggPawnWeapon_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* pawn = obj->Parent->Data1;
	PawnCustomData* pawndata = (PawnCustomData*)obj->Parent->Data2;

	switch (data->Action) {
	case PawnWeaponAction_Init:

		switch (pawndata->pawnweapon) {
		case EggPawnWeapon::Lance:
			data->Object = EggPawnMdls[2]->getmodel();
			Collision_Init(obj, PawnWeaponCollisionData, 2, 4);
			break;
		case EggPawnWeapon::Pistol:
			data->Object = EggPawnMdls[2]->getmodel()->child;
			Collision_Init(obj, PawnWeaponCollisionData, 1, 4);
			break;
		case EggPawnWeapon::Cannon:
			data->Object = EggPawnMdls[2]->getmodel()->child->child;
			Collision_Init(obj, PawnWeaponCollisionData, 1, 4);
			break;
		}

		obj->DisplaySub = EggPawnWeapon_Display;
		data->Action = PawnWeaponAction_Run;
		break;
	case PawnWeaponAction_Run:
		data->Position = UnitMatrix_GetPoint(&pawn->Position, &pawn->Rotation, 10, 10, -5);

		if (pawndata->pawnweapon > EggPawnWeapon::Lance) {
			if (pawn->Action == EggPawnAction_Shoot) {
				pawn->Action = EggPawnAction_Aim;
				data->Action = PawnWeaponAction_Shoot;

				data->Rotation.x = pawn->Rotation.x;
				data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[pawn->CharIndex - 1]->Position).y - 0x4000;
				data->Rotation.z = pawn->Rotation.z;

				if (pawndata->pawnweapon == EggPawnWeapon::Pistol) {
					LoadMissile(&data->Position, &data->Rotation, 1);
				}
				else {
					LoadMissile(&data->Position, &data->Rotation, 2);
				}
				
				int i = 0;
			}
		}

		break;
	case PawnWeaponAction_Shoot:
		if (pawn->Action == EggPawnAction_Check) {
			data->Action = PawnWeaponAction_Run;
		}

		break;
	}

	AddToCollisionList(data);
	obj->DisplaySub(obj);
}

void EggPawnShield_Display(ObjectMaster* obj) {
	if (!MissedFrames) {
		EntityData1* data = obj->Data1;

		Direct3D_PerformLighting(6);
		njPushMatrix(0);
		njSetTexture(&EGGPAWN_TEXLIST);
		memcpy(_nj_current_matrix_ptr_, EggPawnMatrices[1], sizeof(NJS_MATRIX));
		njDrawModel_SADX(data->Object->basicdxmodel);
		njPopMatrix(1);
		Direct3D_PerformLighting(0);
	}
}

bool EggPawnShield_CheckDestroy(PawnCustomData* pawndata) {
	switch (pawndata->pawnshield) {
	case EggPawnShield::Normal:
		if (pawndata->damage > 2) {
			return true;
		}
		break;
	case  EggPawnShield::Spike:
		if (pawndata->damage > 4) {
			return true;
		}
		break;
	case  EggPawnShield::Rock:
		if (pawndata->damage > 8) {
			return true;
		}
		break;
	}

	return false;
}

void EggPawnShield_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	EntityData1* pawn = obj->Parent->Data1;
	PawnCustomData* pawndata = (PawnCustomData*)obj->Parent->Data2;

	if (data->Action == 0) {
		switch (pawndata->pawnshield) {
		case EggPawnShield::Normal:
			data->Object = EggPawnMdls[2]->getmodel()->child->child->child;
			Collision_Init(obj, PawnShieldCollisionData, 1, 3);
			break;
		case  EggPawnShield::Spike:
			data->Object = EggPawnMdls[2]->getmodel()->child->child->child->child;
			Collision_Init(obj, PawnShieldCollisionData, 2, 3);
			break;
		case  EggPawnShield::Rock:
			data->Object = EggPawnMdls[2]->getmodel()->child->child->child->child->child;
			Collision_Init(obj, PawnShieldCollisionData, 1, 3);
			break;
		}

		obj->DisplaySub = EggPawnShield_Display;
		data->Action = 1;
	}
	else {
		data->Position = UnitMatrix_GetPoint(&pawn->Position, &pawn->Rotation, -10, 10, -5);

		if (EggPawnShield_CheckDestroy(pawndata)) {
			LoadModelBroken(&data->Position, &pawn->Rotation, data->Object, &EGGPAWN_TEXLIST);
			DeleteObject_(obj);
			return;
		}
	}

	AddToCollisionList(data);
	obj->DisplaySub(obj);
}

void EggPawn_DeleteFiles() {
	if (IsEggPawnInitialized == true) {
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
		CURRENTEGGPAWN_TEXLIST.textures[1] = EGGPAWN_TEXLIST.textures[4];
		break;
	case EggPawnType::Turquoise:
	case EggPawnType::Blue:
		CURRENTEGGPAWN_TEXLIST.textures[1] = EGGPAWN_TEXLIST.textures[10];
		break;
	}
	
	CURRENTEGGPAWN_TEXLIST.textures[0] = EGGPAWN_TEXLIST.textures[(int)pawndata->pawntype];
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

inline void EggPawn_SetDefaultTextures() {
	CURRENTEGGPAWN_TEXLIST.textures[0] = EGGPAWN_TEXLIST.textures[0];
	CURRENTEGGPAWN_TEXLIST.textures[1] = EGGPAWN_TEXLIST.textures[1];
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
	
	//the turquoise variant changes the whole body
	if (pawndata->pawntype == EggPawnType::Turquoise) {
		body = data->Object->child->child->child->basicdxmodel;
		data->Object->child->child->child->basicdxmodel = EggPawnMdls[3]->getmodel()->basicdxmodel;
	}

	Direct3D_PerformLighting(6);
	njSetTexture(&CURRENTEGGPAWN_TEXLIST);
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

	EggPawn_SetDefaultTextures();

	//set back the original body if modified
	if (body) data->Object->child->child->child->basicdxmodel = body;
}

bool EggPawnCollide(EntityData1* data) {
	if (data->CollisionInfo->CollidingObject) {
		ObjectMaster* colobj = data->CollisionInfo->CollidingObject->Object;

		if (colobj->MainSub == EggPawnWeapon_Main || colobj->MainSub == EggPawnShield_Main) {
			return false;
		}

		return true;
	}
	
	return false;
}

//Get the height of the and make it fall if needed.
bool EggPawn_CheckRobotFalling(EntityData1* data, PawnCustomData* pawndata) {
	//store the ground height
	float y = GetGroundPositionEntity(data, false);
	y -= data->Position.y;

	if (y < -100000) {
		data->Position.y - 100;
	}
	else if (pawndata->miny && (y + data->Position.y) < pawndata->miny) {
		data->Position.y = pawndata->miny;
		return true;
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
		data->Action = EggPawnAction_Check;
	}
}

//	Make the egg pawn run toward the player, then normal behaviour
void EggPawn_RunToPlayer(EntityData1* data, PawnCustomData* pawndata) {
	EntityData1* playerdata = EntityData1Ptrs[data->CharIndex - 1];

	if (EggPawnCollide(data) == false && playerdata && IsSpecificPlayerInSphere(&data->Position, 300, data->CharIndex - 1)) {
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
		else {
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
	if (EggPawnCollide(data) == false) {
		if (anim % 4 == 0 && (data->Action == EggPawnAction_Stand ||
			data->Action == EggPawnAction_RunForward ||
			data->Action == EggPawnAction_RunToPlayer)) {

			NJS_VECTOR front = { 0, 0, 0 };
			Rotation3 temp = { 0, 0, 0 };
			float y = data->Position.y;

			front = EggPawn_GetPoint(&data->Position, &data->Rotation, 20, 10);
			y -= GetGroundYPosition(front.x, front.y, front.z, &temp);

			if (y > -100000 && pawndata->miny && data->Position.y < pawndata->miny) {
				pawndata->CanMove = true;
				return;
			}

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
			data->Position = EggPawn_GetPoint(&data->Position, &temp, 1, 0.2f);
			pawndata->pawnanim = EggPawnAnim::WALK;
		}
	}
}

bool EggPawn_CheckDamage(EntityData1* data, PawnCustomData* pawndata) {
	if (OhNoImDead(data, (ObjectData2*)pawndata)) {

		if (pawndata->pawnshield > EggPawnShield::None) {
			char check = 4;

			switch (pawndata->pawnshield) {
			case EggPawnShield::Spike:
				check = 6;
				break;
			case EggPawnShield::Rock:
				check = 10;
				break;
			}

			if (pawndata->damage < check) {
				PlayHeroesSound_Pos(CommonSound_Explosion, &data->Position, 300, 2, false);
				++pawndata->damage;
				return false;
			}
		}

		SpawnAnimal(2, PosToVector(data->Position));
		Score += 100;

		PlayHeroesSound_Pos(CommonSound_Explosion, &data->Position, 300, 5, false);
		LoadObjectBreaker(&data->Position, &data->Rotation, data->Object->child->child, &EGGPAWN_TEXLIST);

		return true;
	}

	return false;
}

void EggPawn_Main(ObjectMaster* obj) {
	if (ClipSetObject(obj)) return; //	Delete the object if too far away

	EntityData1* data = obj->Data1;
	PawnCustomData* pawndata = (PawnCustomData*)obj->Data2;
	float frame = 0;
	
	//Run the logic if close enough
	if (IsPlayerInsideSphere_(&data->Position, 500) && pawndata->pawnanim != EggPawnAnim::DROP2) {

		//	Check if the enemy is destroyed by the player, or a player's object (missiles, hammer...)
		if (EggPawn_CheckDamage(data, pawndata)) {
			UpdateSetDataAndDelete(obj);
			return;
		}

		EggPawn_CanMoveAndRunPushed(data, pawndata);

		switch (data->Action) {
		case EggPawnAction_Stand:
			if (EggPawn_CheckRobotFalling(data, pawndata) == true) { //if the robot isn't falling:
				
				data->Position.y = GetGroundPositionEntity(data, true);

				if (pawndata->miny) {
					data->Position.y = pawndata->miny;
					data->Rotation.x = 0;
					data->Rotation.z = 0;
				}

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

				pawndata->pawnanim = EggPawnAnim::PANIC;

				if (data->Unknown == 1) {
					// Load icon "!"
					PlayHeroesSound_Entity(CommonSound_Detect1, obj, 200, false);
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

				if (pawndata->pawnweapon >= EggPawnWeapon::Pistol) {
					data->Action = EggPawnAction_Aim;
					data->Unknown = 0;
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
		case EggPawnAction_Aim:
			data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[data->CharIndex - 1]->Position).y - 0x4000;

			if (data->Scale.x == 5) data->Action = EggPawnAction_Shoot;

			if (data->Unknown == 0) {
				pawndata->pawnanim = EggPawnAnim::CANNON;
			}
			else {
				pawndata->pawnanim = EggPawnAnim::SEARCH;
			}
			
			break;
		case EggPawnAction_Shoot:
			data->Rotation.y = -fPositionToRotation(&data->Position, &EntityData1Ptrs[data->CharIndex - 1]->Position).y - 0x4000;

			if (pawndata->pawnweapon == EggPawnWeapon::Pistol) {
				if (++data->InvulnerableTime == 100) {
					data->Action = EggPawnAction_Check;
					data->InvulnerableTime = 0;
					data->Unknown = 0;
				}
			}
			else {
				if (++data->InvulnerableTime == 200) {
					data->Action = EggPawnAction_Check;
					data->InvulnerableTime = 0;
					data->Unknown = 0;
				}
			}

			if (data->Unknown == 0) {
				pawndata->pawnanim = EggPawnAnim::CANNON;
			}
			else {
				pawndata->pawnanim = EggPawnAnim::SEARCH;
			}

			break;
		}

		PlayHeroesAnimation(obj, (uint8_t)pawndata->pawnanim, EggPawnAnimData, 0, frame);
		AddToCollisionList(data);
	}
	else {
		if (data->Action == EggPawnAction_Sleep) pawndata->pawnanim = EggPawnAnim::DROP1;
		else pawndata->pawnanim = EggPawnAnim::SEARCH;
	}
	
	obj->DisplaySub(obj);
	RunObjectChildren(obj);
}

inline void EggPawn_LoadFiles() {
	LoadPVM("eggpawn", &EGGPAWN_TEXLIST);
	LoadModelListFuncPtr(arrayptrandlength(EggPawnMdlNames), EggPawnMdls, LoadEnemyModel);
	LoadAnimListFuncPtr(arrayptrandlength(EggPawnAnmNames), EggPawnAnms, LoadEnemyAnim);
	FillAnimDataTable(EggPawnAnms, arrayptrandlength(EggPawnAnimData), nullptr);
}

void EggPawn_Init(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	
	//	If the configs are ok, load an Egg Pawn
	if (Enemies_CanSwap || data->Scale.x == 99) {

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
		pawndata->miny = data->Rotation.z;

		data->Action = pawndata->startaction;

		//	The King type is another model, swap
		if (pawndata->pawntype == EggPawnType::King) {
			data->Object = EggPawnMdls[1]->getmodel();
		}
		else {
			data->Object = EggPawnMdls[0]->getmodel();
		}

		if (pawndata->pawnweapon > EggPawnWeapon::None) {
			LoadChildObject(LoadObj_Data1, EggPawnWeapon_Main, obj);
		}

		if (pawndata->pawnshield > EggPawnShield::None) {
			LoadChildObject(LoadObj_Data1, EggPawnShield_Main, obj);
		}

		EggPawn_SetDefaultTextures();

		//	Reset the object properties
		data->Position.y -= 4;
		data->Rotation = { 0, data->Rotation.y + 0x8000 , 0 };
		data->Scale = { 0, 0, 0 };

		//	Object functions
		obj->MainSub = EggPawn_Main;
		obj->DisplaySub = EggPawn_Display;
	}
	else {
		ObjectFunc(original, KikiTrampoline->Target()); // Otherwise, load a kiki.
		original(obj);
	}
}