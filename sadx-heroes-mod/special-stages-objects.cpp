#include "stdafx.h"
#include "mod.h"
#include "objects.h"

ModelInfo * SS_SSWAVES;

SOI_LIST2 FluidWaves[]{
	{0},{0, {500, 0, 0}},{0, {1000, 0, 0}},{0, {1500, 0, 0}},{0, {2000, 0, 0}},{0, {2500, 0, 0}},{0, {3000, 0, 0}},{0, {3500, 0, 0}},{0, {4000, 0, 0}},{0, {4500, 0, 0}},{0, {5000, 0, 0}},{0, {5500, 0, 0}},{0, {6000, 0, 0}},{0, {6500, 0, 0}},{0, {7000, 0, 0}},{0, {7500, 0, 0}},
	{5, {8005, 0, 1487},{0, -2730, 0}},{5, {8005, 0, 1487}, {0, -8330, 0}},{5, {8005, 0, 1467}, {0, -13950, 0}},{5, {8005, 0, 1467}, {0, -19270, 0}},{5, {8020, 0, 1488}, {0, -24793, 0}},{5, {8020, 0, 1496}, {0, -30253, 0}},
	{0, {8000, 0, 3000},{0, 0x8000, 0}},{0, {7500, 0, 3000}, {0, 0x8000, 0}},{0, {7000, 0, 3000}, {0, 0x8000, 0}},{0, {6500, 0, 3000}, {0, 0x8000, 0}},{0, {6000, 0, 3000}, {0, 0x8000, 0}},{0, {5500, 0, 3000}, {0, 0x8000, 0}},{0, {5000, 0, 3000}, {0, 0x8000, 0}},{0, {4500, 0, 3000}, {0, 0x8000, 0}},
	{5, {4500, 0, 4250},{0, 2000, 0}},{5, {4500, 0, 4250}, {0, 9000, 0}},{5, {4500, 0, 4250}, {0, 13000, 0}},{5, {4500, 0, 4250}, {0, 19000, 0}},
	{0, {4000, 0, 5000}},{0, {4500, 0, 5000}},{0, {5000, 0, 5000}},{0, {5500, 0, 5000}}
};

void SSWaves_Main(ObjectMaster *a1) {
	if (!DroppedFrames) {

		for (int i = 0; i < LengthOfArray(FluidWaves); ++i) {
			if (CheckModelDisplay2(FluidWaves[i])) {
				SOI_LIST2 item = FluidWaves[i];

				njSetTexture((NJS_TEXLIST*)CurrentLevelTexlist);
				njPushMatrix(0);
				njTranslate(nullptr, item.Position.x, item.Position.y, item.Position.z);
				njRotateXYZ(nullptr, item.Rotation[0], item.Rotation[1], item.Rotation[2]);
				DrawQueueDepthBias = -6000;

				switch (item.Model) {
				case 0: njDrawModel_SADX(SS_SSWAVES->getmodel()->basicdxmodel); break;
				case 1: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->basicdxmodel); break;
				case 2: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->basicdxmodel); break;
				case 3: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->child->basicdxmodel); break;
				case 4: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->child->child->basicdxmodel); break;
				case 5: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->child->child->child->basicdxmodel); break;
				case 6: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->child->child->child->child->basicdxmodel); break;
				case 7: njDrawModel_SADX(SS_SSWAVES->getmodel()->child->child->child->child->child->child->child->basicdxmodel); break;
				}

				DrawQueueDepthBias = 0;
				njPopMatrix(1u);
			}
		}
	}
}

void __cdecl SSWaves(ObjectMaster *a1)
{
	a1->MainSub = &SSWaves_Main;
	a1->DisplaySub = &SSWaves_Main;
}

PVMEntry SpecialStagesObjectTextures[] = {
	{ "E_BOMB", &E_BOMB_TEXLIST },
	{ 0 }
};

ObjectListEntry SpecialStagesObjectList_list[] = {
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x450370, "RING   " } /* "RING   " */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A4C30, "SPRING " } /* "SPRING " */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A4E50, "SPRINGB" } /* "SPRINGB" */,
	{ 3, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A4450, "O AXPNL" } /* "O AXPNL" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A30E0, "O TOGE" } /* "O TOGE" */,
	{ 3, 3, 1, 360000, 0, (ObjectFuncPtr)0x4A3420, "O EME P" } /* "O EME P" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x46B170, "O RELEASE" } /* "O RELEASE" */,
	{ 6, 3, 1, 360000, 0, (ObjectFuncPtr)0x4CBA80, "O SWITCH" } /* "O SWITCH" */,
	{ 10, 3, 1, 160000, 0, (ObjectFuncPtr)0x7A2B60, "CMN KUSA" } /* "CMN KUSA" */,
	{ 14, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A26F0, "CMN_DRING" } /* "CMN_DRING" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x7A21C0, "O BALOON" } /* "O BALOON" */,
	{ 14, 2, 1, 360000, 0, (ObjectFuncPtr)0x4CA530, "Rocket H" } /* "Rocket H" */,
	{ 2, 2, 1, 4000000, 0, (ObjectFuncPtr)0x4B8DC0, "O JPanel" } /* "O JPanel" */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x4D4850, "WALL   " } /* "WALL   " */,
	{ 2, 3, 1, 360000, 0, (ObjectFuncPtr)0x79B2F0, "O_GRING" } /* "O_GRING" */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D4700, "C SPHERE" } /* "C SPHERE" */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D4770, "C CYLINDER" } /* "C CYLINDER" */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x4D47E0, "C CUBE" } /* "C CUBE" */,
	{ 2, 2, 1, 360000, 0, (ObjectFuncPtr)0x7A1AA0, "O TIKAL" } /* "O TIKAL" */,
	{ 15, 3, 1, 160000, 0, (ObjectFuncPtr)0x4C07D0, "O ItemBoxAir" } /* "O ItemBoxAir" */,
	{ 2, 2, 0, 0, 0, (ObjectFuncPtr)0x7A4E50, "O SPRING B" } /* "O SPRING B" */,
	{ 2, 3, 0, 0, 0, (ObjectFuncPtr)0x79F860, "O SPRING H" } /* "O SPRING H" */,
};
ObjectList SpecialStagesObjectList = { arraylengthandptrT(SpecialStagesObjectList_list, int) };

void SpecialStagesObjects_Init() {
	TexLists_Obj[35] = SpecialStagesObjectTextures;
	ObjLists[280] = &SpecialStagesObjectList;
}