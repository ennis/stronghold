#ifndef _CONSTRAINTINDEX_HPP_
#define _CONSTRAINTINDEX_HPP_

#include "Constraint.hpp"

class ConstraintIndex : public Constraint {
    public :
        ConstraintIndex (int index, bool no = false) : Constraint(), index(index), no(no) {
        };

        bool verify(Shape const& shape, Context &ct);

    private:
        int index;
        bool no;
};

#endif

