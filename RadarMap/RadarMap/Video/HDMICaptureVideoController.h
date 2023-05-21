#pragma once

#include "XmlConfigReader.h"

#include <RLib/Window/Window.h>
#include <Video/DefMonitors.h>
#include <Video/HDMICapture.h>

namespace Video {

class HDMICaptureVideoController : 
	protected HDMICapture,
	private CDefMonitors
{
public:
	HDMICaptureVideoController(radarmap_29k::CsItems& io, RLib::Window::Window& window);
	~HDMICaptureVideoController();
	void Check();

private:
	RLib::Window::Window& mWindow;
	
	std::vector<CppCliConfigExample::NativeConfig::VideoRegion> mVideoRegions;
	mgcoord2i mMonitorOrigin;

	int mCurrentRegionId;
};

}
