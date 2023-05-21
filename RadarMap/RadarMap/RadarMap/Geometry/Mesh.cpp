#include "StdAfx.h"
#include "Mesh.h"

RadarMap::Geometry::Mesh::Mesh(mgrec* meshRec):
mMeshRec(meshRec)
{
	mPool = mgMeshGetVtxPool(meshRec);
	mStride = mgMeshGetVtxStride(meshRec);
	mVtxOffset = mgMeshGetVtxOffset(meshRec, MMESH_VTXCOORD);

	mgGetAttList(	meshRec, fltMeshNumPrimitives, &mNumPrim,	MG_NULL);
	mNumVtxPrim.resize(mNumPrim);

	mNumIndices = 0;
	for(int primNo = 0; primNo < mNumPrim; ++primNo)
	{
		mNumVtxPrim[primNo] = mgMeshPrimitiveGetNumVtx(meshRec, primNo);
		mNumIndices	+= mNumVtxPrim[primNo];
	}
	
	mIndices.resize(mNumIndices);
	mMultiIndices.resize(mNumPrim);
	for (int primNo = 0, startInd = 0; primNo < mNumPrim; startInd += mNumVtxPrim[primNo++])
	{
		mgMeshPrimitiveGetVtxIndexArray(meshRec, primNo, (int*)&mIndices[startInd], mNumVtxPrim[primNo]);
		mMultiIndices[primNo] = &mIndices[startInd];
	}
}

RadarMap::Geometry::Mesh::~Mesh(void)
{
}

void RadarMap::Geometry::Mesh::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, mStride,(unsigned char*)(mPool) + mVtxOffset);

	glMultiDrawElementsEXT
	(
		GL_TRIANGLE_STRIP, 
		&mNumVtxPrim[0], 
		GL_UNSIGNED_INT, 
		(const void**)&mMultiIndices[0], 
		mNumPrim
	);

	glDisableClientState(GL_VERTEX_ARRAY);
}