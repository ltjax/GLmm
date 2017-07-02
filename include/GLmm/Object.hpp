/*

Cataclysm-Software Disaster Engine
----------------------------------

Copyright:
Marius Elvert (marius.elvert@cataclysm-software.com) 2006-2011

*/

#ifndef GLMM_OBJECT_HPP
#define GLMM_OBJECT_HPP

#include <gl_core_3_2.h>
#include <sstream>
#include <stdexcept>

namespace GLmm
{

/** Check for OpenGL errors.
    If there are errors, the function will send an std::runtime_error with the errorcodes.
*/
void CheckErrorsAt(const char* File, const unsigned int Line);

#ifdef _DEBUG
#define GLMM_CHECK_ERRORS() GLmm::CheckErrorsAt(__FILE__, __LINE__)
#else
#define GLMM_CHECK_ERRORS()
#endif

/** Macro to easiely throw errors.
*/
#define GLMM_THROW_ERROR(ARGS)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        std::ostringstream Str;                                                                                        \
        Str << ARGS;                                                                                                   \
        throw std::runtime_error(Str.str());                                                                           \
    } while (false)
}

#endif // _GLMM_OBJECT_HPP_
