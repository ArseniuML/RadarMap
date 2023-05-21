#pragma once
#include <RadarImage.h>
#include <ProjectedMap.h>

class ImageMapLink
{
public:
	ImageMapLink(RadarImage& radarImage, ProjectedMap& projectedMap);
	~ImageMapLink(void);

	RadarImage& mrRadarImage;
	ProjectedMap& mrProjectedMap;

	std::vector<float> LIA;
	std::vector<RadarVector> Coords;

	FwdLUT fwdLUT;
	BwdLUT bwdLUT;
};

