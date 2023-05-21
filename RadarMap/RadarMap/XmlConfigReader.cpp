//#include <msclr\marshal.h>
//#include <msclr\marshal_cppstd.h>

//#include "StdAfx.h"
#include "XmlConfigReader.h"

#include "ClrVideoConfig.h"
#include "NativeVideoConfig.h"

using namespace System::Configuration;
using namespace System::Reflection;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
//using namespace msclr::interop;

namespace CppCliConfigExample
{

	XmlConfigReader::XmlConfigReader(void)
	{
	}

	XmlConfigReader::~XmlConfigReader(void)
	{
	}

	NativeConfig::VideoConfig XmlConfigReader::ReadVideoConfig(const char* fileName)
	{
		ClrConfig::VideoConfig ^clrConfig;

		try
		{
			XmlSerializer  ^xmlSerializer = gcnew XmlSerializer(ClrConfig::VideoConfig::typeid);
			FileStream ^fs                = gcnew FileStream(gcnew String(fileName), FileMode::Open, FileAccess::Read);
			XmlReader ^xmlReader          = XmlReader::Create(fs);
			clrConfig = (ClrConfig::VideoConfig ^) xmlSerializer->Deserialize(xmlReader);
			xmlReader->Close();
			fs->Close();
		}
		catch (Exception ^ex)
		{
			char *strBuf = (char*)Marshal::StringToHGlobalAnsi(ex->ToString()).ToPointer();
			std::exception stdex(strBuf);
			Marshal::FreeHGlobal((IntPtr)strBuf);

			throw stdex;
		}


		NativeConfig::VideoConfig nativeConfig;

		char* strBuf;

		for each (ClrConfig::GeneratorComputer ^clrComputer in clrConfig->Computers)
		{
			NativeConfig::GeneratorComputer computer;

			strBuf = (char*)Marshal::StringToHGlobalAnsi(clrComputer->ComputerName).ToPointer();
			computer.computerName = strBuf;
			Marshal::FreeHGlobal((IntPtr)strBuf);

			strBuf = (char*)Marshal::StringToHGlobalAnsi(clrComputer->Description).ToPointer();
			computer.description = strBuf;
			Marshal::FreeHGlobal((IntPtr)strBuf);

			nativeConfig.computers.push_back(computer);
		}

		for each (ClrConfig::GeneratorScreen ^clrScreen in clrConfig->Screens)
		{
			NativeConfig::GeneratorScreen screen;
			screen.screenId = clrScreen->ScreenId;
			screen.displayNumber = clrScreen->DisplayNumber;

			if (!String::IsNullOrEmpty(clrScreen->ComputerName))
			{
				strBuf = (char*)Marshal::StringToHGlobalAnsi(clrScreen->ComputerName).ToPointer();
				screen.computerName = strBuf;
				Marshal::FreeHGlobal((IntPtr)strBuf);
			}

			if (!String::IsNullOrEmpty(clrScreen->Description))
			{			
				strBuf = (char*)Marshal::StringToHGlobalAnsi(clrScreen->Description).ToPointer();
				screen.description = strBuf;
				Marshal::FreeHGlobal((IntPtr)strBuf);
			}

			nativeConfig.screens.push_back(screen);
		}

		for each (ClrConfig::VideoRegion ^clrRegion in clrConfig->Regions)
		{
			NativeConfig::VideoRegion region;
			region.regionId = clrRegion->RegionId;

			if (!String::IsNullOrEmpty(clrRegion->Description))
			{
				strBuf = (char*)Marshal::StringToHGlobalAnsi(clrRegion->Description).ToPointer();
				region.description = strBuf;
				Marshal::FreeHGlobal((IntPtr)strBuf);
			}

			region.screenId = clrRegion->ScreenId;

			region.left   = clrRegion->Left;
			region.top    = clrRegion->Top;
			region.width  = clrRegion->Width;
			region.height = clrRegion->Height;

			for each (String ^strSource in clrRegion->SupportedSources)
			{
				if (strSource == "RLMap")
					region.supportedSources.push_back(virtualdevices_29k::VirtualDeviceType::RLMap);
			}

			nativeConfig.regions.push_back(region);
		}
		return nativeConfig;
	};
};