#pragma once
#include "Mesh.h"

namespace RadarMap {
	namespace Geometry {

//���������� ������
//������� ����� ��� ������� Terrain::Tile � Targets::Model

class GeometricObject  
{
public:
	GeometricObject();
	~GeometricObject();

	void LoadGeometry(const char* fileName);
	void UnloadGeometry();
	void Render();

private:
	static mgbool LoadDB(mgrec *db, mgrec *parent, mgrec *rec, void *userData);
	
protected:
	mgrec* mDb;
	std::vector<Mesh*> mMeshes;
};

	}
}