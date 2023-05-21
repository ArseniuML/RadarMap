#include "StdAfx.h"
#include "World.h"
#include <RLib/Processing/Processing.h>

void RadarMap::World::Render( RLib::Collection::Texture& resultTex, const Request& req)
{
	Config::TechConfig::Mode& mode = techConfig.Modes[req.Mode];

	float gain = 1.6f * 0.5f * (req.FOV.DistanceMax + req.FOV.DistanceMin) / req.Airplane.Height;

	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho 
	(	
		req.FOV.AzimuthHRadius, -req.FOV.AzimuthHRadius,
		req.FOV.DistanceMin, req.FOV.DistanceMax, 
		0.0, 1.0
	);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	RLib::Processing::FramebufferTextureAttachment attachment0(resultTex);
	
	{
		RLib::Shader::ProgramBinder binder(mBackgroundProgram);
		glUniform2fARB(mBackgroundProgram.mUniVpSize, resultTex.Width(), resultTex.Height());
		glUniform1fARB(mBackgroundProgram.mUniHeight,req.Airplane.Height);
		glUniform1fARB(mBackgroundProgram.mUniGain, gain);
		mTerrain.RenderBackground(mBackgroundProgram);
	}

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	gluLookAt
	(
		req.Airplane.X, req.Airplane.Y, req.Airplane.Z,
		req.Airplane.X, req.Airplane.Y, req.Airplane.Z - 1,
		-sin(req.FOV.Azimuth), cos(req.FOV.Azimuth), 0.0
	);
			
	{
		RLib::Shader::ProgramBinder binder(mGeometryProgram);

		glUniform2fARB(mGeometryProgram.mUniVpSize, resultTex.Width(), resultTex.Height());
		glUniform1fARB(mGeometryProgram.mUniGain, gain);
		glUniform1fARB(mGeometryProgram.mUniHeight, req.Airplane.Height);

		glShadeModel(GL_FLAT);

		glUniform1fARB(mGeometryProgram.mUniFlatBrightness, -1);
		mTerrain.Render(mGeometryProgram);
		
		if (techConfig.Modes[req.Mode].TargetRender == Config::TechConfig::Render::Geometry)
		{
			mTargetCollection.RenderTargets(mGeometryProgram, req);
		}

		glShadeModel(GL_SMOOTH);
	}
}

void RadarMap::World::RenderPoints( RLib::Collection::Texture& resultTex, const Request& req )
{
	if (techConfig.Modes[req.Mode].TargetRender == Config::TechConfig::Render::Point)
	{
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glOrtho 
		(	
			req.FOV.AzimuthHRadius, -req.FOV.AzimuthHRadius,
			req.FOV.DistanceMin, req.FOV.DistanceMax, 
			0.0, 1.0
		);
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();

		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		gluLookAt
		(
			req.Airplane.X, req.Airplane.Y, req.Airplane.Z,
			req.Airplane.X, req.Airplane.Y, req.Airplane.Z - 1,
			-sin(req.FOV.Azimuth), cos(req.FOV.Azimuth), 0.0
		);

		RLib::Processing::FramebufferTextureAttachment attachment0(resultTex);
		RLib::Shader::ProgramBinder binder(mPointProgram);

		mTargetCollection.RenderTargetsAsPoints(mPointProgram);
	}
}
