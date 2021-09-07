#ifndef GL_EXTRA_H_
#define GL_EXTRA_H_

#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLint compile_shader(GLuint shader, const GLchar *shader_src);

GLint link_shaders(GLuint program, GLuint *shaders, size_t n);

#endif // GL_EXTRA_H_
