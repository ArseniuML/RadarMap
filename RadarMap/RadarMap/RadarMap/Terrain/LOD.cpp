#include "StdAfx.h"
#include "LOD.h"
#include "Terrain.h"
#include <RadarMap/Timer.h>

#include <RLib/Serialization/EXR.h>
#include <RLib/Processing/Processing.h>

RadarMap::Terrain::LOD::LOD(Terrain* pTerrain, int numLevel, const mgcoord2i& divisions, float texelSize, int textureSubLevels):
mpTerrain(pTerrain),
mDivisions(divisions),
mTexelSize(texelSize),
mTextureSubLevels(textureSubLevels)
{
	mPath = boost::str(boost::format("\\v%02d") % numLevel);
	
	mElementSize.x = mpTerrain->mCoverage.Extent.x / (float) mDivisions.x;
	mElementSize.y = mpTerrain->mCoverage.Extent.y / (float) mDivisions.y;

	mTexHeightBounds.Resize(divisions.x, divisions.y);
	
	mTexElevationBounds.Resize(divisions.x, divisions.y);
	mArElevationBounds.Resize(divisions.x, divisions.y);

	mTexElementsVisible.Resize(divisions.x, divisions.y);
	mArElementsVisible.Resize(divisions.x, divisions.y);
	
	/*RLib::Serialization::EXR::LoadEXR<Imf::FLOAT>
	(
		mGeometrySize, 
		(pTerrain->mPath + boost::str(boost::format("\\Geometry\\Index\\SizeIndex%d.exr") % numLevel)).c_str()
	);*/

	RLib::Collection::Array2D<float, 2> arHeightBounds;
	RLib::Serialization::EXR::LoadEXR<Imf::FLOAT>
	(
		arHeightBounds, 
		(pTerrain->mPath + boost::str(boost::format("\\Geometry\\Index\\HeightIndex%d.exr") % numLevel)).c_str()
	);
	mTexHeightBounds.FromArray(&arHeightBounds(0, 0)[0]);

	if (mTextureSubLevels > 0)
	{
		RLib::Serialization::EXR::LoadEXR<Imf::UINT>
		(
			mTextureInsets, 
			(pTerrain->mPath + boost::str(boost::format("\\Textures\\Index\\SubLevels%d.exr") % numLevel)).c_str()
		);
	}

	mTiles.resize(mDivisions.x * mDivisions.y);
	int n = 0; mgcoord2i tileCoord;
	for (tileCoord.y = 0; tileCoord.y < mDivisions.y; ++tileCoord.y)
	for (tileCoord.x = 0; tileCoord.x < mDivisions.x; ++tileCoord.x, ++n)
	{
		mTiles[n].Init(this, tileCoord);
	}
}

RadarMap::Terrain::LOD::~LOD()
{
	Clear();
}

//Выбор тайлов, находящихся в области видимости
//Возможен подсчет времени, требуемого для загрузки
float RadarMap::Terrain::LOD::SelectTiles( Request& req )
{
	glViewport(0, 0, mDivisions.x, mDivisions.y);

	{
		RLib::Processing::FramebufferTextureAttachment attachment0(mTexElevationBounds);
		RLib::Processing::TextureSource source0(mTexHeightBounds);
		RLib::Shader::ProgramBinder programBinder(mpTerrain->mElementsFOVProgram);

		glUniform1iARB(mpTerrain->mElementsFOVProgram.mUniZBoundsMap, 0);
		glUniform2fARB
		(
			mpTerrain->mElementsFOVProgram.mUniGrid.Origin, 
			mpTerrain->mCoverage.Origin.x - req.Airplane.X, 
			mpTerrain->mCoverage.Origin.y - req.Airplane.Y
		);
		glUniform2fARB(mpTerrain->mElementsFOVProgram.mUniGrid.ElementSize, mElementSize.x, mElementSize.y);

		glUniform2fARB
		(
			mpTerrain->mElementsFOVProgram.mUniReqRangeBounds2, 
			req.FOV.DistanceMin * req.FOV.DistanceMin, 
			req.FOV.DistanceMax * req.FOV.DistanceMax
		);

		float reqAzimuthBounds[6];
		reqAzimuthBounds[0] = -cosf(req.FOV.Azimuth - req.FOV.AzimuthHRadius);
		reqAzimuthBounds[1] = -sinf(req.FOV.Azimuth - req.FOV.AzimuthHRadius);
		reqAzimuthBounds[2] = cosf(req.FOV.Azimuth + req.FOV.AzimuthHRadius);
		reqAzimuthBounds[3] = sinf(req.FOV.Azimuth + req.FOV.AzimuthHRadius);
		reqAzimuthBounds[4] = -sinf(req.FOV.Azimuth);
		reqAzimuthBounds[5] = cosf(req.FOV.Azimuth);

		glUniform2fvARB(mpTerrain->mElementsFOVProgram.mUniReqAzimuthBounds, 3, reqAzimuthBounds);
		glUniform1fARB(mpTerrain->mElementsFOVProgram.mUniHeight, req.Airplane.Z);

		mpTerrain->mElementsFOVProgram.Pass();	
	}
	
	mTexElevationBounds.ToArray(&mArElevationBounds(0, 0)[0]);
	//RLib::Serialization::EXR::SaveEXR<Imf::FLOAT>(mArElevationBounds, "C:\\test.exr");

	mFOVTiles.clear();
	mShadowTiles.clear();

	for (int y = 0, n = 0; y < mArElevationBounds.Height(); ++y)
	for (int x = 0; x < mArElevationBounds.Width(); ++x, ++n)
	{
		float el = mArElevationBounds(x, y)[0];
		if (el > 0.0f)
		{
			mFOVTiles.insert(n);
		}
	}

	return 0;
}

