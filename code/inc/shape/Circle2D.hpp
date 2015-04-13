#ifndef CIRCLE2D_HPP
#define CIRCLE2D_HPP

#include <Shape.hpp>
#include <Mesh.hpp>
#include <ShaderManager.hpp>
#include <RenderData.hpp>

class Circle2D : public Shape {
private:
	double radius;
public:
	Circle2D() : Shape(ST_Circle) {};
	Circle2D(double rayon) : Shape(ST_Circle) {};
	Circle2D(double rayon, glm::vec3 position) : Shape(ST_Circle, position) {
		radius = rayon;
	};
	double getRadius() const {
		return radius;
	}
    static const int nbSegments = 50;
};

#endif
