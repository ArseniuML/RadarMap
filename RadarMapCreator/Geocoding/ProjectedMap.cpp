#include "StdAfx.h"
#include "ProjectedMap.h"

#include "MathUtils.h"
#include "GeoConverter.h"

inline float Backscattering(float A, float cosAlpha)
{
	if (cosAlpha < 0) return 0;
	//return A * cosAlpha;

	return A * (cosAlpha / powf(sqrtf(1.f - cosAlpha * cosAlpha) + 0.111f * cosAlpha, 3.f));
}

inline float Weight(float x, float y, float Rx, float Ry)
{
	return expf(-((x * x) / (Rx * Rx) + (y * y) / (Ry * Ry)));
}

ProjectedMap::ProjectedMap(const char* DEM_Filename)
{
	mDsDem = GDALOpen(DEM_Filename, GA_ReadOnly);

	double transform[6];
	GDALGetGeoTransform(mDsDem, transform);

	GeoRef.X0 = transform[0];
	GeoRef.dX = transform[1];
	GeoRef.Y0 = transform[3];
	GeoRef.dY = transform[5];

	GDALRasterBandH rbDEM = GDALGetRasterBand(mDsDem, 1);
	
	int nX0 = 1000;
	int nY0 = 650;

	GeoRef.Width = 502; //GDALGetRasterBandXSize(rbDEM);
	GeoRef.Height = 502; //GDALGetRasterBandYSize(rbDEM);

	GeoRef.X0 += nX0 * GeoRef.dX;
	GeoRef.Y0 += nY0 * GeoRef.dY;

	mMap.resize(GeoRef.Width * GeoRef.Height, 0);
	mDEM.resize(GeoRef.Width * GeoRef.Height);
	
	GDALRasterIO
	(
		rbDEM, GF_Read,
		nX0, 
		nY0,
		GeoRef.Width, 
		GeoRef.Height,
		&mDEM[0],
		GeoRef.Width, 
		GeoRef.Height,
		GDT_Int16,
		0,	0
	);

	mpGeoConverter = new GeoConverter(37, 1);

	/*Normals.resize((GeoRef.Width - 2) * (GeoRef.Height - 2));

	for (int ny = 0; ny < GeoRef.Height - 2; ++ ny)
	for (int nx = 0; nx < GeoRef.Width - 2; ++nx)
	{
		int nxDem = nx + 1;
		int nyDem = ny + 1;

		double h1 = DEM[(nyDem + 1) * GeoRef.Width + nxDem - 1];
		double h2 = DEM[(nyDem + 1) * GeoRef.Width + nxDem];
		double h3 = DEM[(nyDem + 1) * GeoRef.Width + nxDem + 1];

		double h4 = DEM[nyDem * GeoRef.Width + nxDem - 1];
		double h5 = DEM[nyDem * GeoRef.Width + nxDem + 1];

		double h6 = DEM[(nyDem - 1) * GeoRef.Width + nxDem - 1];
		double h7 = DEM[(nyDem - 1) * GeoRef.Width + nxDem];
		double h8 = DEM[(nyDem - 1) * GeoRef.Width + nxDem + 1];

		double normalX = h1 - h3 + 2.0 * (h4 - h5) + h6 - h8;
		double normalY = h1 - h6 + 2.0 * (h2 - h7) + h3 - h8;
		double normalZ = 8.0 * abs(GeoRef.dX);

		double denNormal = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
		normalX /= denNormal;
		normalY /= denNormal;
		normalZ /= denNormal;

		int n = ny * (GeoRef.Width - 2) + nx;
		Normals[n].X = normalX;
		Normals[n].Y = normalY;
		Normals[n].Z = normalZ;
	}*/
}

ProjectedMap::~ProjectedMap()
{
	delete mpGeoConverter;
	for (int i = 0; i < mRadarImages.size(); ++i)
	{
		delete mRadarImages[i];
	}
	GDALClose(mDsDem);
}

void ProjectedMap::AddRadarImage(const char* imageFilename)
{
	mRadarImages.push_back(new RadarImage(this, imageFilename));
}

