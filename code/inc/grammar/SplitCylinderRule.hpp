#ifndef _SPLITCYLINDERRULE_HPP_
#define _SPLITCYLINDERRULE_HPP_

#include "Rule.hpp"
#include "Shape.hpp"

class SplitCylinderRule : public Rule {
    public:
        SplitCylinderRule (int index, CSymbol * pred, int numDivisions) : Rule(index, pred), numDivisions(numDivisions) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        int numDivisions;
};

#endif

