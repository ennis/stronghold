#ifndef _CONSTRAINTEVEN_HPP_
#define _CONSTRAINTEVEN_HPP_

#include "Constraint.hpp"

class ConstraintEven : public Constraint {
    public:
        ConstraintEven () : Constraint() {};

        bool verify(Shape const& shape, Context &ct);
};

#endif

