#pragma once
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace World {
		namespace Shading {

class ShadeMapProgram: public RLib::Shader::Program
{
public:
	ShadeMapProgram()
	{
		//AttachUnit(GL_VERTEX_SHADER_ARB, hModule, IDR_SHADER_SHADING_SHADEMAP_V);
		//AttachUnit(GL_VERTEX_SHADER_ARB, hModule, IDR_SHADER_SHADING_AZIMUTHCOTANELEVATION);
		//AttachUnit(GL_GEOMETRY_SHADER_EXT, hModule, IDR_SHADER_SHADING_SHADEMAP_G);		
		//AttachUnit(GL_GEOMETRY_SHADER_EXT, hModule, IDR_SHADER_SHADING_AZIMUTHCOTANELEVATION);
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_SHADEMAP_F);		
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_AZIMUTHCOTANELEVATION);		
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_DISTANCE_F);		

		Link(GL_TRIANGLES, GL_TRIANGLES, 3);

		mUniVpSize = glGetUniformLocationARB(mhProgObj, "vpSize");
		mUniHeight = glGetUniformLocationARB(mhProgObj, "height");
	}

	int mUniVpSize;
	int mUniHeight;
};

class ShadeMapAdditionalProgram: public RLib::Shader::Program
{
public:
	ShadeMapAdditionalProgram()
	{
		//AttachUnit(GL_VERTEX_SHADER_ARB, hModule, IDR_SHADER_SHADING_SHADEMAP_V);
		//AttachUnit(GL_VERTEX_SHADER_ARB, hModule, IDR_SHADER_SHADING_AZIMUTHELEVATION);
		//AttachUnit(GL_GEOMETRY_SHADER_EXT, hModule, IDR_SHADER_SHADING_SHADEMAP_G);		
		//AttachUnit(GL_GEOMETRY_SHADER_EXT, hModule, IDR_SHADER_SHADING_AZIMUTHELEVATION);
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_SHADEMAP_F);		
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_AZIMUTHELEVATION);		
		//AttachUnit(GL_FRAGMENT_SHADER_ARB, hModule, IDR_SHADER_SHADING_DISTANCE_F);		

		Link(GL_TRIANGLES, GL_TRIANGLES, 6);

		mUniVpSize = glGetUniformLocationARB(mhProgObj, "vpSize");
	}

	int mUniVpSize;
};

		}
	}
}