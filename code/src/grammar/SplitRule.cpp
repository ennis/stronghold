#include <SplitRule.hpp>
#include <Cuboid.hpp>
#include <stdexcept>
#include <Grammar.hpp>

void SplitRule::execute(Grammar &grammar, Context &context, Shape &shape)
{
	if (shape.getShapeType() != ST_Cuboid) {
		throw std::runtime_error("Sorry, I can only split cuboids");
	}

	Cuboid const& cuboid = static_cast<Cuboid const&>(shape);

	std::vector<Cuboid> out;
	split(cuboid, out);

	CSymbol *succ = *(successors.begin());
	int index = 0;
	for (std::vector<Cuboid>::iterator it = out.begin(); it != out.end(); ++it, ++index) {
		it->index = index;
		grammar.resolve(succ, context, *it);
	}
}
