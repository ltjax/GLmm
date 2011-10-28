project(GLmm)

include_directories(
  ${replay_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIR}
  ${GLEW_INCLUDE_PATH}
)

add_library(GLmm
	Object.cpp
	Object.hpp
	Shader.cpp
	Shader.hpp
	Program.cpp
	Program.hpp
	BufferObject.cpp
	BufferObject.hpp
	Texture.cpp
	Texture.hpp
	Framebuffer.cpp
	Framebuffer.hpp
)
