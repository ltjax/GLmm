set(TARGET_NAME GLmm)

add_library(GLmm STATIC
  include/gl_core_3_2.h
  source/gl_core_3_2.c
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

target_include_directories(${TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/GLmm
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  
target_link_libraries(${TARGET_NAME}
  replay)
