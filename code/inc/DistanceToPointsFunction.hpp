#ifndef _DIST2POINTS_FUNCTION_
#define _DIST2POINTS_FUNCTION_

#include <FieldFunction.hpp>
#include <glm/glm.hpp>

//Distances additionnées avec à plusieurs points
class DistanceToPointsFunction : public FieldFunction
{
public:
	//default constructor
	DistanceToPointsFunction()
	{}

	DistanceToPointsFunction(std::vector<glm::vec2> targets)
		: targets(targets), exponant(1), infactor(1.f), outfactor(1.f)
	{}

	DistanceToPointsFunction(std::vector<glm::vec2> targets, int exp, float infactor, float outfactor)
		: targets(targets), exponant(exp), infactor(infactor), outfactor(outfactor)
	{}

	DistanceToPointsFunction(glm::vec2 target, int exp, float infactor, float outfactor)
		: exponant(exp), infactor(infactor), outfactor(outfactor)
	{
		targets.push_back(target);
	}

	float eval(float x, float y) const{
		return  eval(glm::vec2(x,y));
	}
	float eval(glm::vec2 v) const{
		float res = 0.f;
		for (std::vector<glm::vec2>::const_iterator ite = targets.begin(); ite != targets.end(); ite++){
			res += outfactor * pow(infactor * glm::distance(v, *ite), exponant);
		}
		return res;
	}

private:
	std::vector<glm::vec2> targets;
	int exponant;
	float infactor;
	float outfactor;

};

#endif