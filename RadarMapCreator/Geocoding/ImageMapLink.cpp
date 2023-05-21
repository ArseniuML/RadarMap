#include "StdAfx.h"
#include "ImageMapLink.h"

inline float Weight(float x, float y, float Rx, float Ry)
{
	return expf(-((x * x) / (Rx * Rx) + (y * y) / (Ry * Ry)));
}

ImageMapLink::ImageMapLink(RadarImage& radarImage, ProjectedMap& projectedMap):
mrRadarImage(radarImage),
mrProjectedMap(projectedMap)
{
	Coords.resize((geoCoder->GeoRef.Width - 2) * (geoCoder->GeoRef.Height - 2));
	LIA.resize((geoCoder->GeoRef.Width - 2) * (geoCoder->GeoRef.Height - 2));

	double rgMin = DBL_MAX;
	double rgMax = - DBL_MAX;

	double azMin = DBL_MAX;
	double azMax = - DBL_MAX;

	for (int ny = 0; ny < geoCoder->GeoRef.Height - 2; ++ ny)
		for (int nx = 0; nx < geoCoder->GeoRef.Width - 2; ++nx)
		{
			int nMap = ny * (geoCoder->GeoRef.Width - 2) + nx;

			DecartVector EarthPoint, Normal;
			geoCoder->GeocentricPoint(nx, ny, EarthPoint, Normal);

			DecartVector lookVec;
			RadarPoint(EarthPoint, &lookVec, &Coords[nMap]);

			rgMin = std::min(rgMin, Coords[nMap].Range);
			rgMax = std::max(rgMax, Coords[nMap].Range);

			azMin = std::min(azMin, Coords[nMap].Azimuth);
			azMax = std::max(azMax, Coords[nMap].Azimuth);
		}

	mX0 = floor(rgMin);
	mY0 = floor(azMin);

	mWidth = ceil(rgMax) - mX0 + 1;
	mHeight = ceil(azMax) - mY0 + 1;

	Image.resize(mWidth * mHeight);
	GDALRasterIO
	(
		rbImage, GF_Read,
		mX0, mY0,
		mWidth, mHeight,
		&Image[0],
		mWidth,	mHeight,
		GDT_Float32,
		0,	0
	);

	int FwdKernHSz = 2;
	int FwdKernSz = 2 * FwdKernHSz;

	fwdLUT.Resize(Coords.size(), FwdKernSz * FwdKernSz);
	bwdLUT.SetImageSize(Image.size());

	for (int n = 0; n < Coords.size(); ++n)
	{
		int rgn0 = floor(Coords[n].Range) - (FwdKernHSz - 1) - mX0;
		int azn0 = floor(Coords[n].Azimuth) - (FwdKernHSz - 1) - mY0;

		for (int azn = azn0; azn < azn0 + FwdKernSz; ++azn)
			for (int rgn = rgn0; rgn < rgn0 + FwdKernSz; ++rgn)
			{
				if ( rgn >= 0 && rgn < mWidth &&
					azn >= 0 && azn < mHeight)
				{
					int nImg = azn * mWidth + rgn;
					bwdLUT.IncreaseItemSize(nImg);
				}
			}
	}

	bwdLUT.Prepare();

	double corrRg =  1;
	double corrAz = 1;

	for (int n = 0, nFwdItem = 0; n < Coords.size(); ++n, nFwdItem += fwdLUT.KernelSize())
	{
		int rgn0 = floor(Coords[n].Range) - (FwdKernHSz - 1) - mX0;
		int azn0 = floor(Coords[n].Azimuth) - (FwdKernHSz - 1) - mY0;

		float den = 0;

		for (int nk = 0, azn = azn0; azn < azn0 + FwdKernSz; ++azn)
			for (int rgn = rgn0; rgn < rgn0 + FwdKernSz; ++rgn, ++nk)
			{
				if ( rgn >= 0 && rgn < mWidth &&
					azn >= 0 && azn < mHeight)
				{
					int nImg = azn * mWidth + rgn;

					float weight = Weight
						(
						Coords[n].Range - double(rgn + mX0), 
						Coords[n].Azimuth - double(azn + mY0), 
						corrRg,
						corrAz 
						);

					int nFwd = nFwdItem + nk; 
					fwdLUT(nFwd).Index = nImg;
					fwdLUT(nFwd).Weight = weight;
					bwdLUT.Fill(nImg, nFwd);

					den += weight;
				}
			}

			for(int nk = 0; nk < fwdLUT.KernelSize(); ++nk)
			{
				fwdLUT(nFwdItem + nk).Weight /= den;
			}
	}
}


ImageMapLink::~ImageMapLink(void)
{
}
