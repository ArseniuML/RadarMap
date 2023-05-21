#include "stdafx.h"
#include "GeometricObject.h"

RadarMap::Geometry::GeometricObject::GeometricObject() :	
mDb(NULL)
{
}

RadarMap::Geometry::GeometricObject::~GeometricObject()
{
}

void RadarMap::Geometry::GeometricObject::LoadGeometry( const char* fileName )
{
	if (!mDb)
	{
		mDb = mgOpenDb (fileName);	
		if (mDb) 
		{
			mgWalk (mDb, LoadDB, MG_NULL, this, 0);
			if (appConfig.Debug.Trace)
			{
				cout << "RadarMap: load geometry " << fileName << endl << flush;
				cout << "RadarMap: mMeshes.size =  " << mMeshes.size() << endl << flush;
			}
		}
	}
}

void RadarMap::Geometry::GeometricObject::UnloadGeometry()
{
	if (mDb)
	{
		for(int m = 0; m < mMeshes.size(); ++m)
		{
			delete mMeshes[m];
		}
		mMeshes.clear();
		mgCloseDb(mDb);
		mDb = NULL;
	}
}

void RadarMap::Geometry::GeometricObject::Render()
{
	for(int m = 0; m < mMeshes.size(); ++m)
	{
		mMeshes[m]->Render();
	}
}

mgbool RadarMap::Geometry::GeometricObject::LoadDB( mgrec *db, mgrec *parent, mgrec *rec, void *userData )
{
	GeometricObject*  thisPtr = (GeometricObject*)userData; 

	mgcode recCode = mgGetCode(rec);
	switch (recCode)
	{
	case fltMesh:
		thisPtr->mMeshes.push_back(new Mesh(rec));
		break;
	}
	return MG_TRUE;
}
