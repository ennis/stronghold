#include "SelectEdgesRule.hpp"
#include "Grammar.hpp"
#include "Cuboid.hpp"
#include "Rectangle2D.hpp"
#include <stdexcept>

void SelectEdgesRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Cuboid) {
        // La règle ne s'applique que sur un cuboide
        throw std::runtime_error("select_edges can only be used on Cuboid primitives");
    }

    Cuboid& cube = static_cast<Cuboid&>(shape);

    CoordSystem cs = cube.getCoordSystem();
    glm::vec3 dim = cube.dimensions;

    glm::vec3 vdy = glm::vec3(0, dim.y, 0);
    glm::vec3 vx = glm::vec3(1, 0, 0);
    glm::vec3 vz = glm::vec3(0, 0, 1);
    CoordSystem newCs;

    float widthLeft = width, heightLeft = dim.z;
    newCs = CoordSystem::fromVectors(cs, vdy, vx, vz);
    Rectangle2D left(newCs, widthLeft, heightLeft);

    float widthRight = width, heightRight = dim.z;
    newCs = CoordSystem::fromVectors(cs, glm::vec3(dim.x - width, dim.y, 0), vx, vz);
    Rectangle2D right(newCs, widthRight, heightRight);

    float widthTop = dim.x, heightTop = width;
    newCs = CoordSystem::fromVectors(cs, glm::vec3(0, dim.y, dim.z - width), vx, vz);
    Rectangle2D top(newCs, widthTop, heightTop);

    float widthBottom = dim.x, heightBottom = width;
    newCs = CoordSystem::fromVectors(cs, vdy, vx, vz);
    Rectangle2D bottom(newCs, widthBottom, heightBottom);

    // Successeurs
    if (successors.size() != 5) {
        throw std::runtime_error("select_edges operation must have only five successors");
    }

    std::list<CSymbol*>::const_iterator itSucc = successors.begin();

    CSymbol *succ = *itSucc++;
    grammar.resolve(succ, context, cube);

    succ = *itSucc++;
    grammar.resolve(succ, context, left);

    succ = *itSucc++;
    grammar.resolve(succ, context, right);

    succ = *itSucc++;
    grammar.resolve(succ, context, top);

    succ = *itSucc++;
    grammar.resolve(succ, context, bottom);
}

