#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <Shape.hpp>

class Rectangle2D : public Shape
{
public:
	Rectangle2D() : Shape(ST_Rectangle)
	{
	}


	Rectangle2D(CoordSystem const &cs, float width, float height) :
		Shape(ST_Rectangle, cs), width(width), height(height)
	{
	}

	Rectangle2D(glm::vec3 const &position, glm::quat const &rotation, float width, float height) :
		Shape(ST_Rectangle, position, rotation), width(width), height(height)
	{
	}

	float width, height;
};

#endif
