#include "RectangleRule.hpp"
#include "Prism.hpp"
#include "Cuboid.hpp"
#include "Grammar.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

using namespace std;

RectangleRule::RectangleRule(int index, CSymbol *pred, float width, float height, float length) : Rule(index, pred), width(width), height(height), length(length) {
}

void RectangleRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Prism) {
        // La règle ne s'applique que sur un prisme
        throw std::runtime_error("build_rectangle can only be used on Prism primitives");
    }
    Prism& prism = static_cast<Prism&>(shape);
    std::vector<glm::vec2> const &points = prism.poly.getPoints();
    const int n = points.size();

	CoordSystem cs = prism.getCoordSystem();
	
	glm::vec2 point;

	glm::vec2 bla = glm::vec2(cs.position.x , cs.position.z);

	int j = 0;

	//point à l'intersection de deux murs (point du polygone de base)
	for (int i = 0; i < n; i++) {
		point = points[i];
		if (point == bla) {
			j = i;
			break;
		}
	}

	//autres points du prisme
	glm::vec2 point1 = points[(j + 1) % n];
	glm::vec2 point2 = points[(j + 2) % n];

	//coefficient directeur de la droite
	double a = (point1.x - point2.x) / (point1.y - point2.y);

	//angle pour le changement de repère (entre la droite de coeff a et l'axe x du repère du monde)
	float angle;

	if (a >= 0)
		angle = atan(a) + M_PI;
	else
		angle = atan(a);

	//décalage du point d'originie du cuboid
	cs.position.x -= (length / 2)*sin(angle);
	cs.position.z -= (length / 2)*cos(angle);

	cs.position.x -= (width / 2) * cos(angle);
	cs.position.z += (width / 2) * sin(angle);

    glm::vec3 dimensions(width, height, length);
	glm::vec3 axis = glm::vec3(0, 1, 0);

	//création du cuboid : l'axe x de l'origine du repère local passe par le point du polygone de base et est parallèle à la droite formée par les 2 autres points
	Cuboid cube(cs.getFinalPosition(), glm::angleAxis((float)angle,axis), dimensions);

    // Successeurs
    if (successors.size() != 1) {
        throw std::runtime_error("build_rectangle operation must have only one successor");
    }

    CSymbol *succ = *successors.begin();
    grammar.resolve(succ, context, cube);
}

