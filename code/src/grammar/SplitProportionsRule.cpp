#include "SplitProportionsRule.hpp"


SplitProportionsRule::~SplitProportionsRule()
{
}

void SplitProportionsRule::addProportions(float prop){
	proportions.push_back(prop);
}

void SplitProportionsRule::split(Cuboid cuboid, std::vector<Cuboid> &out){
	cuboid.splitProportions(direction, proportions, out);
}

