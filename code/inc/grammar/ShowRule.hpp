#ifndef SHOWRULE_HPP
#define SHOWRULE_HPP

#include "Rule.hpp"
#include <string>
#include "Texture.hpp"

class Polygon2D;
class Trapezoid2D;
class Rampart;
class Cuboid;
class Rectangle2D;
class Tetrahedron;
class Cone;
class Prism;
class Cylinder;
class Cone;

void createPolygonRenderable(Polygon2D const &poly2D, Context &context);
void createTrapezoidRenderable(Trapezoid2D const &trapezoid, Context &context);
void createRampartRenderable(Rampart const &trapezoid, Context &context);
void createCuboidRenderable(Cuboid const &cuboid, Context &context, Texture *tex = NULL);
void createRectangleRenderable(Rectangle2D const &rect, Context &context);
void createTetrahedronRenderable(Tetrahedron const &tetra, Context &context);
glm::vec4 getRandomColor();
void createPrismRenderable(Prism const &prism, Context &context);
void createCylinderRenderable(Cylinder const &cylindre, Context &context);
void createConeRenderable(Cone const &cone, Context &context);

// Règle de débogage:
// Affiche le volume sur lequel la règle est exécutée
class ShowRule : public Rule
{
public:
	ShowRule(int i, CSymbol *pred) : Rule(i, pred), texture(NULL)
	{}

	void execute(Grammar &grammar, Context &context, Shape &shape);

	void setTexture(std::string const &tex);

private:
	Texture *texture;
};

#endif