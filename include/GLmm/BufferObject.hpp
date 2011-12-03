
#ifndef GLMM_BUFFER_OBJECT_HPP
#define GLMM_BUFFER_OBJECT_HPP

#include <vector>
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

	BufferObject&			operator=(BufferObject&& Rhs);
	
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
		\param Size The number of elements of the the specified data type.
		\param Data Pointer to an array of elements.
		\param Usage Which policy to use for storing.
	*/
	template <class T>
	void					SetData(GLenum Target, const std::vector<T>& Data, GLenum Usage)
	{
		static_assert(std::has_trivial_copy_constructor<T>::value, "T needs to have a trivial copy-constructor");
		SetData(Target, sizeof(T)*Data.size(),
			reinterpret_cast<const GLubyte*>(Data.data()), Usage); 
	}

	/** Transfer a part of the data to the buffer object.
		Does not reallocate.
	*/
	void					SetSubData( GLenum Target, std::size_t Size,
										std::size_t Offset, const GLubyte* Data );

	/** Map the buffer into local address space.
	*/
	GLubyte*				Map( GLenum Target, GLenum Access );

	bool					Unmap( GLenum Target );
	void					Bind( GLenum Target ) const;

	static void				Unbind( GLenum Target );

	void					Swap( BufferObject& Rhs );

	GLuint					GetGLObject() const {return mGLObject;}


private:
	
	GLuint					mGLObject;
	bool					mInvalid;
};

typedef stdext::checked_array_iterator<GLubyte*> BufferIterator;

template <class T>
void SetSubData(BufferObject& Object, GLenum Target, std::size_t Size, std::size_t Offset, T Function)
{
	// First attempt with a map	
	if (GLubyte* MappedBuffer = Object.Map(Target, GL_WRITE_ONLY))
	{
		try {
			Function(BufferIterator(MappedBuffer+Offset, Size));
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
	Function(BufferIterator(Buffer.data(), Size));

	// Write it out
	Object.SetSubData(Target, Size, Offset, Buffer.data());
}

}


#endif
