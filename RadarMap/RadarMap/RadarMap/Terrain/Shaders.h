#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Terrain {

class ElementsFOVProgram : public RLib::Shader::Program
{
public:
	ElementsFOVProgram()
	{
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Terrain\\ElementsFOV.fs");
		Link();

		mUniZBoundsMap = glGetUniformLocationARB(mhProgObj, "zBoundsMap");
		mUniHeight = glGetUniformLocationARB(mhProgObj, "height");

		mUniGrid.Origin =  glGetUniformLocationARB(mhProgObj, "grid.Origin");
		mUniGrid.ElementSize =  glGetUniformLocationARB(mhProgObj, "grid.ElementSize");
		
		mUniReqAzimuthBounds = glGetUniformLocationARB(mhProgObj, "reqAzimuthBounds");
		mUniReqRangeBounds2 = glGetUniformLocationARB(mhProgObj, "reqRangeBounds2");		
	}

public:
	int mUniZBoundsMap;
	struct {int Origin; int ElementSize;} 	mUniGrid;
	int mUniReqRangeBounds2;
	int mUniReqAzimuthBounds;
	int mUniHeight;
};

class ElementsShadeProgram : private RLib::Shader::Program
{
public:
	ElementsShadeProgram()
	{
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_TERRAIN_ELEMENTSFOV_F);
		Link();

		mUniZCoordMap = glGetUniformLocationARB(mhProgObj, "zCoordMap");
		mUniElevationMap = glGetUniformLocationARB(mhProgObj, "elevationMap");

		mUniGrid.Origin =  glGetUniformLocationARB(mhProgObj, "grid.Origin");
		mUniGrid.ElementSize =  glGetUniformLocationARB(mhProgObj, "grid.ElementSize");

		mUniBoundPlane = glGetUniformLocationARB(mhProgObj, "boundPlane");
		mUniHeight = glGetUniformLocationARB(mhProgObj, "height");
	}

	void Select(const Request& req, float tanElMin, float tanElMax, const mgcoord2d& origin, const mgcoord2d& elementSize)
	{
		struct RadialCoord
		{
			double sinAz;
			double cosAz;
			double hDist;
		}; 

		RadialCoord radialPos[2];
		mgcoord3d decartPos[2];	

		double dMinHor = req.Airplane.Z / tanElMax;

		radialPos[0].sinAz = sin(req.FOV.Azimuth - req.FOV.AzimuthHRadius);
		radialPos[0].cosAz = cos(req.FOV.Azimuth - req.FOV.AzimuthHRadius);
		radialPos[0].hDist = dMinHor;

		radialPos[1].sinAz = sin(req.FOV.Azimuth + req.FOV.AzimuthHRadius);
		radialPos[1].cosAz = cos(req.FOV.Azimuth + req.FOV.AzimuthHRadius);
		radialPos[1].hDist = dMinHor;

		for (int i = 0; i < 2; ++i)
		{
			decartPos[i].x = - radialPos[i].hDist * radialPos[i].sinAz;
			decartPos[i].y = radialPos[i].hDist * radialPos[i].cosAz;
			decartPos[i].z =  - req.Airplane.Z;
		}

		mgcoord3d boundPlaned = mgCoord3dCross(&decartPos[0], &decartPos[1]);
		mgcoord3f boundPlane;
		boundPlane.x = boundPlaned.x;
		boundPlane.y = boundPlaned.y;
		boundPlane.z = boundPlaned.z;

		Bind();

		glUniform1iARB(mUniZCoordMap, 0);
		glUniform1iARB(mUniElevationMap, 1);
		glUniform2fARB(mUniGrid.Origin, origin.x - req.Airplane.X, origin.y - req.Airplane.Y);
		glUniform2fARB(mUniGrid.ElementSize, elementSize.x, elementSize.y);
		glUniform3fvARB(mUniBoundPlane, 1, &boundPlane.x);
		glUniform1fARB(mUniHeight, req.Airplane.Z);

		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);	

		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();

		glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);	
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f); 
		glVertex2f(-1.0f, 1.0f);
		glEnd();

		Unbind();
	}

private:
	int mUniZCoordMap, mUniElevationMap;
	struct {int Origin; int ElementSize;} 	mUniGrid;
	int mUniBoundPlane;
	int mUniHeight;
};
	}
}