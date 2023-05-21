#include "StdAfx.h"
#include "HDMICaptureVideoController.h"

#include <virtualdevices_29k.h>
using namespace virtualdevices_29k;

using namespace CppCliConfigExample;
using namespace CppCliConfigExample::NativeConfig;

Video::HDMICaptureVideoController::HDMICaptureVideoController(radarmap_29k::CsItems& io, RLib::Window::Window& window):
HDMICapture(io),
mWindow(window),
mCurrentRegionId(-1)
{
	mMonitorOrigin.x = 0;
	mMonitorOrigin.y = 0;

	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD couputerNameSize;

	memset(computerName, 0, (MAX_COMPUTERNAME_LENGTH + 1) * sizeof(char));
	GetComputerNameA(computerName, &couputerNameSize);

	cout << "RadarMap: computerName " << computerName << endl <<flush;

	VideoConfig config = XmlConfigReader::ReadVideoConfig("videochannels.config");

	BOOST_FOREACH (VideoRegion& region, config.regions)
	{
		if (region.regionId < 0 && region.regionId >= config.regions.size())
			throw
				std::exception("Radar Map: error in tag VideoRegion.regionId in videochannels.config");
	
		if (region.screenId < 0 && region.screenId >= config.screens.size())
			throw
				std::exception("Radar Map: error in tag VideoRegion.screenId in videochannels.config");

		GeneratorScreen screen = config.screens[region.screenId];

		//if (screen.computerName == std::string(computerName))
		{
			mgcoord2i monitorSize;
			GetMonitorParams
			(
				screen.displayNumber,
				monitorSize.x,
				monitorSize.y,
				mMonitorOrigin.x,
				mMonitorOrigin.y
			);

			BOOST_FOREACH (int src, region.supportedSources)
			{
				cout << "RadarMap: supported source " << src << endl << flush;

				if (src == VirtualDeviceType::RLMap)
				{
					cout << "RadarMap: region id = " << region.regionId << endl << flush;
					cout << "RadarMap: region screenId = " << region.screenId << endl << flush;
					cout << "RadarMap: region left = " << region.left << endl << flush;
					cout << "RadarMap: region top = " << region.top << endl << flush;
					cout << "RadarMap: region width = " << region.width << endl << flush;
					cout << "RadarMap: region height = " << region.height << endl << flush;
					
					mVideoRegions.push_back(region);
					break;
				}
			}
		}
	}
}

Video::HDMICaptureVideoController::~HDMICaptureVideoController()
{
	BOOST_FOREACH (VideoRegion& region, mVideoRegions)
	{
		if (GetCurrent(region.regionId)  == VirtualDeviceType::RLMap)
		{
			SetCurrent(region.regionId, VirtualDeviceType::None);					
			cout << "RadarMap: Remove from region " << region.regionId <<  endl << flush;
		}
	}
}

void Video::HDMICaptureVideoController::Check()
{
	if (appConfig.Debug.StaticWindow)
	{
		static bool shown = false;
		if (!shown)
		{
			mWindow.Show
			(
				0, 
				0, 
				592, 
				592
			);
			shown = true;
			cout << "RadarMap: request to show window static " << endl << flush;
		}
	}
	else
	{
		bool required = false;
		BOOST_FOREACH (VideoRegion& region, mVideoRegions)
		{
			if (GetRequired(region.regionId) == VirtualDeviceType::RLMap)
			{
				required = true;

				if (region.regionId != mCurrentRegionId)
				{ 
					mWindow.Show
					(
						mMonitorOrigin.x + region.left, 
						mMonitorOrigin.y + region.top, 
						region.width, 
						region.height
					);
					SetCurrent(region.regionId, VirtualDeviceType::RLMap);
					mCurrentRegionId = region.regionId;
					cout << "RadarMap: request to show window on region " << region.regionId << " accepted" << endl << flush;
				}

				break;
			}
			
			if (!required)
			{
				if (mCurrentRegionId != -1)
				{
					mWindow.Hide();
					mCurrentRegionId = -1;
					cout << "RadarMap: hide" << endl << flush;
				}
			}

			BOOST_FOREACH (VideoRegion& region, mVideoRegions)
			{
				if (region.regionId != mCurrentRegionId && GetCurrent(region.regionId) == VirtualDeviceType::RLMap)
				{
					SetCurrent(region.regionId, VirtualDeviceType::None);
					cout << "RadarMap: VirtualDeviceType::None has written to region" << region.regionId << endl << flush;
				}
			}		
		}
	}
}
