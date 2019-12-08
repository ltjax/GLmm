#pragma once

#include "Object.hpp"
#include <filesystem>
#include <boost/optional/optional.hpp>
#include <boost/utility.hpp>
#include <iostream>
#include <stdexcept>

namespace GLmm
{

/** A GL programmable pipeline compilation unit.
*/
class Shader : public boost::noncopyable
{
public:
    /** Error that is thrown when Shader::Compile() fails.
    */
    struct CompileError : public std::runtime_error
    {
        CompileError(const std::string& Message)
        : std::runtime_error(Message)
        {
        }
    };

    explicit Shader(GLenum Type, const std::string& Source);
    explicit Shader(GLenum Type, const std::vector<std::string>& Sources);
    Shader(Shader&& Other);
    ~Shader();

    Shader& operator=(Shader&& Other);

    GLuint GetGLObject() const
    {
        return mObject;
    }

private:
    void SetSource(const std::string& Source);
    void SetSources(const std::vector<std::string>& Sources);

    /** Compile the shader.
        Throws CompileError on a compile-time error.
    */
    void Compile();

    GLuint mObject;
};

/** Get the OpenGL shader type from a filename extension
*/
GLenum GetShaderTypeFromExtension(const std::filesystem::path& Filename);

/** Create a shader from a file stream.
*/
Shader CreateShaderFromFile(GLenum Type, std::istream& File);

/** Create a shader from a file.
*/
Shader CreateShaderFromFile(GLenum Type, const std::filesystem::path& Filename);

/** Create a shader from a file and try to guess the shader type.
*/
Shader CreateShaderFromFile(const std::filesystem::path& Filename);
}

