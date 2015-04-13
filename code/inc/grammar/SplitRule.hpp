#ifndef _SPLITRULE_HPP_
#define _SPLITRULE_HPP_

#include <Rule.hpp>
#include <vector>
#include <Cuboid.hpp>
#include <SplitEnums.hpp>


class SplitRule : public Rule
{
public:
	SplitRule(int i, Symbol const *pred, SplitDirection direction) :
		Rule(i, pred), direction(direction)
	{}

	// Seulement pour le cuboid pour le moment
	virtual void split(Cuboid cuboid, std::vector<Cuboid> &out) = 0;

	//	Seulement un cuboid pour le moment
	void execute(Grammar &grammar, Context &context, Shape &shape);

	// Direction : dans le repere local de la shape
	// 3 directions possible (1,0,0); (0,1,0); (0,0,1)
	SplitDirection direction;

};

#endif