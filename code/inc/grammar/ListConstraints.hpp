#ifndef _LISTCONSTRAINTS_HPP_
#define _LISTCONSTRAINTS_HPP_

#include <list>
#include "Constraint.hpp"

class ListConstraints {
    public:
        ListConstraints () {};

        void addConstraint (Constraint *c);
        bool verifyConstraints (Shape const& shape, Context &ct);

    private:
        std::list <Constraint *> constraints;
};

#endif

