#include <ShaderManager.hpp>
#include <sstream>

Program *ShaderManager::loadProgram(const char *vsFileName, const char *fsFileName)
{
	std::stringstream key;
	key << vsFileName << ";" << fsFileName;

	std::pair<std::map<std::string, Program*>::iterator, bool> ins =
		programs.insert(std::make_pair(key.str(), static_cast<Program*>(NULL)));

	Program *prog;

	if (ins.second) {
		// just created
		prog = new Program("");
		ins.first->second = prog;
		Shader *vs = loadShader(vsFileName, GL_VERTEX_SHADER);
		Shader *fs = loadShader(fsFileName, GL_FRAGMENT_SHADER);
		prog->attachShader(*vs);
		prog->attachShader(*fs);
		prog->link();
	}
	else {
		// already loaded
		prog = ins.first->second;
	}

	return prog;
}

Shader *ShaderManager::loadShader(const char *shaderFileName, GLenum shaderType)
{
	std::pair<std::map<std::string, Shader*>::iterator, bool> ins =
		shaders.insert(std::make_pair(std::string(shaderFileName), static_cast<Shader*>(NULL)));

	Shader *s;

	if (ins.second) {
		// just created
		s = new Shader();
		ins.first->second = s;
		s->load(shaderFileName, shaderType);
	}
	else {
		// already loaded
		s = ins.first->second;
	}

	return s;
}