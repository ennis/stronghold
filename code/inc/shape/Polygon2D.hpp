#ifndef POLYGON2D_HPP
#define POLYGON2D_HPP

#include <Shape.hpp>
#include <Rectangle2D.hpp>
#include <vector>
#include <string>

//
// Une classe représentant une polygone délimitant 
// une parcelle sur le terrain
//
// Référentiel: celui du terrain
class Polygon2D : public Shape
{
public:
	Polygon2D();

	//
	// Ajout d'un point
	void addPoint(glm::vec2 const &point);

	//
	//
	std::string toString() const;

	std::vector<glm::vec2> const &getPoints() const {
		return points;
	}

	int size() const { return static_cast<int>(points.size()); }

	//
	// Test d'inclusion d'un point dans un polygone
    bool inside_polygon (glm::vec2 pt) const;

	//
	// Retourne la boite englobante alignée sur les axes (AABB) du polygone
	void getBounds(glm::vec2 &a, glm::vec2 &b) const;

private:
	std::vector<glm::vec2> points;
};

#endif
