#include "TranslationRule.hpp"
#include "Prism.hpp"
#include "Polygon2D.hpp"
#include "Grammar.hpp"
#include <stdexcept>
using namespace std;

TranslationRule::TranslationRule(int index, CSymbol *pred, float x, float y) : Rule(index, pred), x(x), y(y) {}

void TranslationRule::execute(Grammar &grammar, Context &context, Shape &shape) {
	if (shape.getShapeType() != ST_Polygon) {
		// La règle ne s'applique que sur un polygon
		throw std::runtime_error("translation can only be used on Polygon2D primitives");
	}
	Polygon2D const& poly = static_cast<Polygon2D const&>(shape);
	std::vector<glm::vec2> points = poly.getPoints();
	const int n = points.size();

	for (int i = 0; i < n; i++) {
		points[i].x += x;
		points[i].y += y;
	}

	CoordSystem cs = poly.getCoordSystem();

	cs.position.x += x;
	cs.position.y += y;

	Polygon2D res;

	for (int i = 0; i < n; i++) {
		res.addPoint(points[i]);
	}

	res.getCoordSystem().position = cs.position;
	res.getCoordSystem().rotation = cs.rotation;

	// Successeurs
	if (successors.size() != 1) {
		throw std::runtime_error("translation operation must have only one successor");
	}

	CSymbol *succ = *successors.begin();
	grammar.resolve(succ, context, res);
}

