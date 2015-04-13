#include "BuildDungeonRule.hpp"
#include "Polygon2D.hpp"
#include "Cylinder.hpp"
#include "Circle2D.hpp"
#include "Grammar.hpp"
#include <iostream>

// TODO : new shape = point ?
void BuildDungeonRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Polygon) {
        // La règle ne s'applique que sur un polygone
        throw std::runtime_error("build_dungeon can only be used on Polygon primitives");
    }

    Polygon2D const& poly = static_cast<Polygon2D const&>(shape);

    if (poly.size() != 1) {
        // La règle ne s'applique que sur un point
        throw std::runtime_error("build_dungeon can only be used on a point");
    }

    glm::vec2 pos = poly.getPoints()[0];
    glm::vec3 pos3d(pos.x, context.terrain->getHeight(pos), pos.y);

    Circle2D base(radius, pos3d);
    Cylinder dungeon(base, height);

    if (successors.size() != 1) {
        throw std::runtime_error("build_dungeon operation must have only one successor");
    }

    CSymbol *succ = *successors.begin();
    grammar.resolve(succ, context, dungeon);

    // TODO: splits, crenels
}

