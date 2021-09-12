#include "glfw.h"

Vec2f glfw_window_size(GLFWwindow *window)
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return vec2f((float) w, (float) h);
}

Vec2f glfw_mouse_pos(GLFWwindow *window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return vec2f((float) xpos, (float) ypos);
}
