#ifndef MODELMANAGER_HPP
#define MODELMANAGER_HPP

#include <Model.hpp>
#include <map>
#include <string>

class ModelManager
{
public:
	ModelManager()
	{}
	
	~ModelManager()
	{}

	/*
	* Cree un modèle à partir d'un image, et la renvoie.
	* Si le modèle a deja été créé, on le renvoie juste.
	*/
	Model *loadModel(const char *modelName);


	static ModelManager& Instance() {
		static ModelManager singleton;
		return singleton;
	}

private:
	std::map<std::string, Model*> modelMap;
};

#endif