#ifndef _TRANSLATIONRULE_HPP_
#define _TRANSLATIONRULE_HPP_

#include <Rule.hpp>
#include <Shape.hpp>

// R�gle permettant de construire des tours en formes de pav�s sur les coins
class TranslationRule : public Rule {
public:
	TranslationRule(int index, CSymbol *pred, float x, float y);

	void execute(Grammar &grammar, Context &context, Shape &shape);

private:
	float x;
	float y;
};

#endif
