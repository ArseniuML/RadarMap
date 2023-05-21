#include "StdAfx.h"
#include "RadarImage.h"
#include "ProjectedMap.h"


RadarImage::RadarImage(ProjectedMap* pGeocoder, const char* filename):
mMetadata(SARScapeHeader((std::string(filename) + std::string(".sml")).c_str()))
{
	mDsImage = GDALOpen(filename, GA_ReadOnly);
	GDALRasterBandH rbImage = GDALGetRasterBand(mDsImage, 1);
}

RadarImage::~RadarImage()
{
	GDALClose(mDsImage);
}
