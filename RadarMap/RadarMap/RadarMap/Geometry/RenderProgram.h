#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Geometry {
			
class RenderProgram: public RLib::Shader::Program
{
public:
	RenderProgram()
	{
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Geometry\\AzimuthRange.coord");
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Geometry\\RenderGeometry.vs");
		AttachUnit(GL_GEOMETRY_SHADER_EXT, "Shaders\\RadarMap\\Geometry\\AzimuthRange.coord");		
		AttachUnit(GL_GEOMETRY_SHADER_EXT, "Shaders\\RadarMap\\Geometry\\GeometricFunctions.sh");		
		AttachUnit(GL_GEOMETRY_SHADER_EXT, "Shaders\\RadarMap\\Geometry\\RenderGeometry.gs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Microwave.reflection");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Geometry\\SignalFromPos.fs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Geometry\\IntersectTriangle.fs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Geometry\\RenderGeometry.fs");		
		Link(GL_TRIANGLES, GL_TRIANGLES, 6);

		mUniVpSize = glGetUniformLocationARB(mhProgObj, "vpSize");
		mUniTexture = glGetUniformLocationARB(mhProgObj, "texInput");
		mUniTexElement.Origin = glGetUniformLocationARB(mhProgObj, "texInputElement.Origin");
		mUniTexElement.Size = glGetUniformLocationARB(mhProgObj, "texInputElement.Size");
		
		mUniHeight = glGetUniformLocationARB(mhProgObj, "height");
		mUniGain = glGetUniformLocationARB(mhProgObj, "gain");
		mUniFlatBrightness = glGetUniformLocationARB(mhProgObj, "flatBrightness");
	}

	int mUniVpSize;
	int mUniTexture;
	struct  
	{
		int Origin;
		int Size;
	} mUniTexElement;

	int mUniHeight;
	int mUniGain;
	int mUniFlatBrightness;
};

	}
}
