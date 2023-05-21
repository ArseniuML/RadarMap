#pragma once

#include <RadarMap/Request.h>
#include "Shaders.h"
#include "Noise.h"

namespace RadarMap {
	namespace Effects {

//Класс служит для наложения искажений
//на идеализированное изображение РЛ карты

class Processor
{
public:
	Processor();
	~Processor();

	void Resize(int width, int height);

	void Speckle(Request& req);
	void Blur(Request& req);
	void AdditiveNoise(Request& req);
	
	RLib::Collection::Texture& ActiveTexture() {return mTexFloatR2D[mActiveTexture];}

private:
	void KernelGauss(float radius, int size);

private:
	BlurAzimuthProgram mBlurAzimuthProgram;
	BlurRangeProgram mBlurRangeProgram;
	SpeckleProgram mSpeckleProgram;
	AdditiveNoiseProgram mAdditiveNoiseProgram;

	RLib::Collection::TextureFloatR mTexFloatR2D[2];
	int mActiveTexture;

	Noise mNoise;
	std::vector<float> mBlurKernel;
};

	}
}

