#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <OpenGL.hpp>
#include "Shader.hpp"

//
// Classe représentant un programme OpenGL (vertex shader + fragment shader)
// 
// Les attributs nommés 'position', 'normal', 'color' et 'texcoord' sont associés aux
// locations 0, 1, 2 et 3 respectivement
//
class Program {
public:
    Program (std::string const& name);
    void attachShader (Shader const& shader);
    void bindFragDataLocation(const char *fragData, GLuint idFragData = 0);
    void link ();
    void use ();
    void deactivate () const;
    GLuint getProgram () const;
	GLuint getUniformLocation(const char *uniform) const;

	void uniform1i(const char *uniform, int i);
	void uniform1f(const char *uniform, float f);
	void uniform2f(const char *uniform, glm::vec2 const &v);
	void uniform3f(const char *uniform, glm::vec3 const &v); 
	void uniform4f(const char *uniform, glm::vec4 const &v);
	void uniformMatrix4fv(const char *uniform, glm::mat4 const &mat);

    ~Program ();

private:
    std::string name;
    bool linked;
    GLuint id;
};

#endif

