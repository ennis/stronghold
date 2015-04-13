#ifndef _GRADIENT_FUNCTION_
#define _GRADIENT_FUNCTION_

#include <FieldFunction.hpp>
#include <Terrain.hpp>

class GradientFunction: public FieldFunction
{
public:
	//default constructor
	GradientFunction()
	{}

	GradientFunction(Terrain * terrain)
		: terrain(terrain), smooth(false), exponant(1), infactor(1.f), outfactor(1.f)
	{}

	GradientFunction(Terrain * terrain, bool smooth, int exp, float infactor, float outfactor)
		: terrain(terrain), smooth(smooth), exponant(exp), infactor(infactor), outfactor(outfactor)
	{}

	float eval(float x, float y) const{
		if (smooth){
			return outfactor * pow(infactor * glm::length2(terrain->getSmoothGradient(x, y)), exponant);
		}
		else{
			return outfactor * pow(infactor * glm::length2(terrain->getGradient(x, y)), exponant);
		}
	}
	float eval(glm::vec2 v) const{
		return eval(v.x, v.y);
	}

private:
	Terrain * terrain;
	bool smooth;
	int exponant;
	float infactor;
	float outfactor;
};

#endif