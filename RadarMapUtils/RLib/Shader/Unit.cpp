#include "stdafx.h"
#include "Unit.h"

RLib::Shader::Unit::Unit( GLenum shaderType, const char* fileName ):
mhUnit(glCreateShaderObjectARB(shaderType))
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open()) 
		throw 
			std::exception((std::string("Can not open shader source file ") + fileName).c_str());
	
	int size = (int)file.tellg();
	std::string shaderCode;
	shaderCode.resize(size);
	
	file.seekg (0, std::ios::beg);
	file.read (&shaderCode[0], size);
	file.close();
	
	Compile(&shaderCode[0], size);
}

RLib::Shader::Unit::Unit(GLenum shaderType, HMODULE hModule, int id, LPCTSTR lpType):
mhUnit(glCreateShaderObjectARB(shaderType))
{
	HRSRC rc = ::FindResource(hModule, MAKEINTRESOURCE(id), lpType);
	HGLOBAL rcData = ::LoadResource(hModule, rc);
	DWORD size = ::SizeofResource(hModule, rc);
	const char* data = static_cast<const char*>(::LockResource(rcData));
	Compile(data, size);
}

RLib::Shader::Unit::~Unit()
{
	glDeleteObjectARB(mhUnit);
}

void RLib::Shader::Unit::Compile( const char* code, int size)
{
	glShaderSourceARB(mhUnit, 1, (const GLcharARB**)&code, &size);
	glCompileShaderARB(mhUnit);

	GLint success;
	glGetObjectParameterivARB(mhUnit, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if (!success)
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(mhUnit, MAX_INFO_LOG_SIZE, NULL, infoLog);
		throw 
			std::exception(infoLog);
	}
}
