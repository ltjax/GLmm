/*  Cataclysm-Software Disaster Engine

    $Id: Object.cpp 920 2011-04-19 23:10:57Z ltjax $

    Copyright 2006-2007 Marius Elvert
*/

#include "Object.hpp"
#include <cassert>

namespace
{

/** Translate the error code into a human readable string.
*/
inline const char* GetErrorString(GLenum Code)
{
    switch (Code)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
        return "UNKNOWN GL ERRORCODE";
    }
}
}

void GLmm::CheckErrorsAt(const char* File, unsigned int Line)
{
    GLenum ErrorCode = glGetError();

    if (ErrorCode != GL_NO_ERROR)
    {
        std::ostringstream Str;
        Str << '(' << File << ':' << Line << ')' << "OpenGL error: " << GetErrorString(ErrorCode);

        // the counter is needed or else this could potentially not terminate
        for (size_t i = 0; i < 32 && (ErrorCode = glGetError()) != GL_NO_ERROR; ++i)
            Str << "," << GetErrorString(ErrorCode);

        throw std::runtime_error(Str.str());
    }
}
