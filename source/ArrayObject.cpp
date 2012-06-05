
#include "ArrayObject.hpp"
#include <boost/numeric/conversion/cast.hpp>


GLmm::ArrayObject::ArrayObject()
: mObject(0)
{
	glGenVertexArrays(1, &mObject);
	GLMM_CHECK_ERRORS();

	// Check that we actually created a non-zero object
	assert(mObject != 0);
}

GLmm::ArrayObject::ArrayObject(ArrayObject&& Rhs)
: mObject(Rhs.mObject)
{
	Rhs.mObject=0;
}

GLmm::ArrayObject&
GLmm::ArrayObject::operator=(ArrayObject&& Rhs)
{
	// FIXME: clumsy implementation
	if (this == &Rhs)
		return *this;

	if (mObject != 0)
		glDeleteVertexArrays(1, &mObject);

	mObject = Rhs.mObject;
	Rhs.mObject = 0;

	return *this;
}

void
GLmm::ArrayObject::Bind()
{
	// Cannot bind moved objects
	assert(mObject != 0);

	glBindVertexArray(mObject);
	GLMM_CHECK_ERRORS();
}

GLmm::ArrayObject::~ArrayObject()
{
	// Handle moved objects
	if (mObject != 0)
		glDeleteVertexArrays(1, &mObject);
}

GLmm::ArrayObject&	GLmm::ArrayObject::SetElementBuffer(
	GLmm::BufferObject const&	ElementBuffer
)
{
	Bind();
	ElementBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
	return *this;
}

GLmm::ArrayObject&	GLmm::ArrayObject::SetAttribPointer(
	int							Location,
	GLmm::BufferObject const&	Buffer,
	GLint						Size,
	GLenum						Type,
	bool						Normalized,
	std::size_t					Stride,
	std::size_t					Offset
)
{
	Bind();

	if (Location == -1)
		return *this;

	glEnableVertexAttribArray(Location);
	Buffer.Bind(GL_ARRAY_BUFFER);
	glVertexAttribPointer(Location, Size, Type, Normalized ? GL_TRUE : GL_FALSE, boost::numeric_cast<GLsizei>(Stride), reinterpret_cast<GLvoid*>(Offset));

	return *this;
}

void GLmm::ArrayObject::DrawArrays(GLenum Mode, GLint First, GLsizei Count)
{
	Bind();
	glDrawArrays(Mode, First, Count);
	GLMM_CHECK_ERRORS();
}

void GLmm::ArrayObject::DrawElements(GLenum Mode, GLsizei Count, GLenum Type, std::size_t Offset)
{
	Bind();
	glDrawElements(Mode, Count, Type, reinterpret_cast<GLvoid*>(Offset));
	GLMM_CHECK_ERRORS();
}

void GLmm::ArrayObject::DrawRangeElements(GLenum Mode, GLuint Start, GLuint End, GLsizei Count, GLenum Type, std::size_t Offset)
{
	Bind();
	glDrawRangeElements(Mode, Start, End, Count, Type, reinterpret_cast<GLvoid*>(Offset));
	GLMM_CHECK_ERRORS();
}