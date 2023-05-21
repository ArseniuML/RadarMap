#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Effects {

class AdditiveNoiseProgram :  public RLib::Shader::Program
{
public:
	AdditiveNoiseProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\AdditiveNoise.fs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\Permutation.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniTexNoise = glGetUniformLocationARB(mhProgObj, "texNoise");
		mUniTexPermutation = glGetUniformLocationARB(mhProgObj, "texPermutation");
		mUniNoiseParams.Mean = glGetUniformLocationARB(mhProgObj, "noiseParams.Mean");
		mUniNoiseParams.Dev = glGetUniformLocationARB(mhProgObj, "noiseParams.Dev");
	}

	int mUniTexInput;
	int mUniTexNoise;
	int mUniTexPermutation;
	
	struct
	{
		int Mean;
		int Dev;
	} mUniNoiseParams;
};

class SpeckleProgram :  public RLib::Shader::Program
{
public:
	SpeckleProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\Speckle.fs");
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\Permutation.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniTexNoise = glGetUniformLocationARB(mhProgObj, "texNoise");
		mUniTexPermutation = glGetUniformLocationARB(mhProgObj, "texPermutation");
		mUniNumLooks = glGetUniformLocationARB(mhProgObj, "numLooks");
	}

	int mUniTexInput;
	int mUniTexNoise;
	int mUniTexPermutation;
	int mUniNumLooks;
};

class BlurAzimuthProgram : public RLib::Shader::Program
{
public:
	BlurAzimuthProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\BlurAzimuth.fs");
		Link();
		
		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniKernelSize = glGetUniformLocationARB(mhProgObj, "kernelSize");
		mUniKernelValue = glGetUniformLocationARB(mhProgObj, "kernelValue");
	}
	
	int mUniTexInput;
	int mUniKernelSize;
	int mUniKernelValue;
};
	
class BlurRangeProgram : public RLib::Shader::Program
{
public:
	BlurRangeProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Effects\\BlurRange.fs");
		Link();

		mUniTexInput = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniKernelSize = glGetUniformLocationARB(mhProgObj, "kernelSize");
		mUniKernelValue = glGetUniformLocationARB(mhProgObj, "kernelValue");
	}

	int mUniTexInput;
	int mUniKernelSize;
	int mUniKernelValue;
};
	}
}