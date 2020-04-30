#include "stdafx.h"
#include "mystic-common-objects.h"

ModelInfo * MM_MOVPLAT;
ModelInfo * MM_MYSDOOR;
ModelInfo * MM_MYSWALL;
ModelInfo * HC_HFLAMES;
ModelInfo * HC_SPKWARP;
ModelInfo * HC_SPKDOOR;

float hclight = 0;

void HCWarp_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, 0, 1);
		njScale(nullptr, 1.5f, 1.5f, 1.5f);
		DrawQueueDepthBias = -6000.0f;
		njDrawModel_SADX(HC_SPKWARP->getmodel()->basicdxmodel);
		
		if (a1->Data1->Action < 2) {
			njTranslate(0, 0, 8, 0);
			njRotateY(0, a1->Data1->Rotation.y);
			njDrawModel_SADX(HC_SPKWARP->getmodel()->child->basicdxmodel);
			njDrawModel_SADX(HC_SPKWARP->getmodel()->child->child->basicdxmodel);
		}
		else {
			njTranslate(0, 0, 5, 0);
			njDrawModel_SADX(HC_SPKWARP->getmodel()->child->basicdxmodel);
		}

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCWarp_Main(ObjectMaster *obj) {
	EntityData1* data = obj->Data1;

	if (!ClipSetObject(obj)) {
		ObjectData2 *od2 = (ObjectData2*)obj->Data2;

		if (data->Action == 0) {
			data->Rotation.y += 100;

			od2->vector_a = data->Position;
			od2->vector_a.y += 10;

			if (GetCollidingEntityA(data) || GetCollidingEntityB(data)) {
				PlayCustomSound(LevelSound_Mys_Warp1);
				EntityData1 *entity = EntityData1Ptrs[0];
				od2->vector_a = entity->Position;
				data->Action = 1;
			}
		}

		if (data->Action == 1) {
			EntityData1 *entity = EntityData1Ptrs[0];
			entity->Position = od2->vector_a;
			if (data->Rotation.z == 2) od2->vector_a.y += 0.05f;
			else od2->vector_a.y -= 0.05f;

			entity->Status = 0;
			CharObj2 * co2 = CharObj2Ptrs[0];
			if (GetCharacterID(0) == Characters_Sonic) {
				if ((co2->Upgrades & Upgrades_SuperSonic) == 0) co2->AnimationThing.Index = 18;
				else co2->AnimationThing.Index = 141;
			}
			else if (GetCharacterID(0) == Characters_Tails || GetCharacterID(0) == Characters_Knuckles) {
				co2->AnimationThing.Index = 19;
			}
			if (data->NextAction < 100) {
				data->NextAction += 1;

				if (IsLantern) {
					set_specular_blend_ptr(0, 4);
					set_specular_blend_ptr(1, 4);
					set_specular_blend_ptr(2, 4);
					set_specular_blend_ptr(3, 4);
					set_specular_blend_ptr(7, 4);
				}
				
				hclight = data->NextAction;
				hclight /= 100;
				if (IsLantern) set_specular_blend_factor_ptr(hclight);
			}
			else {
				if (data->Scale.x != 0) {

					if (CurrentLevel == HeroesLevelID_MysticMansion && data->Rotation.z == 2) {
						entity->Position.x += data->Scale.x;
						entity->Position.y += data->Scale.y;
						entity->Position.z += data->Scale.z;
					}
					else {
						entity->Position = data->Scale;
					}

					PlayCustomSound(LevelSound_Mys_Warp2);
					if (data->Rotation.z == 1) Camera_Data1->Position = entity->Position;
				}
				data->Action = 2;
			}
		}

		if (data->Action == 2) {
			if (hclight > 0) {
				hclight -= 0.05f;
				if (IsLantern) set_specular_blend_factor_ptr(hclight);
			}
			else {
				if (data->Rotation.z == 2) data->Action = 0;
				else data->Action = 3;
				if (IsLantern) set_blend_ptr(-1, -1);
			}
		}

		AddToCollisionList(data);
		HCWarp_Display(obj);
	}
}

