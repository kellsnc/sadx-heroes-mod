#include "stdafx.h"

NJS_OBJECT* PropellerModel = &Sphere_Model;

enum PropellerActions {
	PropellerAction_Init,
	PropellerAction_CheckForPlayer,
	PropellerAction_Attached
};

void Propeller_Delete(ObjectMaster* obj) {
	if (obj->Parent) {
		obj->Parent->Data1->Action = PropellerAction_CheckForPlayer;
	}
}

void Propeller_Display(ObjectMaster* obj) {
	if (!MissedFrames && IsPlayerInsideSphere(&obj->Data1->Position, 1000)) {
		EntityData1* data = obj->Data1;
		
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(0, &data->Position);

		njRotateY(nullptr, data->Rotation.y);
		njRotateX(0, data->Rotation.x);

		njDrawModel_SADX(data->Object->basicdxmodel);
		njPopMatrix(1u);
	}
}

void Propeller_Main(ObjectMaster* obj) {
	if (!ClipSetObject(obj)) {
		EntityData1* data = obj->Data1;
		EntityData1* player = EntityData1Ptrs[data->CharIndex];
		LoopHead* path = (LoopHead*)data->LoopData;
		int point = data->InvulnerableTime;

		PathHandler(data, path, 9);

		if (point < path->Count - 3) {
			player->Position = data->Position;
			player->Rotation = data->Rotation;
		}
		else if (point == path->Count) {
			DeleteObject_(obj);
			return;
		}
		
		obj->DisplaySub(obj);
	}
}

inline void Propeller_Init(ObjectMaster* obj, char player) {
	ObjectMaster* child = LoadChildObject(LoadObj_Data1, Propeller_Main, obj);
	child->Data1->CharIndex = player - 1;
	child->Data1->LoopData = obj->Data1->LoopData;
	child->Data1->Position = obj->Data1->Position;
	child->Data1->Object = obj->Data1->Object;
	child->DisplaySub = obj->DisplaySub;
	child->DeleteSub = Propeller_Delete;
}

void PropellerPath(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;
	LoopHead* path = (LoopHead*)data->LoopData;

	char player = 0;

	switch (data->Action) {
	case PropellerAction_Init:
		obj->DisplaySub = Propeller_Display;
		data->Object = PropellerModel;
		data->Action = PropellerAction_CheckForPlayer;
		data->Position = path->LoopList->Position;

		break;
	case PropellerAction_CheckForPlayer:
		if (IsPlayerInsideSphere(&obj->Data1->Position, 1000)) {
			player = IsPlayerInsideSphere_(&data->Position, 10);

			if (player) {
				Propeller_Init(obj, player);
				data->Action = PropellerAction_Attached;
			}

			obj->DisplaySub(obj);
		}

		break;
	case PropellerAction_Attached:
		RunObjectChildren(obj);
		break;
	}
}