#ifndef _POLYGON_2D_
#define _POLYGON_2D_

#include <Rule.hpp>
#include <Polygon2D.hpp>
#include <string>
#include <list>
#include <glm/glm.hpp>

// Operation creant un polygone 2D
class Polygon2DRule : public Rule
{
public:
	Polygon2DRule();
	Polygon2DRule(int i, Symbol const *pred);

	std::string toString() const;

	//
	// Ajout d'un point
	void addPoint(glm::vec2 const &point);

	//
	// Exécution de la règle
	// (crée une instance de Polygon2D et appelle son successeur)
	void execute(Grammar &grammar, Context &context, Shape &shape);

private:
	Polygon2D points;
};

#endif
