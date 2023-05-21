#include "StdAfx.h"
#include "TVCaptureVideoController.h"

#include <virtualdevices_29k.h>
using namespace virtualdevices_29k;

Video::TVCaptureVideoController::TVCaptureVideoController(radarmap_29k::CsItems& io, RLib::Window::Window& window):
TVCapture(io),
mWindow(window),
mCurrentMonitor(-1)
{
}

Video::TVCaptureVideoController::~TVCaptureVideoController()
{
  for (int i = 0; i < 4; ++i)
  {
	  if (GetCurrent(i)  == VirtualDeviceType::RLMap)
	  {
		  SetCurrent(i, VirtualDeviceType::None);					
		  cout << "Remove from monitor number " << i <<  endl << flush;
	  }
  }
}

void Video::TVCaptureVideoController::Check()
{
  for (int i = 0; i < 4; ++i)
  {
	  if (GetRequired(i) == VirtualDeviceType::RLMap)
	  {
		  if (mCurrentMonitor != i)
		  { 
			  Show(i);
		  }
		  break;
	  }
  }

  for (int i = 0; i < 4; ++i)
  {
	  if (GetCurrent(i) == VirtualDeviceType::RLMap && mCurrentMonitor !=i)
	  {
		  SetCurrent(i, VirtualDeviceType::None);
		  cout << "RadarMap: write CS, remove from monitor" << i << endl << flush;
	  }
  }

  if (mCurrentMonitor != -1)
  {
	  if (GetRequired(mCurrentMonitor) == VirtualDeviceType::RLMap)
	  {
		  if (GetCurrent(mCurrentMonitor) != VirtualDeviceType::RLMap)
		  {
			  SetCurrent(mCurrentMonitor, VirtualDeviceType::RLMap);
			  cout << "RadarMap: write CS, set current = 101 to monitor" << mCurrentMonitor << endl << flush; 
		  }
	  }
	  else
	  {
		  Hide();
	  }
  }
}

void Video::TVCaptureVideoController::Show(int iMonitor)
{
	//ќпредел€ем координаты области, куда выводить окно

	RECT rect; mWindow.GetClientRect(&rect);

	BOOST_FOREACH(Video::Monitor& sysMonitor, list_monitors)
	{
		if (sysMonitor.nmb == iMonitor + 1)
		{
			rect = sysMonitor.info_ex.rcMonitor;
			break;
		}
	}

	mWindow.Show
	(
		rect.left, 
		rect.top, 
		rect.right - rect.left, 
		rect.bottom - rect.top
	);

	mCurrentMonitor = iMonitor;

	cout << "RadarMap: show monitor" << iMonitor << endl ;
	cout << rect.left << " " << " " <<  rect.right << " " <<  rect.bottom << " " <<  rect.top << endl << flush;
}	

void Video::TVCaptureVideoController::Hide()
{
	mWindow.Hide();
	mCurrentMonitor = -1;
}
