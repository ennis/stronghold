#ifndef _CONSTRAINTODD_HPP_
#define _CONSTRAINTODD_HPP_

#include "Constraint.hpp"

class ConstraintOdd : public Constraint {
    public:
        ConstraintOdd () : Constraint() {};

        bool verify(Shape const& shape, Context &ct);
};

#endif

