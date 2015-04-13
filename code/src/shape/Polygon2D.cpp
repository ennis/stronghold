#include "Polygon2D.hpp"
#include <sstream>

Polygon2D::Polygon2D() : Shape(ST_Polygon, glm::vec3(), glm::quat())
{}

void Polygon2D::addPoint(glm::vec2 const &point)
{
	points.push_back(point);
}

std::string Polygon2D::toString() const
{
	std::ostringstream ss;
	std::vector<glm::vec2>::const_iterator it = points.begin();

	while (it != points.end()) {
		ss << "(" << it->x << "," << it->y << ") ";
		it++;
	}
	return ss.str();
}

bool Polygon2D::inside_polygon (glm::vec2 pt) const {
    int i, j;
    bool c = false;
    int npol = points.size();

    for (i = 0, j = npol-1; i < npol; j = i++) {
        if ((((points[i].y<=pt.y) && (pt.y<points[j].y)) ||
             ((points[j].y<=pt.y) && (pt.y<points[i].y))) &&
            (pt.x < (points[j].x - points[i].x) * (pt.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))

            c = !c;
    }
    return c;
}

void Polygon2D::getBounds(glm::vec2 &a, glm::vec2 &b) const
{
	float minx = 0.f, maxx = 0.f;
	float miny = 0.f, maxy = 0.f;

	for (std::vector<glm::vec2>::const_iterator it = points.begin(); it != points.end(); ++it) {
		glm::vec2 v = *it;
		if (v.x > maxx) {
			maxx = v.x;
		}
		if (v.x < minx) {
			minx = v.x;
		}
		if (v.y > maxy) {
			maxy = v.y;
		}
		if (v.y < miny) {
			miny = v.y;
		}
	}

	a = glm::vec2(minx, miny);
	b = glm::vec2(maxx, maxy);
}
