#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Circle2D.hpp"
#include <vector>

class Cylinder : public Shape {
private:
	Circle2D circle;
	double height;
public:
	Cylinder();
	Cylinder(Circle2D cercle, double hauteur);
	Circle2D getCircle() const;
	double getHeight() const;

    void split (int numDivisions, std::vector<Cylinder> &out) const;
};

#endif
