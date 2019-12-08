
#include "Shader.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include <vector>

GLmm::Shader::Shader(GLuint Type, const std::string& Source)
{
    mObject = glCreateShader(Type);

    if (!mObject)
    {
        GLMM_THROW_ERROR("Unable to create shader object (type:" << Type << ")");
    }

    // Finalize this object
    SetSource(Source);
    Compile();
}

GLmm::Shader::Shader(GLenum Type, const std::vector<std::string>& Sources)
{
    mObject = glCreateShader(Type);

    if (!mObject)
    {
        GLMM_THROW_ERROR("Unable to create shader object (type:" << Type << ")");
    }

    // Finalize this object
    SetSources(Sources);
    Compile();
}

GLmm::Shader::Shader(Shader&& Other)
{
    mObject = Other.mObject;
    Other.mObject = 0;
}

GLmm::Shader& GLmm::Shader::operator=(Shader&& Other)
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
    const GLint Length = (const int)Source.length();
    const GLchar* String[1] = { Source.data() };

    glShaderSource(mObject, 1, String, &Length);
}

void GLmm::Shader::SetSources(const std::vector<std::string>& Sources)
{
    std::vector<GLint> Lengths;
    std::vector<GLchar const*> Strings;

    for (auto const& Each : Sources)
    {
        Lengths.push_back(Each.length());
        Strings.push_back(Each.data());
    }

    glShaderSource(mObject, static_cast<GLsizei>(Sources.size()), Strings.data(), Lengths.data());
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
        std::vector<char> Buffer(InfoLength + 1);
        glGetShaderInfoLog(mObject, GLsizei(Buffer.size()), 0, Buffer.data());

        // Throw it!
        throw CompileError(Buffer.data());
    }
}

GLmm::Shader GLmm::CreateShaderFromFile(GLenum Type, const std::filesystem::path& Filename)
{
    std::ifstream File(Filename);

    if (!File.good())
    {
        auto ErrorString = std::string("Unable to open file:") + Filename.string();
        throw std::runtime_error(ErrorString);
    }

    try
    {
        return CreateShaderFromFile(Type, File);
    }
    catch (Shader::CompileError& Error)
    {

        // Tag the filename onto the error message
        throw std::runtime_error(Filename.string() + " : " + Error.what());
    }
}

GLmm::Shader GLmm::CreateShaderFromFile(const std::filesystem::path& Filename)
{
    return CreateShaderFromFile(GetShaderTypeFromExtension(Filename), Filename);
}

GLmm::Shader GLmm::CreateShaderFromFile(GLenum Type, std::istream& File)
{
    std::string Contents;

    // Scan lines
    std::string Line;
    while (std::getline(File, Line))
    {
        Contents += Line + '\n';
    }

    return Shader(Type, Contents);
}

GLenum GLmm::GetShaderTypeFromExtension(const std::filesystem::path& Filename)
{
    using boost::algorithm::iequals;
    auto Extension = Filename.extension().string();

    if (iequals(Extension, ".frag") || iequals(Extension, ".fs"))
        return GL_FRAGMENT_SHADER;
    else if (iequals(Extension, ".vert") || iequals(Extension, ".vs"))
        return GL_VERTEX_SHADER;
    else if (iequals(Extension, ".geom"))
        return GL_GEOMETRY_SHADER;

    throw std::runtime_error("Unable to deduce shader type from extension: " + Extension);
}
