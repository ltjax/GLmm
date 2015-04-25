project(GLmm)

include_directories(
  ${replay_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/GLmm
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(GLmm
	include/flextGL.h
	source/flextGL.c
	source/ArrayObject.cpp
	include/GLmm/ArrayObject.hpp
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
