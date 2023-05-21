#pragma once

#include "NativeVideoConfig.h"
#include <string>

namespace CppCliConfigExample
{
	class XmlConfigReader
	{
	public:
		XmlConfigReader(void);
		~XmlConfigReader(void);

	public:
		static NativeConfig::VideoConfig ReadVideoConfig(const char* fileName);
	};
};

