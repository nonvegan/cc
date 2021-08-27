#include <stdio.h>

#include "gl.h"

void compile_shader(GLuint shader, const GLchar *shader_src)
{
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE) {
        fprintf(stderr, "ERROR: Could not compile shader %u\n", shader);
        exit(1);
    }

    printf("INFO: Compiled shader %u\n", shader);
}

void link_shaders(GLuint program, GLuint *shaders, size_t n)
{
    for(size_t i = 0; i < n; i++) {
        glAttachShader(program, shaders[i]);
        printf("INFO: Attached shader %u to program %u\n", shaders[i], program);
    }
    glLinkProgram(program);

    GLint link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if(link_status != GL_TRUE) {
        fprintf(stderr, "ERROR: Could not link program %u\n", program);
        exit(1);
    }

    printf("INFO: Linked program %u\n", program);
}
