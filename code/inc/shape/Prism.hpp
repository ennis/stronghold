#ifndef _PRISM_HPP_
#define _PRISM_HPP_

#include "Polygon2D.hpp"
#include "Shape.hpp"

class Prism : public Shape {
    public:
        Prism (Polygon2D const &p, float h, glm::vec3 position);
		Prism() : Shape(ST_Prism) {};
  
        Polygon2D poly;
        float height;
};

#endif

