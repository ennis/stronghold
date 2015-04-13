#include "CylinderRule.hpp"
#include "Prism.hpp"
#include "Cylinder.hpp"
#include "Circle2D.hpp"
#include "Random.hpp"
#include "Grammar.hpp"

CylinderRule::CylinderRule (int index, CSymbol *pred, float radius_min, float radius_max, float height_min, float height_max) : Rule(index, pred), radius_min(radius_min), radius_max(radius_max), height_min(height_min), height_max(height_max) {
}

void CylinderRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Prism) {
        // La règle ne s'applique que sur un prisme
        throw std::runtime_error("build_cylinder can only be used on Prism primitives");
    }

    Prism const& prism = static_cast<Prism const&>(shape);
    std::vector<glm::vec2> const &points = prism.poly.getPoints();
    const int n = points.size();

    // Find the centroid
    glm::vec2 centroid;
    for (int i = 0; i < n; i++) {
        centroid += points[i];
    }
    centroid /= n;
    glm::vec3 centroid3d(centroid.x, 0, centroid.y);
    centroid3d = centroid3d + glm::vec3(0, context.terrain->getHeight(centroid), 0);

    // Build a cylinder of center 'centroid' of radius 'radius' and height 'height'
    float radius = GRandom.RandomNumber(radius_min, radius_max);
	float height = GRandom.RandomNumber(height_min, height_max);

    Circle2D circle(radius, centroid3d);
    Cylinder cyl(circle, height);

    // Successeurs
    if (successors.size() != 1) {
        throw std::runtime_error("build_cylinder operation must have only one successor");
    }

    CSymbol *succ = *successors.begin();
    grammar.resolve(succ, context, cyl);
}
