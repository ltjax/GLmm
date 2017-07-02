
#ifndef GLMM_PROGRAM_HPP
#define GLMM_PROGRAM_HPP

#include "Shader.hpp"
#include <tuple>

namespace GLmm
{

/** An executable for the programmable GL pipeline.
*/
class Program : public boost::noncopyable
{
public:
    /** Error that is thrown when Program::Link() fails.
    */
    struct LinkError : public std::runtime_error
    {
        LinkError(const std::string& Message)
        : std::runtime_error(Message)
        {
        }
    };

    Program();
    Program(GLenum BinaryFormat, const std::vector<char>& BinaryData);
    Program(Program&& Rhs);
    ~Program();

    void Attach(const Shader& Rhs);

    int GetUniformLocation(const char* Name) const;
    int GetAttribLocation(const char* Name) const;

    /** Retrieve this program in binary form, if possible.
        \returns An enum describing the binary format, and the program in binary form.
    */
    std::tuple<GLenum, std::vector<char>> GetBinary() const;

    void SetRetrieveableHint(bool Rhs);

    void Link();

    void Use() const;

    void BindFragDataLocation(GLuint ColorIndex, const std::string& Name);

    GLuint GetGLObject() const
    {
        return mGLObject;
    }

    Program& operator=(Program Rhs)
    {
        Swap(Rhs);
        return *this;
    }

    void UniformBlockBinding(GLuint BlockIndex, GLuint BlockBinding);
    GLuint GetUniformBlockIndex(const char* BlockName);
    void UniformBlockBinding(const char* BlockName, GLuint BlockBinding);

private:
    void Swap(Program& Rhs)
    {
        std::swap(mGLObject, Rhs.mGLObject);
    }
    GLuint mGLObject;
};

template <class FileRangeType, class FragmentOutputType>
Program CreateProgramFromFiles(FileRangeType&& FileList, FragmentOutputType&& FragmentOutputList)
{
    Program Result;

    // Attach ALL THE files
    for (auto i = FileList.begin(); i != FileList.end(); ++i)
        Result.Attach(CreateShaderFromFile(*i));

    // Bind the fragment outputs
    for (auto i = FragmentOutputList.begin(); i != FragmentOutputList.end(); ++i)
        Result.BindFragDataLocation(i->first, i->second);

    Result.Link();

    return Result;
}

std::vector<GLenum> GetProgramBinaryFormats();
}

#endif
