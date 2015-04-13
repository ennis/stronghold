#include <Terrain.hpp>
#include <TextureManager.hpp>
#include <ShaderManager.hpp>
#include <MeshRenderable.hpp> // temporaire ?
#include <glm/gtc/random.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <stdint.h>
#include "Grammar.hpp"


Terrain::Terrain() :
program(NULL),
width(0),
height(0)
{
}

Terrain::~Terrain()
{
    //grad.free();
}

void Terrain::init(Image &heightmap_, const char *aoPath)
{
	loadHeightmap(heightmap_, aoPath);
	computeGradientLaplacian();

	// Application de filtres sur la heigtmap:
	//    Filtres passe-bas bidimensionnels (de Gauss).
	//    Empiriquement, on utilise d'abord un filtre avec un masque 3x3
	//    puis un autre filtre avec un masque 5x5.
	smoothed_heightmap = smoothing3x3(*heightmap);
	smoothed_heightmap = smoothing5x5(smoothed_heightmap);
	computeSmoothGradientLaplacian();

	loadShaders();
	generateGrid();
}

glm::vec2 Terrain::getSize() const 
{
	return glm::vec2(width*64.f, height*64.f);
}


std::vector<glm::vec2> Terrain::getReachableNeighbors(int x, int z, int step) const{
	int width = heightmap->getWidth();
	int height = heightmap->getHeight();

	std::vector<glm::vec2> list;

	if (x > step-1){
		if (z > step-1){
			list.push_back(glm::vec2(x - step, z - step));
		}
		list.push_back(glm::vec2(x - step, z));
		if (z < height - step){
			list.push_back(glm::vec2(x - step, z + step));
		}
	}
	if (z < height - step){
		list.push_back(glm::vec2(x, z + step));
	}
	if (x < width - step){
		if (z < height - step){
			list.push_back(glm::vec2(x + step, z + step));
		}
		list.push_back(glm::vec2(x + step, z));
		if (z > step-1){
			list.push_back(glm::vec2(x + step, z - step));
		}
	}
	if (z > step-1){
		list.push_back(glm::vec2(x, z - step));
	}

	return list;
}

float Terrain::getHeight(float x, float z) const {
	// scaling hardcodé
    if (x > getSize().x) {
		x = getSize().x - 1;
    } else if (x < 0) {
		x = 0;
    }

    if (z > getSize().y) {
		z = getSize().y - 1;
    } else if (z < 0) {
		z = 0;
    }

	return 200.f * heightmap->sample(x, z, 0);
}

float Terrain::getHeight(glm::vec2 const &v) const{
	return getHeight(v.x, v.y);
}

glm::vec2 Terrain::getElemGrad(int i, int j) const{
	return grad[i*heightmap->getWidth() + j];
}
float Terrain::getElemLaplacian(int i, int j) const{
	return laplacian[i*heightmap->getWidth() + j];
}

void Terrain::computeGradientLaplacian(){
	int width = heightmap->getWidth();
	int height = heightmap->getHeight();

	// Initialisation
	grad.resize(width*height);
	laplacian.resize(width*height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			grad[i*width + j] = glm::vec2(0, 0);
			laplacian[i*width + j] = 0.0;
		}
	}

	//Calcul du gradiant
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			grad[i*width + j] =
				glm::vec2((getHeight(i + 1, j) - getHeight(i - 1, j)) / 2.0,
				(getHeight(i, j + 1) - getHeight(i, j - 1)) / 2.0);
		}
	}
	//Calcul du laplacien
	for (int i = 2; i < width - 2; i++) {
		for (int j = 2; j < height - 2; j++) {
			//Schéma de calcul du laplacien discret
			float d2x = getHeight(i + 1, j) + getHeight(i - 1, j) - 2.0 * getHeight(i, j);
			float d2y = getHeight(i, j + 1) + getHeight(i, j - 1) - 2.0 * getHeight(i, j);
			laplacian[i*width + j] = d2x + d2y;
		}
	}
}

