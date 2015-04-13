#ifndef _PATH_REUSE_FUNCTION_
#define _PATH_REUSE_FUNCTION_

#include <FieldFunction.hpp>
#include <glm/glm.hpp>

#include <Path.hpp>

//Reutilisation de chemins
class PathReuseFunction : public FieldFunction
{
public:
	//default constructor
	PathReuseFunction()
	{}

	PathReuseFunction(CaseMatrix * board)
		: board(board), base_value(30), factor(0.5f)
		{}

	PathReuseFunction(CaseMatrix * board, float base_value, float factor)
		: board(board), base_value(base_value), factor(factor)
		{}

	float eval(float x, float y) const{
		return  eval(glm::vec2(x, y));
	}
	float eval(glm::vec2 v) const{
		int freq = (*board)(v).frequency;
		return base_value * exp(-1 * factor * freq);
	}

private:
	CaseMatrix * board;

	// Valeur de base, lorsque l'on ne passe pas par la case
	float base_value;
	float factor;

};

#endif