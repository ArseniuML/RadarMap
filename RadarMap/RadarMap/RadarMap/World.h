#pragma once

#include "Terrain/Terrain.h"
#include "Targets/Targets.h"
#include "Geometry/RenderProgram.h"

namespace RadarMap {

//Класс реализует процесс построения неискаженного кадра РЛ карты
//чтение параметров кадра и списка активных целей,
//загрузка оптимального LODа,
//растеризация в координатах азимут - дальность

//Цели, имеющие малый (по сравнению с размером пикселя) размер,
//растеризуются, как точки

class World
{
public:
	World() {}

	void LoadDescription(const std::string& fileName)
	{
		mTerrain.LoadDescription(fileName);
	}

	void LoadOptimalLevel(Request& req, bool& stop)
	{
		mTerrain.LoadOptimalLevel(req, stop);
	}

	void LoadShapes(const std::string& fileName)
	{
		mTargetCollection.LoadModels(fileName);
	}

	void ActivateTargets(const graph_29k::TActiveObjects2& activeObjectParams)
	{
		mTargetCollection.ActivateTargets(activeObjectParams);
	}

	void Render(RLib::Collection::Texture& resultTex, const Request& req);
	void RenderPoints(RLib::Collection::Texture& resultTex, const Request& req);

private:
	Terrain::Terrain mTerrain;
	Terrain::Background::RenderProgram mBackgroundProgram;

	Targets::Targets mTargetCollection;
	Targets::Point::RenderProgram mPointProgram;

	Geometry::RenderProgram mGeometryProgram;
};

}
