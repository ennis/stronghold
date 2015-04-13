#include <Window.hpp>
#include <iostream>

GLFWwindow *GMainWindow;

// Callbacks
static void error_callback(int error, const char * desc);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void size_callback(GLFWwindow* window, int width, int height);


static void initGLFW() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw!" << std::endl;
		exit(EXIT_FAILURE);
	}


	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
}

GLFWwindow *createMainWindow(int width, int height, const char *title) {
	initGLFW();

	glfwSetErrorCallback(error_callback);
	GMainWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwSetKeyCallback(GMainWindow, key_callback);
	glfwSetWindowSizeCallback(GMainWindow, size_callback);

	if (!GMainWindow) {
		std::cerr << "Failed to open the window!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(GMainWindow);

	// glew init
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// inutile?
	glEnable(GL_DEPTH_TEST);

	return GMainWindow;
}

void error_callback(int error, const char * desc) {
	std::cerr << "Error : " << desc << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;

	default:
		break;
	}
}

void size_callback(GLFWwindow* window, int width, int height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
}