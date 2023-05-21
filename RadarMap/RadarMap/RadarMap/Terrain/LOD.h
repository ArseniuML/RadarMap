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

	//Номера тайлов:
	std::set<int> mFOVTiles; //Находящихся в облаcти видимости
	std::set<int> mShadowTiles; //Вне области видимости, требуемых для построения теней (пока не исп.)
	std::set<int> mGeometryLoaded; //С загруженной сеткой высот (геометрией)
	std::set<int> mTexturesLoaded; //С загруженными текстурами
	
	//Пространственная индексация тайлов:
	RLib::Collection::TextureFloatRG mTexHeightBounds;
	RLib::Collection::TextureFloatRG mTexElevationBounds;
	RLib::Collection::Array2D<float, 2> mArElevationBounds;
	RLib::Collection::TextureFloatR mTexElementsVisible;
	RLib::Collection::Array2D<float, 1> mArElementsVisible;
	
	//Индексация тайлов в соответствии со скоростью подгрузки:
	RLib::Collection::Array2D<float, 1> mGeometrySize;
	
	//Вставки с текстурами высокого разрешения:
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
