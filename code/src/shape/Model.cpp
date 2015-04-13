#include "Model.hpp"
#include <stdexcept>
#include <vector>

void Model::initRender()
{
	mesh.setNumVertices(importedMesh->mNumVertices);
	mesh.setPosition(&importedMesh->mVertices[0][0]);
	mesh.setNormals(&importedMesh->mNormals[0][0]);


	std::vector<glm::vec2> texcoord;
	for (int i = 0; i < importedMesh->mNumVertices; ++i) {
		texcoord.push_back(
			glm::vec2(
				importedMesh->mTextureCoords[0][i].x,
				importedMesh->mTextureCoords[0][i].y));
	}

	mesh.setTexCoord(glm::value_ptr(*texcoord.data()));

	std::vector<int> indices;

	for (int i = 0; i < importedMesh->mNumFaces; ++i) {
		for (int ii = 0; ii < importedMesh->mFaces[i].mNumIndices; ++ii) {
			indices.push_back(importedMesh->mFaces[i].mIndices[ii]);
		}
	}
	
	mesh.setIndices(indices.data(), indices.size());
}