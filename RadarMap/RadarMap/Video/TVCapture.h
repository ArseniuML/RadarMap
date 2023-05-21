#pragma once

#import "..\..\common\dll\CommonTypes.dll"  no_namespace, named_guids exclude("IRT")  exclude("ISTC")

#include <radarmap_29k.h>
#include <graph_29k.h>
#include <CommServerData.h>

namespace Video {

class TVCapture
{
public:
	TVCapture(radarmap_29k::CsItems& io):
	mIo(io)
	{
	}

	int GetRequired(int number)
	{
		// В случае, если РЛ карта запускается с машины ОЛС, 
		//	номера доступных мониторов находятся в диапазоне [4..7]
		switch (number + 4)
		{
		case 4:
			return mIo.Requests.Monitor4.Required;
		case 5:
			return mIo.Requests.Monitor5.Required;
		case 6:
			return mIo.Requests.Monitor6.Required;
		case 7:
			return mIo.Requests.Monitor7.Required;
		}
		return virtualdevices_29k::VirtualDeviceType::None;
	}

	int GetCurrent(int number)
	{
		switch (number + 4)
		{
		case 4:
			return mIo.Requests.Monitor4.Current;
		case 5:
			return mIo.Requests.Monitor5.Current;
		case 6:
			return mIo.Requests.Monitor6.Current;
		case 7:
			return mIo.Requests.Monitor7.Current;
		}
		return virtualdevices_29k::VirtualDeviceType::None;
	}

	void SetCurrent(int number, int virtualDeviceType)
	{
		switch (number + 4)
		{
		case 4:
			mIo.Requests.Monitor4.Current = virtualDeviceType; return;
		case 5:
			mIo.Requests.Monitor5.Current = virtualDeviceType; return;
		case 6:
			mIo.Requests.Monitor6.Current = virtualDeviceType; return;
		case 7:
			mIo.Requests.Monitor7.Current = virtualDeviceType; return;
		}
		return;
	}

private:
	radarmap_29k::CsItems&	mIo;
};
	
}
