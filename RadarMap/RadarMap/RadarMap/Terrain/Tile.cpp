#include "stdafx.h"
#include "Tile.h"

#include "LOD.h"
#include "Terrain.h"

#include <RLib/Collection/Texture.h>
#include <RLib/Collection/Array2D.h>
#include <RLib/Processing/Processing.h>

#include <Rlib/Serialization/EXR.h>

RLib::Collection::Array2D<half, 3> sArray2D;

RadarMap::Terrain::Tile::Tile():
mpLevel(NULL),
mpTex(NULL)
{
}

RadarMap::Terrain::Tile::~Tile()
{
}

void RadarMap::Terrain::Tile::Init( LOD* pLevel, const mgcoord2i& coord)
{
	mpLevel = pLevel;
	mCoord = coord;
	sprintf(mRelPath, "\\r%03d\\c%03d", mCoord.y, mCoord.x);
}

void RadarMap::Terrain::Tile::LoadGeometry()
{
	std::string fullPath(mpLevel->mpTerrain->mPath);
	fullPath.append("\\Geometry");
	fullPath.append(mpLevel->mPath);
	fullPath.append(mRelPath);
	fullPath.append(".flt");

	GeometricObject::LoadGeometry(fullPath.c_str());
}

void RadarMap::Terrain::Tile::LoadTexture( const std::string& subLevel)
{
	if (mpTex && mSubLevel != subLevel)
	{
		delete mpTex;
		mpTex = NULL;
	}
	mSubLevel = subLevel;
	
	if (!mpTex)
	{
		try
		{
			std::string fullPath(mpLevel->mpTerrain->mPath);
			fullPath.append("\\Textures");
			fullPath.append(mpLevel->mPath);
			fullPath.append(mRelPath);
			fullPath.append(mSubLevel);
			fullPath.append(".exr");

			RLib::Serialization::EXR::LoadEXR<Imf::HALF>(sArray2D, fullPath.c_str());

			mpTex = new RLib::Collection::TextureHalfRGB();
			mpTex->Filter(GL_LINEAR, GL_LINEAR);
			mpTex->Resize(sArray2D.Width(), sArray2D.Height());
			mpTex->FromArray(&sArray2D(0, 0)[0]);

			if (appConfig.Debug.Trace)
			{
				cout << "RadarMap: load texture " << fullPath << endl << flush;
			}
		}
		catch (Iex::EnoentExc& ex)
		{
		}
	}
}


void RadarMap::Terrain::Tile::UnloadTexture()
{
	if (mpTex)
	{
		delete mpTex;
		mpTex = NULL;
	}
	mSubLevel.clear();
}

void RadarMap::Terrain::Tile::Render( Geometry::RenderProgram& renderGeometryProgram )
{
	if (mpTex)
	{
		mgcoord2d pixelSize;
		pixelSize.x = mpLevel->mElementSize.x / float(mpTex->Width() - 2);
		pixelSize.y = -mpLevel->mElementSize.y / float(mpTex->Height() - 2);

		mgcoord2d origin;
		origin.x = mpLevel->mpTerrain->mCoverage.Origin.x + mpLevel->mElementSize.x * float(mCoord.x) - pixelSize.x;
		origin.y = mpLevel->mpTerrain->mCoverage.Origin.y + mpLevel->mElementSize.y * float(mCoord.y + 1) - pixelSize.y;

		RLib::Processing::TextureSource source(*mpTex, GL_TEXTURE1);
		glUniform1iARB(renderGeometryProgram.mUniTexture, 1);
		glUniform2fARB(renderGeometryProgram.mUniTexElement.Origin, origin.x, origin.y); 
		glUniform2fARB(renderGeometryProgram.mUniTexElement.Size, pixelSize.x, pixelSize.y);
		GeometricObject::Render();
	}
}



