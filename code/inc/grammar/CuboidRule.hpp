#ifndef _CUBOIDRULE_
#define _CUBOIDRULE_

#include <Rule.hpp>
#include <Shape.hpp>

//Regle permettant de construire un Cuboid (pavé) à un emplacement
class CuboidRule : public Rule
{
public:
	CuboidRule(int index, CSymbol *pred, float size_X, float size_Y, float size_Z) :
		Rule(index, pred), size_X(size_X), size_Y(size_Y), size_Z(size_Z)
	{}

	void execute(Grammar &grammar, Context &context, Shape &shape);

private:
	float size_X;
	float size_Y;
	float size_Z;

};

#endif