//Загрузка и выгрузка тайлов в соответствии с областью видимости
void RadarMap::Terrain::LOD::Load(const Request& req, bool& stop)
{
	std::set<int> geometry;
	std::set_union
	(
		mFOVTiles.begin(), mFOVTiles.end(),
		mShadowTiles.begin(), mShadowTiles.end(),
		std::inserter(geometry, geometry.end())
	);
	
	std::set<int> geometryToUnload;
	std::set_difference
	(
		mGeometryLoaded.begin(), mGeometryLoaded.end(),
		geometry.begin(), geometry.end(),
		std::inserter(geometryToUnload, geometryToUnload.end())
	);
	
	BOOST_FOREACH(int el, geometryToUnload) 
	{
		mTiles[el].UnloadGeometry();
		mGeometryLoaded.erase(el);
	}
	
	std::set<int> texturesToUnload;
	std::set_difference
	(
		mTexturesLoaded.begin(), mTexturesLoaded.end(),
		mFOVTiles.begin(), mFOVTiles.end(),
		std::inserter(texturesToUnload, texturesToUnload.end())
	);
	
	BOOST_FOREACH(int el, texturesToUnload)
	{
		mTiles[el].UnloadTexture();
		mTexturesLoaded.erase(el);
	}

	std::string reqSubLevel;
	if (mTextureSubLevels > 0)
	{
		float reqTexelSize = (req.FOV.DistanceMax - req.FOV.DistanceMin) / 
			float(techConfig.Modes[req.Mode].Elements.Azimuth);
		reqTexelSize = reqTexelSize / 1.5;
		int nSubLevel = floor(log(mTexelSize / reqTexelSize) / log(2.0));
		nSubLevel = std::min(nSubLevel, mTextureSubLevels);
		if (nSubLevel > 0)
		{
			reqSubLevel = boost::str(boost::format("s%01d") % nSubLevel);
		}
	}

	BOOST_FOREACH(int el, mFOVTiles)
	{
		if (stop) return;

		mTiles[el].LoadGeometry();
		mGeometryLoaded.insert(el);

		std::string subLevel;
		if (mTextureSubLevels > 0 && mTextureInsets(el)[0] > 0)
		{
			subLevel = reqSubLevel;
		}
		mTiles[el].LoadTexture(subLevel);
		mTexturesLoaded.insert(el);
	}

	BOOST_FOREACH(int el, mShadowTiles)
	{
		if (stop) return;
		mTiles[el].LoadGeometry();
		mGeometryLoaded.insert(el);
	}
}

void RadarMap::Terrain::LOD::Clear()
{
	BOOST_FOREACH(int el, mTexturesLoaded)
	{
		mTiles[el].UnloadTexture();
	}
	mTexturesLoaded.clear();

	BOOST_FOREACH(int el, mGeometryLoaded)
	{
		mTiles[el].UnloadGeometry();
	}
	mGeometryLoaded.clear();
}

void RadarMap::Terrain::LOD::Render( Geometry::RenderProgram& renderGeometryProgram )
{
	if (appConfig.Debug.Trace)
	{
		cout << "RadarMap: time to update = " << Timer::GetTimeToUpdate() << 
			", render level " << mPath << endl <<flush;
		cout << "RadarMap: mFOVtiles.size = " << mFOVTiles.size();

		if (mGeometryLoaded.size() < mFOVTiles.size() + mShadowTiles.size() ||
			mTexturesLoaded.size() < mFOVTiles.size())
		{
			cout << ", mShadowTiles.size = " << mShadowTiles.size() << endl << flush;
			cout << "RadarMap: mGeometryLoaded.size = " <<  mGeometryLoaded.size() <<
				", mTexturesLoaded.size = " << mTexturesLoaded.size();
		}

		cout <<endl << flush;
	}

	BOOST_FOREACH(int el, mGeometryLoaded)
	{
		mTiles[el].Render(renderGeometryProgram);
	}
}
