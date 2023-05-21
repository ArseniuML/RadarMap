#pragma once

using namespace System;
using namespace System::Configuration;
using namespace System::Xml;
using namespace System::Xml::XPath;
using namespace System::Xml::Serialization;
using namespace System::Collections::Generic;

namespace CppCliConfigExample
{
	namespace ClrConfig
	{
		public ref class VideoRegion
		{
		public:
			property int  RegionId;
			property String^ Description;

			property int  ScreenId;
			property int  Left;
			property int  Top;
			property int  Width;
			property int  Height;			

			property array<String^>^ SupportedSources;
		};

		public ref class GeneratorComputer
		{
		public:
			property String^ ComputerName;
			property String^ Description;
		};

		public ref class GeneratorScreen
		{
		public:
			property UInt32  DisplayNumber;
			property UInt32  ScreenId;
			property String^ ComputerName;
			property String^ Description;
		};

		public ref class VideoConfig
		{			
		public:
			property array<GeneratorComputer^>^ Computers;
			property array<GeneratorScreen^>^ Screens;
			property array<VideoRegion^>^ Regions;
		};		
	};
};