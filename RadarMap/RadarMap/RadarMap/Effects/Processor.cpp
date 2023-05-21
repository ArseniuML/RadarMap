#include "stdafx.h"
#include "Processor.h"

#include <RLib/Processing/Processing.h>
#include <RadarMap/Constants.h>

RadarMap::Effects::Processor::Processor():
mActiveTexture(0),
mBlurKernel(32)
{
}

RadarMap::Effects::Processor::~Processor()
{

}

void RadarMap::Effects::Processor::Resize( int width, int height )
{
	mTexFloatR2D[0].Resize(width, height);
	mTexFloatR2D[1].Resize(width, height);
	mNoise.Resize(width, height);
}

void RadarMap::Effects::Processor::Speckle( Request& req )
{
	if (appConfig.PostProcessing.SpeckleNoise)
	{
		mNoise.Permute();

		RLib::Processing::FramebufferTextureAttachment attachment0(mTexFloatR2D[!mActiveTexture]);
		RLib::Processing::TextureSource source0(mTexFloatR2D[mActiveTexture]);
		RLib::Processing::TextureSource source1(mNoise.GetNoiseTexture(), GL_TEXTURE1);
		RLib::Processing::TextureSource source2(mNoise.GetPermTexture(), GL_TEXTURE2);
		RLib::Shader::ProgramBinder binder(mSpeckleProgram);

		glUniform1iARB(mSpeckleProgram.mUniTexInput, 0);
		glUniform1iARB(mSpeckleProgram.mUniTexNoise, 1);
		glUniform1iARB(mSpeckleProgram.mUniTexPermutation, 2);
		glUniform1iARB(mSpeckleProgram.mUniNumLooks, techConfig.Looks);
		mSpeckleProgram.Pass();

		mActiveTexture = !mActiveTexture;
	}
}

void RadarMap::Effects::Processor::Blur( Request& req )
{
	Config::TechConfig::Mode& mode = techConfig.Modes[req.Mode];
	
	if (appConfig.PostProcessing.BeamPattern)
	{
		{
			RLib::Processing::FramebufferTextureAttachment attachment0(mTexFloatR2D[!mActiveTexture]);
			RLib::Processing::TextureSource source0 (mTexFloatR2D[mActiveTexture]);
			RLib::Shader::ProgramBinder binder(mBlurAzimuthProgram);

			float pixelSize = 2.0f * req.FOV.AzimuthHRadius / float(mTexFloatR2D[0].Width());
			KernelGauss(mode.Resolution.Azimuth / pixelSize, mBlurKernel.size());

			glUniform1iARB(mBlurAzimuthProgram.mUniTexInput, 0);
			glUniform1iARB(mBlurAzimuthProgram.mUniKernelSize, mBlurKernel.size());
			glUniform1fvARB(mBlurAzimuthProgram.mUniKernelValue, mBlurKernel.size(), &mBlurKernel[0]);

			mBlurAzimuthProgram.Pass();
			mActiveTexture = !mActiveTexture;
		}

		{
			RLib::Processing::FramebufferTextureAttachment attachment0(mTexFloatR2D[!mActiveTexture]);
			RLib::Processing::TextureSource source0 (mTexFloatR2D[mActiveTexture]);
			RLib::Shader::ProgramBinder binder(mBlurRangeProgram);

			KernelGauss(mode.Resolution.Range, 4);

			glUniform1iARB(mBlurRangeProgram.mUniTexInput, 0);
			glUniform1iARB(mBlurRangeProgram.mUniKernelSize, 4);
			glUniform1fvARB(mBlurRangeProgram.mUniKernelValue, 4, &mBlurKernel[0]);

			mBlurRangeProgram.Pass();
			mActiveTexture = !mActiveTexture;
		}
	}
}


void RadarMap::Effects::Processor::AdditiveNoise( Request& req )
{
	if (appConfig.PostProcessing.AdditiveNoise)
	{
		mNoise.Permute();

		RLib::Processing::FramebufferTextureAttachment attachment0(mTexFloatR2D[!mActiveTexture]);
		RLib::Processing::TextureSource source0(mTexFloatR2D[mActiveTexture]);
		RLib::Processing::TextureSource source1(mNoise.GetAddNoiseTexture(), GL_TEXTURE1);
		RLib::Processing::TextureSource source2(mNoise.GetPermTexture(), GL_TEXTURE2);
		RLib::Shader::ProgramBinder binder(mAdditiveNoiseProgram);

		glUniform1iARB(mAdditiveNoiseProgram.mUniTexInput, 0);
		glUniform1iARB(mAdditiveNoiseProgram.mUniTexNoise, 1);
		glUniform1iARB(mAdditiveNoiseProgram.mUniTexPermutation, 2);
		glUniform1fARB(mAdditiveNoiseProgram.mUniNoiseParams.Mean, techConfig.AdditiveNoise.Mean);
		glUniform1fARB(mAdditiveNoiseProgram.mUniNoiseParams.Dev, techConfig.AdditiveNoise.Dev);
		mAdditiveNoiseProgram.Pass();

		mActiveTexture = !mActiveTexture;
	}
}

void RadarMap::Effects::Processor::KernelGauss(float radius, int size)
{
	float k = 1.6651 / radius;

	mBlurKernel[0]  = 1;
	for (int i = 1; i < size; ++i)
	{
		float kx = k * float(i);
		mBlurKernel[i] = exp(-kx * kx);
	}

	float norm = mBlurKernel[0];
	for (int i = 1; i < size; ++i) norm = norm + mBlurKernel[i] * 2;
	for (int i = 0; i < size; ++i) mBlurKernel[i] = mBlurKernel[i] / norm;
}