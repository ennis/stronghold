#ifndef DUMMYSHAPE_HPP
#define DUMMYSHAPE_HPP

#include <Shape.hpp>

//
// Volume de d�part (celui pass� � l'axiome)
// Il en faut un, m�me si la premi�re r�gle n'en a pas vraiment besoin
//
// Note: en fait, le volume de d�part pourrait �tre la heightmap 
class DummyShape : public Shape
{
public:
	DummyShape() : Shape(ST_Dummy)
	{}
};

#endif