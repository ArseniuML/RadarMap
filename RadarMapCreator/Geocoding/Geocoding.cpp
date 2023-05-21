#include "stdafx.h"

#include "ProjectedMap.h"

int _tmain(int argc, _TCHAR* argv[])
{
	GDALAllRegister();
	
	ProjectedMap geoCoder("D:\\Projects\\Syria\\DEM\\ASTER\\Geocoding\\lr_utm.tif");
	geoCoder.AddRadarImage("D:\\Projects\\Syria\\PWR\\LR2\\slc2_pwr");
	geoCoder.BuildReflectivityMap();
	
	return 0;
}