#pragma once

#include <RLib/Window/Window.h>
#include <Video/DefMonitors.h>
#include <Video/TVCapture.h>

namespace Video {

class TVCaptureVideoController : 
	protected TVCapture, 
	private CDefMonitors
{
public:
	TVCaptureVideoController(radarmap_29k::CsItems& io, RLib::Window::Window& window);
	~TVCaptureVideoController();
	void Check();

private:
	void Show(int iMonitor);
	void Hide();

private:
	 RLib::Window::Window& mWindow;
	int mCurrentMonitor;
};

}
