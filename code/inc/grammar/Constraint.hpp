#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_

#include "Shape.hpp"

struct Context;

class Constraint {
    public:
        Constraint () {};

        virtual bool verify(Shape const& shape, Context &ct) = 0;
};

#endif

