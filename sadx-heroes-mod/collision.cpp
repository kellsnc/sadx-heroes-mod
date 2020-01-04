#include "stdafx.h"

typedef struct {
	NJS_VECTOR p1;
	NJS_VECTOR p2;
	NJS_VECTOR p3;
} MESH;

std::vector<NJS_OBJECT*> TempColMdls;
std::vector<MESH> TempColMesh;

void GetModelsByRadius(NJS_VECTOR* pos) {
	LandTable* land = CurrentLandTable;

	for (int i = 0; i < land->COLCount; ++i) {
		COL* col = &land->Col[i];

		if (col->Flags & ColFlags_Solid && IsPointInsideSphere(&col->Center, pos, col->Radius)) {
			TempColMdls.push_back(col->Model);
		}
	}
}

void GetCloseModels(NJS_VECTOR* pos) {
	std::vector<NJS_OBJECT*> mdls;

	for (int i = 0; i < TempColMdls.size(); ++i) {
		NJS_VECTOR min = { 0, 0, 0 };
		NJS_VECTOR max = { 0, 0, 0 };

		NJS_OBJECT *obj = TempColMdls.at(i);

		for (int j = 0; j < obj->basicdxmodel->nbPoint; ++j) {
			NJS_VECTOR point = obj->basicdxmodel->points[j];
			point.x += obj->pos[0];
			point.y += obj->pos[1];
			point.z += obj->pos[2];

			if (point.x < min.x) min.x = point.x;
			if (point.x > max.x) max.x = point.x;
			if (point.y < min.y) min.y = point.y;
			if (point.y > max.y) max.y = point.y;
			if (point.z < min.z) min.z = point.z;
			if (point.z > max.z) max.z = point.z;
		}

		min.x -= 10; min.y -= 10; min.z -= 10;
		max.x += 10; max.y += 10; max.z += 10;

		if (pos->x > min.x && pos->x < max.x &&
			pos->y > min.y && pos->y < max.y &&
			pos->z > min.z && pos->z < max.z) {
			mdls.push_back(obj);
		}
	}

	if (mdls.size()) {
		TempColMdls.clear();

		for (int l = 0; l < mdls.size(); ++l) {
			TempColMdls.push_back(mdls.at(l));
		}

		mdls.clear();
	}
}

void GetCloseMeshes(NJS_VECTOR* pos, float radius) {
	for (int i = 0; i < TempColMdls.size(); ++i) {
		NJS_OBJECT* obj = TempColMdls.at(i);
		NJS_MODEL_SADX* mdl = obj->basicdxmodel;
		NJS_VECTOR point = { obj->pos[0], obj->pos[1], obj->pos[2] };
		MESH mesh;

		for (int j = 0; j < mdl->nbMeshset; ++j) {
			NJS_MESHSET_SADX* meshset = &mdl->meshsets[j];

			for (int k = 0; k < meshset->nbMesh; ++k) {
				mesh.p1 = mdl->points[meshset->meshes[k * 3]];
				mesh.p2 = mdl->points[meshset->meshes[k * 3 + 1]];
				mesh.p3 = mdl->points[meshset->meshes[k * 3 + 2]];

				njAddVector(&mesh.p1, &point);
				njAddVector(&mesh.p2, &point);
				njAddVector(&mesh.p3, &point);
				
				NJS_VECTOR center;
				center.x = (mesh.p1.x + mesh.p2.x + mesh.p3.x) / 3;
				center.y = (mesh.p1.y + mesh.p2.y + mesh.p3.y) / 3;
				center.z = (mesh.p1.z + mesh.p2.z + mesh.p3.z) / 3;

				NJS_VECTOR min = center;
				NJS_VECTOR max = center;

				for (uint8_t l = 0; l < 3; ++l) {
					NJS_VECTOR point;
					if (l == 0) point = mesh.p1;
					else if (l == 1) point = mesh.p2;
					else if (l == 2) point = mesh.p3;

					if (point.x - radius < min.x) min.x = point.x - radius;
					if (point.x + radius > max.x) max.x = point.x + radius;
					if (point.y - radius < min.y) min.y = point.y - radius;
					if (point.y + radius > max.y) max.y = point.y + radius;
					if (point.z - radius < min.z) min.z = point.z - radius;
					if (point.z + radius > max.z) max.z = point.z + radius;
				}

				if (pos->x > min.x && pos->x < max.x &&
					pos->y > min.y && pos->y < max.y &&
					pos->z > min.z && pos->z < max.z) {

					TempColMesh.emplace_back(mesh);
				}
			}
		}
	}
}

bool IsPointInMeshes(NJS_VECTOR* pos) {
	return TempColMesh.size() > 0;
}

bool IsPointInCollision(NJS_VECTOR* pos, float radius) {
	GetModelsByRadius(pos);
	GetCloseModels(pos);
	GetCloseMeshes(pos, radius);
	TempColMdls.clear();
	bool result = IsPointInMeshes(pos);
	TempColMesh.clear();
	return result;
}