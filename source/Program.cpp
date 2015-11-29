/*  GLmm

	Copyright 2006-2012 Marius Elvert
*/


#include <vector>
#include "Program.hpp"
#include <boost/numeric/conversion/cast.hpp>

namespace {

void CheckLinkStatus(GLuint Program)
{
	GLint Status=0;
	glGetProgramiv(Program, GL_LINK_STATUS, &Status);
	GLMM_CHECK_ERRORS();

	// check for eventual link errors.
	if (!Status)
	{
		GLint InfoLength=0;
		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLength);
		GLMM_CHECK_ERRORS();

		std::vector<GLchar> Buffer(InfoLength+1);

		glGetProgramInfoLog(Program, GLsizei(InfoLength + 1), 0, Buffer.data());
		GLMM_CHECK_ERRORS();

		throw GLmm::Program::LinkError(Buffer.data());
	}
}

}

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

GLmm::Program::Program(GLenum BinaryFormat, const std::vector<char>& BinaryData)
: mGLObject(0)
{
    if (!ogl_ext_ARB_get_program_binary)
		throw std::runtime_error("GL_ARB_get_program_binary is not supported");

	// Create a program and load the binary
	mGLObject = glCreateProgram();
	glProgramBinary(mGLObject, BinaryFormat, BinaryData.data(), boost::numeric_cast<GLsizei>(BinaryData.size()));

	// Check if this went well
	try {
		CheckLinkStatus(mGLObject);
	}
	catch (std::exception&)
	{
		glDeleteProgram(mGLObject);
		throw;
	}
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

	CheckLinkStatus(mGLObject);
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

std::tuple<
	GLenum,
	std::vector<char>
>
GLmm::Program::GetBinary() const
{
    if (!ogl_ext_ARB_get_program_binary)
		throw std::runtime_error("GL_ARB_get_program_binary is not supported");

	GLint binaryLength=0;
	glGetProgramiv(mGLObject, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

	std::vector<char> Buffer(binaryLength);
	GLenum format = 0;

	glGetProgramBinary(mGLObject, binaryLength, NULL, &format, Buffer.data());

	return std::make_tuple(format, Buffer);
}

std::vector<GLenum>
GLmm::GetProgramBinaryFormats()
{
	std::vector<GLenum> Result;

	// Report no available formats if the extension is not supported
    if (!ogl_ext_ARB_get_program_binary)
		return Result;

	// Otherwise, query the formats from the API
	GLint ParameterCount=0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &ParameterCount);
	Result.resize(ParameterCount);
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, reinterpret_cast<GLint*>(Result.data()));

	static_assert(sizeof(GLint)==sizeof(GLenum),
		"Cannot get program binaries as GLenum");

	return Result;
}

void GLmm::Program::UniformBlockBinding(GLuint BlockIndex, GLuint BlockBinding)
{
	glUniformBlockBinding(mGLObject, BlockIndex, BlockBinding);
	GLMM_CHECK_ERRORS();
}

GLuint GLmm::Program::GetUniformBlockIndex(const char* BlockName)
{
	GLuint const Result=glGetUniformBlockIndex(mGLObject, BlockName);
	GLMM_CHECK_ERRORS();
	return Result;
}
