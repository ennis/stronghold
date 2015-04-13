#include "Program.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

Program::Program (std::string const& name) : name(name) {
    std::cout << "Creating program " << name << std::endl;
    id = glCreateProgram();
}

void Program::attachShader (Shader const& shader) {
    glAttachShader(id, shader.getShader());
}

void Program::bindFragDataLocation (const char *fragData, GLuint idFragData) {
    glBindFragDataLocation(id, idFragData, fragData);
}

GLuint Program::getUniformLocation(const char *uniform) const
{
	return glGetUniformLocation(id, uniform);
}

void Program::link () {
	std::cout << "Linking " << name;

	// bind attribute locations
	// must be done before linking for some reason
	glBindAttribLocation(id, 0, "position");
	glBindAttribLocation(id, 1, "normal");
	glBindAttribLocation(id, 2, "color");
	glBindAttribLocation(id, 3, "texcoord");

    glLinkProgram(id);

    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetProgramInfoLog(id, 512, NULL, buffer);
        std::cout << " failed :" << buffer << std::endl;
		assert(false);
    }

    linked = true;
    std::cout << " success !" << std::endl;

}

void Program::use () {
    if (linked) {
        //std::cout << "Using " << name << std::endl;
        glUseProgram(id);
    } else {
        std::cout << "Program not linked !" << std::endl;
		assert(false);
	}
}

void Program::uniform1i(const char *uniform, int i)
{
	GLuint id = getUniformLocation(uniform);
	glUniform1i(id, i);
}

void Program::uniform1f(const char *uniform, float f)
{
	GLuint id = getUniformLocation(uniform);
	glUniform1f(id, f);
}

void Program::uniform2f(const char *uniform, glm::vec2 const &v)
{
	GLuint id = getUniformLocation(uniform);
	glUniform2f(id, v.x, v.y);
}

void Program::uniform3f(const char *uniform, glm::vec3 const &v)
{
	GLuint id = getUniformLocation(uniform);
	glUniform3f(id, v.x, v.y, v.z);
}

void Program::uniform4f(const char *uniform, glm::vec4 const &v)
{
	GLuint id = getUniformLocation(uniform);
	glUniform4f(id, v.x, v.y, v.z, v.w);
}

void Program::uniformMatrix4fv(const char *uniform, glm::mat4 const &mat)
{
	GLuint id = getUniformLocation(uniform);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::deactivate () const {
    glUseProgram(0);
}

GLuint Program::getProgram () const {
    return id;
}

Program::~Program () {
    glDeleteProgram(id);
}

