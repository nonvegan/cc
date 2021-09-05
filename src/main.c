#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "canvas.h"
#include "yuv.h"
#include "gl.h"

#define WIDTH_PX 256
#define HEIGHT_PX 256
#define RADIUS_PX (WIDTH_PX / 3)
#define BG_COLOR 0x282C34
#define FG_COLOR 0xF92672
#define VIDEO_DURATION 10
#define VIDEO_FPS 30
#define AA_X 4

#define MIN_ZOOM 0.2
#define MAX_ZOOM 2
#define ZOOM_SPEED 0.1

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

typedef struct {
    float x, y;
} Vec2f;

Vec2f camera_pos = {0}; 
Vec2f anchor_pos = {0};
float camera_scale = 1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) action;
    (void) mods;

    switch(key) {
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_R:
            camera_pos.x = 0;
            camera_pos.y = 0;
            camera_scale = 1.0f;
            break;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // TODO(#6): Implement a proper linear algebra TL
        float dx = xpos - anchor_pos.x;
        float dy = ypos - anchor_pos.y;
        camera_pos.x -= dx;
        camera_pos.y -= dy;
        anchor_pos.x = xpos;
        anchor_pos.y = ypos;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void) mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        anchor_pos.x = xpos;
        anchor_pos.y = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void) window;
    camera_scale = fmin(MAX_ZOOM, fmax(MIN_ZOOM , camera_scale + yoffset * ZOOM_SPEED));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void) window;
    glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
    Canvas *canvas = canvas_create(WIDTH_PX, HEIGHT_PX);
    canvas_clear(canvas, BG_COLOR);

    if (argc < 2 || strncmp("opengl", argv[1], 6)) {
        canvas_draw_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR);
        canvas_save_to_ppm(canvas, "circle.ppm");

        canvas_clear(canvas, BG_COLOR);
        canvas_draw_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR);
        canvas_save_to_ppm(canvas, "filled_circle.ppm");

        canvas_clear(canvas, BG_COLOR);
        canvas_draw_anti_aliased_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f, RADIUS_PX, FG_COLOR, BG_COLOR, AA_X);
        canvas_save_to_ppm(canvas, "anti_aliased_filled_circle.ppm");

        Y4m2 *y4m2 = y4m2_open_video("circle.y4m", WIDTH_PX, HEIGHT_PX, VIDEO_FPS);
        if(y4m2 == NULL) {
            canvas_free(canvas);
            exit(1);
        }

        size_t frame_count = VIDEO_FPS * VIDEO_DURATION;
        for(size_t i = 0; i < frame_count; i++) {
            canvas_clear(canvas, BG_COLOR);
            canvas_draw_anti_aliased_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f,
                    fabs(cos(i/24.0f)) * RADIUS_PX, FG_COLOR, BG_COLOR, AA_X);
            y4m2_dump_canvas_frame(y4m2, canvas, 1);
            printf("INFO: Generating %s: %.0f%%\r", y4m2->file_name, i * 100.0f / frame_count);
            fflush(stdout);
        }

        y4m2_close_video(y4m2);
    } else {
        if (!glfwInit()) {
            fprintf(stderr, "ERROR: Could not initialize GLFW");
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Initialized GLFW\n");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(WIDTH_PX, HEIGHT_PX, "C Circles", NULL, NULL);
        if(window == NULL) {
            const char* description;
            glfwGetError(&description);
            fprintf(stderr, "ERROR: Could not create a GLFW Window: %s\n", description);
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Created GLFW window\n");

        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glfwMakeContextCurrent(window);
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "ERROR: Could not initialize GLEW");
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Initialized GLEW\n");

        GLuint vertex_array;
        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        GLuint canvas_texture;
        glGenTextures(1, &canvas_texture);
        glBindTexture(GL_TEXTURE_2D, canvas_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH_PX, HEIGHT_PX,
                     0, GL_RGB, GL_UNSIGNED_BYTE, canvas->ctx);

        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        printf("INFO: Created vertex shader %u\n", vert_shader);
        GLchar *vert_shader_src =
            "#version 330 core                                                      \n\
            uniform vec2 camera_pos;                                                \n\
            uniform vec2 resolution;                                                \n\
            uniform float camera_scale;                                             \n\
            out vec2 uv;                                                            \n\
            void main(){                                                            \n\
                uv = vec2(gl_VertexID & 1, gl_VertexID >> 1);                       \n\
                vec2 camera = vec2(1, -1) * camera_pos / camera_scale / resolution; \n\
                gl_Position = vec4(2 * (uv - camera) - 1, 0.0, 1 / camera_scale);   \n\
            }";
        if(compile_shader(vert_shader, vert_shader_src) != GL_TRUE) {
            fprintf(stderr, "ERROR: Could not compile vertex shader %u\n", vert_shader);
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Compiled vertex shader %u\n", vert_shader);

        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        printf("INFO: Created fragment shader %u\n", frag_shader);
        GLchar *frag_shader_src =
            "#version 330 core                                                      \n\
            uniform sampler2D frame;                                                \n\
            in vec2 uv;                                                             \n\
            out vec4 color;                                                         \n\
            void main(){                                                            \n\
                color = texture(frame, vec2(uv.x, 1-uv.y));                         \n\
            }";
        if(compile_shader(frag_shader, frag_shader_src) != GL_TRUE) {
            fprintf(stderr, "ERROR: Could not compile fragment shader %u\n", frag_shader);
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Compiled fragment shader %u\n", frag_shader);

        GLuint program = glCreateProgram();
        printf("INFO: Created program %u\n", program);

        GLuint shaders[] = {vert_shader, frag_shader};
        if(link_shaders(program, shaders, ARRAY_SIZE(shaders)) != GL_TRUE) {
            fprintf(stderr, "ERROR: Could not link program %u\n", program);
            canvas_free(canvas);
            exit(1);
        }
        printf("INFO: Linked program %u\n", program);

        glUseProgram(program);

        GLint time_uniform_location = glGetUniformLocation(program, "time");
        GLint camera_pos_uniform_location = glGetUniformLocation(program, "camera_pos");
        GLint camera_scale_uniform_location = glGetUniformLocation(program, "camera_scale");
        GLint resolution_uniform_location = glGetUniformLocation(program, "resolution");

        while(!glfwWindowShouldClose(window)) {
            float glfw_time = (float) glfwGetTime();
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);

            glUniform1f(time_uniform_location, glfw_time);
            glUniform1f(camera_scale_uniform_location, camera_scale);
            glUniform2f(camera_pos_uniform_location, camera_pos.x, camera_pos.y);
            glUniform2f(resolution_uniform_location, w, h);
            
            glClear(GL_COLOR_BUFFER_BIT);
            canvas_clear(canvas, BG_COLOR);
            canvas_draw_anti_aliased_filled_circle(canvas, WIDTH_PX * 0.5f, HEIGHT_PX * 0.5f,
                                                   fabs(cos(glfw_time))* RADIUS_PX,
                                                   FG_COLOR, BG_COLOR, AA_X);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH_PX, HEIGHT_PX,
                            GL_RGB, GL_UNSIGNED_BYTE, canvas->ctx);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    canvas_free(canvas);
    return 0;
}
