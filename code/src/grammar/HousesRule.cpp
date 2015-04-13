#include "HousesRule.hpp"
#include "Polygon2D.hpp"
#include "Rectangle2D.hpp"
#include "Grammar.hpp"
#include "Random.hpp"
#include "Cuboid.hpp"
#include <stdexcept>

static std::vector<glm::vec2> compute_rect_equations (Rectangle2D const& house);

void HousesRule::addDimension (glm::vec3 const& dim) {
    dimensions.push_back(dim);
}

void HousesRule::setNum (int num) {
    this->num = num;
}

static bool is_lines_intersect (float m1, float m2, float p1, float p2, glm::vec2 min, glm::vec2 max) {
    // Droites parallèles
    if (m1 == m2)
        return false;

    // Intersection point
    float xi = (p2 - p1) / (m1 - m2);
    float yi = m1 * xi + p1;

    // Does it lie on the segment [min, max] ?
    return (xi >= min.x && xi <= max.x) && (yi >= min.y && yi <= max.y);
}

// Intersection between an house and the walls
static bool intersect_rect_poly (Rectangle2D const& house, Polygon2D const& muraille) {
    // Compute slope of each face of the house
    std::vector<glm::vec2> equations = compute_rect_equations(house);

    std::vector<glm::vec2> points = muraille.getPoints();
    for (int i = 0; i < points.size() - 1; i++) {
        // Compute equation for the current face of the fortification
        glm::vec2 eq;
        eq.x = float(points[i].y - points[i+1].y) / (points[i].x - points[i+1].x);
        eq.y = points[i].y - eq.x * points[i].x;

        for (int j = 0; j < 3; j++) {
            if (is_lines_intersect(eq.x, eq.y, equations[i].x, equations[i].y, points[i], points[i+1])) {
                return true;
            }
        }
    }

    return false;
}

static std::vector<glm::vec2> compute_rect_equations (Rectangle2D const& house) {
    std::vector<glm::vec2> equations(4);
    CoordSystem cs = house.getCoordSystem();

    // Points on the rectangle
    glm::vec3 v[4];
    v[0] = cs.transform(glm::vec3(0, 0, 0));
    v[1] = cs.transform(glm::vec3(house.width, 0, 0));
    v[2] = cs.transform(glm::vec3(0, 0, house.height));
    v[3] = cs.transform(glm::vec3(house.width, 0, house.height));

    equations[0].x = (v[1].z - v[0].z) / (v[1].x - v[0].x);
    equations[0].y = v[0].z - equations[0].x * v[0].x;

    equations[1].x = (v[2].z - v[0].z) / (v[2].x - v[0].x);
    equations[1].y = v[0].z - equations[1].x * v[0].x;

    equations[2].x = (v[3].z - v[1].z) / (v[3].x - v[1].x);
    equations[2].y = v[3].z - equations[2].x * v[3].x;

    equations[3].x = (v[3].z - v[2].z) / (v[3].x - v[2].x);
    equations[3].y = v[3].z - equations[3].x * v[3].x;

    return equations;
}

// Collisions between two houses
static bool intersect_poly_poly (Rectangle2D const& house1, std::vector<Cuboid> const& houses) {
    std::vector<glm::vec2> eq1 = compute_rect_equations(house1);

    CoordSystem cs1 = house1.getCoordSystem();

    // Points on the rectangle
    glm::vec3 v[4];
    v[0] = cs1.transform(glm::vec3(0, 0, 0));
    v[1] = cs1.transform(glm::vec3(house1.width, 0, 0));
    v[2] = cs1.transform(glm::vec3(0, 0, house1.height));
    v[3] = cs1.transform(glm::vec3(house1.width, 0, house1.height));

    std::vector<std::pair<glm::vec2, glm::vec2> > points(4);
    points[0].first.x = v[0].x;
    points[0].first.y = v[0].z;
    points[0].second.x = v[1].x;
    points[0].second.y = v[1].z;

    points[1].first = points[0].first;
    points[1].second.x = v[2].x;
    points[1].second.y = v[2].z;

    points[2].first = points[0].second;
    points[2].second.x = v[3].x;
    points[2].second.y = v[3].z;

    points[3].first = points[1].second;
    points[3].second = points[2].second;

    for (std::vector<Cuboid>::const_iterator it = houses.begin(); it != houses.end(); it++) {
        CoordSystem cs = it->getCoordSystem();
        glm::vec3 dim = it->dimensions;
        Rectangle2D house(cs.position, cs.rotation, dim.x, dim.z);
        glm::vec3 pos3d = cs.position;
        glm::vec2 pos(pos3d.x, pos3d.z);

        std::vector<glm::vec2> eq2 = compute_rect_equations(house);

        for (int j = 0; j < eq2.size(); j++) {
            for (int i = 0; i < eq1.size(); i++) {
                if (is_lines_intersect(eq1[i].x, eq1[i].y, eq2[j].x, eq2[j].y, points[i].first, points[i].second)) {
                    return true;
                }
           }
        }
    }

    return false;
}

void HousesRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Polygon) {
        // La règle ne s'applique que sur un polygone
        throw std::runtime_error("build_houses can only be used on Polygon primitives");
    }

    Polygon2D const& poly = static_cast<Polygon2D const&>(shape);
    CoordSystem cs = poly.getCoordSystem();

    // Génération des maisons
    std::vector<Cuboid> houses;

    // Nombre de fois où on essaie de prendre un point aléatoire
    int N = 10, n;
    bool place;

    for(int i = 0; i < num;) {
        // Essai de placer une maison
        i++;
        place = false;
        n = N;
        while (! place && n > 0) {
            glm::vec2 pt = GRandom.RandomPoint(poly);
            n--;
            float h = context.terrain->getHeight(pt);

            for (std::list<glm::vec3>::iterator it = dimensions.begin(); it != dimensions.end(); it++) {
                glm::vec3 pt3d(pt.x, h, pt.y);
                Cuboid cube(pt3d, cs.rotation, *it);
                CoordSystem cs = cube.getCoordSystem();
                glm::vec3 dim = cube.dimensions;
                // TODO: orientation des maisons
                Rectangle2D rect(cs.position, cs.rotation, dim.x, dim.z);

                if (! (intersect_rect_poly(rect, poly) || intersect_poly_poly(rect, houses))) {
                    houses.push_back(cube);
                    place = true;
                    break;
                }
            }
        }
    }

    // Successeurs
    if (successors.size() != 1) {
        throw std::runtime_error("build_houses operation must have only one successor");
    }

    CSymbol *succ = *successors.begin();
    for (std::vector<Cuboid>::iterator it = houses.begin(); it != houses.end(); it++) {
        grammar.resolve(succ, context, *it);
    }
}

