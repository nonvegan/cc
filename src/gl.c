#include "gl.h"

#include <stdio.h>

GLint compile_shader(GLuint shader, const GLchar *shader_src)
{
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    return compile_status;
}

GLint link_shaders(GLuint program, GLuint *shaders, size_t n)
{
    for(size_t i = 0; i < n; i++) {
        glAttachShader(program, shaders[i]);
        printf("INFO: Attached shader %u to program %u\n", shaders[i], program);
    }
    glLinkProgram(program);

    GLint link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    return link_status;
}

GLuint gl_gen_vertex_array(unsigned int n)
{
    GLuint va;
    glGenVertexArrays(n, &va);
    glBindVertexArray(va);

    return va;
}

GLuint gl_gen_canvas_texture(Canvas *canvas)
{
    GLuint canvas_texture;
    glGenTextures(1, &canvas_texture);
    glBindTexture(GL_TEXTURE_2D, canvas_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canvas->width, canvas->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, canvas->ctx);

    return canvas_texture;
}

void gl_update_canvas_texture(Canvas *canvas)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, canvas->width, canvas->height,
                    GL_RGB, GL_UNSIGNED_BYTE, canvas->ctx);
}


