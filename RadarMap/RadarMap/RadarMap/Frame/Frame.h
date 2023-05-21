#pragma once

#include "Shaders.h"
#include "CubicKernel.h"
#include <RadarMap/Request.h>

namespace RadarMap {
	namespace Frame {

// ласс нужен дл€ преобразовани€ –Ћ изображени€ 
//в формате floating-point и в системе координат азимут - дальность
//в изображение, непосредственно выводимое на экран (градации серого 0-255),
//а также дл€ преобразовани€ в декартову систему координат (при необходимости)

class Frame
{
public:
	Frame();
	~Frame();
	
	void SetProjectedSize(int width, int height);
	
	void SetRequest(const Request& req);
	void SetFrame(RLib::Collection::Texture& signalTex);
	
	void Render(float brightness);

private:
	GLuint mFB;
	
	Request mRequest;
	RLib::Collection::TextureFloatR mSignalTex;
	RLib::Collection::TextureRGB mBrightnessTex;
	CubicKernel mCubicKernelTexture;
	
	BrightnessRegulatorProgram mBrightnessRegulatorProgram;
	DecartProjectorProgram mDecartProjectorProgram;
	PolarProjectorProgram mPolarProjectorProgram;

	int mProjectedWidth;
	int mProjectedHeight;
	
	bool mReady;
};

	}
}
