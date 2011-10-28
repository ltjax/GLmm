
#ifndef GLMM_PROGRAM_HPP
#define GLMM_PROGRAM_HPP

#include "Shader.hpp"

namespace GLmm {

/** An executable for the programmable GL pipeline.
*/
class Program :
	public boost::noncopyable
{
public:

	/** Error that is thrown when Program::Link() fails.
	*/
	struct LinkError :
		public std::runtime_error
	{
		LinkError(const std::string& Message) :
			std::runtime_error( Message )
		{
		}
	};

										Program();
										Program(Program&& rhs);
										~Program();


	void								Attach(const Shader& Rhs);

	int									GetUniformLocation( const char* Name ) const;
	int									GetAttribLocation( const char* Name ) const;

	void								Link();
	
	void								Use() const;

	void								BindFragDataLocation(GLuint ColorIndex, const std::string& Name);

	GLuint								GetGLObject() const { return mGLObject; }

	Program&							operator=(Program Rhs) {Swap(Rhs); return *this;}

private:
	void								Swap(Program& Rhs) {std::swap(mGLObject, Rhs.mGLObject);}
	GLuint								mGLObject;
};

template <class FileRangeType, class FragmentOutputType>
Program CreateProgramFromFiles(FileRangeType&& FileList, FragmentOutputType&& FragmentOutputList)
{
	Program Result;

	// Attach ALL THE files
	for (auto i=FileList.begin(); i!=FileList.end(); ++i)
		Result.Attach(CreateShaderFromFile(*i));

	// Bind the fragment outputs
	for (auto i=FragmentOutputList.begin(); i!=FragmentOutputList.end(); ++i)
		Result.BindFragDataLocation(i->first, i->second);

	Result.Link();

	return Result;
}

}

#endif
