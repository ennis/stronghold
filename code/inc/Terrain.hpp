#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <OpenGL.hpp>
#include <Transform.hpp>
#include <Image.hpp>
#include <Shader.hpp>
#include <Program.hpp>
#include <Texture.hpp>
#include <RenderData.hpp>
#include <glm/glm.hpp>
#include <vector>

struct Context;


class Terrain
{
public:
	Terrain();
	~Terrain();

	void init(Image &heightmap_, const char *aoPath = NULL);
	void render(RenderData const &data);

	glm::vec2 getSize() const;


	// Renvoie la liste des voisin accessible à la position entiere (i,j)
	// sans depasser des bord de la carte
	std::vector<glm::vec2> getReachableNeighbors(int i, int j, int step = 1) const;

	/*
	* Acceder à la hauteur du terrain au point (x,z) / au point v
	*/
	float getHeight(float x, float z) const;
	float getHeight(glm::vec2 const &v) const;


	/*
	* Acceder au gradient de la hauteur du terrain au point (x,z) / au point v
	* Acceder au laplacien de la hauteur du terrain au point (x,z) / au point v
	*/
	glm::vec2 getElemGrad(int i, int j) const; // acces simple au tableau
	glm::vec2 getGradient(float x, float z) const;
	glm::vec2 getGradient(glm::vec2 const &v) const;
	float getGradientX(float x, float z) const;
	float getGradientX(glm::vec2 const &v) const;
	float getGradientZ(float x, float z) const;
	float getGradientZ(glm::vec2 const &v) const;
	float getElemLaplacian(int i, int j) const; // acces simple au tableau
	float getLaplacian(float x, float z) const;
	float getLaplacian(glm::vec2 const &v) const;

	/*
	* Accesseurs pour les hauteur gradient et laplaciens de la carte à laquelle on a appliquer un filtre
	*/
	float getSmoothHeight(float x, float z) const;
	float getSmoothHeight(glm::vec2 const &v) const;
	glm::vec2 getElemSmoothGrad(int i, int j) const; // acces simple au tableau
	glm::vec2 getSmoothGradient(float x, float z) const;
	glm::vec2 getSmoothGradient(glm::vec2 const &v) const;
	float getElemSmoothLaplacian(int i, int j) const; // acces simple au tableau
	float getSmoothLaplacian(float x, float z) const;
	float getSmoothLaplacian(glm::vec2 const &v) const;

private:
	static const int TerrainLODLevels = 6;

	void loadHeightmap(Image &heightmap_, const char *aoPath);
	void loadShaders();
	void generateGrid();
	void renderPatch(int x, int y);

	Transform transform;
	Image *heightmap;
	Program *program;

	Texture textureHeightMap;
	bool enableAmbientOcclusion;
	Texture textureAO;

	// calcule et stocke les gradiant et laplacien de la hauteur du terrain
	void computeGradientLaplacian();
	std::vector<glm::vec2> grad;
	std::vector<float> laplacian;

	// Le code ici est à factoriser: 2 facon de le faire
	// - mettre les fonctions dans image et templetre image
	// - changer le proto des fcts et faire les modifs dans Terrain
	Image smoothed_heightmap;
	void computeSmoothGradientLaplacian();
	std::vector<float> smoothed_laplacian;
	std::vector<glm::vec2> smoothed_grad;

	Texture *rockTex;
	Texture *grassTex;
	Texture *rockTexBump;
	Texture *grassTexBump;

	// Attention à ne pas confondre avec la taille de l'image
	int width;	// in tiles
	int height;	// in tiles

	GLuint lodVertexBuffers[TerrainLODLevels];
	GLuint lodIndexBuffers[TerrainLODLevels];
	GLuint lodVertexArrays[TerrainLODLevels];
	int lodNumIndices[TerrainLODLevels];

};

// Fonctions 'utilitaires'
// Détermine la 'platitude' du terrain dans un disque de rayon 'r' autour du point 'p'
float flatnessScoreWall (glm::vec2 const& p, float r, Context &context);


#endif
