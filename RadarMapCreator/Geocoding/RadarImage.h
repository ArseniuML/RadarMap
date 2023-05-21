#pragma once

#include "RangeDopplerModel.h"
#include "LookupTables.h"

class RadarImage
{
public:
	RadarImage(const char* filename);
	~RadarImage();

	RangeDopplerModel mMetadata;

	GDALDatasetH mDsImage;
	int mX0, mY0;
	int mWidth, mHeight;
	std::vector<float> Image;
};