//***************************
glm::vec2 Terrain::getElemSmoothGrad(int i, int j) const{
	return smoothed_grad[i*heightmap->getWidth() + j];
}
glm::vec2 Terrain::getSmoothGradient(float x, float z) const{
	int ix = int(x);
	int iz = int(z);

	if (ix + 1 >= heightmap->getWidth()) ix = heightmap->getWidth() - 2;
	if (iz + 1 >= heightmap->getHeight()) iz = heightmap->getHeight() - 2;

	float v1x = getElemGrad(ix, iz).x;
	float v2x = getElemGrad(ix + 1, iz).x;
	float v3x = getElemGrad(ix, iz + 1).x;
	float v4x = getElemGrad(ix + 1, iz + 1).x;

	float v1z = getElemGrad(ix, iz).y;
	float v2z = getElemGrad(ix + 1, iz).y;
	float v3z = getElemGrad(ix, iz + 1).y;
	float v4z = getElemGrad(ix + 1, iz + 1).y;

	// interpolation bilinéaire
	float gradx, gradz;
	gradx= (ix + 1 - x) * (iz + 1 - z) * v1x
		+ (x - ix) * (iz + 1 - z) * v2x
		+ (ix + 1 - x) * (z - iz) * v3x
		+ (x - ix) * (z - iz) * v4x;

	gradz= (ix + 1 - x) * (iz + 1 - z) * v1z
		+ (x - ix) * (iz + 1 - z) * v2z
		+ (ix + 1 - x) * (z - iz) * v3z
		+ (x - ix) * (z - iz) * v4z;

	return glm::vec2(gradx, gradz);
}
glm::vec2 Terrain::getSmoothGradient(glm::vec2 const &v) const{
	return getSmoothGradient(v.x, v.y);
}
float Terrain::getElemSmoothLaplacian(int i, int j) const{
	return smoothed_laplacian[i*heightmap->getWidth() + j];
}
float Terrain::getSmoothLaplacian(float x, float z) const{
	int ix = int(x);
	int iz = int(z);

	if (ix + 1 >= heightmap->getWidth()) ix = heightmap->getWidth() - 2;
	if (iz + 1 >= heightmap->getHeight()) iz = heightmap->getHeight() - 2;

	float v1 = getElemSmoothLaplacian(ix, iz);
	float v2 = getElemSmoothLaplacian(ix + 1, iz);
	float v3 = getElemSmoothLaplacian(ix, iz + 1);
	float v4 = getElemSmoothLaplacian(ix + 1, iz + 1);

	// interpolation bilinéaire
	return (ix + 1 - x) * (iz + 1 - z) * v1
		+ (x - ix) * (iz + 1 - z) * v2
		+ (ix + 1 - x) * (z - iz) * v3
		+ (x - ix) * (z - iz) * v4;
}
float Terrain::getSmoothLaplacian(glm::vec2 const &v) const{
	return getSmoothLaplacian(v.x, v.y);
}

float Terrain::getSmoothHeight(float x, float z) const {
	// scaling hardcodé
	return 200.f * smoothed_heightmap.sample(x, z, 0);
}
float Terrain::getSmoothHeight(glm::vec2 const &v) const{
	return getSmoothHeight(v.x, v.y);
}

void Terrain::computeSmoothGradientLaplacian(){
	int width = heightmap->getWidth();
	int height = heightmap->getHeight();

	// Initialisation
	smoothed_laplacian.resize(width*height);
	smoothed_grad.resize(width*height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			smoothed_grad[i*width + j] = glm::vec2(0, 0);
			smoothed_laplacian[i*width + j] = 0.0;
		}
	}

	//Calcul du gradiant
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			smoothed_grad[i*width + j] =
				glm::vec2((getSmoothHeight(i + 1, j) - getSmoothHeight(i - 1, j)) / 2.0,
				(getSmoothHeight(i, j + 1) - getSmoothHeight(i, j - 1)) / 2.0);
		}
	}

	//Calcul du laplacien
	for (int i = 2; i < width - 2; i++) {
		for (int j = 2; j < height - 2; j++) {
			//Schéma de calcul du laplacien discret
			float d2x = getSmoothHeight(i + 1, j) + getSmoothHeight(i - 1, j) - 2.0f * getSmoothHeight(i, j);
			float d2y = getSmoothHeight(i, j + 1) + getSmoothHeight(i, j - 1) - 2.0f * getSmoothHeight(i, j);
			smoothed_laplacian[i*width + j] = d2x + d2y;
		}
	}
}

//**************************

