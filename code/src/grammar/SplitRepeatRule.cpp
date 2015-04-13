#include "SplitRepeatRule.hpp"


SplitRepeatRule::~SplitRepeatRule()
{
}

void SplitRepeatRule::addLength(float length){
	lengths.push_back(length);
}
void SplitRepeatRule::setNbRepeat(int nb){
	nb_repeat = nb;
}
void SplitRepeatRule::setAlignment(SplitAlignment align){
	alignment = align;
}

void SplitRepeatRule::split(Cuboid cuboid, std::vector<Cuboid> &out){
	cuboid.splitRepeat(direction, lengths, nb_repeat, alignment, out);
}