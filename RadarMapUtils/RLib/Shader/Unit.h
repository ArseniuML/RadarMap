#pragma once

namespace RLib {
		namespace Shader {

class Unit
{
public:
	Unit(GLenum shaderType, const char* fileName);
	Unit(GLenum shaderType, HMODULE hModule, int id, LPCTSTR lpType = _T("SHADER"));
	~Unit();

private:
	void Compile(const char* code, int size);

	friend class Program;
	GLhandleARB mhUnit;
};

	}
}
															  