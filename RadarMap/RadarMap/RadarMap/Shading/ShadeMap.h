#pragma once

#include <RadarMap/Request.h>
#include <RadarMap/World/Terrain/Terrain.h>
#include <RadarMap/World/Shading/Shaders.h>

#include <RLib/Shader/Program.h>
#include <RLib/Collection/Texture.h>
#include <RLib/Collection/Renderbuffer.h>
#include <RLib/Processing/Processing.h>

namespace RadarMap {
	namespace World {
		namespace Shading {

class ShadeMap: public  RLib::Collection::TextureFloatRG
{
public:
	ShadeMap()
	{
	}

	void Resize(int width, int height)
	{
		Texture::Resize(width, height);
		mRenderbufferDepth.Resize(width, height);
	}

	void Build(Terrain::Terrain& terrain, const Request& req)
	{
		/*mShadeRange.Min = req.Height / req.TanElMax; 
		mShadeRange.Max = req.Height / req.TanElMin;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho 
		(	
		req.AzHalfRadius, -req.AzHalfRadius,
		mShadeRange.Min, mShadeRange.Max, 
		0.0, 1.0
		);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
		gluLookAt
		(
		req.Eye.x, req.Eye.y, req.Eye.z,
		req.Eye.x, req.Eye.y, req.Eye.z - 1,
		-req.sinAzCenter, req.cosAzCenter, 0.0
		);

		{
		RLib::Processing::FramebufferTextureAttachment attachment(*this);
		RLib::Processing::FramebufferRenderbufferAttachment depthAttachment(mRenderbufferDepth, GL_DEPTH_ATTACHMENT);
		RLib::Shader::ProgramBinder binder(mShadeMapProgram);

		glUniform1fARB(mShadeMapProgram.mUniHeight, req.Height);
		glUniform2fARB(mShadeMapProgram.mUniVpSize, Width(), Height());

		glDepthRangedNV(0, req.DMax);
		glEnable(GL_DEPTH_TEST);

		glClearColor(-1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		terrain.Render();

		glDisable(GL_DEPTH_TEST);
		glDepthRangedNV(0, 1);
		}*/
	}

private:
	ShadeMapProgram mShadeMapProgram;
	RLib::Collection::RenderbufferDepth32F mRenderbufferDepth;
	
public:
	struct
	{
		float Min;
		float Max;
	} mShadeRange;
};
		
		}
	}
}
