#include "MeshProcessor.h"

#include <boost/bind.hpp>
#include <Strips/Strips.h>

MeshProcessor::MeshProcessor(void)
{
}


MeshProcessor::~MeshProcessor(void)
{
}

void MeshProcessor::ProcessFlt( const char* sourceFile, const char* targetFile )
{
	mgrec* db = mgOpenDb(sourceFile);	
	if (db)
	{
		mgSetNewOverwriteFlag(MG_TRUE);
		mgrec* tgtDb = mgNewDb(targetFile);
		if(tgtDb)
		{
			std::vector<mgrec*> nodesToDelete;


			for(mgrec* node = mgGetChild(tgtDb); node; node = mgGetNext(node))
			{
				nodesToDelete.push_back(node);
			}

			for (int i = 0; i < nodesToDelete.size(); ++i)
			{
				mgDelete(nodesToDelete[i]);
			}

			mPolyVtx.clear();
			mgWalk(db, &MeshProcessor::ReadDB, MG_NULL, this, 0);

			mgrec* mesh = MakeMesh();
			if (mesh)
			{
				mgAttach(tgtDb, mesh);
			}

			mgWriteDb(tgtDb);
			mgCloseDb(tgtDb);
		}
		mgCloseDb(db);
	}
}

mgbool MeshProcessor::ReadDB( mgrec* db, mgrec* parent, mgrec* rec, void *userData )
{
	MeshProcessor* thisPtr = (MeshProcessor*)userData;

	mgcode code = mgGetCode(rec);
	switch(code)
	{
	case fltPolygon:
		thisPtr->AddPolygon(parent, rec);
		break;
	}

	return MG_TRUE;
}

void MeshProcessor::AddPolygon( mgrec* parent, mgrec* poly )
{
	int i = 0;
	mgcoord3d pos[3];

	for (mgrec* vtx = mgGetChild(poly); vtx && i < 3; vtx = mgGetNext(vtx), ++i)
	{
		mgGetVtxCoord(vtx, &pos[i].x, &pos[i].y, &pos[i].z);
	}

	for (i = 0; i < 3; ++i)
	{
		mPolyVtx.push_back(pos[i]);
	}
}

void MeshProcessor::MarkAsSame(RLib::Sort::Index firstInd, RLib::Sort::Index lastInd)
{
	for (RLib::Sort::Index i = firstInd; i < lastInd; ++i)
	{
		mPoolInd[mSortInd[i]] = mPoolSize;
	}
	++mPoolSize;
}

mgrec* MeshProcessor::MakeMesh()
{
	mPoolInd.resize(mPolyVtx.size());
	mSortInd.resize(mPolyVtx.size());

	for (int i = 0; i < mPolyVtx.size(); ++i)
	{
		mSortInd[i] = i;
		mPoolInd[i] = -1;
	}
	mPoolSize = 0;

	mStripLengths.clear();
	mIndexArray.clear();

	RLib::Sort::MergeSort sorter(mPolyVtx.size());
	RLib::Sort::SpatialSort
		(
		sorter, 
		&mPolyVtx[0],  
		&mSortInd[0], 
		0, mPolyVtx.size(),  
		boost::bind(&MeshProcessor::MarkAsSame, this, _1, _2), 
		0.000001, 
		true
		);

	STRIPERCREATE sc;
	sc.DFaces = &mPoolInd[0];
	sc.NbFaces	= mPolyVtx.size() / 3;
	sc.AskForWords = true;
	sc.ConnectAllStrips	= false;
	sc.OneSided = true;
	sc.SGIAlgorithm	= true;

	Striper Strip;
	Strip.Init(sc);

	STRIPERRESULT sr;
	if (Strip.Compute(sr))
	{
		int totalStripLength = 0;
		for (int i = 0; i < sr.NbStrips; ++i)
		{
			mStripLengths.push_back(sr.StripLengths[i]);
			totalStripLength += sr.StripLengths[i];
		}

		mIndexArray.reserve(mIndexArray.size() + totalStripLength);
		for (int i = 0; i < totalStripLength; ++i)
		{
			mIndexArray.push_back(((uword*)sr.StripRuns)[i]);
		}

		mgrec* mesh = mgNewRec(fltMesh);

		mgMeshCreateVtxPool(mesh, MMESH_VTXCOORD, mPoolSize);
		for (int i = 0; i < mPolyVtx.size(); ++i)
		{
			mgcoord3d& pos = mPolyVtx[i];
			if (mPoolInd[i] >= 0)
			{
				mgMeshSetVtxCoord(mesh, mPoolInd[i], pos.x, pos.y, pos.z);
			}
		}

		mgMeshCreatePrimitives(mesh, mStripLengths.size());

		int* pIndexArray = &mIndexArray[0];
		for (int i = 0; i < mStripLengths.size(); ++i)
		{
			mgMeshPrimitiveSetType(mesh, i, MPRIM_TRI_STRIP);
			mgMeshPrimitiveSetVtxIndexArray(mesh, i, pIndexArray, mStripLengths[i]);
			pIndexArray += mStripLengths[i];
		}

		return mesh;
	}
	return NULL;
}