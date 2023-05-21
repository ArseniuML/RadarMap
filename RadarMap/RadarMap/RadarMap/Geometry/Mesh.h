#pragma once

namespace RadarMap {
	namespace Geometry {

class Mesh
{
public:
	Mesh(mgrec* meshRec);
	~Mesh(void);
	
	void Render();
	
protected:
	mgrec* mMeshRec;

	void* mPool;
	unsigned int mStride;
	int mVtxOffset;

	unsigned int mNumPrim;
	std::vector<int> mNumVtxPrim;

	unsigned int	mNumIndices;
	std::vector<unsigned int> mIndices;
	std::vector<unsigned int*> mMultiIndices;
};

	}
}
