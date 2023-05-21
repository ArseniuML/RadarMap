#pragma once

#include <vector>
#include <string>
#include <virtualdevices_29k.h>

namespace CppCliConfigExample
{
	namespace NativeConfig
	{
		struct VideoRegion
		{
			int regionId;
			std::string description;
			int screenId;

			int left;
			int top;
			int width;
			int height;
			
			std::vector<unsigned int> supportedSources;
		};

		struct GeneratorComputer
		{
			std::string computerName;
			std::string description;
		};

		struct GeneratorScreen
		{
			int displayNumber;
			int screenId;
			std::string computerName;
			std::string description;
		};

		struct VideoConfig
		{			
			std::vector<GeneratorComputer> computers;
			std::vector<GeneratorScreen> screens;
			std::vector<VideoRegion> regions;
		};	
	};
};