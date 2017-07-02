/*  Cataclysm-Software Disaster Engine

    $Id: Framebuffer.cpp 920 2011-04-19 23:10:57Z ltjax $

    Copyright 2006-2007 Marius Elvert
*/

#include "Framebuffer.hpp"
#include <stdexcept>

GLmm::Renderbuffer::Renderbuffer()
{
    glGenRenderbuffers(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

GLmm::Renderbuffer::~Renderbuffer()
{
    glDeleteRenderbuffers(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

void GLmm::Renderbuffer::SetStorage(GLenum InternalFormat, GLsizei w, GLsizei h)
{
    glBindRenderbuffer(GL_RENDERBUFFER, GLObject);
    glRenderbufferStorage(GL_RENDERBUFFER, InternalFormat, w, h);
    GLMM_CHECK_ERRORS();
}

GLmm::Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

GLmm::Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, GLObject);
    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLmm::Framebuffer::AttachColorbuffer(GLuint Index, const Texture2D& Texture, GLint Level)
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Index, GL_TEXTURE_2D, Texture.GetGLObject(), Level);
    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::DetachColorbuffer(GLuint Index)
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Index, GL_TEXTURE_RECTANGLE, 0, 0);
    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::Attach(GLenum Attachment, const Texture2D& Texture, GLint Level)
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, Texture.GetGLObject(), Level);
    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::Attach(GLenum Attachment, const Renderbuffer& Renderbuffer)
{
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, Attachment, GL_RENDERBUFFER, Renderbuffer.GLObject);

    GLMM_CHECK_ERRORS();
}

void GLmm::Framebuffer::Detach(GLenum Attachment)
{
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_RECTANGLE, 0, 0);

    GLMM_CHECK_ERRORS();
}

/** Throws an exception if the framebuffer is not complete.
    FIXME: maybe use a better name for this?
*/
void GLmm::Framebuffer::TestCompleteness()
{
    unsigned int Value = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch (Value)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        throw std::runtime_error("FRAMEBUFFER_INCOMPLETE_ATTACHMENT");

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        throw std::runtime_error("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        throw std::runtime_error("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        throw std::runtime_error("FRAMEBUFFER_INCOMPLETE_READ_BUFFER");

    case GL_FRAMEBUFFER_UNSUPPORTED:
        throw std::runtime_error("FRAMEBUFFER_UNSUPPORTED");

    default:

        throw std::runtime_error("framebuffer not complete");
    };

    return;
}
