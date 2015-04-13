#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cassert>

static std::string file_contents(const char *filename);

Shader::Shader() : shaderType(0), shader(0)
{}

void Shader::load(const char *filename, GLenum shaderType_) 
{
	shaderType = shaderType_;
    // Retrieving the source of the shader
	std::string shaderSource = file_contents(filename);
	const char *sourcePtr = shaderSource.c_str();

    // Creating the shader
    shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &sourcePtr, NULL);

    // Compiling it
    std::cout << std::endl << "Compiling " << filename;
#ifndef NO_TRACE
    std::cout << ":" << std::endl << shaderSource;
#endif
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);

        std::cerr << "Error during compilation !" << std::endl;
        std::cerr << buffer << std::endl;

		assert(false);
    }

    std::cout << "Compilation OK !" << std::endl;
}

Shader::~Shader () {
	if (shader)
		glDeleteShader(shader);
}

GLuint Shader::getShader () const {
    return shader;
}

GLenum Shader::getShaderType () const {
    return shaderType;
}

// Get the contents of a file
static std::string file_contents (const char *filename) {
    std::ifstream file(filename);

    std::string content( (std::istreambuf_iterator<char>(file) ),
                         (std::istreambuf_iterator<char>()    ) );

    return content;
}
