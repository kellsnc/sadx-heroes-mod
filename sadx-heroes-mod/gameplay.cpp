#include "stdafx.h"
#include "mod.h"

#define _HIBYTE(x)   (*((Uint8*)&(x)+1))

ObjectMaster * TeamObj;

ObjectFuncPtr TeamSonic[]{
	Sonic_Main,
	Tails_Main,
	Knuckles_Main
};

void Team_SwapCharacter(ObjectMaster* obj, ObjectMaster* player) {
	EntityData1* data = obj->Data1;
	EntityData2* data2 = (EntityData2*)obj->Data2;
	EntityData2* objdata2 = (EntityData2*)player->Data2;
	EntityData1* objdata = player->Data1;

	if (EntityData1Ptrs[0]->Action > 5) return;

	if (!data2->field_30) {
		if (PressedButtons[0] & Buttons_C) {
			data->Index -= 1;
			if (data->Index < 0) data->Index = 2;
		}

		if (PressedButtons[0] & Buttons_Z) {
			data->Index += 1;
			if (data->Index > 2) data->Index = 0;
		}

		if (PressedButtons[0] & Buttons_C || PressedButtons[0] & Buttons_Z) {
			EntityData1Ptrs[0]->Status = Status_Ground;
			data2->field_30 = objdata->Action;
			data2->VelocityDirection = objdata2->CharacterData->Speed;
			player->MainSub = TeamSonic[data->Index];
			player->Data1->Action = 0;
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
}

void AI_Main(ObjectMaster* obj) {
	//the Team Handler gives each AI a point which it must try to reach
	
}

void SideKickAI(ObjectMaster* obj) {
	//Handle the AI
	EntityData1* data = obj->Data1;
	
	if (data->Action == 0) {
		data->Action = 1;
		
		data->Index = data->CharIndex + 1;
		LoadChildObject((LoadObj)(LoadObj_UnknownA | LoadObj_Data1 | LoadObj_Data2), TeamSonic[data->Index], obj);
		EntityData1* childdata = obj->Child->Data1;
		
		childdata->CharIndex = 6 + data->CharIndex;
		childdata->Position = { 500, 4230, 5320 };
	}
	else {
		EntityData1* childdata = obj->Child->Data1;

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
			Team_SwapCharacter(obj, GetCharacterObject(childdata->CharIndex));
			AI_Main(obj);

			break;
		}
		
		RunObjectChildren(obj);
	}
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
		LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, obj);
		LoadChildObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), SideKickAI, obj)->Data1->CharIndex = 1;
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

			break;
		}

		RunObjectChildren(obj); //Run the ai for this frame;
	}
}

void GamePlay_OnFrame() {
	if (!TeamObj && GetCharacterID(0) == Characters_Sonic) 
		TeamObj = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 1, TeamHandler);
}