void HCWarp(ObjectMaster *a1)
{
	if (CurrentLevel == 10) {
		HC_SPKWARP->getmodel()->basicdxmodel->mats[0].attr_texId = 154;
		HC_SPKWARP->getmodel()->child->basicdxmodel->mats[0].attr_texId = 154;
		HC_SPKWARP->getmodel()->child->child->basicdxmodel->mats[0].attr_texId = 155;
	}
	else {
		HC_SPKWARP->getmodel()->basicdxmodel->mats[0].attr_texId = 98;
		HC_SPKWARP->getmodel()->child->basicdxmodel->mats[0].attr_texId = 98;
		HC_SPKWARP->getmodel()->child->child->basicdxmodel->mats[0].attr_texId = 99;
	}

	Collision_Init(a1, Warps_col, 2, 2u);
	if (a1->Data1->Rotation.y == 1) a1->Data1->Action = 2;
	AllocateObjectData2(a1, a1->Data1);

	a1->MainSub = &HCWarp_Main;
	a1->DisplaySub = &HCWarp_Display;
	a1->DeleteSub = &DynCol_Delete;
}

void HCDoor_Display(ObjectMaster *a1) {
	if (!MissedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		njTranslate(0, -40, 0, 0);

		NJS_OBJECT * object;
		if (CurrentLevel == HeroesLevelID_HangCastle) {
			object = HC_SPKDOOR->getmodel();
			njTranslate(0, 10.2f, 0, 0);
		} 
		else object = MM_MYSDOOR->getmodel();

		if (a1->Data1->Action == 0) {
			njDrawModel_SADX(object->basicdxmodel);
		}
		else if (a1->Data1->Action == 2) {
			njRotateY(0, (a1->Data1->NextAction * 130));
			njDrawModel_SADX(object->basicdxmodel);
			njRotateY(0, -(a1->Data1->NextAction * 130));
		}
		else if (a1->Data1->Action == 3) {
			njRotateY(0, 13000);
			njDrawModel_SADX(object->basicdxmodel);
			njRotateY(0, -13000);
		}

		njTranslate(0, 80, 0, 0);
		if (CurrentLevel == HeroesLevelID_HangCastle) njTranslate(0, -20.4f, 0, 0);
		
		if (a1->Data1->Action == 0) {
			njDrawModel_SADX(object->child->basicdxmodel);
		}
		else if (a1->Data1->Action == 2) {
			njRotateY(0, -(a1->Data1->NextAction * 130));
			njDrawModel_SADX(object->child->basicdxmodel);
			njRotateY(0, (a1->Data1->NextAction * 130));
		}
		else if (a1->Data1->Action == 3) {
			njRotateY(0, -13000);
			njDrawModel_SADX(object->child->basicdxmodel);
			njRotateY(0, 13000);
		}

		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCDoor_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action == 0) {
			DynColRadius(a1, 100, 0);

			if (a1->Data1->Scale.y == 0) {
				if (IsSwitchPressed(a1->Data1->Scale.x))
					a1->Data1->Action = 1;
			}
			else {
				if (IsPlayerInsideSphere(&a1->Data1->Scale, 100))
					a1->Data1->Action = 1;
			}
		}

		if (a1->Data1->Action == 1) {
			a1->Data1->Action = 2;
			DynCol_Delete(a1);
			PlayCustomSound(LevelSound_Mys_Door1);
		}

		if (a1->Data1->Action == 2) {
			if (a1->Data1->NextAction < 100) a1->Data1->NextAction += 2;
			else {
				a1->Data1->Action = 3;
				PlayCustomSound(LevelSound_Mys_Door2);
			}
		}

		a1->DisplaySub(a1);
	}
}

void HCDoor(ObjectMaster* a1)
{
	a1->Data1->Object = &CP_DOORCOL;
	if (a1->Data1->Scale.z == 1) a1->Data1->Action = 2;

	a1->MainSub = HCDoor_Main;
	a1->DisplaySub = HCDoor_Display;
	a1->DeleteSub = DynCol_Delete;
}

