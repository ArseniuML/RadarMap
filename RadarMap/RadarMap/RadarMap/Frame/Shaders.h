#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Frame {

class BrightnessRegulatorProgram : public RLib::Shader::Program
{
public:
	BrightnessRegulatorProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Frame\\BrightnessRegulator.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniBrightness = glGetUniformLocationARB(mhProgObj, "brightness");
	}

	int mUniTexInput;
	int mUniBrightness;
};

class DecartProjectorProgram: public RLib::Shader::Program
{
public:
	DecartProjectorProgram()
	{
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Frame\\Projector.vs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Frame\\BicubicFilter.fs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Frame\\DecartProjector.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniTexInputSize = glGetUniformLocationARB(mhProgObj, "texInputSize");

		mUniKernelTex = glGetUniformLocationARB(mhProgObj, "kernelTex");
		mUniKernelSize = glGetUniformLocationARB(mhProgObj, "kernelSize");
	}

	int mUniTexInput;
	int mUniTexInputSize;

	int mUniKernelTex;
	int mUniKernelSize;
};

class PolarProjectorProgram: public RLib::Shader::Program
{
public:
	PolarProjectorProgram()
	{
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Frame\\Projector.vs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Frame\\BicubicFilter.fs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Frame\\PolarProjector.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");

		mUniKernelTex = glGetUniformLocationARB(mhProgObj, "kernelTex");
		mUniKernelSize = glGetUniformLocationARB(mhProgObj, "kernelSize");
	}

	int mUniTexInput;
	int mUniKernelTex;
	int mUniKernelSize;
};


	}
}