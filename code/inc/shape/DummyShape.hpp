#ifndef DUMMYSHAPE_HPP
#define DUMMYSHAPE_HPP

#include <Shape.hpp>

//
// Volume de départ (celui passé à l'axiome)
// Il en faut un, même si la première règle n'en a pas vraiment besoin
//
// Note: en fait, le volume de départ pourrait être la heightmap 
class DummyShape : public Shape
{
public:
	DummyShape() : Shape(ST_Dummy)
	{}
};

#endif