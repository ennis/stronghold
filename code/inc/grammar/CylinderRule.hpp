#ifndef _CYLINDERRULE_HPP_
#define _CYLINDERRULE_HPP_

#include <stdexcept>
#include <Rule.hpp>

class Shape;

// Règle permettant de construire des tours cylindriques sur les coins
class CylinderRule : public Rule {
    public:
        CylinderRule (int index, CSymbol *pred, float radius_min = 10.0f, float radius_max = 20.0f, float height_min = 10.0f, float height_max = 20.0f);

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        float radius_min;
        float radius_max;

        float height_min;
        float height_max;
};

#endif

