#ifndef GLFW_EXTRA_H
#define GLFW_EXTRA_H

#include <GLFW/glfw3.h>

#include "vec.h"

Vec2f glfw_window_size(GLFWwindow *window);

Vec2f glfw_mouse_pos(GLFWwindow *window);

#endif // GLFW_EXTRA_H
