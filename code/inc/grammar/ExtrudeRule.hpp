#ifndef _EXTRUDERULE_HPP_
#define _EXTRUDERULE_HPP_

#include "Rule.hpp"
#include <Polygon2D.hpp>
#include <Rectangle2D.hpp>

// Extrusion d'une forme 2D (soit Rectangle2D soit Polygon2D)
class ExtrudeRule : public Rule {
    public:
		ExtrudeRule (int index, CSymbol *pred, float height = 10.0f) : Rule(index, pred), height(height)
		{};

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:

		void extrudePrism(Grammar &grammar, Context &context, Polygon2D const &poly2D);
		void extrudeFace(Grammar &grammar, Context &context, Rectangle2D const &rect);

        float height;
};

#endif