void HCTorch_Display(ObjectMaster *a1) {
	if (a1->Data1->Action != 0 && a1->Data1->Action != 3 && a1->Data1->Action != 4) DrawObjModel(a1, a1->Data1->Object->basicdxmodel, false);

	if (a1->Data1->Action < 5) {
		if (a1->Data1->Action == 0 || a1->Data1->Action == 3) {
			if (!IsPlayerInsideSphere(&a1->Data1->Position, 300)) return;
		}

		//replace will billboard sprite somehow
		if (!MissedFrames) {
			SetHeroesLeveltex();
			njPushMatrix(0);
			njTranslateV(0, &a1->Data1->Position);
			njRotateXYZ(nullptr, a1->Data1->Rotation.x, Camera_Data1->Rotation.y, a1->Data1->Rotation.z);
			njTranslate(0, 0, 33, 0);
			njScale(nullptr, a1->Data1->Scale.z, a1->Data1->Scale.z, a1->Data1->Scale.z);
			DrawQueueDepthBias = -9000.0f;
			if (a1->Data1->Action == 2 || a1->Data1->Action == 3) njDrawModel_SADX(HC_HFLAMES->getmodel()->child->basicdxmodel);
			else njDrawModel_SADX(HC_HFLAMES->getmodel()->basicdxmodel);
			DrawQueueDepthBias = 0;
			njPopMatrix(1u);
		}
	}
}

void HCTorch_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action != 0 && a1->Data1->Action != 3 && a1->Data1->Action != 4)
			DynColRadius(a1, 30, 0);

		if (a1->Data1->Action == 0 || a1->Data1->Action == 3) {
			if (IsPlayerInsideSphere(&a1->Data1->Position, 150)) {
				if (a1->Data1->Scale.z < 1) a1->Data1->Scale.z += 0.1f;
			}
			else {
				if (a1->Data1->Scale.z > 0) a1->Data1->Scale.z -= 0.05f;
			}
		}

		HCTorch_Display(a1);
	}
}

void HCTorch(ObjectMaster *a1)
{
	HC_HFLAMES->getmodel()->basicdxmodel->mats[0].attr_texId = 55;
	HC_HFLAMES->getmodel()->child->basicdxmodel->mats[0].attr_texId = 71;

	a1->Data1->Action = a1->Data1->Scale.x;
	if (a1->Data1->Action == 0) a1->Data1->Position.y -= 20;
	else if (a1->Data1->Action == 3) a1->Data1->Position.y -= 30;
	else if (a1->Data1->Action == 4) {
		a1->Data1->Scale.z = 6;
		if (a1->Data1->Rotation.x != 0xFFFF8000) a1->Data1->Position.y -= 5;
		else a1->Data1->Position.y += 10;
	}
	else {
		a1->Data1->Object = &HC_TORCH;
		a1->Data1->Scale.z = 2;
	}

	a1->MainSub = HCTorch_Main;
	a1->DisplaySub = HCTorch_Display;
	a1->DeleteSub = DynCol_Delete;
}

