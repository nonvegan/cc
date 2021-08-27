#ifndef GL_H_
#define GL_H_

#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void compile_shader(GLuint shader, const GLchar *shader_src);

void link_shaders(GLuint program, GLuint *shaders, size_t n);

#endif // GL_H_
