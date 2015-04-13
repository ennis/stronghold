#ifndef _BORDER_FUNCTION_
#define _BORDER_FUNCTION_

#include <FieldFunction.hpp>
#include <Polygon2D.hpp>


// Fonction renvoyant interior_value si on est dans le polygone
// et exterior value si on est à l'exterieur du polygone
class BorderFunction : public FieldFunction
{
public:
	//default constructor
	BorderFunction()
	{}

	BorderFunction(Polygon2D *polygon)
		: polygon(polygon), interior_value(0.f), exterior_value(1000000.f)
	{}

	BorderFunction(Polygon2D *polygon, float interior_value, float exterior_value)
		: polygon(polygon), interior_value(interior_value), exterior_value(exterior_value)
	{}

	float eval(float x, float y) const{
		return  eval(glm::vec2(x, y));
	}
	float eval(glm::vec2 v) const{
		if (polygon->inside_polygon(v)){
			return interior_value;
		}
		else{
			return exterior_value;
		}
	}

private:
	Polygon2D *polygon;
	float interior_value;
	float exterior_value;
};

#endif
