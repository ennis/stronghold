#ifndef MODEL_HPP
#define MODEL_HPP

#include <Mesh.hpp>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(aiMesh *mesh_) : importedMesh(mesh_)
	{
		initRender();
    }

    /*float* getVertexArray () const;
    float* getNormalArray () const;
    int getNumVertices () const;*/

	aiMesh *getImportedMesh() {
		return importedMesh;
	}

	Mesh &getMesh() {
		return mesh;
	}

private:
	void initRender();
	aiMesh *importedMesh;
	Mesh mesh;
    //int numVerts;
    //float *vertexArray;
    //float *normalArray;
};

#endif

