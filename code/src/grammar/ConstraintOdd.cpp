#include "ConstraintOdd.hpp"
#include "Grammar.hpp"

bool ConstraintOdd::verify(Shape const& shape, Context &ct) {
    return shape.index % 2 == 1;
}

