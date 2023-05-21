#include "stdafx.h"
#include "Unit.h"

RLib::Shader::Unit::Unit( GLenum shaderType, const char* fileName ):
mhUnit(glCreateShaderObjectARB(shaderType))
{
	if (mhUnit == 0)
		throw std::exception(std::string("Can't create shader object").c_str());
	
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open()) 
	{
		glDeleteObjectARB(mhUnit);
		mhUnit = 0;
		throw 
			std::exception((std::string("Can not open shader source file ") + fileName).c_str());
	}
	
	int size = (int)file.tellg();
	std::string shaderCode;
	shaderCode.resize(size);
	
	file.seekg (0, std::ios::beg);
	file.read(&shaderCode[0], size);
	file.close();
	
	char* code = &shaderCode[0];
	glShaderSourceARB(mhUnit, 1, (const GLcharARB**)&code, &size);
	glCompileShaderARB(mhUnit);

	GLint success;
	glGetObjectParameterivARB(mhUnit, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if (!success)
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(mhUnit, MAX_INFO_LOG_SIZE, NULL, infoLog);
		
		glDeleteObjectARB(mhUnit);
		mhUnit = 0;
		
		throw 
			std::exception
			(
				(std::string("Can't compile shader ") + 
				std::string(fileName)+
				std::string(" : ") + std::string(infoLog)).c_str()
			);
	}
}


RLib::Shader::Unit::~Unit()
{
	glDeleteObjectARB(mhUnit);
}