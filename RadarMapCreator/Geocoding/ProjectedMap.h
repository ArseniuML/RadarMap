#pragma once
#include "RadarImage.h"
#include "SimpleTypes.h"
#include "GeoConverter.h"

class ProjectedMap
{
public:
	struct RasterParams
	{
		int Width, Height;
		double X0, Y0;
		double dX, dY;
	};	

	ProjectedMap(const char* DEM_Filename);
	~ProjectedMap();

	void AddRadarImage(const char* imageFilename);
	void GeocentricPoint(int nx, int ny, DecartVector& earthPoint, DecartVector& normal);
	void BuildReflectivityMap();

private:
	GDALDatasetH mDsMap;
	std::vector<float> mMap;
	GDALDatasetH mDsDem;
	std::vector<RadarImage*> mRadarImages;
	
	std::vector<short> mDEM;
	GeoConverter* mpGeoConverter;

public:
	RasterParams GeoRef;
};

