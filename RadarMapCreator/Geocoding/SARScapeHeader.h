#pragma once



#include "SimpleTypes.h"

struct SARScapeHeader
{
	SARScapeHeader(const char* fileName);

	struct  
	{
		int PixelsPerLine;
		int LinesPerImage;
	} RasterInfo;

	struct
	{
		std::vector<double> SlantRange;
		
		double PixelSpacingRg;
		double PixelSpacingAz;		
		
		boost::posix_time::ptime AzimuthStartTime;
		boost::posix_time::ptime AzimuthStopTime;
	} Processing;
	
	struct  
	{
		boost::posix_time::ptime TimeFirst;
		double TimeDelta;
		std::vector<DecartVector> pos;
		std::vector<DecartVector> vel;
	} StateVectorData;
};

