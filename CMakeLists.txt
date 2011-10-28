project(GLmm)

include_directories(
  ${replay_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIR}
  ${GLEW_INCLUDE_PATH}
)

add_library(GLmm
	source/Object.cpp
	include/GLmm/Object.hpp
	source/Shader.cpp
	include/GLmm/Shader.hpp
	source/Program.cpp
	include/GLmm/Program.hpp
	source/BufferObject.cpp
	include/GLmm/BufferObject.hpp
	source/Texture.cpp
	include/GLmm/Texture.hpp
	source/Framebuffer.cpp
	include/GLmm/Framebuffer.hpp
)
