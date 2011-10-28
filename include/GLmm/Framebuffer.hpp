/*  Cataclysm-Software Disaster Engine

	$Id: Framebuffer.hpp 920 2011-04-19 23:10:57Z ltjax $

	Copyright 2006-2007 Marius Elvert
*/

#ifndef _GLMM_FRAMEBUFFER_HPP_
#define _GLMM_FRAMEBUFFER_HPP_

#include <boost/utility.hpp>
#include "Object.hpp"
#include "Texture.hpp"

namespace GLmm {

/** A possible framebuffer component.
*/
class Renderbuffer :
	public boost::noncopyable
{
public:
	void				SetStorage( GLenum InternalFormat, GLsizei w, GLsizei h );

						Renderbuffer();
						~Renderbuffer();

private:
	GLuint				GLObject;

	friend class Framebuffer;
};

/** Additional rendering target.
*/
class Framebuffer :
	public boost::noncopyable
{
public:

						Framebuffer();
						~Framebuffer();

	void				Bind() const;
	static void			Unbind();

	void				TestCompleteness();

	void				AttachColorbuffer( GLuint Index, const Texture2D& Texture, GLint Level = 0 );
	void				DetachColorbuffer( GLuint Index );

	void				Attach( GLenum Attachment, const Texture2D& Texture, GLint Level = 0 );
	void				Attach( GLenum Attachment, const Renderbuffer& Renderbuffer );
	void				Detach( GLenum Attachment );

private:
	GLuint				GLObject;

};

}

#endif //_GLMM_FRAMEBUFFER_HPP_

