#ifndef MESH_HPP
#define MESH_HPP

#include <OpenGL.hpp>

// 
// Classe représentant un maillage
// et ses buffers OpenGL
//
// Utilisation:
// mesh.setNumVertices(<nombre de sommets>)
// mesh.setMeshMode(GL_LINES ou GL_TRIANGLES, par défaut GL_TRIANGLES)
//
// mesh.setPosition(...)
//
class Mesh
{
public:
	Mesh();
	~Mesh();

	void setNumVertices(int numVertices);
	int getNumVertices() const {
		return numVertices;
	}
	void setMeshMode(GLenum mode);
	void setPosition(const float *data);
	void setPosition2D(const float *data);
	void setNormals(const float *data);
	void setColor(const float *data);
	void setTexCoord(const float *data);
	void setTexCoord3D(const float *data);
	void setIndices(const int *indices, int numIndices);

	// TODO helper methods
	//void loadFromOBJ(const char *filename);

	void draw();

private:
	GLuint createBuffer(GLuint buf, int size, const void *data, GLenum type = GL_STATIC_DRAW);
	void initVAO();

	GLenum mode;
	int numVertices;
	int numIndices;
	int numPositionComponents;	// 2 ou 3
	int numTexcoordComponents;

	GLuint vao;
	GLuint vbPosition;	
	GLuint vbColor; //optionnel
	GLuint vbNormals; //optionnel
	GLuint vbTexcoords; //optionnel
	GLuint ib;
};

#endif