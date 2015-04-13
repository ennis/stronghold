#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP


#include <Shader.hpp>
#include <Program.hpp>
#include <map>
#include <string>

class ShaderManager
{
public:

	//
	// Chargement d'un programme depuis deux fichiers source
	// Si le programme est déjà chargé et compilé, la fonction renvoie un pointeur 
	// vers l'instance du programme.
	// Sinon, les shaders sont chargés, compilés, liés au programme, etc. et l'instance
	// du programme est créée.
	//
	// Utilisation
	//
	// ShaderManager::Instance().loadProgram("path to vertex shader", "path to fragment shader");
	//
	Program *loadProgram(const char *vsFileName, const char *fsFileName);

	static ShaderManager& Instance() {
		static ShaderManager singleton;
		return singleton;
	}

private:
	Shader *loadShader(const char *fileName, GLenum shaderType);

	std::map<std::string, Program*> programs;
	std::map<std::string, Shader*> shaders;
};

#endif