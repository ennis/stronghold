#ifndef _SPLITPROPORTIONSRULE_
#define _SPLITPROPORTIONSRULE_

#include "SplitRule.hpp"

class SplitProportionsRule : public SplitRule
{
public:
	SplitProportionsRule(int i, Symbol const *pred, SplitDirection direction, std::vector<float> props) :
		SplitRule(i, pred, direction), proportions(props)
	{}

	SplitProportionsRule(int i, Symbol const *pred, SplitDirection direction) :
		SplitRule(i, pred, direction)
	{}

	~SplitProportionsRule();

	void addProportions(float prop);

	void split(Cuboid cuboid, std::vector<Cuboid> &out);

	// Tableau contenant les proportions de splitting
	std::vector<float> proportions;
};

#endif