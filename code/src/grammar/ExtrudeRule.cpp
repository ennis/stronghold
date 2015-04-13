#include "ExtrudeRule.hpp"
#include "Grammar.hpp"
#include "Polygon2D.hpp"
#include "Rectangle2D.hpp"
#include "Cuboid.hpp"
#include "Prism.hpp"

#include "ShowRule.hpp"

#include <stdexcept>

void ExtrudeRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    switch (shape.getShapeType()) {
        case ST_Polygon:
			extrudePrism(grammar, context, static_cast<Polygon2D const&>(shape));
            break;

		case ST_Rectangle:
			extrudeFace(grammar, context, static_cast<Rectangle2D const&>(shape));
			break;

        default:
            throw std::runtime_error("extrude can only be used on polygons or rectangles");
            break;
    }
}

void ExtrudeRule::extrudePrism(Grammar &grammar, Context &context, Polygon2D const &poly2D)
{
	Prism p(poly2D, height, poly2D.getCoordSystem().position);

	// get successor
	CSymbol * succ = *(successors.begin());

	grammar.resolve(succ, context, p);
}

void ExtrudeRule::extrudeFace(Grammar &grammar, Context &context, Rectangle2D const &rect)
{
	// extrude along Y axis
	float w = rect.width;
	float h = rect.height;

	// also output the original face
	createRectangleRenderable(rect, context);

	Cuboid cuboid = Cuboid(rect.getCoordSystem(), glm::vec3(w, height, h));

	CSymbol * succ = *(successors.begin());
	grammar.resolve(succ, context, cuboid);
}
