#include "ConstraintEven.hpp"
#include "Grammar.hpp"

bool ConstraintEven::verify(Shape const& shape, Context &ct) {
    return shape.index % 2 == 0;
}

