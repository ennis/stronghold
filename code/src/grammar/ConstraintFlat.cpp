#include "ConstraintFlat.hpp"

#include "Grammar.hpp"
#include "Cuboid.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

double ConstraintFlat::threshold = 50;
double ConstraintFlat::searchArea = 10;
double ConstraintFlat::step = 0.1;

bool ConstraintFlat::verify(Shape const& shape, Context &ct) {
    if (shape.getShapeType() != ST_Cuboid) {
        throw std::runtime_error("For a door constraint, the shape must be a cuboid!");
    }

    Cuboid const &cube = static_cast<Cuboid const&> (shape);
    score = 0;
    glm::vec2 ref = glm::vec2(cube.getCoordSystem().position.x, cube.getCoordSystem().position.z);
    ref = glm::normalize(ref);

    //C'est faux : remettre dans le repère du monde
    for (double i = 0; i < cube.dimensions.x; i += step) {
        for (double j = -searchArea; j < searchArea; j += step) {
            /* double diff = ct.terrain->getHeight(ref.x + cube.dimensions.y / 2, j + ref.y + cube.dimensions.x / 2) - ct.terrain->getHeight(ref.x + i, j + ref.y); */

            /* score += abs(diff); */
        }
    }

    std::cout << "SCORE" << std::endl;
    std::cout << score << std::endl;

    return (score < threshold);
}

