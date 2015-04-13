#ifndef _CONSTRAINTTOWER_HPP_
#define _CONSTRAINTTOWER_HPP_

#include "Constraint.hpp"

class ConstraintTower : public Constraint {
    public:
        ConstraintTower () : Constraint () {};

        bool verify(Shape const& shape, Context &ct);
    private:
};

#endif

