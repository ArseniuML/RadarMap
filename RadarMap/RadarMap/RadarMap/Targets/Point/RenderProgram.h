#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Targets {
		namespace Point {
			
class RenderProgram: public RLib::Shader::Program
{
public:
	RenderProgram()
	{
		AttachUnit(GL_VERTEX_SHADER_ARB, "Shaders\\RadarMap\\Targets\\Point\\PointRender.vs");		
		AttachUnit(GL_FRAGMENT_SHADER_ARB, "Shaders\\RadarMap\\Targets\\Point\\PointRender.fs");		
		Link();

		mUniBrightness = glGetUniformLocationARB(mhProgObj, "brightness");
	}

	int mUniBrightness;
};

		}
	}
}
