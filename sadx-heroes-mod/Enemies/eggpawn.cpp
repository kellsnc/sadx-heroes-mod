#include "stdafx.h"

bool IsEggPawnInitialized;

#define MDL_COUNT 4
#define ANM_COUNT 17

ModelInfo*		EggPawnMdls[MDL_COUNT];
AnimationFile*	EggPawnAnms[ANM_COUNT];
AnimData		EggPawnAnimData[ANM_COUNT];

NJS_TEXNAME EGGPAWN_TEXNAMES[15];
NJS_TEXLIST EGGPAWN_TEXLIST = { arrayptrandlength(EGGPAWN_TEXNAMES) };

CollisionData EggPawnCollisionData[]{
	{0, CollisionShape_Cylinder, 0x77, 0x21, 0x2400, { 0, 0, 0}, {8, 10, 0}},
	{0, CollisionShape_Sphere, 0x77, 0x21, 0x802400, { 0, 10, 0}, {8, 6, 0}}
};

enum EggPawnAnimNames {
	EN_PAWN_AT,
	EN_PAWN_ATRUN,
	EN_PAWN_CANNON,
	EN_PAWN_DAMAGE,
	EN_PAWN_DROP1,
	EN_PAWN_DROP2,
	EN_PAWN_FALL,
	EN_PAWN_PANIC,
	EN_PAWN_RUN,
	EN_PAWN_SEARCH,
	EN_PAWN_SHLDAWAY,
	EN_PAWN_STANDUP,
	EN_PAWN_WALK,
	EN_SUB_CANNON,
	EN_SUB_LANCE,
	EN_SUB_MGUN,
	EN_SUB_SHIELD
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

void EggPawn_LoadFiles() {
	LoadPVM("eggpawn", &EGGPAWN_TEXLIST);
	LoadModelListFuncPtr(arrayptrandlength(EggPawnMdlNames), EggPawnMdls, LoadEnemyModel);
	LoadAnimListFuncPtr(arrayptrandlength(EggPawnAnmNames), EggPawnAnms, LoadEnemyAnim);
	FillAnimDataTable(EggPawnAnms, arrayptrandlength(EggPawnAnimData), EggPawnMdls[0]->getmodel());
}

void EggPawn_Display(ObjectMaster* obj) {
	if (MissedFrames || obj->Data1->Status & 0x200) return;

	EntityData1* data = obj->Data1;
	Direct3D_PerformLighting(6);

	njSetTexture(&EGGPAWN_TEXLIST);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);

	if (data->Action == EggPawnAction_Sleep) njTranslate(0, 0, -2, 0);

	*NodeCallbackFuncPtr = nullptr;
	njAction(EggPawnAnimData[data->Index].Animation, data->Scale.x);
	*NodeCallbackFuncPtr = nullptr;

	njPopMatrix(1);
	Direct3D_PerformLighting(0);

	DrawShadow(data, 2);
}

bool EggPawn_CheckRobotFalling(EntityData1* data) {
	//store the ground height
	float y = GetGroundPositionEntity(data, false);
	y -= data->Position.y;

	//run the object if there is a floor
	if (y < -100000) {
		y = data->Position.y;
		data->Status |= 0x200; //skip drawing
		return false;
	}
	else {
		data->Status &= ~0x200;
	}

	// make the robot fall
	if (y < -3) {
		data->Index = EN_PAWN_DROP2; //anim
		data->Position.y -= 2;
	}
	else if (y < -2) {
		data->Index = EN_PAWN_DROP1; //anim
	}
	else {
		return true;
	}

	return false;
}

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

