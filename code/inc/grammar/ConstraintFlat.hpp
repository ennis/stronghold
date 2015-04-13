#ifndef _CONSTRAINTFLAT_HPP_
#define _CONSTRAINTFLAT_HPP_

#include "Constraint.hpp"

class ConstraintFlat : public Constraint {
    private:
        static double threshold;
        static double searchArea;
        static double step;
        double score;

    public:
        ConstraintFlat() : Constraint(), score(0) {};

        bool verify(Shape const& shape, Context &ct);
};

#endif

