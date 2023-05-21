#pragma once
#include "SimpleTypes.h"

class GeoConverter
{
public:
	GeoConverter(int UTM_Zone, int north);
	~GeoConverter();

	void Convert(DecartVector& vec);

private:
	OGRSpatialReferenceH mUTM, mGeoc;
	OGRCoordinateTransformationH mTransform;
};

