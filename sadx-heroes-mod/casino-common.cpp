#include "stdafx.h"
#include "mod.h"
#include "levels.h"

extern uint8_t SlotState = 0;

extern bool NoPinball;
extern bool inSlot;

bool IsRolling = false;
bool IsPinRolling = false;

ModelInfo * CP_SKYMDLS;

void CasinoSkybox(ObjectMaster *a1) {
	if (!MissedFrames) {
		if (a1->Data1->Action == 0) {
			a1->Data1->Action = 1;
			a1->DisplaySub = a1->MainSub;
			HeroesSkybox_Main(a1);
		}
		
		DisableFog();
		njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
		njPushMatrix(0);
		njTranslateV(nullptr, &EntityData1Ptrs[0]->Position);
		njScale(0, 0.83f, 0.83f, 0.83f);
		DrawQueueDepthBias = -6000;
		njDrawModel_SADX(CP_SKYMDLS->getmodel()->basicdxmodel);
		DrawQueueDepthBias = 0;
		njPopMatrix(1u);
		ToggleStageFog();
	}
}

void PinTableCamera(EntityData1 *entity) {
	if (!inSlot && !IsRolling) {
		camera_flags = 6;
		DoSomethingWithFOV(18473);
		if (CurrentLevel == 3) {
			Camera_Data1->Position.x = entity->Position.x;
			Camera_Data1->Position.y = entity->Position.y;
			Camera_Data1->Position.z = entity->Position.z;

			if (entity->Position.z > -1419 && entity->Position.x > -8208) Camera_Data1->Position.z = entity->Position.z + 100;
			else if (entity->Position.x > -9698 && entity->Position.y > 2161) { Camera_Data1->Position.x = entity->Position.x + 100; Camera_Data1->Position.y = entity->Position.y + 150; }
			else if (entity->Position.x > -14755 && entity->Position.z < -867) Camera_Data1->Position.x = entity->Position.x + 100;
			else if (entity->Position.x > -15614 && entity->Position.z < 1397) Camera_Data1->Position.z = entity->Position.z - 100;
			else if (entity->Position.z < 3830 && entity->Position.y > 1138) { Camera_Data1->Position.z = entity->Position.z - 100; Camera_Data1->Position.y = entity->Position.y + 150; }
			else Camera_Data1->Position.z = entity->Position.z - 100;
		}
		else {
			if ((entity->Position.z < -2554 && entity->Position.z > -3526)
				|| (entity->Position.z < -11416 && entity->Position.z > -13670 && entity->Position.y > -4375)
				|| (entity->Position.z < -17219 && entity->Position.z > -18293 && entity->Position.x > 15942)
				|| (entity->Position.z < -24260)) {
				Camera_Data1->Position.x = entity->Position.x;
				Camera_Data1->Position.y = entity->Position.y;
				Camera_Data1->Position.z = entity->Position.z + 100;
			}
			else {
				Camera_Data1->Position.y = entity->Position.y + 80;
				CharObj2 * co2 = CharObj2Ptrs[0];
				co2->PhysicsData.Gravity = 0.2f;
			}
		}
	}
}

void PinTablePhysics() {
	for (uint8_t i = 0; i < 8; ++i) {
		EntityData1 *entity = EntityData1Ptrs[i];
		CharObj2 * co2 = CharObj2Ptrs[i];

		if (i == 0) {
			if (entity && co2) {
				if (!NoPinball && ((GetCharacterID(i) == Characters_Sonic && (co2->Upgrades & Upgrades_SuperSonic) == 0) || GetCharacterID(i) == Characters_Tails)) {
					/* On pintable */
					if (co2->SurfaceFlags == 0x81 && EntityData1Ptrs[0] == entity) IsPinRolling = true;

					if (IsPinRolling) {
						PinTableCamera(entity);
						IsPinRolling = true;
						entity->Status = Status_Ball;
						if (GetCharacterID(i) == Characters_Sonic) {
							LoadSonicDashTrail(entity);
							entity->Action = 62;
							co2->AnimationThing.Index = 125;
						}
						if (GetCharacterID(i) == Characters_Tails) {
							entity->Action = 48;
							co2->AnimationThing.Index = 4;
						}
					}

					/* Out of pintable */
					if ((anim % 11 == 0 && entity->Rotation.x == 0 && IsPinRolling && co2->SurfaceFlags != 0x81)
						|| (entity->NextAction == 1)) {
						SetCameraMode_(1);
						IsPinRolling = false;
						IsRolling = false;

						if (entity->NextAction != 1) {
							entity->Status = 0;
							entity->Action = 1;
							co2->AnimationThing.Index = 0;
						}

						DoSomethingWithFOV(12743);
					}

					if (IsRolling && entity->Status == Status_Ground && co2->SurfaceFlags == 0) IsRolling = false;
				}
			}
		}
		else {
			if (co2) {
				if (co2->SurfaceFlags == 0x81) {
					entity->Status = Status_Ball;
					if (GetCharacterID(i) == Characters_Sonic && (CharObj2Ptrs[i]->Upgrades & Upgrades_SuperSonic) == 0) {
						LoadSonicDashTrail(entity);
						entity->Action = 62;
						co2->AnimationThing.Index = 125;
					}
					if (GetCharacterID(i) == Characters_Tails) {
						entity->Action = 48;
						co2->AnimationThing.Index = 4;
					}
				}
				else {
					if (entity->Action > 61 && entity->Action < 69) entity->Action = 2;
				}
			}
		}
	}
}

void CasinoCommon_Delete(ObjectMaster * a1) {
	IsPinRolling == false;
	IsRolling == false;
	LevelHandler_Delete(a1);
}

void CasinoCommon_OnFrame() {
	EntityData1 *entity = EntityData1Ptrs[0];
	CharObj2 *co2 = CharObj2Ptrs[0];

	if (anim % 250 == 0) SlotState += 1;
	if (SlotState == 6) SlotState = 0;

	if (inSlot) co2->Speed = { 0, 3, 0 };

	if (CurrentLandTable) PinTablePhysics();
}