
#include "Shader.hpp"
#include <fstream>
#include <boost/filesystem/fstream.hpp>
#include <vector>

GLmm::Shader::Shader(GLuint Type, const std::string& Source)
{
	mObject = glCreateShader( Type );

	if (!mObject)
	{
		GLMM_THROW_ERROR("Unable to create shader object (type:" << Type << ")");
	}

	// Finalize this object
	SetSource(Source);
	Compile();
}

GLmm::Shader::Shader(Shader&& Other)
{
	mObject = Other.mObject;
	Other.mObject = 0;
}

GLmm::Shader&
GLmm::Shader::operator=(Shader&& Other)
{
	if (this == &Other)
		return *this;

	mObject = Other.mObject;
	Other.mObject = 0;

	return *this;
}

GLmm::Shader::~Shader()
{
	if (mObject != 0)
		glDeleteShader(mObject);
}

void GLmm::Shader::SetSource(const std::string& Source)
{
	const GLint	Length = (const int)Source.length();
	const GLchar* String[1] = {Source.data()};

	glShaderSource(mObject, 1, String, &Length);
}

void GLmm::Shader::Compile()
{
	GLint Status = 0;
	glCompileShader(mObject);
	glGetShaderiv(mObject, GL_COMPILE_STATUS, &Status);

	// Have there been errors?
	if (Status == 0)
	{
		// Get the length of the compile log
		GLint InfoLength = 0;
		glGetShaderiv(mObject, GL_INFO_LOG_LENGTH, &InfoLength);

		// Get memory and download the log
		std::vector<char> Buffer(InfoLength+1);
		glGetShaderInfoLog(mObject, GLsizei(Buffer.size()), 0, Buffer.data());

		// Throw it!
		throw CompileError(Buffer.data());
	}
}

GLmm::Shader
GLmm::CreateShaderFromFile(GLenum Type, const boost::filesystem::path& Filename)
{
	boost::filesystem::ifstream File(Filename);

	if (!File.good())
	{
		auto ErrorString = std::string("Unable to open file:") + Filename.string();
		throw std::runtime_error(ErrorString);
	}

	std::string Contents;

	// Scan lines
	std::string Line;
	while (std::getline(File, Line))
	{
		Contents += Line + '\n';
	}

	try {
		return Shader(Type, Contents);
	}
	catch (Shader::CompileError& Error)
	{

		// Tag the filename onto the error message
		throw std::runtime_error(Filename.string() + " : " + Error.what());
	}
}

GLmm::Shader
GLmm::CreateShaderFromFile(const boost::filesystem::path& Filename)
{
	auto Extension = Filename.extension().string();
	
	if (Extension == ".frag")
		return CreateShaderFromFile(GL_FRAGMENT_SHADER, Filename);
	else if (Extension == ".vert")
		return CreateShaderFromFile(GL_VERTEX_SHADER, Filename);
	else if (Extension == ".geom")
		return CreateShaderFromFile(GL_GEOMETRY_SHADER, Filename);

	throw std::runtime_error("Unable to guess shader type from extension: " + Extension);
}
