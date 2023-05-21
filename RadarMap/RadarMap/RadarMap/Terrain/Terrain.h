#pragma once

#include "LOD.h"
#include "Shaders.h"
#include "Background/RenderProgram.h"

namespace RadarMap {
	namespace Terrain {

//Класс реализует логику работы LADBM - 
//загрузка заголовка LADBM,
//загрузка оптимального LODа,
//растеризация тайлов

class Terrain
{
public:
	Terrain();
	~Terrain();
	
	void LoadDescription(const std::string& fileName);
	void LoadOptimalLevel(Request& req, bool& stop);
	void RenderBackground(Background::RenderProgram& renderBackroundProgram);
	void Render(Geometry::RenderProgram& renderGeometryProgram);

private:
	void Clear();

private:
	
	typedef std::pair<float, LOD*> LOD_Pair;
	typedef std::map<float, LOD*> LOD_Map;

	LOD_Map mLevels;
	LOD* mpOptimalLevel;
	std::string mPath;
	
	ElementsFOVProgram mElementsFOVProgram;

	struct
	{
		mgcoord2d Origin;
		mgcoord2d Extent;
	} mCoverage;

	struct  
	{
		float Brightness;
	} mBackgroundMaterial;
	
	friend class LOD;
	friend class Tile;
};

	}
}
