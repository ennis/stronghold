#include "Prism.hpp"

Prism::Prism(Polygon2D const &p, float h, glm::vec3 position) : Shape(ST_Prism, position, glm::quat()) {
    poly = p;
    height = h;
}
