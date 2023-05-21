#include "StdAfx.h"
#include "Frame.h"

#include <RLib/Processing/Processing.h>

RadarMap::Frame::Frame::Frame():
mCubicKernelTexture(256),
mReady(false)
{
	glGenFramebuffers(1, &mFB);

	if (techConfig.Modes.size() > 0)
	{
		Config::TechConfig::Mode& mode = techConfig.Modes[0];
		mSignalTex.Resize(mode.Elements.Azimuth, mode.Elements.Range);
		mBrightnessTex.Resize(mode.Elements.Azimuth, mode.Elements.Range);
	}
}


RadarMap::Frame::Frame::~Frame()
{
	glDeleteFramebuffers(1, &mFB);
}

void RadarMap::Frame::Frame::SetProjectedSize( int width, int height )
{
	mProjectedWidth = width;
	mProjectedHeight = height;
	mReady = false;
}

void RadarMap::Frame::Frame::SetRequest(const Request& req)
{
	mRequest = req;
	Config::TechConfig::Mode& mode = techConfig.Modes[mRequest.Mode];
	mSignalTex.Resize(mode.Elements.Azimuth, mode.Elements.Range);
	mBrightnessTex.Resize(mode.Elements.Azimuth, mode.Elements.Range);
}

void RadarMap::Frame::Frame::SetFrame( RLib::Collection::Texture& signalTex)
{
	RLib::Processing::FramebufferTextureAttachment tgt(mSignalTex);
	RLib::Processing::TextureSource src(signalTex);
	
	glViewport(0, 0, signalTex.Width(), signalTex.Height());
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-1, -1);
		glTexCoord2f(signalTex.Width(), 0); glVertex2f(1, -1);
		glTexCoord2f(signalTex.Width(), signalTex.Height()); glVertex2f(1, 1);
		glTexCoord2f(0, signalTex.Height()); glVertex2f(-1, 1);
	glEnd();
	glDisable(GL_TEXTURE_RECTANGLE_ARB);

	mReady = true;
}

void RadarMap::Frame::Frame::Render(float brightness)
{
	if (mReady)
	{
		glViewport(0, 0, mBrightnessTex.Width(), mBrightnessTex.Height());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFB);
		{
			RLib::Processing::FramebufferTextureAttachment tgt(mBrightnessTex);
			RLib::Processing::TextureSource src(mSignalTex);
			RLib::Shader::ProgramBinder binder(mBrightnessRegulatorProgram);

			glUniform1iARB(mBrightnessRegulatorProgram.mUniTexInput, 0);
			glUniform1fARB(mBrightnessRegulatorProgram.mUniBrightness, 0.01f * brightness);
			mBrightnessRegulatorProgram.Pass();
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		glViewport(0, 0, mProjectedWidth, mProjectedHeight);
		if (techConfig.Modes[mRequest.Mode].ResultProjection == Config::TechConfig::Projection::Decart)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho 
			(	
				mRequest.FOV.AzimuthHRadius, -mRequest.FOV.AzimuthHRadius,
				mRequest.FOV.DistanceMin, mRequest.FOV.DistanceMax, 
				0.0, 1.0
			);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glOrtho 
			(	
				mRequest.Extent.Left, mRequest.Extent.Right,
				mRequest.Extent.Bottom, mRequest.Extent.Top, 
				0, mRequest.Airplane.Height 
			); 


			RLib::Processing::TextureSource src0(mBrightnessTex, GL_TEXTURE0);
			RLib::Processing::TextureSource src1(mCubicKernelTexture, GL_TEXTURE1);
			RLib::Shader::ProgramBinder binder(mDecartProjectorProgram);

			glUniform1iARB(mDecartProjectorProgram.mUniTexInput, 0);
			glUniform2fARB(mDecartProjectorProgram.mUniTexInputSize, mBrightnessTex.Width(), mBrightnessTex.Height());

			glUniform1iARB(mDecartProjectorProgram.mUniKernelTex, 1);
			glUniform1iARB(mDecartProjectorProgram.mUniKernelSize, mCubicKernelTexture.Width());

			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_QUADS);
			glVertex3f(mRequest.Extent.Left, mRequest.Extent.Bottom, -mRequest.Airplane.Height);
			glVertex3f(mRequest.Extent.Right, mRequest.Extent.Bottom, -mRequest.Airplane.Height);	
			glVertex3f(mRequest.Extent.Right, mRequest.Extent.Top, -mRequest.Airplane.Height);
			glVertex3f(mRequest.Extent.Left, mRequest.Extent.Top, -mRequest.Airplane.Height);
			glEnd();
		}
		else
		{
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode( GL_MODELVIEW ); glLoadIdentity();
			glOrtho(0, mBrightnessTex.Width(), 0, mBrightnessTex.Height(), -1, 1); 

			RLib::Processing::TextureSource src0(mBrightnessTex, GL_TEXTURE0);
			RLib::Processing::TextureSource src1(mCubicKernelTexture, GL_TEXTURE1);
			RLib::Shader::ProgramBinder binder(mPolarProjectorProgram);

			glUniform1iARB(mPolarProjectorProgram.mUniTexInput, 0);
			glUniform1iARB(mPolarProjectorProgram.mUniKernelTex, 1);
			glUniform1iARB(mPolarProjectorProgram.mUniKernelSize, mCubicKernelTexture.Width());

			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(mBrightnessTex.Width(), 0);
			glVertex2f(mBrightnessTex.Width(), mBrightnessTex.Height());
			glVertex2f(0, mBrightnessTex.Height());
			glEnd();
		}
	}
	else
	{
		glViewport(0, 0, mProjectedWidth, mProjectedHeight);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
