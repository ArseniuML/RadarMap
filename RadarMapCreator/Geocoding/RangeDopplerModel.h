#pragma once

#include "SarScapeHeader.h"

class RangeDopplerModel
{
public:
	struct RasterParams
	{
		int PixelsPerLine;
		int LinesPerImage;
	};

	struct ProcessingParams
	{
		double SlantRangeNear;
		double PixelSpacingRg;
		double AzimuthStartTime;
		double LineTimeInterval;	
	};

	RangeDopplerModel(const SARScapeHeader& header);
	
	const RasterParams& Raster()
	{
		return mRaster;
	}

	const ProcessingParams& Processing()
	{
		return mProcessing;
	}

	bool RadarPoint(DecartVector earthPoint, DecartVector* lookVec, RadarVector* imagePoint);

private:
	double EarthPointZeroDopplerTime(DecartVector earthPoint);
	double DopplerFrequency(int rgIndex, DecartVector earthPoint);

	RasterParams mRaster;
	ProcessingParams mProcessing;

	struct  
	{
		std::vector<double> Time;
		std::vector<DecartVector> Pos;
		std::vector<DecartVector> Vel;
	} mStateVectorData;
	
	struct  
	{
		std::vector<DecartVector> Pos;
		std::vector<DecartVector> Vel;	
	} mOrbit;
};

