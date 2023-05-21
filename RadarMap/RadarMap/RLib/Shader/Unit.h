#pragma once

#define MAX_INFO_LOG_SIZE 8192

namespace RLib {
		namespace Shader {

class Unit
{
public:
	Unit(GLenum shaderType, const char* fileName);
	~Unit();

private:

	friend class Program;
	GLhandleARB mhUnit;
};

	}
}
															  