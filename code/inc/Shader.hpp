#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

#include <string>

#define NO_TRACE

class Shader {
    public:
		Shader();
        void load(const char *filename, GLenum shaderType);
        GLenum getShaderType () const;
        GLuint getShader () const;
        ~Shader ();

    private:
        std::string filename;
        GLenum shaderType;
        GLuint shader;
};


#endif

