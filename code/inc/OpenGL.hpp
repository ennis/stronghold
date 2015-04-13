#ifndef OPENGL_HPP
#define OPENGL_HPP

#include <cassert>

#ifdef _MSC_VER
#include <Windows.h>
#endif

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// M_PI not defined on windows
#ifdef _MSC_VER
#define M_PI 3.141592653589793238
#endif


#endif

