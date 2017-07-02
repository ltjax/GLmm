/*  Cataclysm-Software Disaster Engine

    $Id: Texture.hpp 920 2011-04-19 23:10:57Z ltjax $

    Copyright 2006-2007 Marius Elvert
*/

#ifndef GLMM_TEXTURE_HPP
#define GLMM_TEXTURE_HPP

#include <boost/filesystem/path.hpp>
#include <boost/utility.hpp>
#include <replay/pixbuf.hpp>

#include "Object.hpp"

namespace GLmm
{

enum class Colorspace
{
    Linear,
    sRGB
};

typedef boost::filesystem::path Path;

/** generic texture object.
*/
class Texture : public boost::noncopyable
{
public:
    virtual void Bind() const = 0;
    void BindTo(GLenum TextureUnit) const;

protected:
    Texture();
    virtual ~Texture()
    {
    }
};

/** texture 3d object.
*/
class Texture3D : public Texture
{

public:
    Texture3D();
    ~Texture3D();

    /** bind this texture object.
    */
    void Bind() const;
    void Unbind() const;

    /** set texture parameters.
    */
    void SetFilter(GLint MinFilter, GLint MagFilter);
    void SetWrap(GLint SWrap, GLint TWrap, GLint RWrap);
    inline void SetWrap(GLint Wrap)
    {
        SetWrap(Wrap, Wrap, Wrap);
    }

    /** get the object id as used by Opengl
    */
    unsigned int GetGLObject() const
    {
        return GLObject;
    }

    /** Load a volume from a tiled picture.
    */
    void LoadFromFileTiled(const Path& Filename, unsigned int x, unsigned int y);

private:
    GLuint GLObject;
};

/** texture 2d object. this is somewhat similar to a 3d texture,
    with the difference that there's no filtering between layers.
*/
class Texture2DArray : public Texture
{
public:
    Texture2DArray();
    ~Texture2DArray();

    /** bind this texture object.
    */
    void Bind() const;
    void Unbind() const;

    /** set texture parameters.
    */
    void SetFilter(GLint MinFilter, GLint MagFilter);
    void SetWrap(GLint SWrap, GLint TWrap);
    inline void SetWrap(GLint Wrap)
    {
        SetWrap(Wrap, Wrap);
    }

    /** set the image data.
    */
    void SetImage(GLint Level,
                  GLint InternalFormat,
                  GLsizei Width,
                  GLsizei Height,
                  GLsizei Depth,
                  GLenum Format,
                  GLenum Type,
                  GLvoid* Data);

private:
    GLuint GLObject;
};

/** texture 2d object.
*/
class Texture2D : public Texture
{
public:
    Texture2D();
    ~Texture2D();
    /** bind this texture object.
    */
    void Bind() const;
    void Unbind() const;

    /** set texture parameters.
    */
    void SetFilter(GLint MinFilter, GLint MagFilter);
    void SetWrap(GLint SWrap, GLint TWrap);
    inline void SetWrap(GLint Wrap)
    {
        SetWrap(Wrap, Wrap);
    }
    void SetCompareMode(GLint Mode);
    void SetCompareFunc(GLint Function);
    void GenerateMipmap();

    /** Image upload.
    */
    void SetImage(GLint Level,
                  GLint InternalFormat,
                  GLsizei Width,
                  GLsizei Height,
                  GLenum Format,
                  GLenum Type,
                  const GLvoid* Data);

    void SetImage(GLint Level, GLint InternalFormat, GLsizei Width, GLsizei Height);

    void SetImage(const replay::pixbuf& Source, Colorspace Space);

    void SetSubImage(
        GLint Level, GLint x, GLint y, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, const GLvoid* Data);

    void LoadFromFile(const Path& Filename, Colorspace Space);

    /** Image download.
    */
    replay::shared_pixbuf GetImage(GLint Level) const;

    /** get the object id as used by OpenGL
    */
    GLuint GetGLObject() const
    {
        return GLObject;
    }

private:
    GLuint GLObject;
};
}

#endif
