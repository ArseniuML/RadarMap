#pragma once

#include "Tile.h"
#include <RadarMap/Request.h>

#include <RLib/Collection/Array2D.h>
#include <RLib/Collection/Texture.h>

namespace RadarMap {
	namespace Terrain {

class Terrain;

class LOD
{
public:
	LOD(Terrain* pTerrain, int numLevel, const mgcoord2i& divisions, float texelSize, int textureSubLevels = 0);
	~LOD();
	
	float SelectTiles(Request& req);
	void Load(const Request& req, bool& stop);
	void Clear();
	void Render(Geometry::RenderProgram& renderGeometryProgram);

private:
	Terrain* mpTerrain;
	
	std::vector<Tile> mTiles;

	//������ ������:
	std::set<int> mFOVTiles; //����������� � ����c�� ���������
	std::set<int> mShadowTiles; //��� ������� ���������, ��������� ��� ���������� ����� (���� �� ���.)
	std::set<int> mGeometryLoaded; //� ����������� ������ ����� (����������)
	std::set<int> mTexturesLoaded; //� ������������ ����������
	
	//���������������� ���������� ������:
	RLib::Collection::TextureFloatRG mTexHeightBounds;
	RLib::Collection::TextureFloatRG mTexElevationBounds;
	RLib::Collection::Array2D<float, 2> mArElevationBounds;
	RLib::Collection::TextureFloatR mTexElementsVisible;
	RLib::Collection::Array2D<float, 1> mArElementsVisible;
	
	//���������� ������ � ������������ �� ��������� ���������:
	RLib::Collection::Array2D<float, 1> mGeometrySize;
	
	//������� � ���������� �������� ����������:
	int mTextureSubLevels;
	float mTexelSize;
	RLib::Collection::Array2D<unsigned int, 1> mTextureInsets;
	
	std::string mPath;
	mgcoord2i mDivisions;
	mgcoord2d mElementSize;
	
	friend class Tile;
};

	}
}
