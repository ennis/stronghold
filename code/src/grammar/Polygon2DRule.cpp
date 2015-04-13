#include "Polygon2DRule.hpp"
#include <Grammar.hpp>
#include <stdexcept>

#include <sstream>

Polygon2DRule::Polygon2DRule(int i, Symbol const *pred) : Rule(i, pred)
{
}

Polygon2DRule::Polygon2DRule() {
}

std::string Polygon2DRule::toString() const {
	return points.toString();
}

void Polygon2DRule::addPoint(glm::vec2 const& point) {
	points.addPoint(point);
}

void Polygon2DRule::execute(Grammar &grammar, Context &context, Shape &shape)
{
    if (points.size() < 3) {
		throw std::runtime_error("Polygon2D operation must have at least three points");
    }

	if (successors.size() != 1) {
		throw std::runtime_error("Polygon2D operation must have only one successor");
	}
	grammar.resolve(*successors.begin(), context, points);
}

