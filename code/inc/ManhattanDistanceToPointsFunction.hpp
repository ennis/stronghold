#ifndef _MANHATTAN_DIST2POINTS_FUNCTION_
#define _MANHATTAN_DIST2POINTS_FUNCTION_

#include <FieldFunction.hpp>
#include <glm/glm.hpp>

//Distances de Manhattan additionnées avec à plusieurs points
class ManhattanDistanceToPointsFunction : public FieldFunction
{

public:
	//default constructor
	ManhattanDistanceToPointsFunction()
	{}

	ManhattanDistanceToPointsFunction(std::vector<glm::vec2> targets)
		: targets(targets), exponant(1), infactorX(1.f), infactorY(1.f), outfactor(1.f)
	{}

	ManhattanDistanceToPointsFunction(std::vector<glm::vec2> targets, int exp, float infactorX, float infactorY, float outfactor)
		: targets(targets), exponant(exp), infactorX(infactorX), infactorY(infactorY), outfactor(outfactor)
	{}

	ManhattanDistanceToPointsFunction(glm::vec2 target, int exp, float infactorX, float infactorY, float outfactor)
		: exponant(exp), infactorX(infactorX), infactorY(infactorY), outfactor(outfactor)
	{
		targets.push_back(target);
	}

	float eval(float x, float y) const{
		float res = 0.f;
		for (std::vector<glm::vec2>::const_iterator ite = targets.begin(); ite != targets.end(); ite++){
			res += outfactor * pow(infactorX *abs(x - (*ite).x) + infactorY * abs(y - (*ite).y), exponant);
		}
		return  res;
	}
	float eval(glm::vec2 v) const{
		return eval(v.x, v.y);
	}

private:
	std::vector<glm::vec2> targets;
	int exponant;
	float infactorX;
	float infactorY;
	float outfactor;

};

#endif