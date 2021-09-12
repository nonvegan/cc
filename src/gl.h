#ifndef GL_EXTRA_H_
#define GL_EXTRA_H_

#include <stdlib.h>

#include "canvas.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLint compile_shader(GLuint shader, const GLchar *shader_src);

GLint link_shaders(GLuint program, GLuint *shaders, size_t n);

GLuint gl_gen_vertex_array(unsigned int n);

GLuint gl_gen_canvas_texture(Canvas *canvas);

void gl_update_canvas_texture(Canvas *canvas);

#endif // GL_EXTRA_H_
