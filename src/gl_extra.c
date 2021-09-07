#include <stdio.h>

#include "gl_extra.h"

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
