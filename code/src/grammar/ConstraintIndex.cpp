#include "ConstraintIndex.hpp"

bool ConstraintIndex::verify(Shape const& shape, Context &ct) {
    if (no)
        return shape.index != index;

    return shape.index == index;
}

