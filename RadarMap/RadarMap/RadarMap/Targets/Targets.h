#pragma once

#include "Model.h"
#include <graph_29k.h>

namespace RadarMap {
		namespace Targets {
	
class Targets
{
public:
	Targets();
	~Targets();
	
	void LoadModels(const std::string& fileName);
	void ActivateTargets(const graph_29k::TActiveObjects2& activeObjectParams );
	void RenderTargets(Geometry::RenderProgram& geometryRenderProgram, const Request& req);
	void RenderTargetsAsPoints(Point::RenderProgram& pointRenderProgram);

private:
	void Clear();

private:
	typedef std::pair<const long, Model*> TargetPair;
	typedef std::map<const long, Model*> TargetMap;
	
	TargetMap mTargetMap;
	std::set<Model*> mActiveTargets;
};

	}
}
