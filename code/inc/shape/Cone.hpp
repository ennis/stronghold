#ifndef _CONE_HPP_
#define _CONE_HPP_

#include "Shape.hpp"
#include "Circle2D.hpp"

class Cone : public Shape {
    public:
        Cone (Circle2D base, glm::vec3 position, float height) : Shape(ST_Cone, position), height(height), base(base) {
        };

        float height;
        Circle2D base;
};

#endif

