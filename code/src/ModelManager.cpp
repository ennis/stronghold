#include "ModelManager.hpp"
#include <iostream>
#include <stdexcept>

static Model *assimpLoadModel(const char *path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::string str = "Error during loading of ";
		str += path;
		throw std::runtime_error(str);
	}

	aiMesh *mesh = scene->mMeshes[0];

	return new Model(mesh);

	//numVerts = mesh->mNumFaces * 3;

	//vertexArray = new float[mesh->mNumFaces * 3 * 3];
	//normalArray = new float[mesh->mNumFaces * 3 * 3];

	/*for (unsigned int i = 0; i<mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];

		for (int j = 0; j<3; j++) {
			aiVector3D normal = mesh->mNormals[face.mIndices[j]];
			memcpy(normalArray, &normal, sizeof(float)* 3);
			normalArray += 3;

			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			memcpy(vertexArray, &pos, sizeof(float)* 3);
			vertexArray += 3;
		}
	}*/

	//normalArray -= mesh->mNumFaces * 3 * 3;
	//vertexArray -= mesh->mNumFaces * 3 * 3;
}

Model *ModelManager::loadModel(const char *modelName)
{
	std::pair<std::map<std::string, Model*>::iterator, bool> ins =
		modelMap.insert(std::make_pair(modelName, static_cast<Model*>(NULL)));

	Model *m;

	// On a déja chargé cette texture auparavant
	if (ins.second) {
		// just created
		m = assimpLoadModel(modelName);
		ins.first->second = m;
		std::clog << "Loading model " << modelName << "\n";
	}
	else {
		// already loaded
		m = ins.first->second;
		//std::clog << "Returning loaded texture " << texName << "(" << tex->getId() << ")\n";
	}

	return m;
}
