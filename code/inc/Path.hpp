#ifndef _PATH_
#define _PATH_

#include <OpenGL.hpp>
#include <vector>
#include <list>
#include <FieldFunction.hpp>


class Path;
struct PathTree{
	PathTree()
	: path(NULL), parent(NULL)
	{
		children.clear();
	}

	Path *path;
	PathTree *parent;
	std::vector<PathTree *> children;
};


struct Case{
	float score_H;
	float score_G;
	glm::vec2 parent;
	int frequency;
	bool found; // pour la construction des chemins

	Case() :
		score_H(0.f), score_G(0.f), parent(glm::vec2(0, 0)), frequency(0), found(false)
	{}

	Case(float H, float G, glm::vec2 p)
		: score_H(H), score_G(G), parent(p), frequency(0), found(false)
	{}
	
	// Renvoie la somme du score H et du score G
	float getScoreF(){
		return score_G + score_H;
	}
};

struct CaseMatrix{
	std::vector<Case> mat;
	int width, height;

	CaseMatrix(int width, int height)
		: width(width), height(height)
	{
		mat.resize(width*height);
	}

	Case & operator()(int i, int j){
		return mat[j*width + i];
	}
	Case operator()(int i, int j) const{
		return mat[j*width + i];
	}

	Case &operator()(glm::ivec2 v){
		return mat[v.y*width + v.x];
	}
	Case operator()(glm::ivec2 v) const{
		return mat[v.y*width + v.x];
	}
};


struct Context;

class Path{
public:
	// Constructeur par default
	Path();

	// DEBUG: Affichage d'un ligne brisée
	void createPathRenderable(Context &context);

	std::vector<glm::vec2> points;
	
	float value; // 0.f par default

};

// Calcul du chemin de moindre cout pour aller de v1 à v2
// Resultat dans une liste de points  de coordonnées entieres constituant un chemin.
// Cette methode utilise l'algorithme A*
// L'heuristique et les fonctions de calcul des couts sont en parametre
Path *computeMinCostPath(CaseMatrix *board, int step, glm::vec2 start, glm::vec2 goal,
						Context &context, FieldFunction * heuristic_function, std::list<FieldFunction *> function_list);

// Trouver le chemin le plus ancien de tree qui contient le point v
PathTree *findBestRoadContainingPoint(PathTree* tree, glm::vec2 v);


// Renvoie V2, tel que:
// V2.x = V.X - (V.X % step)
// V2.y = V.y - (V.y % step)
glm::vec2 getCut(glm::vec2 v, int step = 1);

#endif