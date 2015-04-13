#ifndef _SPLITREPEATRULE_
#define _SPLITREPEATRULE_

#include <SplitRule.hpp>


class SplitRepeatRule : public SplitRule
{
public:
	SplitRepeatRule(int i, Symbol const *pred, SplitDirection direction, std::vector<float> lengths, int nb_repeat, SplitAlignment alignment) :
		SplitRule(i, pred, direction), lengths(lengths), nb_repeat(nb_repeat), alignment(alignment)
	{}

	SplitRepeatRule(int i, Symbol const *pred, SplitDirection direction) :
		SplitRule(i, pred, direction)
	{}

	~SplitRepeatRule();

	void addLength(float length);
	void setNbRepeat(int nb);
	void setAlignment(SplitAlignment align);

	void split(Cuboid cuboid, std::vector<Cuboid> &out);

	std::vector<float> lengths;
	int nb_repeat;
	SplitAlignment alignment;
};

#endif