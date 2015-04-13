#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <OpenGL.hpp>

GLFWwindow *createMainWindow(int width, int height, const char *title);
extern GLFWwindow *GMainWindow;

#endif