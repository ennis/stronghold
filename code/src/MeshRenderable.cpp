#include "MeshRenderable.hpp"
#include "ShaderManager.hpp"
#include <iostream>

MeshRenderable::MeshRenderable() : currentBatch(NULL), renderInited(false)
{}

MeshRenderable::~MeshRenderable()
{
	// delete batches
	for (std::map<VertexBatchIndex, VertexBatch*>::iterator it = batches.begin();
		it != batches.end(); ++it) 
	{
		delete it->second;
	}
}

void MeshRenderable::clear()
{
	batches.clear();
	currentBatch = NULL;
	renderInited = false;
}

void MeshRenderable::setColor(glm::vec4 const &color_)
{
	if (currentBatch == NULL) {
		// TODO create batch
	}
	else {
		currentBatch->color = color_;
	}
}

void MeshRenderable::beginBatch(Texture *texture, Texture * texBumpMap)
{
	VertexBatchIndex index = { texture, texBumpMap };
	VertexBatch *batch;

	std::pair<std::map<VertexBatchIndex, VertexBatch*>::iterator, bool> ins =
		batches.insert(std::make_pair(index, static_cast<VertexBatch*>(NULL)));

	if (ins.second)
	{	// just created
		batch = new VertexBatch();
		ins.first->second = batch;
		// TODO delete batches...
	}
	else {
		batch = ins.first->second;
	}

	currentBatch = batch;
}

void MeshRenderable::endBatch()
{

	// rien à faire?
}

void MeshRenderable::addTriangle(glm::vec3 const &p1, glm::vec3 const &p2, glm::vec3 const &p3, CoordSystem const &cs)
{
	glm::vec2 tc = glm::vec2(0, 0);

	glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	glm::vec3 u;
	glm::vec3 v;

	glm::vec3 cu = cs.rotate(glm::vec3(0, 0, 1));
	glm::vec3 cv = cs.rotate(glm::vec3(1, 0, 0));

	if (glm::length(glm::cross(n, cu)) < 0.01f) {
		v = glm::normalize(glm::cross(n, cv));
	}
	else {
		v = glm::normalize(glm::cross(n, cu));
	}
	u = glm::cross(n, v);


	glm::vec2 t1 = glm::vec2(glm::dot(p1, u), glm::dot(p1, v));
	glm::vec2 t2 = glm::vec2(glm::dot(p2, u), glm::dot(p2, v));
	glm::vec2 t3 = glm::vec2(glm::dot(p3, u), glm::dot(p3, v));

	addTriangle(p1, p2, p3, t1, t2, t3);
}

void MeshRenderable::addTriangle(
	glm::vec3 const &p1, glm::vec3 const &p2, glm::vec3 const &p3,
	glm::vec2 const &t1, glm::vec2 const &t2, glm::vec2 const &t3)
{
	if (currentBatch == NULL) {
		// start batch
		beginBatch(NULL, NULL);
	}

	currentBatch->positions.push_back(p1);
	currentBatch->positions.push_back(p2);
	currentBatch->positions.push_back(p3);

	// TODO texcoords
	currentBatch->texcoords.push_back(t1);
	currentBatch->texcoords.push_back(t2);
	currentBatch->texcoords.push_back(t3);

	// normals
	glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	currentBatch->normals.push_back(n);
	currentBatch->normals.push_back(n);
	currentBatch->normals.push_back(n);
}

void MeshRenderable::render(RenderData const &data)
{
	if (!renderInited) {
		initRender();
	}

	//glm::mat4 mvp = data.projMatrix * data.viewMatrix;

	prog->use();
	setShaderUniforms(prog, data);
	prog->uniform1i("diffuseMap", 0);
	prog->uniform1i("bumpMap", 1);
	prog->uniformMatrix4fv("modelMatrix", glm::mat4(1.f));
	
	// render all batches
	for (std::map<VertexBatchIndex, VertexBatch*>::iterator it = batches.begin();
		it != batches.end(); ++it)
	{
		VertexBatch *batch = it->second;
		VertexBatchIndex const &textures = it->first;
		// bind textures
		if (textures.diffuseMap) {
			textures.diffuseMap->setActive(0);
			prog->uniform1i("hasDiffuseMap", 1);
		}
		else {
			prog->uniform1i("hasDiffuseMap", 0);
		}
		if (textures.bumpMap) {
			textures.bumpMap->setActive(1);
			prog->uniform1i("hasBumpMap", 1);
		}
		else {
			prog->uniform1i("hasBumpMap", 0);
		}
		prog->uniform4f("color", batch->color);
		batch->mesh.draw();
	}
}

void MeshRenderable::initRender()
{
	// load shaders
	prog = ShaderManager::Instance().loadProgram("shaders/batch.vert.glsl", "shaders/batch.frag.glsl");
	
	// create mesh for each batch
	for (std::map<VertexBatchIndex, VertexBatch*>::iterator it = batches.begin();
		it != batches.end(); ++it)
	{
		VertexBatch *batch = it->second;
		batch->mesh.setNumVertices(batch->positions.size());
		batch->mesh.setPosition(glm::value_ptr(batch->positions[0]));
		batch->mesh.setNormals(glm::value_ptr(batch->normals[0]));
		batch->mesh.setTexCoord(glm::value_ptr(batch->texcoords[0]));
	}

	renderInited = true;
}

void MeshRenderable::exportToOBJ(std::ostream &os)
{
	int totalNumVertices = 0;
	// for each batch, output vertices
	for (std::map<VertexBatchIndex, VertexBatch*>::iterator it = batches.begin();
		it != batches.end(); ++it)
	{
		VertexBatch *batch = it->second;
		for (std::vector<glm::vec3>::const_iterator vit = batch->positions.begin(); vit != batch->positions.end(); ++vit) {
			os << "v " << vit->x << ' ' << vit->y << ' ' << vit->z << '\n';
			++totalNumVertices;
		}
	}

	// for each batch, output normals
	for (std::map<VertexBatchIndex, VertexBatch*>::iterator it = batches.begin();
		it != batches.end(); ++it)
	{
		VertexBatch *batch = it->second;
		for (std::vector<glm::vec3>::const_iterator vit = batch->normals.begin(); vit != batch->normals.end(); ++vit) {
			os << "vn " << vit->x << ' ' << vit->y << ' ' << vit->z << '\n';
		}
	}

	// output faces
	for (int i = 1; i <= totalNumVertices; i += 3)
	{
		os << "f " << i << "//" << i << ' '; 
		os << i+1 << "//" << i+1 << ' '; 
		os << i+2 << "//" << i+2 << '\n'; 
	}
}