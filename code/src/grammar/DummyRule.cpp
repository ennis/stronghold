#include <DummyRule.hpp>
#include <Grammar.hpp>
#include <stdexcept>

void DummyRule::execute(Grammar &grammar, Context &context, Shape &shape)
{
	// on ne fait rien, on passe juste le volume au successeur
	/*if (successors.size() != 1) {
		throw std::runtime_error("dummy operation must have only one successor");
	}*/

	for (std::list<Symbol const*>::iterator it = successors.begin(); it != successors.end(); ++it) {
		grammar.resolve(*it, context, shape);
	}
}
