

#include "BufferObject.hpp"

GLmm::BufferObject::BufferObject()
: mInvalid(false)
{
    glGenBuffers(1, &mGLObject);
}

GLmm::BufferObject::BufferObject(BufferObject&& Rhs)
: mGLObject(Rhs.mGLObject)
, mInvalid(Rhs.mInvalid)
{
    Rhs.mInvalid = true;
}

GLmm::BufferObject::~BufferObject()
{
    if (!mInvalid)
        glDeleteBuffers(1, &mGLObject);
}

void GLmm::BufferObject::SetData(GLenum Target, std::size_t Size, const GLubyte* Data, GLenum Usage)
{
    glBindBuffer(Target, mGLObject);
    glBufferData(Target, Size, Data, Usage);

    GLMM_CHECK_ERRORS();
}

void GLmm::BufferObject::SetSubData(GLenum Target, std::size_t Size, std::size_t Offset, const GLubyte* Data)
{
    glBindBuffer(Target, mGLObject);
    glBufferSubData(Target, Offset, Size, Data);

    GLMM_CHECK_ERRORS();
}

GLubyte* GLmm::BufferObject::Map(GLenum Target, GLenum Access)
{
    glBindBuffer(Target, mGLObject);
    return reinterpret_cast<GLubyte*>(glMapBuffer(Target, Access));
}

bool GLmm::BufferObject::Unmap(GLenum Target)
{
    glBindBuffer(Target, mGLObject);
    return glUnmapBuffer(Target) != 0;
}

void GLmm::BufferObject::Bind(GLenum Target) const
{
    glBindBuffer(Target, mGLObject);
}

void GLmm::BufferObject::Unbind(GLenum Target)
{
    glBindBuffer(Target, 0);
}

void GLmm::BufferObject::Swap(BufferObject& Rhs)
{
    std::swap(this->mGLObject, Rhs.mGLObject);
    std::swap(this->mInvalid, Rhs.mInvalid);
}
