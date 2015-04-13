#include "ConstraintTower.hpp"

bool ConstraintTower::verify(Shape const& shape, Context &ct) {
    return shape.tower;
}

