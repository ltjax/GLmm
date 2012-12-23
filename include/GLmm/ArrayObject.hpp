
#ifndef GLMM_ARRAY_OBJECT_HEADER
#define GLMM_ARRAY_OBJECT_HEADER

#include <boost/utility.hpp>
#include "BufferObject.hpp"

namespace GLmm {

template <class T> struct AttribTraits
{
};

class ArrayObject :
	public boost::noncopyable
{
public:
	ArrayObject();
	ArrayObject(ArrayObject&& Rhs);
	~ArrayObject();

	ArrayObject& operator=(ArrayObject Rhs); 

	ArrayObject&	SetElementBuffer(
		GLmm::BufferObject const&	ElementBuffer
	);

	ArrayObject&	SetAttribPointer(
		GLint						Location,
		GLmm::BufferObject const&	Buffer,
		GLint						Size,
		GLenum						Type,
		bool						Normalized,
		std::size_t					Stride,
		std::size_t					Offset
	);

	template <class T, class M>
	ArrayObject&	SetAttribPointer(
		int							Location,
		GLmm::BufferObject const&	Buffer,
		M							(T::*Member)
	)
	{
		const T* Object=nullptr;
		const M* Ptr=&(Object->*Member);
		const std::size_t Offset=reinterpret_cast<const char*>(Ptr)-reinterpret_cast<const char*>(Object);
#if _MSC_VER >= 1600
		static_assert(std::has_trivial_copy_constructor<M>::value, "Vertex-attrib must trivially copyable");
#endif
        
		typedef AttribTraits<M> Traits;
		return SetAttribPointer(Location, Buffer, Traits::Size, Traits::Type, Traits::Normalized, sizeof(T), Offset);
	}

	void	DrawArrays(GLenum Mode, GLint First, GLsizei Count) const;
	void	DrawElements(GLenum Mode, GLsizei Count, GLenum Type, std::size_t Offset) const;
	void	DrawRangeElements(GLenum Mode, GLuint Start, GLuint End, GLsizei Count, GLenum Type, std::size_t Offset) const;
	void	DrawRangeElementsBaseVertex(GLenum Mode, GLuint Start, GLuint End, GLsizei Count, GLenum Type, std::size_t Offset, GLint BaseVertex) const;
	void	Swap(ArrayObject& Rhs);
private:

	void	Bind() const;
	GLuint	mObject;
};

inline ArrayObject& ArrayObject::operator=(ArrayObject Rhs)
{
	Swap(Rhs);
	return *this;
}

};

#endif
