#include "StdAfx.h"
#include "GeoConverter.h"

GeoConverter::GeoConverter(int UTM_Zone, int north)
{
	mUTM = OSRNewSpatialReference(0);
	OSRSetProjCS(mUTM, boost::str(boost::format("UTM %02d / WGS84") % UTM_Zone).c_str());
	OSRSetWellKnownGeogCS(mUTM, "WGS84");
	OSRSetUTM(mUTM, UTM_Zone, TRUE);

	mGeoc = OSRNewSpatialReference(0);
	OSRSetGeocCS(mGeoc, "Geocentric CS");
	OSRSetWellKnownGeogCS(mGeoc, "WGS84");
	OSRSetLinearUnits(mGeoc, "meter", 1.0);

	mTransform = OCTNewCoordinateTransformation(mUTM, mGeoc);
}


GeoConverter::~GeoConverter()
{
	OCTDestroyCoordinateTransformation(mTransform);
	OSRDestroySpatialReference(mGeoc);
	OSRDestroySpatialReference(mUTM);
}

void GeoConverter::Convert(DecartVector& vec)
{
	OCTTransform(mTransform, 1, &(vec.X), &(vec.Y), &(vec.Z));
}
