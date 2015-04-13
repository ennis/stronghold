#ifndef _FIELD_FUNCTION_
#define _FIELD_FUNCTION_

#include <glm/glm.hpp>

// Fonctions lié au terrain, à sa morphologie, à ses particularités
// ou à ce qui se situe dessus
// Classe abstraite
class FieldFunction
{
public:
	FieldFunction(){}

	// evalue la fonction au point (x,y) / v
	virtual float eval(float x, float y) const = 0;
	virtual float eval(glm::vec2 v) const = 0;

};

#endif