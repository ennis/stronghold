#ifndef DUMMYRULE_HPP
#define DUMMYRULE_HPP

#include "Rule.hpp"
#include <iostream>

// R�gle no-op: d�rive un non-terminal vers un autre non-terminal
class DummyRule : public Rule
{
public:
	DummyRule(int i, CSymbol *pred) : Rule(i, pred)
	{
	}

	void execute(Grammar &grammar, Context &context, Shape &shape);
};

#endif