void HCWall_Display(ObjectMaster *a1)
{
	if (!MissedFrames && a1->SETData.SETData->SETEntry->Properties.z != 1) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;

		if (a1->Data1->Action == 0) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_CLOSE2);
				else njDrawModel_SADX(&HC_WALL_CLOSE);
			}
		}
		else if (a1->Data1->Action == 1) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->child->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_OPEN2);
				else njDrawModel_SADX(&HC_WALL_OPEN);
			}
			njTranslate(0, 0, -(a1->Data1->Scale.y / 3), 0);

			njTranslate(nullptr, 0, 0, 12 + a1->Data1->Scale.y / 90);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
			njTranslate(nullptr, 0, 40, 0);

			njTranslate(nullptr, 0, -(a1->Data1->Scale.y), (a1->Data1->Scale.y / 3));
			njRotateY(0, 0x5000 + a1->Data1->Scale.y * 5);
			njScale(0, 0.8f, 0.8f, 0.8f);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
			njRotateY(0, -(0x5000 + a1->Data1->Scale.y * 5));

			njTranslate(nullptr, 0, -(a1->Data1->Scale.y * 2), -(a1->Data1->Scale.y / 5));
			njRotateY(0, -0x3000);
			njScale(0, 0.6f, 0.6f, 0.6f);
			if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_PIECE2);
			else njDrawModel_SADX(&HC_WALL_PIECE);
		}
		else if (a1->Data1->Action == 2) {
			if (CurrentLevel == 10) {
				njDrawModel_SADX(MM_MYSWALL->getmodel()->child->basicdxmodel);
			}
			else {
				if (a1->Data1->Scale.x == 1) njDrawModel_SADX(&HC_WALL_OPEN2);
				else njDrawModel_SADX(&HC_WALL_OPEN);
			}
		}
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCWall_Main(ObjectMaster *a1)
{
	if (!ClipSetObject(a1)) {
		if (a1->Data1->Action == 0) {
			if (OhNoImDead(a1->Data1, (ObjectData2*)a1->Data2)) {
				DynCol_Delete(a1);
				PlayCustomSound(CommonSound_BoxBreak);
				a1->Data1->Action = 1;
				a1->Data1->Object->pos[1] = 0;
			}
			else {
				DynColRadius(a1, 100, 0);
				AddToCollisionList(a1->Data1);
			}
		}

		if (a1->Data1->Action == 1) {
			if (a1->Data1->Scale.y < 100) a1->Data1->Scale.y++;
			else a1->Data1->Action = 2;

			if (a1->Data1->Scale.z != 0) SetSwitchPressedState(a1->Data1->Scale.z, 1);
		}
		HCWall_Display(a1);
	}
}

void HCWall(ObjectMaster *a1)
{
	a1->Data1->Object = &CP_DOORCOL;
	if (a1->Data1->Scale.z == 0) {
		AllocateObjectData2(a1, a1->Data1);
		Collision_Init(a1, &HCWall_col, 1, 5u);
	}
	else {
		if (a1->Data1->Scale.z == 255 || IsSwitchPressed(a1->Data1->Scale.z)) {
			a1->Data1->Action = 2;
		}
		else {
			AllocateObjectData2(a1, a1->Data1);
			Collision_Init(a1, &HCWall_col, 1, 2u);
		}
	}

	a1->MainSub = HCWall_Main;
	a1->DisplaySub = HCWall_Display;
	a1->DeleteSub = DynCol_Delete;
}

