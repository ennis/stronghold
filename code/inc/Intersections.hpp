#ifndef _INTERSECTIONS_HPP_
#define _INTERSECTIONS_HPP_

#include "Rectangle2D.hpp"
#include <glm/glm.hpp>

// Intersection of the two segments
static bool is_lines_intersect (glm::vec3 eq1, glm::vec3 eq2, glm::vec2 min, glm::vec2 max) {
    float D = eq1.x * eq2.y - eq2.x * eq1.y;

    if (D == 0)
        return false;

    // Intersection point
    float xi = (eq2.z * eq1.y - eq1.z * eq2.y) / D;
    float yi = (eq2.x * eq1.z - eq1.x * eq2.z) / D;

    // Does it lie on the segment [min, max] ?
    return (xi >= min.x && xi <= max.x) && (yi >= min.y && yi <= max.y);
}

// Compute the line equation that contains pp1 and pp2
static glm::vec3 compute_eq (glm::vec3 const& pp1, glm::vec3 const& pp2) {
    glm::vec2 p1(pp1.x, pp1.z);
    glm::vec2 p2(pp2.x, pp2.z);

    glm::vec2 dir = glm::normalize(p2 - p1);
    glm::vec3 eq;
    eq.x = dir.y;
    eq.y = -dir.x;
    eq.z = -eq.x * p1.x - eq.y * p1.y;

    return eq;
}

// Compute rectangle equations
static std::vector<glm::vec3> compute_rect_equations (Rectangle2D const& house) {
    std::vector<glm::vec3> equations(4);
    CoordSystem cs = house.getCoordSystem();

    // Points on the rectangle
    glm::vec3 v[4];
    v[0] = cs.transform(glm::vec3(0, 0, 0));
    v[1] = cs.transform(glm::vec3(house.width, 0, 0));
    v[2] = cs.transform(glm::vec3(0, 0, house.height));
    v[3] = cs.transform(glm::vec3(house.width, 0, house.height));

    equations[0] = compute_eq(v[0], v[1]);
    equations[1] = compute_eq(v[0], v[2]);
    equations[2] = compute_eq(v[1], v[3]);
    equations[3] = compute_eq(v[2], v[3]);

    return equations;
}

// Intersection rectangle / rectangle
static bool intersect_houses (Rectangle2D const& house1, Rectangle2D const& house2) {
    std::vector<glm::vec3> eq1 = compute_rect_equations(house1);

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

    std::vector<glm::vec3> eq2 = compute_rect_equations(house2);

    for (int j = 0; j < eq2.size(); j++) {
        for (int i = 0; i < eq1.size(); i++) {
            if (is_lines_intersect(eq1[i], eq2[j], points[i].first, points[i].second)) {
                return true;
            }
        }
    }

    return false;
}

// Intersection between a house and the walls
static bool intersect_rect_poly (Context &context,
                                 Rectangle2D const& house,
                                 Polygon2D const& muraille) {
    // Compute slope of each face of the house
    std::vector<glm::vec3> equations = compute_rect_equations(house);

    std::vector<glm::vec2> points = muraille.getPoints();
    int n = points.size();
    Stronghold s = context.strongholds[context.currentStronghold];
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        Cuboid mur = s.walls[i];
        CoordSystem cs = mur.getCoordSystem();
        glm::vec3 p1 = cs.transform(glm::vec3(0, 0, 0));
        glm::vec3 p2 = cs.transform(glm::vec3(mur.dimensions.x, 0, 0));

        // Compute equation for the current face of the fortification
        /* glm::vec3 eq = compute_eq(glm::vec3(p1.x, 0, p1.y), */
        /*                           glm::vec3(p2.x, 0, p2.y)); */
        glm::vec3 eq = compute_eq(glm::vec3(points[i].x, 0, points[i].y),
                                  glm::vec3(points[j].x, 0, points[j].y));

        for (int j = 0; j < 3; j++) {
            if (is_lines_intersect(eq, equations[j], points[i], points[j])) {
                return true;
            }
        }
    }

    return false;
}

#endif
