#include "Mesh.hpp"
#include <iostream>
#include <stdint.h>


Mesh::Mesh() : 
mode(GL_TRIANGLES), numVertices(0), numIndices(0), numPositionComponents(0),
vao(-1), vbPosition(-1), vbNormals(-1), vbColor(-1), 
vbTexcoords(-1), ib(-1)
{
}

Mesh::~Mesh()
{
	std::clog << "Deleting mesh\n";
	if (vao != -1) {
		glDeleteVertexArrays(1, &vao);
	}
	if (vbPosition != -1) {
		glDeleteBuffers(1, &vbPosition);
	}
	if (vbNormals != -1) {
		glDeleteBuffers(1, &vbNormals);
	}
	if (vbColor != -1) {
		glDeleteBuffers(1, &vbColor);
	}
	if (vbTexcoords != -1) {
		glDeleteBuffers(1, &vbTexcoords);
	}
	if (ib != -1) {
		glDeleteBuffers(1, &ib);
	}
}

void Mesh::setNumVertices(int numVertices_)
{
	numVertices = numVertices_;
}

void Mesh::setMeshMode(GLenum mode_)
{
	mode = mode_;
}

void Mesh::setPosition(const float *data)
{
	assert(numVertices != 0);
	vbPosition = createBuffer(vbPosition, numVertices * sizeof(float) * 3, data);
	numPositionComponents = 3;
}

void Mesh::setPosition2D(const float *data)
{
	assert(numVertices != 0);
	vbPosition = createBuffer(vbPosition, numVertices * sizeof(float) * 2, data);
	numPositionComponents = 2;
}

void Mesh::setNormals(const float *data)
{
	assert(numVertices != 0);
	vbNormals = createBuffer(vbNormals, numVertices * sizeof(float) * 3, data);
}

void Mesh::setColor(const float *data)
{
	assert(numVertices != 0);
	vbColor = createBuffer(vbColor, numVertices * sizeof(float) * 3, data);
}

void Mesh::setTexCoord(const float *data)
{	
	assert(numVertices != 0);
	vbTexcoords = createBuffer(vbTexcoords, numVertices * sizeof(float) * 2, data);
	numTexcoordComponents = 2;
}

void Mesh::setTexCoord3D(const float *data)
{
	assert(numVertices != 0);
	vbTexcoords = createBuffer(vbTexcoords, numVertices * sizeof(float)* 3, data);
	numTexcoordComponents = 3;
}

GLuint Mesh::createBuffer(GLuint buf, int size, const void *data, GLenum type)
{
	if (buf == -1) {
		GLuint vb;
		// set buffer data
		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vb;
	}
	else {
		// update data
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return buf;
	}
}

void Mesh::setIndices(const int *indices, int numIndices)
{
	ib = createBuffer(ib, numIndices * sizeof(uint32_t), indices);
	this->numIndices = numIndices;
}

void Mesh::initVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position index = 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbPosition);
	glVertexAttribPointer(0, numPositionComponents, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normals index = 1
	if (vbNormals != -1) {
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vbNormals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// color index = 2
	if (vbColor != -1) {
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, vbColor);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// texcoord0 index = 3
	if (vbTexcoords != -1) {
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, vbTexcoords);
		glVertexAttribPointer(3, numTexcoordComponents, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// TODO other texcoords

	if (ib != -1) {
		// draw with indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	}

	glBindVertexArray(0);
}

void Mesh::draw()
{
	assert(numVertices != 0);

	if (vao == -1) {
		initVAO();
	}

	glBindVertexArray(vao);

	if (ib != -1) {
		// element array buffer already bound in VAO
		assert(numIndices != 0);
		glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		// draw arrays
		glDrawArrays(mode, 0, numVertices);
	}

	glBindVertexArray(0);
}
