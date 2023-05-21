// Indexator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <RLib/Collection/Array2D.h>
#include <RLib/Serialization/EXR.h>

RLib::Collection::Array2D<half, 2> sArray;

int _tmain(int argc, _TCHAR* argv[])
{
	//if (argc ==0) return 1;
	
	
	//std::string fileName = argv[1];
	std::string fileName = "C:\\Data\\India\\RadarMap\\Terrain\\IndiaTerrain.header ";
	std::string root = fileName.substr(0, fileName.find_last_of("\\")) + std::string("\\Textures");
	
	std::string targetIndexRoot = root + "\\Index";
	CreateDirectoryA(targetIndexRoot.c_str(), NULL);

	ticpp::Document doc(fileName);
	doc.LoadFile();

	ticpp::Element* databaseEl = doc.FirstChildElement()->FirstChildElement("Database");

	std::string fileNamePattern = "\\v%1$02d\\r%2$03d\\c%3$03d.";

	mgcoord2i divisions;
	divisions.x = 1;
	divisions.y = 1;
	int numLevel = 0;

	LARGE_INTEGER loadTime;
	memset(&loadTime, 0, sizeof(LARGE_INTEGER));

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	int ntiles = 0;
	
	ticpp::Element* gridStructureEl = databaseEl->FirstChildElement("GridStructure");
	ticpp::Iterator< ticpp::Element> gridLevelIt("GridLevel");
	for (gridLevelIt = gridLevelIt.begin(gridStructureEl); gridLevelIt !=gridLevelIt.end(); ++gridLevelIt, ++numLevel)
	{
		mgcoord2i subDivisions;
		gridLevelIt->FirstChildElement("NumColDivisions")->GetText(&subDivisions.x);
		gridLevelIt->FirstChildElement("NumRowDivisions")->GetText(&subDivisions.y);

		divisions.x *= subDivisions.x;
		divisions.y *= subDivisions.y;
		
		for (int row = 0; row < divisions.y; ++row)
		for (int col = 0; col < divisions.x; ++col)
		{
			++ntiles;

			std::string fileName =  boost::str(boost::format(fileNamePattern) % numLevel % row % col) + "exr";
			std::string targetFileName = root + fileName;

			LARGE_INTEGER start;
			QueryPerformanceCounter(&start);

			Sleep(5000);

			//RLib::Serialization::EXR::LoadEXR<Imf::HALF>(sArray, targetFileName.c_str());

			LARGE_INTEGER finish;
			QueryPerformanceCounter(&finish);

			loadTime.QuadPart = loadTime.QuadPart + finish.QuadPart - start.QuadPart;
			double lTime1 = double(loadTime.QuadPart) / double(freq.QuadPart);
			int qq = 0;
		}
	}

	double lTime = double(loadTime.QuadPart) / double(freq.QuadPart);

	std::cout << std::endl << "time per tile = " << lTime / double(ntiles) << std::endl;
	std::cout << std::endl << "time per pixel = " << lTime / double(ntiles * sArray.Width() * sArray.Height()) << std::endl;

	return 0;
}