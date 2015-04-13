#ifndef _SPLITEQUALLYRULE_
#define _SPLITEQUALLYRULE_

#include "SplitRule.hpp"

class SplitEquallyRule: public SplitRule
{
public:
	SplitEquallyRule(int i, Symbol const *pred, SplitDirection direction, int numDivisions) :
		SplitRule(i, pred, direction), numDivisions(numDivisions)
	{}
	~SplitEquallyRule();

	void split(Cuboid cuboid, std::vector<Cuboid> &out);

	int numDivisions;
};

#endif
