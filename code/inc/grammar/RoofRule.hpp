#ifndef _ROOFRULE_HPP_
#define _ROOFRULE_HPP_

#include "Rule.hpp"

// Construction de toits (à base cylindrique ou rectangulaire)
class RoofRule : public Rule {
    public:
        RoofRule (int index, CSymbol *pred, float height = 10.0f) : Rule(index, pred), height(height) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        float height;
};

#endif