void ProjectedMap::GeocentricPoint(int nx, int ny, DecartVector& earthPoint, DecartVector& normal)
{
	earthPoint.X = GeoRef.X0 + double(nx + 1) * GeoRef.dX;
	earthPoint.Y = GeoRef.Y0 + double(ny + 1) * GeoRef.dY;
	earthPoint.Z = mDEM[(ny + 1) * GeoRef.Width + nx + 1];

	double rightPointHeight = (mDEM[ny * GeoRef.Width + nx + 2] +
		mDEM[(ny + 1) * GeoRef.Width + nx + 2] +
		mDEM[(ny + 2) * GeoRef.Width +nx + 2]) / 3.0;

	double leftPointHeight = (mDEM[ny * GeoRef.Width + nx] +
		mDEM[(ny + 1) * GeoRef.Width + nx] +
		mDEM[(ny + 2) * GeoRef.Width + nx]) / 3.0;

	double upPointHeight = (mDEM[ny * GeoRef.Width + nx] +
		mDEM[ny * GeoRef.Width + nx + 1] +
		mDEM[ny * GeoRef.Width + nx + 2]) / 3.0;

	double downPointHeight = (mDEM[(ny + 2) * GeoRef.Width + nx] +
		mDEM[(ny + 2) * GeoRef.Width + nx + 1] +
		mDEM[(ny + 2) * GeoRef.Width + nx + 2]) / 3.0;
	
	DecartVector rightPoint(earthPoint.X + GeoRef.dX, earthPoint.Y, rightPointHeight);
	DecartVector leftPoint(earthPoint.X - GeoRef.dX, earthPoint.Y, leftPointHeight);
	DecartVector upPoint(earthPoint.X, earthPoint.Y - GeoRef.dY, upPointHeight);
	DecartVector downPoint(earthPoint.X, earthPoint.Y + GeoRef.dY, downPointHeight);

	mpGeoConverter->Convert(earthPoint);
	mpGeoConverter->Convert(rightPoint);
	mpGeoConverter->Convert(leftPoint);
	mpGeoConverter->Convert(upPoint);
	mpGeoConverter->Convert(downPoint);

	DecartVector a(rightPoint.X - leftPoint.X, rightPoint.Y - leftPoint.Y, rightPoint.Z - leftPoint.Z);
	DecartVector b(downPoint.X - upPoint.X, downPoint.Y - upPoint.Y, downPoint.Z - upPoint.Z);
	
	normal.X = a.Y * b.Z - a.Z * b.Y;
	normal.Y = a.Z * b.X - a.X * b.Z;
	normal.Z = a.X * b.Y - a.Y * b.X;

	double len = sqrt(normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z);
	normal.X /= len;
	normal.Y /= len;
	normal.Z /= len;

	if (normal.X * earthPoint.X + normal.Y * earthPoint.Y + normal.Z * earthPoint.Z < 0)
	{
		normal.X = - normal.X;
		normal.Y = - normal.Y;
		normal.Z = - normal.Z;
	}
}

void ProjectedMap::BuildReflectivityMap()
{
	RadarImage* ri = mRadarImages[0];

	for (int it = 0; it < 4; ++it)
	for (int n = 0, ny = 0; ny < GeoRef.Height - 2; ++ny)
	for (int nx = 0; nx < GeoRef.Width - 2; ++nx, ++n)
	{
		//if (mask[n] == 0)
		{
			float num = 0;
			float den = 0;
			
			for (int nk = 0; nk < ri->fwdLUT.KernelSize(); ++nk)
			{
				int nFwd = n * ri->fwdLUT.KernelSize() + nk;
				
				int imgInd = ri->fwdLUT(nFwd).Index;
				float y = ri->Image[imgInd];
				
				for (int nBwd = 0; nBwd <  ri->bwdLUT.ItemSize(imgInd); ++nBwd)
				{
					int nBwdFwd = ri->bwdLUT(imgInd, nBwd);
					int mapInd =  nBwdFwd / ri->fwdLUT.KernelSize();
					
					if (mapInd != n)
					{
						y -= ri->fwdLUT(nBwdFwd).Weight * 
							Backscattering(mMap[mapInd], ri->LIA[mapInd]);
					}
				}

				float w =  ri->fwdLUT(nFwd).Weight;
				num += w * y;
				den += w * w;
			}

			float mn = num / (ri->LIA[n] * den);
			if (mn < 0) mn = 0;
			
			mMap[n] = mn; 
			//mMap[n] + 0.3 * (mn - mMap[n]);

			float sumW = 0;
			float mnew = 0;

			int kHSz;
			if (it < 4)
				kHSz = 5;
			else if (it < 5)
				kHSz = 5;
			else 
				kHSz = 5;

			for (int nk = 0, dny = -kHSz; dny <= kHSz; ++dny)
			for (int dnx = -kHSz; dnx <= kHSz; ++dnx, ++nk)
			{
				int ixnew = nx + dnx;
				int iynew = ny + dny;

				if (ixnew >=0 && ixnew < GeoRef.Width - 2 
					&& iynew >=0 && iynew < GeoRef.Height - 2)
				{
					int nNew = iynew * (GeoRef.Width - 2) + ixnew;
					float w = Weight
					(
						ri->Coords[nNew].Azimuth - ri->Coords[n].Azimuth,
						ri->Coords[nNew].Range - ri->Coords[n].Range, 
						1, 1
					);
					sumW += w;
					mnew += w * mMap[nNew];
				}
			}

			mnew /= sumW;
			mMap[n] = mnew;
		}
	}

	GDALDatasetH ds = GDALCreate
	(
		GDALGetDriverByName("GTiff"),
		"D:\\Projects\\Syria\\GEO\\RMC\\LR2\\test4.tif", 
		GeoRef.Width - 2, 
		GeoRef.Height - 2, 
		1, 
		GDT_Float32,
		NULL
	);

	CPLErr err = GDALRasterIO
	(
		GDALGetRasterBand(ds, 1), GF_Write,
		0, 
		0,
		GeoRef.Width - 2, 
		GeoRef.Height - 2,
		&mMap[0],
		GeoRef.Width - 2, 
		GeoRef.Height - 2,
		GDT_Float32,
		0,	0
	);

	GDALDatasetH dsLIA = GDALCreate
	(
		GDALGetDriverByName("GTiff"),
		"D:\\Projects\\Syria\\GEO\\RMC\\LR2\\testLIA4.tif", 
		GeoRef.Width - 2, 
		GeoRef.Height - 2, 
		1, 
		GDT_Float32,
		NULL
	);

	err = GDALRasterIO
	(
		GDALGetRasterBand(dsLIA, 1), GF_Write,
		0, 
		0,
		GeoRef.Width - 2, 
		GeoRef.Height - 2,
		&mRadarImages[0]->LIA[0],
		GeoRef.Width - 2, 
		GeoRef.Height - 2,
		GDT_Float32,
		0,	0
	);

	GDALClose(ds);
	GDALClose(dsLIA);
}
