#include "stdafx.h"
#include "mod.h"

#define _HIBYTE(x)   (*((Uint8*)&(x)+1))

ObjectMaster * TeamObj;

ObjectFuncPtr TeamSonic[]{
	Sonic_Main,
	Tails_Main,
	Knuckles_Main
};

Rotation3 fPositionToRotation(NJS_VECTOR* orig, NJS_VECTOR* point);
float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);

void AI_Main(ObjectMaster* obj) {
	//the Team Handler gives each AI a point which it must try to reach
	EntityData1* data = obj->Data1;
	ObjectMaster* childobj = (ObjectMaster*)obj->field_30;
	EntityData1* botdata = childobj->Data1;
	EntityData2* botdata2 = (EntityData2*)childobj->Data2;
	CharObj2* botco2 = botdata2->CharacterData;
	EntityData1* playerdata = EntityData1Ptrs[0];

	float dist = GetDistance(&botdata->Position, &data->Scale);
	if (dist > 10) {
		botdata->Rotation.y = fPositionToRotation(&botdata->Position, &data->Scale).y;
		botco2->Speed.x = fmax(dist / 10, CharObj2Ptrs[0]->Speed.x);
	}
}

bool Team_SwapCharacter(ObjectMaster* obj, ObjectMaster* player) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData2* objdata2 = (EntityData2*)player->Data2;
	EntityData1* objdata = player->Data1;

	if (!data2->field_30) {
		if (PressedButtons[0] & Buttons_C) {
			data->Index -= 1;
			data->Unknown += 1;
			if (data->Index < 0) data->Index = 2;
		}

		if (PressedButtons[0] & Buttons_Z) {
			data->Index += 1;
			data->Unknown += 1;
			if (data->Index > 2) data->Index = 0;
		}

		if (data->Unknown % 3 != 0 && (EntityData1Ptrs[0]->Status & Status_Ground) && EntityData1Ptrs[0]->Action < 5) {
			data->Unknown = 0;
			EntityData1Ptrs[0]->Status = Status_Ground;
			data2->field_30 = objdata->Action;
			data2->VelocityDirection = objdata2->CharacterData->Speed;
			player->MainSub = TeamSonic[data->Index];
			player->Data1->Action = 0;
			return true;
		}
	}
	else {
		if (objdata->CollisionInfo) {
			for (short i = 0; i < objdata->CollisionInfo->Count; i++) {
				_HIBYTE(objdata->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;
				objdata->CollisionInfo->CollisionArray[i].field_3 &= ~0x20u;
			}

			objdata->Action = 2;
			objdata2->CharacterData->Speed = data2->VelocityDirection;
			data2->field_30 = 0;
		}
	}

	return false;
}

void SideKickAI(ObjectMaster* obj) {
	//Handle the AI
	EntityData1* data = obj->Data1;
	
	if (data->Action == 0) {
		data->Action = 1;
		
		data->Index = data->CharIndex + 1;
		obj->field_30 = (int)LoadObject((LoadObj)(LoadObj_UnknownA | LoadObj_Data1 | LoadObj_Data2), 3, TeamSonic[data->Index]);
		ObjectMaster* childobj = (ObjectMaster*)obj->field_30;
		EntityData1* childdata = childobj->Data1;
		
		childdata->CharIndex = 6 + data->CharIndex;
	}
	else {
		ObjectMaster* childobj = (ObjectMaster*)obj->field_30;
		EntityData1* childdata = childobj->Data1;

		switch (data->Action) {
		case 1: //Collision flag to avoid getting hurt by player
			if (childdata->CollisionInfo) {
				for (short i = 0; i < childdata->CollisionInfo->Count; i++) {
					_HIBYTE(childdata->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;
					childdata->CollisionInfo->CollisionArray[i].field_3 &= ~0x20u;
				}

				data->Action = 2;
			}
			break;
		case 2: //run the AI
			if (data->Status == 0) {
				childdata->Position = data->Scale;
				childdata->Rotation = EntityData1Ptrs[0]->Rotation;
				data->Status = 1;
			}

			if (Team_SwapCharacter(obj, GetCharacterObject(childdata->CharIndex))) {
				if (IsPlayerInsideSphere(&childdata->Position, 50)) {
					childdata->Position = data->Scale;
					childdata->Rotation = EntityData1Ptrs[0]->Rotation;
				}
			}
			
			AI_Main(obj);

			break;
		}
	}
}

void Team_GetPoints(ObjectMaster* obj) {
	//get the points that the ai must try to reach
	//all sorts of crazy calculations here sorry
	EntityData1* childdata = obj->Child->Data1;
	EntityData1* child2data = obj->Child->Child->Data1;

	EntityData1* data = obj->Data1;
	EntityData1* playerdata = EntityData1Ptrs[0];
	uint8_t formation = data->Index;
	NJS_VECTOR point;
	NJS_VECTOR point2;

	NJS_VECTOR dir = { -10, 0, 0 };
	njPushMatrix(_nj_unit_matrix_);
	njTranslateV(0, &playerdata->Position);
	njRotateY(0, -playerdata->Rotation.y);
	njCalcPoint(0, &dir, &point);

	if (formation == 0) {
		childdata->Scale = point;
		dir.x = -20;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	if (formation == 1) {
		dir.x = -8; dir.z = -2.5f;
		njCalcPoint(0, &dir, &point);
		childdata->Scale = point;
		dir.x = -16; dir.z = 2.5f;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	if (formation == 2) {
		dir.x = -5; dir.z = -8;
		njCalcPoint(0, &dir, &point);
		childdata->Scale = point;
		dir.z = 8;
		njCalcPoint(0, &dir, &child2data->Scale);
	}

	njPopMatrix(1u);
}

void TeamHandler_Delete(ObjectMaster* obj) {
	TeamObj = nullptr;
}

void TeamHandler_Display(ObjectMaster* obj) {
	//Display the HUD
}

void TeamHandler(ObjectMaster* obj) {
	//Handle the character change, the hud, teamblast, etc
	EntityData1* data = obj->Data1;

	if (data->Action == 0) {
		data->Action = 1;
		obj->DeleteSub = TeamHandler_Delete;
		obj->DisplaySub = TeamHandler_Display;

		//Load our two AI
		ObjectMaster* child = LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, obj);
		LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, child)->Data1->CharIndex = 1;
	}
	else {
		if (GetCharacterObject(1)) DeleteObject_(GetCharacterObject(1));
		EntityData1* MainPlayer = EntityData1Ptrs[0];
		
		switch (data->Action) {
		case 1: //we make sonic's collision not offensive to our AIs
			if (MainPlayer->CollisionInfo) {
				for (short i = 0; i < MainPlayer->CollisionInfo->Count; i++)
					_HIBYTE(MainPlayer->CollisionInfo->CollisionArray[i].field_2) &= 0xDFu;

				data->Action = 2;
			}
			break;
		case 2: //check for playerswap, and swap
			Team_SwapCharacter(obj, GetCharacterObject(0));
			Team_GetPoints(obj);

			break;
		}

		RunObjectChildren(obj);
		RunObjectChildren(obj->Child);
	}
}

void GamePlay_OnFrame() {
	if (!TeamObj && GetCharacterID(0) == Characters_Sonic) 
		TeamObj = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, TeamHandler);
}