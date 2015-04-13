#ifndef _BUILDINGGENERATION_HPP_
#define _BUILDINGGENERATION_HPP_

#include "Cuboid.hpp"
#include "SeedGeneration.hpp"
#include "Intersections.hpp"

static float xHouse = 10.0f;
static float yHouse = 10.0f;
static float zHouse = 10.0f;

void generateBuildings (Context& context,
                        std::vector<BuildingSeed>& seeds,
                        std::vector<Cuboid>& houses) {
    for (std::vector<BuildingSeed>::iterator it = seeds.begin();
         it != seeds.end();
         it++) {
        BuildingSeed & bs = *it;

        glm::vec2 road = bs.closest_road_point;
        glm::vec2 u = glm::normalize(road - bs.location);
        glm::vec3 pos = glm::vec3(bs.location.x, context.terrain->getHeight(bs.location), bs.location.y);
        CoordSystem cs = CoordSystem::fromVectors(pos,
                                                  glm::vec3(u.x, 0, u.y),
                                                  glm::vec3(-u.y, 0, u.x));
        /* float alpha = zHouse / 2; */
        /* cs.position = pos - alpha * glm::vec3(u.x, 0, u.y); */
        cs.position = pos;
        glm::vec3 dimensions(xHouse, yHouse, zHouse);
        Cuboid maison(cs, dimensions);
        Rectangle2D house(cs, dimensions.x, dimensions.z);

        bool inserted = true;

        // Collision muraille / maison
        if (intersect_rect_poly(context,
                                house,
                                context.strongholds[context.currentStronghold].base)) {
            inserted = false;
        }

        // Collisions entre maisons
        for (int i = 0; i < houses.size(); i++) {
            Rectangle2D house2(houses[i].getCoordSystem(),
                               houses[i].dimensions.x,
                               houses[i].dimensions.z);
            if (intersect_houses(house, house2)) {
                inserted = false;
                break;
            }
        }

        if (inserted) {
            houses.push_back(maison);
        }
    }
}

#endif