void EggPawn_WalkArround(EntityData1* data, ObjectData* objdata) {
	if (data->CollisionInfo->CollidingObject == nullptr) {
		NJS_VECTOR front = EggPawn_GetPoint(&data->Position, &data->Rotation, 20, 10);
		Rotation3 rot = { 0, 0, 0 };
		float y = GetGroundYPosition(front.x, front.y, front.z, &rot);
		y -= data->Position.y;

		if (y > -30 && y < 30) {
			data->Position = EggPawn_GetPoint(&data->Position, &data->Rotation, 0.5f, 0);
			data->Index = EN_PAWN_WALK;

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
	}
	else {
		data->Action = EggPawnAction_Check;
	}
}

void EggPawn_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	ObjectData* objdata = (ObjectData*)obj->Data2;

	float frame = 0;

	switch (data->Action) {
	case EggPawnAction_Stand:
		if (EggPawn_CheckRobotFalling(data) == true) { //if the robot isn't falling:
			data->Index = EN_PAWN_SEARCH;
			data->Position.y = GetGroundPositionEntity(data, true);

			EggPawn_WalkArround(data, objdata);

			if (++data->InvulnerableTime > 500) {
				data->Action = EggPawnAction_Check;
				data->InvulnerableTime = 0;
			}

			goto checkplayer;
		}
		
		break;
	case EggPawnAction_Sleep:
		data->Index = EN_PAWN_DROP1;

		if (IsPlayerInsideSphere_(&data->Position, 100)) {
			//icon surprise!

			data->Index = EN_PAWN_PANIC;

			if (data->Unknown == 1) {
				data->Action = EggPawnAction_Check;
			}
		}
		else {
			frame = 1;
		}

		break;
	case EggPawnAction_Check:
		data->Index = EN_PAWN_SEARCH;

		if (++data->InvulnerableTime > 500) {
			data->Rotation.y = rand();
			data->Action = EggPawnAction_Stand;
			data->InvulnerableTime = 0;
			break;
		}

checkplayer:
		data->CharIndex = IsPlayerInsideSphere_(&data->Position, 100);
		if (data->CharIndex) {

			//if the robot has a pistol aim, otherwise run toward player
			if (data->Status > 1) {
				data->Action = EggPawnAction_Aim;
			}
			else {
				data->Action = EggPawnAction_RunToPlayer;
			}

		}

		break;
	}

	PlayHeroesAnimation(obj, data->Index, EggPawnAnimData, 0, frame);

	obj->DisplaySub(obj);
	AddToCollisionList(data);
}

void EggPawn_Init(ObjectMaster* obj) {
	//If the configs are ok, load an Egg Pawn

	if (Enemies_CanSwap) {

		//Load the files only once
		if (IsEggPawnInitialized == false) {
			IsEggPawnInitialized = true;
			EggPawn_LoadFiles();
		}

		EntityData1* data = obj->Data1;
		ObjectData* objdata = (ObjectData*)AllocateObjectData2(obj, data);

		objdata->AnimHead_ptr_A = EggPawnAnimData[EN_PAWN_STANDUP].Animation;

		ObjectData2_SetStartPosition(data, (ObjectData2*)objdata);
		objdata->SomeFlags |= 0x7;
		objdata->field_1D8 = 10000.0;
		objdata->field_1DC = 225.0;
		objdata->field_4.x = 0.0;
		objdata->field_4.z = 0.0;
		objdata->field_9C = 0;
		objdata->field_A0 = 8.0;
		objdata->field_A4 = 0;
		objdata->field_A8 = 6.0;
		objdata->field_AC = 15.0;
		objdata->field_B0 = -8.0;
		objdata->field_C0 = 1.0;
		objdata->field_BC = 0.2;

		Collision_Init(obj, EggPawnCollisionData, 2, 3);

		data->Action = data->Scale.x; //Start action in set file
		data->Status = data->Scale.y;
		if (data->Scale.z) data->Status |= 0x100;
		
		data->Scale = { 0, 0, 0 };
		data->Position.y -= 4;
		data->Rotation.y += 0x8000;

		obj->MainSub = EggPawn_Main;
		obj->DisplaySub = EggPawn_Display;
		obj->DeleteSub = EggPawn_Delete;
	}
	else {
		ObjectFunc(original, KikiTrampoline->Target()); // Otherwise, load a kiki.
		original(obj);
	}
}