glm::vec2 Terrain::getGradient(float x, float z) const{
	return glm::vec2(getGradientX(x, z), getGradientZ(x, z));
}
glm::vec2 Terrain::getGradient(glm::vec2 const &v) const{
	return getGradient(v.x, v.y);
}
float Terrain::getGradientX(float x, float z) const{
	int ix = int(x);
	int iz = int(z);

	if (ix + 1 >= heightmap->getWidth()) ix = heightmap->getWidth() - 2;
	if (iz + 1 >= heightmap->getHeight()) iz = heightmap->getHeight() - 2;

	float v1 = getElemGrad(ix, iz).x;
	float v2 = getElemGrad(ix + 1, iz).x;
	float v3 = getElemGrad(ix, iz + 1).x;
	float v4 = getElemGrad(ix + 1, iz + 1).x;

	// interpolation bilinéaire
	return (ix + 1 - x) * (iz + 1 - z) * v1
		+ (x - ix) * (iz + 1 - z) * v2
		+ (ix + 1 - x) * (z - iz) * v3
		+ (x - ix) * (z - iz) * v4;

}
float Terrain::getGradientX(glm::vec2 const &v) const{
	return getGradientX(v.x, v.y);
}
float Terrain::getGradientZ(float x, float z) const{
	int ix = int(x);
	int iz = int(z);

	if (ix + 1 >= heightmap->getWidth()) ix = heightmap->getWidth() - 2;
	if (iz + 1 >= heightmap->getHeight()) iz = heightmap->getHeight() - 2;

	float v1 = getElemGrad(ix, iz).y;
	float v2 = getElemGrad(ix + 1, iz).y;
	float v3 = getElemGrad(ix, iz + 1).y;
	float v4 = getElemGrad(ix + 1, iz + 1).y;

	// interpolation bilinéaire
	return (ix + 1 - x) * (iz + 1 - z) * v1
		+ (x - ix) * (iz + 1 - z) * v2
		+ (ix + 1 - x) * (z - iz) * v3
		+ (x - ix) * (z - iz) * v4;
}
float Terrain::getGradientZ(glm::vec2 const &v) const{
	return getGradientZ(v.x, v.y);
}

float Terrain::getLaplacian(float x, float z) const{
	int ix = int(x);
	int iz = int(z);

	if (ix + 1 >= heightmap->getWidth()) ix = heightmap->getWidth() - 2;
	if (iz + 1 >= heightmap->getHeight()) iz = heightmap->getHeight() - 2;

	float v1 = getElemLaplacian(ix, iz);
	float v2 = getElemLaplacian(ix + 1, iz);
	float v3 = getElemLaplacian(ix, iz + 1);
	float v4 = getElemLaplacian(ix + 1, iz + 1);
	
	// interpolation bilinéaire
	return (ix + 1 - x) * (iz + 1 - z) * v1
		+ (x - ix) * (iz + 1 - z) * v2
		+ (ix + 1 - x) * (z - iz) * v3
		+ (x - ix) * (z - iz) * v4;
}
float Terrain::getLaplacian(glm::vec2 const &v) const{
	return getLaplacian(v.x, v.y);
}

void Terrain::loadShaders()
{
	program = ShaderManager::Instance().loadProgram("shaders/terrain.vert.glsl", "shaders/terrain.frag.glsl");

	program->bindFragDataLocation("outFragColor");

	// load textures
	rockTex = TextureManager::Instance().loadTexture("textures/rock2.png", true);
	grassTex = TextureManager::Instance().loadTexture("textures/grass.png", true);
	rockTexBump = TextureManager::Instance().loadTexture("textures/rock2normal.png", true);
	grassTexBump = TextureManager::Instance().loadTexture("textures/grass.png", true);
}

void Terrain::loadHeightmap(Image &heightmap_, const char *aoPath)
{
	heightmap = &heightmap_;

	if (aoPath) {
		enableAmbientOcclusion = true;
		Image ao;
		ao.load(aoPath);
		textureAO.load(ao);
		textureAO.setClamping(GL_CLAMP_TO_EDGE);
		textureAO.setFiltering(GL_LINEAR);
	}
	else {
		enableAmbientOcclusion = false;
	}

	// number of patches (tiles)
	width = heightmap->getWidth() / 64;
	height = heightmap->getHeight() / 64;

	textureHeightMap.load(*heightmap);
	textureHeightMap.setClamping(GL_CLAMP_TO_EDGE);
	textureHeightMap.setFiltering(GL_LINEAR);
}

