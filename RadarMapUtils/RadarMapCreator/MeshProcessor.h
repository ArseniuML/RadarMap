#pragma once

#include <vector>

#define API_LEV2
#include <mgapiall.h>

#include <RLib/Sort/Sort.h>

class MeshProcessor
{
public:
	MeshProcessor();
	~MeshProcessor();
	
	void ProcessFlt( const char* sourceFile, const char* targetFile);

private:
	static mgbool ReadDB(mgrec* db, mgrec* parent, mgrec* rec, void *userData);
	void AddPolygon(mgrec* parent, mgrec* poly);
	void MarkAsSame(RLib::Sort::Index firstInd, RLib::Sort::Index lastInd);
	mgrec* MakeMesh();

private:
	std::vector<mgcoord3d> mPolyVtx;

	int mPoolSize;
	std::vector<RLib::Sort::Index> mSortInd;
	std::vector<RLib::Sort::Index> mPoolInd;

	std::vector<int> mStripLengths;
	std::vector<int> mIndexArray;
};