void HCPlatform_Display(ObjectMaster *a1) {
	if (!DroppedFrames) {
		SetHeroesLeveltex();
		njPushMatrix(0);
		njTranslateV(0, &a1->Data1->Position);
		njRotateXYZ(nullptr, a1->Data1->Rotation.x, a1->Data1->Rotation.y, a1->Data1->Rotation.z);
		DrawQueueDepthBias = -6000.0f;
		if (CurrentLevel == 7 && a1->Data1->Position.x < 8000) njRotateX(0, 0x8000);
		njDrawModel_SADX(a1->Data1->Object->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
	}
}

void HCPlatform_Main(ObjectMaster *a1) {
	if (!ClipSetObject(a1)) {
		uint8_t type = (uint8_t)a1->Data1->Action;

		if (type != 0) {
			DynColRadiusAuto(a1, 1);
			
			if (anim % 80 == 0) if (envsounds) PlayCustomSound_Entity(LevelSound_Mys_MvPltf, a1, 200, 0);
			if (type == 1) {
				char timer = a1->Data1->NextAction;
				if (timer == 0) {
					float state = a1->Data1->Rotation.z;
					float min = a1->Data1->Scale.y;
					float max = a1->Data1->Scale.z;
					float pos = a1->Data1->Position.y;

					if (state == 0) {
						if (pos < max) a1->Data1->Position.y += 0.5f;
						else {
							a1->Data1->Position.y = max;
							a1->Data1->Rotation.z = 1;
							a1->Data1->NextAction = 10;
						}

					}
					else if (state == 1) {
						if (pos > min) a1->Data1->Position.y -= 0.5f;
						else {
							a1->Data1->Position.y = min;
							a1->Data1->Rotation.z = 0;
							a1->Data1->NextAction = 10;
						}
					}
				}
				else a1->Data1->NextAction--;
			}
			else if (type > 1) {
				ObjectData2 * od2 = (ObjectData2*)a1->Data2;
				NJS_VECTOR orig = od2->vector_a;
				NJS_VECTOR dest = od2->vector_b;

				od2->vector_c = a1->Data1->Position; //get the position before transform

				//some bad things to get a movement speed
				float dist = (2 * (dest.x - orig.x) + 2 * (dest.y - orig.y) + 2 * (dest.z - orig.z)) / 2;
				od2->field_C4 += 1 / (abs(dist) * 2);

				//translate the position
				a1->Data1->Position.x = (dest.x - orig.x) * od2->field_C4 + orig.x;
				a1->Data1->Position.y = (dest.y - orig.y) * od2->field_C4 + orig.y;
				a1->Data1->Position.z = (dest.z - orig.z) * od2->field_C4 + orig.z;

				//if we've reached the destination, swap dest and orig
				if (od2->field_C4 > 1) {
					od2->field_C4 = 0;
					a1->Data1->Scale = od2->vector_b;
					od2->vector_b = od2->vector_a;
					od2->vector_a = a1->Data1->Scale;
				}

				//inacurate check to get if the character is on the platform
				EntityData1 *entity = EntityData1Ptrs[0];

				if (entity->Position.x > a1->Data1->Position.x - 30
					&& entity->Position.x < a1->Data1->Position.x + 30
					&& entity->Position.z > a1->Data1->Position.z - 30
					&& entity->Position.z < a1->Data1->Position.z + 30
					&& entity->Position.y > a1->Data1->Position.y
					&& entity->Position.y < a1->Data1->Position.y + 30) {

					//use the old and new position to offset the character
					entity->Position.x += a1->Data1->Position.x - od2->vector_c.x;
					entity->Position.y += a1->Data1->Position.y - od2->vector_c.y;
					entity->Position.z += a1->Data1->Position.z - od2->vector_c.z;
				}
			}
		}
		else {
			DynColRadiusAuto(a1, 0);
		}

		//Dynamic collision position
		NJS_OBJECT * col = (NJS_OBJECT*)a1->Data1->LoopData;

		if (col) {
			col->pos[0] = a1->Data1->Position.x;
			col->pos[1] = a1->Data1->Position.y;
			col->pos[2] = a1->Data1->Position.z;
		}
		
		a1->DisplaySub(a1);
	}
}

void HCPlatform(ObjectMaster *a1)
{
	a1->Data1->Object = &HC_PLATFORM;
	if (a1->Data1->Rotation.y == 1) a1->Data1->Object = MM_MOVPLAT->getmodel();

	if (CurrentLevel == 10) {
		matlist_8D606F400D46546E108[0].attr_texId = 24;
		matlist_8D606F400D46546E108[2].attr_texId = 101;
		matlist_8D606F400D46546E108[3].attr_texId = 32; //11
		matlist_8D606F400D46546E108[4].attr_texId = 24; //25
		matlist_8D606F400D46546E108[5].attr_texId = 24;
		matlist_8D606F400D46546E108[6].attr_texId = 101;
	}
	else {
		matlist_8D606F400D46546E108[0].attr_texId = 9;
		matlist_8D606F400D46546E108[2].attr_texId = 10;
		matlist_8D606F400D46546E108[3].attr_texId = 11;
		matlist_8D606F400D46546E108[4].attr_texId = 25;
		matlist_8D606F400D46546E108[5].attr_texId = 25;
		matlist_8D606F400D46546E108[6].attr_texId = 32;
	}

	if (a1->Data1->Scale.y == 0) a1->Data1->Action = 0;
	else if (a1->Data1->Scale.x == 0) a1->Data1->Action = 1;
	else {
		/* if all the parameters are set, the platform goes in all directions */

		a1->Data1->Action = 2;
		AllocateObjectData2(a1, a1->Data1);  //we need more object variables
		ObjectData2 * od2 = (ObjectData2*)a1->Data2;
		od2->vector_a = a1->Data1->Position; //orig
		od2->vector_b = a1->Data1->Scale; //dest
	}

	a1->MainSub = HCPlatform_Main;
	a1->DisplaySub = HCPlatform_Display;
	a1->DeleteSub = DynCol_Delete;
}