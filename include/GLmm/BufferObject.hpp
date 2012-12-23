
#ifndef GLMM_BUFFER_OBJECT_HPP
#define GLMM_BUFFER_OBJECT_HPP

#include <vector>
#include <array>
#include <type_traits>
#include "Shader.hpp"

namespace GLmm {

/** An executable for the programmable GL pipeline.
*/
class BufferObject :
	public boost::noncopyable
{
public:

							BufferObject();
							BufferObject(BufferObject&& Rhs);
							~BufferObject();

	BufferObject&			operator=(BufferObject Rhs);
	
	/** Transfer the data to the buffer object.
		Implies a bind.
		\param Target the buffer target type this is to be bound too
		\param Size The number of elements of the the specified data type.
		\param Data Pointer to an array of elements.
		\param Usage Which policy to use for storing.
	*/
	void					SetData(GLenum Target, std::size_t Size,
									const GLubyte* Data, GLenum Usage);
	
	/** Transfer the data to the buffer object.
		Implies a bind.
		\param Target the buffer target type this is to be bound too
		\param Data Vector to an array of elements.
		\param Usage Which policy to use for storing.
	*/
	template <class T>
	void					SetData(GLenum Target, const std::vector<T>& Data, GLenum Usage)
	{
#if _MSC_VER >= 1600
		static_assert(std::has_trivial_copy_constructor<T>::value, "T needs to have a trivial copy-constructor");
#endif
		SetData(Target, sizeof(T)*Data.size(),
			reinterpret_cast<const GLubyte*>(Data.data()), Usage); 
	}

	/** Transfer the data to the buffer object.
		Implies a bind.
		\param Target the buffer target type this is to be bound too
		\param Data std::array to an array of elements.
		\param Usage Which policy to use for storing.
	*/
	template <class T, std::size_t N>
	void					SetData(GLenum Target, const std::array<T, N>& Data, GLenum Usage)
	{
#if _MSC_VER >= 1600
		static_assert(std::has_trivial_copy_constructor<T>::value, "T needs to have a trivial copy-constructor");
#endif
		SetData(Target, sizeof(T)*N,
			reinterpret_cast<const GLubyte*>(Data.data()), Usage); 
	}

	/** Transfer a part of the data to the buffer object.
		Does not reallocate.
	*/
	void					SetSubData( GLenum Target, std::size_t Size,
										std::size_t Offset, const GLubyte* Data );

	/** Map the buffer into local address space.
	*/
	GLubyte*				Map(GLenum Target, GLenum Access);

	bool					Unmap(GLenum Target);
	void					Bind(GLenum Target) const;

	static void				Unbind(GLenum Target);

	void					Swap(BufferObject& Rhs);

	GLuint					GetGLObject() const {return mGLObject;}


private:
	
	GLuint					mGLObject;
	bool					mInvalid;
};

    
#if _MSC_VER >= 1600
typedef stdext::checked_array_iterator<GLubyte*> BufferIterator;
inline BufferIterator MakeBufferIterator(GLubyte* Data, std::size_t Size)
{
    return BufferIterator(Data, Size);
}
#else
typedef GLubyte* BufferIterator;
inline BufferIterator MakeBufferIterator(GLubyte* Data, std::size_t /*Size*/)
{
    return Data;
}
#endif
    
inline BufferObject& BufferObject::operator=(BufferObject Rhs)
{
	Swap(Rhs);
	return *this;
}

template <class T>
void SetSubData(BufferObject& Object, GLenum Target, std::size_t Size, std::size_t Offset, T Function)
{
	// First attempt with a map	
	if (GLubyte* MappedBuffer = Object.Map(Target, GL_WRITE_ONLY))
	{
		try {
			Function(MakeBufferIterator(MappedBuffer+Offset, Size));
		}
		catch (...)
		{
			// Unmap and rethrow
			Object.Unmap(Target);
			throw;
		}

		if (Object.Unmap(Target))
		{
			// Everything is ok! Go!
			return;
		}
	}

	// Map or unmap failed, so try again
	std::vector<GLubyte> Buffer(Size);
	
	// Fill the buffer
	Function(MakeBufferIterator(Buffer.data(), Size));

	// Write it out
	Object.SetSubData(Target, Size, Offset, Buffer.data());
}

}


#endif
