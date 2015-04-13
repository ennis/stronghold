#ifndef _TETRAHEDRON_HPP_
#define _TETRAHEDRON_HPP_

#include "Shape.hpp"
#include "Rectangle2D.hpp"

class Tetrahedron : public Shape {
    public:
        Tetrahedron (Rectangle2D base, glm::vec3 position, float height) : Shape(ST_Tetrahedron, position), height(height), base(base) {
        };

        float height;
        Rectangle2D base;
};

#endif