void Terrain::generateGrid()
{
	// maxi: 64x64 grid (65x65 vertices)
	// ...
	// mini: 1x1 grid (4 vertices)
	glGenBuffers(TerrainLODLevels, lodVertexBuffers);
	glGenBuffers(TerrainLODLevels, lodIndexBuffers);
	glGenVertexArrays(TerrainLODLevels, lodVertexArrays);

	const int maxgridsize = (1 << TerrainLODLevels) + 1;
	std::vector<glm::vec2> vertices;
	std::vector<uint16_t> indices;
	vertices.reserve(maxgridsize*maxgridsize);
	indices.reserve((maxgridsize - 1)*(maxgridsize - 1) * 6);

	for (int k = 0; k < TerrainLODLevels; ++k) {
		const int gridsize = (1 << k) + 1;
		const float step = 1.f / (gridsize - 1);
		const int numVertices = gridsize * gridsize;
		const int numIndices = (gridsize - 1) * (gridsize - 1) * 6;

		vertices.clear();
		indices.clear();
		vertices.resize(numVertices);

		for (int i = 0; i < gridsize; ++i) {
			for (int j = 0; j < gridsize; ++j) {
				vertices[i * gridsize + j].y = i * step;
				vertices[i * gridsize + j].x = j * step;

				if ((i < gridsize - 1) && (j < gridsize - 1)) {
					indices.push_back(i * gridsize + j + 1);
					indices.push_back(i * gridsize + j);
					indices.push_back((i + 1) * gridsize + j);
					indices.push_back(i * gridsize + j + 1);
					indices.push_back((i + 1) * gridsize + j);
					indices.push_back((i + 1) * gridsize + j + 1);
				}
			}
		}

		lodNumIndices[k] = numIndices;

		glBindVertexArray(lodVertexArrays[k]);

		glEnableVertexAttribArray(0);	// 0: position
		glBindBuffer(GL_ARRAY_BUFFER, lodVertexBuffers[k]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lodIndexBuffers[k]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

//
// One vertex buffer (65x65 grid)
// Multiple index buffers
// Heightmap as texture (+ noise?)
//
// to vertex shader: x,y offset
//

void Terrain::renderPatch(int x, int y)
{
	// use max LOD

	// position = (x,y)/numTiles*scale
	// texcoord = (x,y)/numTiles
	glm::vec2 texcoord = glm::vec2(x, y) / glm::vec2(width, height);
	//glm::vec2 position = texcoord * scale;

	program->uniform2f("offset", glm::vec2(x, y));
	program->uniform2f("size", glm::vec2(heightmap->getWidth(), heightmap->getHeight()));

	int lod = TerrainLODLevels - 1;

	glBindVertexArray(lodVertexArrays[lod]);
	glDrawElements(GL_TRIANGLES, lodNumIndices[lod], GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Terrain::render(RenderData const &data)
{
	program->use();
	glm::mat4 modelMatrix = transform.toMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	setShaderUniforms(program, data);
	program->uniformMatrix4fv("modelMatrix", modelMatrix);
	textureHeightMap.setActive(0);
	program->uniform1i("heightmap", 0);
	grassTex->setActive(1);
	program->uniform1i("texFlat", 1);
	rockTex->setActive(2);
	program->uniform1i("texSlope", 2);
	if (enableAmbientOcclusion && data.enableAmbientOcclusion) {
		textureAO.setActive(3);
		program->uniform1i("ambientOcclusion", 3);
		program->uniform1i("enableAmbientOcclusion", 1);
	}
	else {
		program->uniform1i("enableAmbientOcclusion", 0);
	}

	// bump mapping
	grassTexBump->setActive(4);
	program->uniform1i("texFlatBump", 4);
	rockTexBump->setActive(5);
	program->uniform1i("texSlopeBump", 5);

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			// render tile with LOD 0
			renderPatch(x, y);
		}
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


// Fonctions 'utilitaires'
float flatnessScoreWall(glm::vec2 const& p, float r, Context &context) 
{
	// Nombre de 'côtés' du disque
	const int samples = 40;
	const float radius = 4.f;

	float acc_r = 0.f;
	float h = context.terrain->getHeight(p);


	// On regarde maintenant sur le disque et on mesure la platitude
	// = différence de pente entre centre et le point courant
	float score = 0.f;
	int n = 10;

	for (int i = 0; i < samples; i++) {
		glm::vec2 pt = p + glm::diskRand(radius);
		acc_r += glm::distance(pt, p);
		float hh = context.terrain->getHeight(pt);
		score += abs(h - hh);
	}

	score /= acc_r;
	//std::cout << score << std::endl; // TRACE

	return score;
}


