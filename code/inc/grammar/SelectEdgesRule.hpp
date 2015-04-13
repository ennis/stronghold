#ifndef _SELECTEDGESRULE_HPP_
#define _SELECTEDGESRULE_HPP_

#include "Rule.hpp"
#include "Shape.hpp"

class SelectEdgesRule : public Rule {
    public:
        SelectEdgesRule (int index, CSymbol *pred, float width = 10.0f) : Rule(index, pred), width(width) {
        }

        void execute(Grammar &grammar, Context &context, Shape &shape);
    private:
        float width;
};

#endif

