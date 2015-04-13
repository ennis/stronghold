#ifndef _RECTANGLERULE_HPP_
#define _RECTANGLERULE_HPP_

#include <Rule.hpp>
#include <Shape.hpp>

// Règle permettant de construire des tours en formes de pavés sur les coins
class RectangleRule : public Rule {
    public:
		RectangleRule(int index, CSymbol *pred, float width = 10.0f, float height = 20.0f, float length = 30.0f);

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        float width;
        float height;
		float length;
};

#endif

