
#ifndef GLMM_SHADER_HPP
#define GLMM_SHADER_HPP

#include <iostream>
#include <stdexcept>
#include <boost/utility.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional/optional.hpp>
#include "Object.hpp"

namespace GLmm {

/** A GL programmable pipeline compilation unit.
*/
class Shader :
	public boost::noncopyable
{
public:

	/** Error that is thrown when Shader::Compile() fails.
	*/
	struct CompileError :
		public std::runtime_error
	{
		CompileError(const std::string& Message) :
			std::runtime_error(Message)
		{
		}
	};

	explicit				Shader(GLenum Type, const std::string& Source);
							Shader(Shader&& Other);
							~Shader();

	Shader&					operator=(Shader&& Other);


	GLuint					GetGLObject() const {return mObject;}

private:
	void					SetSource(const std::string& Source);
	
	/** Compile the shader.
		Throws CompileError on a compile-time error.
	*/
	void					Compile();

	GLuint					mObject;
};

/** Create a shader from a file.
*/
Shader						CreateShaderFromFile(GLenum Type, const boost::filesystem::path& Filename);

/** Create a shader from a file and try to guess the shader type.
*/
Shader						CreateShaderFromFile(const boost::filesystem::path& Filename);

}

#endif

