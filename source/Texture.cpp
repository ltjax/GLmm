
#include "Texture.hpp"
#include <replay/pixbuf_io.hpp>

using namespace GLmm;

namespace
{

GLuint FormatForChannelCount(int ChannelCount)
{
    switch (ChannelCount)
    {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        GLMM_THROW_ERROR("Unsupported number of channels");
    }
}

GLuint InternalFormatForFormat(GLuint Format, GLmm::Colorspace Space)
{
    if (Space == GLmm::Colorspace::Linear)
        return Format;

    switch (Format)
    {
    case GL_RGB:
        return GL_SRGB8;
    case GL_RGBA:
        return GL_SRGB8_ALPHA8;
    default:
        GLMM_THROW_ERROR("Unsupported format for sRGB");
    }
}
}

void Texture::BindTo(GLenum const TextureUnit) const
{
    glActiveTexture(TextureUnit);
    GLMM_CHECK_ERRORS();

    this->Bind();
}

Texture::Texture()
{
}

void Texture3D::LoadFromFileTiled(const Path& Filename, unsigned int x, unsigned int y)
{
    auto SourceImage = replay::pixbuf_io::load_from_file(Filename);

    unsigned int wi = SourceImage.width() / x;
    unsigned int hi = SourceImage.height() / y;

    glBindTexture(GL_TEXTURE_3D, GLObject);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, wi, hi, x * y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    GLMM_CHECK_ERRORS();

    SetFilter(GL_NEAREST, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    for (unsigned int j = 0; j < y; ++j)
    {
        for (unsigned int i = 0; i < x; ++i)
        {
            auto Image = SourceImage.crop(i * wi, j * hi, wi, hi);

            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, j * x + i, wi, hi, 1, GL_RGBA, GL_UNSIGNED_BYTE, Image.ptr());
            GLMM_CHECK_ERRORS();
        }
    }
}

void Texture3D::SetFilter(GLint MinFilter, GLint MagFilter)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, MinFilter);
    GLMM_CHECK_ERRORS();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, MagFilter);
    GLMM_CHECK_ERRORS();
}

void Texture3D::SetWrap(GLint SWrap, GLint TWrap, GLint RWrap)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, SWrap);
    GLMM_CHECK_ERRORS();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, TWrap);
    GLMM_CHECK_ERRORS();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, RWrap);
    GLMM_CHECK_ERRORS();
}

Texture3D::Texture3D()
{
    glGenTextures(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

Texture3D::~Texture3D()
{
    glDeleteTextures(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

void Texture3D::Bind() const
{
    glBindTexture(GL_TEXTURE_3D, GLObject);
    GLMM_CHECK_ERRORS();
}

void Texture3D::Unbind() const
{
    glBindTexture(GL_TEXTURE_3D, 0);
    GLMM_CHECK_ERRORS();
}
void Texture2DArray::SetFilter(GLint MinFilter, GLint MagFilter)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, MinFilter);
    GLMM_CHECK_ERRORS();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, MagFilter);
    GLMM_CHECK_ERRORS();
}

void Texture2DArray::SetWrap(GLint SWrap, GLint TWrap)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, SWrap);
    GLMM_CHECK_ERRORS();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, TWrap);
    GLMM_CHECK_ERRORS();
}

Texture2DArray::Texture2DArray()
{
    glGenTextures(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

Texture2DArray::~Texture2DArray()
{
    glDeleteTextures(1, &GLObject);
    GLMM_CHECK_ERRORS();
}

void Texture2DArray::SetImage(GLint Level,
                              GLint InternalFormat,
                              GLsizei Width,
                              GLsizei Height,
                              GLsizei Depth,
                              GLenum Format,
                              GLenum Type,
                              GLvoid* Data)
{
    glTexImage3D(GL_TEXTURE_2D_ARRAY, Level, InternalFormat, Width, Height, Depth, 0, Format, Type, Data);
    GLMM_CHECK_ERRORS();
}

void Texture2DArray::Bind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, GLObject);
    GLMM_CHECK_ERRORS();
}

void Texture2DArray::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    GLMM_CHECK_ERRORS();
}

Texture2D::Texture2D()
{
    GLMM_CHECK_ERRORS();
    // Allocate a texture from the GL
    glGenTextures(1, &GLObject);
    GLMM_CHECK_ERRORS();

    // Set the default filter to linear
    SetFilter(GL_LINEAR, GL_LINEAR);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &GLObject);
    // GLMM_CHECK_ERRORS();
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, GLObject);
    GLMM_CHECK_ERRORS();
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetFilter(GLint MinFilter, GLint MagFilter)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
    GLMM_CHECK_ERRORS();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetWrap(GLint SWrap, GLint TWrap)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SWrap);
    GLMM_CHECK_ERRORS();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TWrap);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetCompareMode(GLint Mode)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, Mode);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetCompareFunc(GLint Function)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, Function);
    GLMM_CHECK_ERRORS();
}

void Texture2D::GenerateMipmap()
{
    this->Bind();

    glGenerateMipmap(GL_TEXTURE_2D);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetImage(
    GLint Level, GLint InternalFormat, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, const GLvoid* Data)
{
    this->Bind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLMM_CHECK_ERRORS();

    glTexImage2D(GL_TEXTURE_2D, Level, InternalFormat, Width, Height, 0, Format, Type, Data);
    GLMM_CHECK_ERRORS();
}

void Texture2D::SetSubImage(
    GLint Level, GLint x, GLint y, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, const GLvoid* Data)
{
    this->Bind();

    glTexSubImage2D(GL_TEXTURE_2D, Level, x, y, Width, Height, Format, Type, Data);

    GLMM_CHECK_ERRORS();
}

void Texture2D::SetImage(GLint Level, GLint InternalFormat, GLsizei Width, GLsizei Height)
{
    GLenum Format = GL_RGBA;

    switch (InternalFormat)
    {
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT:
        Format = GL_DEPTH_COMPONENT;
        break;
    default:
        break;
    };

    this->SetImage(Level, InternalFormat, Width, Height, Format, GL_UNSIGNED_BYTE, 0);
}

void Texture2D::SetImage(const replay::pixbuf& Source, Colorspace Space)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto Format = FormatForChannelCount(Source.channel_count());

    SetImage(0, InternalFormatForFormat(Format, Space), Source.width(), Source.height(), Format,
             GL_UNSIGNED_BYTE, Source.ptr());
}

void Texture2D::LoadFromFile(const Path& Filename, Colorspace Space)
{
    // load the texture
    auto Source = replay::pixbuf_io::load_from_file(Filename);

    if (Source.empty())
        GLMM_THROW_ERROR("Unable to load texture: " << Filename.string() << " (unable to load image file)");

    this->SetImage(Source, Space);
}

replay::pixbuf Texture2D::GetImage(GLint Level) const
{
    GLint Width = 0;
    GLint Height = 0;
    this->Bind();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, Level, GL_TEXTURE_WIDTH, &Width);
    GLMM_CHECK_ERRORS();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, Level, GL_TEXTURE_HEIGHT, &Height);
    GLMM_CHECK_ERRORS();

    replay::pixbuf Result(Width, Height, replay::pixbuf::color_format::rgba);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    GLMM_CHECK_ERRORS();
    glGetTexImage(GL_TEXTURE_2D, Level, GL_RGBA, GL_UNSIGNED_BYTE, Result.ptr());
    GLMM_CHECK_ERRORS();

    return Result;
}
