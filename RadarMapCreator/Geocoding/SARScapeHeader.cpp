#include "StdAfx.h"
#include "SARScapeHeader.h"

SARScapeHeader::SARScapeHeader(const char* fileName)
{
	std::stringstream ssTime;
	ssTime.imbue(std::locale(std::locale::classic(), 
		new boost::local_time::local_time_input_facet("%d-%b-%Y " "%H:%M:%S%F")));
	std::string strTime;
	
	ticpp::Document doc(fileName); 
	doc.LoadFile();

	ticpp::Element* elHeader = doc.FirstChildElement();
	
	ticpp::Element* elRasterInfo = elHeader->FirstChildElement("RasterInfo");
	elRasterInfo->FirstChildElement("NrOfPixelsPerLine")->GetText(&RasterInfo.PixelsPerLine);
	elRasterInfo->FirstChildElement("NrOfLinesPerImage")->GetText(&RasterInfo.LinesPerImage);

	ticpp::Element* elProcessing = elHeader->FirstChildElement("Processing");

	ticpp::Element* elSlantRangeVector = elProcessing->FirstChildElement("SlantRange")->
		FirstChildElement("VectorDouble");
	int slantRangeVectorSize = 0; 
	elSlantRangeVector->GetAttribute("NumberOfElements", &slantRangeVectorSize);
	Processing.SlantRange.resize(slantRangeVectorSize);

	ticpp::Iterator<ticpp::Element> itSlantRangeVal;
	itSlantRangeVal = itSlantRangeVal.begin(elSlantRangeVector);
	for (int n = 0; 
		itSlantRangeVal != itSlantRangeVal.end() && n < slantRangeVectorSize; 
		++itSlantRangeVal, ++n)
	{
		itSlantRangeVal->GetText(&Processing.SlantRange[n]);
	}

	elProcessing->FirstChildElement("PixelSpacingRg")->GetText(&Processing.PixelSpacingRg);
	elProcessing->FirstChildElement("PixelSpacingAz")->GetText(&Processing.PixelSpacingAz);

	elProcessing->FirstChildElement("AzimuthStartTime")->GetText(&strTime);
	ssTime << strTime;
	ssTime.exceptions(std::ios::failbit);
	ssTime >> Processing.AzimuthStartTime;

	elProcessing->FirstChildElement("AzimuthStopTime")->GetText(&strTime);
	ssTime << strTime;
	ssTime.exceptions(std::ios::failbit);
	ssTime >> Processing.AzimuthStopTime;

	ticpp::Element* elStateVectorData = elHeader->FirstChildElement("StateVectorData");

	elStateVectorData->FirstChildElement("TimeFirst")->GetText(&strTime);
	ssTime << strTime;
	ssTime.exceptions(std::ios::failbit);
	ssTime >> StateVectorData.TimeFirst;

	elStateVectorData->FirstChildElement("TimeDelta")->GetText(&StateVectorData.TimeDelta);

	ticpp::Element* elPosVector = elStateVectorData->FirstChildElement("pos")->
		FirstChildElement("VectorOfStructs_pos");
	int posVectorSize = 0; 
	elPosVector->GetAttribute("NumberOfElements", &posVectorSize);
	StateVectorData.pos.resize(posVectorSize);

	ticpp::Iterator<ticpp::Element> itPosVal;
	itPosVal = itPosVal.begin(elPosVector);
	for (int n = 0; 
		itPosVal != itPosVal.end() && n < posVectorSize; 
		++itPosVal, ++n)
	{
		itPosVal->FirstChildElement("pos_x")->GetText(&StateVectorData.pos[n].X);
		itPosVal->FirstChildElement("pos_y")->GetText(&StateVectorData.pos[n].Y);
		itPosVal->FirstChildElement("pos_z")->GetText(&StateVectorData.pos[n].Z);
	}

	ticpp::Element* elVelVector = elStateVectorData->FirstChildElement("vel")->
		FirstChildElement("VectorOfStructs_vel");
	int velVectorSize = 0; 
	elVelVector->GetAttribute("NumberOfElements", &velVectorSize);
	StateVectorData.vel.resize(velVectorSize);

	ticpp::Iterator<ticpp::Element> itVelVal;
	itVelVal = itVelVal.begin(elVelVector);
	for (int n = 0; 
		itVelVal != itVelVal.end() && n < velVectorSize; 
		++itVelVal, ++n)
	{
		itVelVal->FirstChildElement("vel_x")->GetText(&StateVectorData.vel[n].X);
		itVelVal->FirstChildElement("vel_y")->GetText(&StateVectorData.vel[n].Y);
		itVelVal->FirstChildElement("vel_z")->GetText(&StateVectorData.vel[n].Z);
	}
}
