#include "SplitEquallyRule.hpp"


SplitEquallyRule::~SplitEquallyRule()
{
}

void SplitEquallyRule::split(Cuboid cuboid, std::vector<Cuboid> &out){
	cuboid.splitEqually(direction, numDivisions, out);
}