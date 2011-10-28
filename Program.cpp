/*  Cataclysm-Software Disaster Engine

	$Id: Program.cpp 920 2011-04-19 23:10:57Z ltjax $

	Copyright 2006-2007 Marius Elvert
*/


#include <vector>
#include "Program.hpp"

GLmm::Program::Program()
: mGLObject(glCreateProgram())
{
	assert(mGLObject != 0);
}

GLmm::Program::Program(Program&& rhs)
: mGLObject(0)
{
	Swap(rhs);
}

GLmm::Program::~Program()
{
	if (mGLObject != 0)
		glDeleteProgram(mGLObject);
}

void GLmm::Program::Link()
{
	glLinkProgram(mGLObject);
	GLMM_CHECK_ERRORS();

	GLint Status=0;
	glGetProgramiv(mGLObject, GL_LINK_STATUS, &Status);
	GLMM_CHECK_ERRORS();

	// check for eventual link errors.
	if ( !Status )
	{
		GLint InfoLength=0;
		glGetProgramiv(mGLObject, GL_INFO_LOG_LENGTH, &InfoLength);
		GLMM_CHECK_ERRORS();

		std::vector<GLchar> Buffer(InfoLength+1);

		glGetProgramInfoLog(mGLObject, GLsizei(InfoLength + 1), 0, Buffer.data());
		GLMM_CHECK_ERRORS();

		throw LinkError(Buffer.data());
	}
}

int GLmm::Program::GetUniformLocation(const char* Name) const
{
	return glGetUniformLocation(mGLObject, Name);
}

int GLmm::Program::GetAttribLocation(const char* Name) const
{
	return glGetAttribLocation(mGLObject, Name);
}
void GLmm::Program::BindFragDataLocation(GLuint ColorIndex, const std::string& Name)
{
	glBindFragDataLocation(mGLObject, ColorIndex, Name.c_str());
}

void GLmm::Program::Use() const
{
	glUseProgram(mGLObject);
	GLMM_CHECK_ERRORS();
}

void GLmm::Program::Attach(const Shader& Rhs)
{
	glAttachShader(mGLObject, Rhs.GetGLObject());
	GLMM_CHECK_ERRORS();
}


