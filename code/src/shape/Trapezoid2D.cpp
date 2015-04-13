#include <Trapezoid2D.hpp>
#include <cmath>
#include <iostream>

#include <glm/gtc/quaternion.hpp>

//
// Constructeur
// Prend deux points dans le repère du terrain, deux angles et une taille
//

Trapezoid2D::Trapezoid2D() : Shape(ST_Trapezoid) {}

Trapezoid2D::Trapezoid2D(glm::vec2  const &Pa, glm::vec2  const &Pb, double aA, double bA, double height) : Shape(ST_Trapezoid)  {
    a = Pa;
    b = Pb;
	aAngle = aA; //Entre 0 et Pi
	bAngle = bA; //Entre 0 et Pi
	hauteur = height/2;

	// génération du référentiel
	glm::vec2 d = glm::normalize(b - a);
	glm::vec3 u0 = glm::vec3(d.x, 0, d.y);
	glm::vec3 v0 = glm::vec3(-d.y, 0, d.x);
	coordSystem = CoordSystem::fromVectors(glm::vec3(Pa.x, 0, Pa.y), u0, v0);

	float longueur = glm::distance(a, b);

    sommets[0] = glm::vec2(hauteur / tanf(aAngle), hauteur);
    sommets[1] = glm::vec2(longueur - hauteur / tanf(bAngle), hauteur);
    sommets[2] = glm::vec2(longueur + hauteur / tanf(bAngle), -hauteur);
    sommets[3] = glm::vec2(-hauteur / tanf(aAngle), -hauteur);
}
