#include "stdafx.h"
#include "Program.h"

RLib::Shader::Program::Program() :
mhProgObj(glCreateProgramObjectARB())
{
}

RLib::Shader::Program::~Program()
{
	glDeleteObjectARB(mhProgObj);
}

void RLib::Shader::Program::Link()
{
	glLinkProgramARB(mhProgObj);

	GLint success; glGetObjectParameterivARB(mhProgObj, GL_OBJECT_LINK_STATUS_ARB, &success);
	
	if (!success)
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(mhProgObj, MAX_INFO_LOG_SIZE, NULL, infoLog);

		glDeleteObjectARB(mhProgObj);
		mhProgObj = 0;

		throw 
			std::exception
			(
				(std::string("Can't link shader program : ") + 
				std::string(infoLog)).c_str()
			);
	}

	glValidateProgramARB(mhProgObj);
	glGetObjectParameterivARB(mhProgObj, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
	if (!success)
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(mhProgObj, MAX_INFO_LOG_SIZE, NULL, infoLog);

		glDeleteObjectARB(mhProgObj);
		mhProgObj = 0;

		throw 
			std::exception
			(
				(std::string("Can't validate shader program : ") + 
				std::string(infoLog)).c_str()
			);
	}
}

void RLib::Shader::Program::Link( int inputPrimitiveType, int outputPrimitiveType, int verticesOut )
{
	Link();
	glProgramParameteriEXT(mhProgObj, GL_GEOMETRY_INPUT_TYPE_EXT, inputPrimitiveType);
	glProgramParameteriEXT(mhProgObj, GL_GEOMETRY_OUTPUT_TYPE_EXT,outputPrimitiveType);
	if (verticesOut == 0)
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &verticesOut);
	glProgramParameteriEXT(mhProgObj, GL_GEOMETRY_VERTICES_OUT_EXT, verticesOut);
}

void RLib::Shader::Program::AttachUnit( GLenum shaderType, const char* fileName )
{
	Unit unit(shaderType, fileName);
	AttachUnit(&unit);
}

void RLib::Shader::Program::AttachUnit( Unit* pUnit )
{
	glAttachObjectARB(mhProgObj, pUnit->mhUnit);
}

void RLib::Shader::Program::DetachUnit( Unit* pUnit )
{
	glDetachObjectARB(mhProgObj, pUnit->mhUnit);
}

void RLib::Shader::Program::Pass()
{
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	glBegin(GL_QUADS);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);
	glEnd();
}
