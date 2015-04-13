#include "Cylinder.hpp"
#include "Circle2D.hpp"
#include <iostream>

Cylinder::Cylinder() : Shape(ST_Cylinder) {}

Cylinder::Cylinder(Circle2D cercle, double hauteur) : Shape(ST_Cylinder) {
	circle = cercle;
	height = hauteur;
}

Circle2D Cylinder::getCircle() const {
	return circle;
}

double Cylinder::getHeight() const {
	return height;
}

void Cylinder::split (int numDivisions, std::vector<Cylinder> &out) const {
    double step = height / numDivisions;
    double radius = getCircle().getRadius();

    for (int i = 0; i < numDivisions; i++) {
        CoordSystem cs = getCircle().getCoordSystem();
        cs.position.y += i * step;
        Circle2D base(radius, cs.position);
        Cylinder c(base, step);
        c.index = i;
        out.push_back(c);
    }
}

