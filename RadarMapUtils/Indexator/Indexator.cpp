// Indexator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <RLib/Collection/Array2D.h>
#include <RLib/Serialization/EXR.h>


bool dbEmpty;

float minZ, maxZ;
float dbSize;

mgbool IndexDB(mgrec *db, mgrec *parent, mgrec *rec, void *userData)
{
	mgcode recCode = mgGetCode(rec);
	switch (recCode)
	{
	case fltMesh:
		{
			unsigned int numVtx, numPrimitives;
			mgGetAttList(rec, fltMeshNumVtx, &numVtx, fltMeshNumPrimitives, &numPrimitives, MG_NULL);

			for (int prim = 0 ; prim < numPrimitives; ++prim)
			{
				dbSize += mgMeshPrimitiveGetNumVtx(rec, prim) - 2;
			}

			for (int vtxInd = 0; vtxInd < numVtx; ++vtxInd)
			{
				mgcoord3d coord;
				mgMeshGetVtxCoord(rec, vtxInd, &coord.x, &coord.y, &coord.z);
				
				minZ = std::min(minZ, float(coord.z));
				maxZ = std::max(maxZ, float(coord.z));

				dbEmpty = false;
			}
		}
		break;
	}
	return MG_TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc ==0) return 1;
	
	mgInit(NULL, NULL);
	
	std::string fileName = argv[1];
	std::string root = fileName.substr(0, fileName.find_last_of("\\")) + std::string("\\Geometry");
	
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

	RLib::Collection::Array2D<float, 2> indexArray2D;
	RLib::Collection::Array2D<float, 1> sizeArray2D;

	ticpp::Element* gridStructureEl = databaseEl->FirstChildElement("GridStructure");
	ticpp::Iterator< ticpp::Element> gridLevelIt("GridLevel");
	for (gridLevelIt = gridLevelIt.begin(gridStructureEl); gridLevelIt !=gridLevelIt.end(); ++gridLevelIt, ++numLevel)
	{
		mgcoord2i subDivisions;
		gridLevelIt->FirstChildElement("NumColDivisions")->GetText(&subDivisions.x);
		gridLevelIt->FirstChildElement("NumRowDivisions")->GetText(&subDivisions.y);

		divisions.x *= subDivisions.x;
		divisions.y *= subDivisions.y;
		
		indexArray2D.Resize(divisions.x, divisions.y);
		sizeArray2D.Resize(divisions.x, divisions.y);

		for (int row = 0; row < divisions.y; ++row)
		for (int col = 0; col < divisions.x; ++col)
		{
			std::string fileName =  boost::str(boost::format(fileNamePattern) % numLevel % row % col) + "flt";
			std::string targetFileName = root + fileName;

			minZ = 0;
			maxZ = 0;

			dbSize = 0;

			mgrec* db = mgOpenDb(targetFileName.c_str());
			if (db)
			{
				minZ = FLT_MAX;
				maxZ = FLT_MIN;

				dbEmpty = true;
				mgWalk (db, IndexDB, MG_NULL, MG_NULL, 0);
					
				if (dbEmpty)
				{
					minZ = 0;
					maxZ = 0;
				}

				mgCloseDb(db);
			}

			indexArray2D(col, row)[0] = minZ;
			indexArray2D(col, row)[1] = maxZ;
			sizeArray2D(col, row)[0] = dbSize;
		}

		std::string indexFile = targetIndexRoot + boost::str(boost::format("\\HeightIndex%d.exr") % numLevel);
		RLib::Serialization::EXR::SaveEXR<Imf::FLOAT>(indexArray2D, indexFile.c_str());

		std::string sizeFile = targetIndexRoot + boost::str(boost::format("\\SizeIndex%d.exr") % numLevel);
		RLib::Serialization::EXR::SaveEXR<Imf::FLOAT>(sizeArray2D, sizeFile.c_str());
	}
	
	mgExit();
	return 0;
}