#pragma once

#include <RadarMap/Config/TechConfig.h>
#include <RadarMap/RequestData.h>

namespace RadarMap {

//Структура для хранения параметров кадра РЛ карты

struct Request
{
	struct
	{
		double X, Y, Z;
		float	Roll, Pitch, Heading;
		float	Height;
	} Airplane;

	struct  
	{
		float Azimuth; 
		float AzimuthHRadius;

		float DistanceMin;
		float DistanceMax;

		float TanElevationMin;
		float TanElevationMax;
	} FOV;

	struct
	{
		float Left;
		float Right;
		float Bottom;
		float Top;
	} Extent;

	int Mode;

	void SetRequest(const RequestData& rP)
	{
		Airplane.X = rP.Airplane.Z;
		Airplane.Y = rP.Airplane.X;
		Airplane.Z = rP.Airplane.Y;
		Airplane.Height = rP.Airplane.Height;

		FOV.Azimuth = 0.5f * (rP.AzimuthLeft + rP.AzimuthRight) + rP.Airplane.Heading;
		FOV.AzimuthHRadius = 0.5f * (rP.AzimuthLeft - rP.AzimuthRight);

		FOV.DistanceMin = rP.DMin;
		FOV.DistanceMax = rP.DMax;

		Extent.Left = - 0.5f * rP.MapWidth;
		Extent.Right = 0.5f * rP.MapWidth;

		float mapCenter = sqrtf(rP.DCenter * rP.DCenter - rP.Airplane.Height * rP.Airplane.Height);
		Extent.Bottom = mapCenter - 0.5f * rP.MapWidth;
		Extent.Top = mapCenter + 0.5f * rP.MapWidth;

		Mode = rP.Mode - 1;

		if (appConfig.Debug.Trace)
		{
			cout << "RadarMap: request X " << Airplane.X << " Y " << Airplane.Y << " Z " << Airplane.Z << " height " << Airplane.Height << endl << flush; 
			cout << "RadarMap: request azimuth " << FOV.Azimuth << " half radius " << FOV.AzimuthHRadius << endl << flush;
			cout << "RadarMap: request DMin " << FOV.DistanceMin << " DMax " << FOV.DistanceMax << endl << flush;
			cout << "RadarMap: extent left right top bottom " << Extent.Left << " " << Extent.Right << " " << Extent.Top << " " << Extent.Bottom << endl << flush;
			cout << "RadarMap: request mode = " << Mode << endl << flush;
		}
	}
};
}