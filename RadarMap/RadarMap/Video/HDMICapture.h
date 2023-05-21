#pragma once

#import "..\..\common\dll\CommonTypes.dll"  no_namespace, named_guids exclude("IRT")  exclude("ISTC")

#include <radarmap_29k.h>
#include <graph_29k.h>
#include <CommServerData.h>

namespace Video {

class HDMICapture
{
public:
	HDMICapture(radarmap_29k::CsItems& io):
	mIo(io)
	{
	}

	int GetRequired(int number)
	{
		return mIo.VideoChannels.Generator.Required[number];
	}

	int GetCurrent(int number)
	{
		return mIo.VideoChannels.Generator.Current[number];
	}

	void SetCurrent(int number, int virtualDeviceType)
	{
		mIo.VideoChannels.Generator.Current[number] = virtualDeviceType;
	}

private:
	radarmap_29k::CsItems&	mIo;
};
	
}
