#include "stdafx.h"
#include "Terrain.h"

#include <RadarMap/World.h>
#include <RadarMap/Timer.h>

RadarMap::Terrain::Terrain::Terrain()
{
	Clear();
}

RadarMap::Terrain::Terrain::~Terrain()
{
	Clear();
}

void RadarMap::Terrain::Terrain::Clear()
{
	mpOptimalLevel = NULL;
	for (LOD_Map::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		delete it->second;
	}
	mLevels.clear();
}

void RadarMap::Terrain::Terrain::LoadDescription(const std::string& fileName)
{
	Clear();

	try
	{
		cout << "RadarMap: loading file " << fileName << endl << flush;

		mPath = fileName.substr(0, fileName.find_last_of("\\"));
		ticpp::Document doc(fileName); 
		doc.LoadFile();

		ticpp::Element* databaseEl = doc.FirstChildElement()->FirstChildElement("Database");

		ticpp::Element* coverageEl = databaseEl->FirstChildElement("Coverage");

		ticpp::Element* originEl = coverageEl->FirstChildElement("Origin");
		originEl->FirstChildElement("x")->GetText(&mCoverage.Origin.x);
		originEl->FirstChildElement("y")->GetText(&mCoverage.Origin.y);

		ticpp::Element* extentEl = coverageEl->FirstChildElement("Extent");
		extentEl->FirstChildElement("x")->GetText(&mCoverage.Extent.x);
		extentEl->FirstChildElement("y")->GetText(&mCoverage.Extent.y);

		mgcoord2i divisions;
		divisions.x = 1;
		divisions.y = 1;

		float texelSize = 1;
		int numLevel = 0;

		ticpp::Element* gridStructureEl = databaseEl->FirstChildElement("GridStructure");
		ticpp::Iterator< ticpp::Element> gridLevelIt("GridLevel");
		for (gridLevelIt = gridLevelIt.begin(gridStructureEl); gridLevelIt !=gridLevelIt.end(); ++gridLevelIt, ++numLevel)
		{
			mgcoord2i subDivisions;
			gridLevelIt->FirstChildElement("NumColDivisions")->GetText(&subDivisions.x);
			gridLevelIt->FirstChildElement("NumRowDivisions")->GetText(&subDivisions.y);

			divisions.x *= subDivisions.x;
			divisions.y *= subDivisions.y;

			float mulTexelSize;	gridLevelIt->FirstChildElement("TexelSize")->GetText(&mulTexelSize);
			texelSize *= mulTexelSize;
			int textureSubLevels; gridLevelIt->FirstChildElement("TextureSubLevels")->GetText(&textureSubLevels);
			
			mLevels.insert(LOD_Pair(texelSize, new LOD(this, numLevel, divisions, texelSize, textureSubLevels)));
		}

		ticpp::Element* bgMaterialEl = databaseEl->FirstChildElement("BackgroundMaterial");
		bgMaterialEl->FirstChildElement("Brightness")->GetText(&mBackgroundMaterial.Brightness);
		mBackgroundMaterial.Brightness = float(half(mBackgroundMaterial.Brightness));
	}
	catch(ticpp::Exception& ex)
	{
		Clear();
		cout << "RadarMap: exception while loading file " << fileName << endl << flush;
		cout << "RadarMap: " << ex.what() << endl << flush;
	}
}

//Выбор LODа с разрешением текстур, наиболее близким к требуемому, и загрузка LODа
//Возможен учет времени подгрузки (с небольшими доработками)
void RadarMap::Terrain::Terrain::LoadOptimalLevel( Request& req, bool& stop )
{
	int timeToUpdate = Timer::GetTimeToUpdate();

	float pixelSize = (req.FOV.DistanceMax - req.FOV.DistanceMin) / (float)techConfig.Modes[req.Mode].Elements.Range;

	LOD* pNewOptimalLevel = NULL;
		
	for (LOD_Map::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
	{
		LOD* pLevel = it->second;
		if (it == --mLevels.end())
		{
			pLevel->SelectTiles(req);
			pNewOptimalLevel = pLevel;
		}
		else if (pixelSize < 1.5 * it->first )
		{
			int timeToLoad = pLevel->SelectTiles(req);
			if (timeToLoad <= timeToUpdate) 
			{
				pNewOptimalLevel = pLevel;
				break;
			}
		}
	}


	if (mpOptimalLevel != NULL && mpOptimalLevel != pNewOptimalLevel)
	{
		mpOptimalLevel->Clear();
	}
	mpOptimalLevel = pNewOptimalLevel;
	if (mpOptimalLevel) mpOptimalLevel->Load(req, stop);
}

void RadarMap::Terrain::Terrain::RenderBackground(Background::RenderProgram& renderBackroundProgram)
{
	glUniform1fARB(renderBackroundProgram.mUniMaterial.Brightness, mBackgroundMaterial.Brightness);
	glRectf(-1, -1, 1, 1);
}

void RadarMap::Terrain::Terrain::Render( Geometry::RenderProgram& renderGeometryProgram )
{
	if (mpOptimalLevel) 
	{
		mpOptimalLevel->Render(renderGeometryProgram);
	}
}




