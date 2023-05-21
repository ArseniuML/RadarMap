#pragma once

namespace RadarMap {
	namespace Config {

//Настройки приложения

struct TechConfig
{
	struct Projection
	{
		enum
		{
			Decart,
			Polar
		};
	};

	struct Render
	{
		enum
		{
			Geometry,
			Point
		};
	};

	struct Mode
	{
		struct  
		{
			int Azimuth;
			int Range;
		} Elements;
		
		struct  
		{
			float Azimuth;
			float Range;
		} Resolution;

		int PartialFrames;
		int ResultProjection;
		int TargetRender;
	};
	
	int Looks;
	struct  
	{
		float Mean;
		float Dev;
	} AdditiveNoise;

	std::vector<Mode> Modes;

	void Load(const char* fileName);
};

	}
}
