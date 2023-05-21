#pragma once

#include <RadarMap/Geometry/RenderProgram.h>
#include <RadarMap/Geometry/GeometricObject.h>
#include <Rlib/Collection/Texture.h>

namespace RadarMap {
		namespace Terrain {

class Terrain;
class LOD;

class Tile : public Geometry::GeometricObject  
{
public:
	Tile();
	~Tile();
	
	void Init(LOD* pLevel, const mgcoord2i& coord);
	
	void LoadGeometry();
	void LoadTexture(const std::string& subLevel = "");
	void UnloadTexture();

	void Render(Geometry::RenderProgram& renderGeometryProgram);

private:
	LOD* mpLevel;
	
	char mRelPath[11]; 
	mgcoord2i mCoord;
	
	RLib::Collection::Texture* mpTex;
	std::string mSubLevel;
};

	}
}