#pragma once
#include <RLib/Shader/Unit.h>

namespace RLib {
	namespace Shader {

class Program
{
public:
	Program():
	mhProgObj(glCreateProgramObjectARB())
	{
	}

	~Program()
	{
		glDeleteObjectARB(mhProgObj);
	}

	void AttachUnit(GLenum shaderType, const char* fileName);
	void AttachUnit(GLenum shaderType, HMODULE hModule, int id, LPCTSTR lpType = _T("SHADER"));
	void AttachUnit(Unit* pUnit);
	void DetachUnit(Unit* pUnit);

	void Link();
	void Link(int inputPrimitiveType, int outputPrimitiveType, int verticesOut);

	inline void Bind()
	{
		glUseProgramObjectARB(mhProgObj);
	}
	inline void Unbind()
	{
		glUseProgramObjectARB(0);
	}

	void Pass();


protected:
	GLhandleARB	mhProgObj;
};

class ProgramBinder
{
public:
	ProgramBinder(Program& program):
	mProgram(program)
	{
		mProgram.Bind();
	}

	~ProgramBinder()
	{
		mProgram.Unbind();
	}

private:
	Program& mProgram;
};

	}
}
															  