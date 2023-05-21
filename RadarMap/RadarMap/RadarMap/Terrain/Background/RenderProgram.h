#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Terrain {
		namespace Background {

class RenderProgram: public RLib::Shader::Program
{
public:
	RenderProgram()
	{
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Terrain\\Background\\RenderBackground.vs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Terrain\\Background\\RenderBackground.fs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Microwave.reflection");		
		Link();
		
		mUniVpSize = glGetUniformLocationARB(mhProgObj, "vpSize");
		mUniMaterial.Brightness = glGetUniformLocationARB(mhProgObj, "material.Brightness");
		mUniGain = glGetUniformLocationARB(mhProgObj, "gain");
		mUniHeight = glGetUniformLocationARB(mhProgObj, "height");
	}

	int mUniVpSize;
	
	struct UniMaterial
	{
		int Brightness;
	};
	UniMaterial mUniMaterial;
	
	int mUniGain;
	int mUniHeight;
};

		}
	}
}