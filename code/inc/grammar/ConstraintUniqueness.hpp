#ifndef _CONSTRAINTUNIQUENESS_HPP_
#define _CONSTRAINTUNIQUENESS_HPP_

#include "Constraint.hpp"

class ConstraintUniqueness : public Constraint {
    public:
        ConstraintUniqueness () : Constraint() {};

        bool verify(Shape const& shape, Context &ct);
};

#endif

