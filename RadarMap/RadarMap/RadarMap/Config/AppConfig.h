#pragma once

namespace RadarMap {
	namespace Config {

//Настройки моделируемого радара

struct AppConfig  
{
	struct  
	{
		bool Shading;
		bool SpeckleNoise;
		bool BeamPattern;
		bool AdditiveNoise;
	} PostProcessing;
	
	struct  
	{
		float Geometry;
		float Textures;
	} HDDRate;

	struct
	{
		std::string Terrain;
		std::string Targets;
	} LoadOnStartup;

	struct  
	{
		bool Trace;
		bool StaticRun;
		bool StaticWindow;
	} Debug;

	void Load(const char* fileName);
};
	
	}
}