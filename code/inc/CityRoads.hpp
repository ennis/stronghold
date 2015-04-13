#ifndef _CITYROADS_
#define _CITYROADS_

#include <Path.hpp>
#include <MeshRenderable.hpp>

struct Context;


struct FrequencyMatrix{
private:
	std::vector<int> mat;
	int width;
	int height;

public:
	FrequencyMatrix(int width, int height)
		: width(width), height(height)
	{mat.resize(width*height); }

	int & operator()(int i, int j){
		return mat[j*width + i];
	}
	int operator()(int i, int j) const{
		return mat[j*width + i];
	}

	int &operator()(glm::ivec2 v){
		return mat[v.y*width + v.x];
	}
	int operator()(glm::ivec2 v) const{
		return mat[v.y*width + v.x];
	}
};

class CityRoads : public Renderable
{
public:
	
	CityRoads() : renderInited(false)
	{}

	//render : TODO

	void create(Context &context);

	void render(RenderData const &data);
	void initRender(Context &context);


	glm::vec2 door_pos;
	PathTree *roads;
	Path * main_road; // included in roads
	MeshRenderable mesh;
	Texture *roadTexture;
	CaseMatrix *board;

private:
	int step; // discretisation de l'espace

	void createMainRoads(Context &context);
	void createRoadsToHouses(Context &context);

	void recursiveCreatePathTree(Context &context, PathTree *tree, glm::vec2 pos, glm::vec2 pos_prec);
	void createPathTreeFromFrequencies(Context &context);

	bool renderInited;

	// recursif
	void renderPath(Context &context, PathTree * tree);

	//void updateFrequencies(Path * path);
	PathTree * findBestRoadContainingPoint(glm::vec2 v);

};

#endif