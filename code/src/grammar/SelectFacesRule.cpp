#include "SelectFacesRule.hpp"
#include "Grammar.hpp"
#include "Cuboid.hpp"
#include "Rectangle2D.hpp"
#include <stdexcept>

void SelectFacesRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Cuboid) {
        throw std::runtime_error("select_faces can only be used on cuboids");
    }

    Cuboid const &cube = static_cast<Cuboid const &>(shape);
    std::vector<Rectangle2D *> faces(6);
	CoordSystem const &cs = cube.getCoordSystem();

    // compute faces of the cuboid
    glm::vec3 dimensions = cube.dimensions;

	float dx = cube.dimensions.x;
	float dy = cube.dimensions.y;
	float dz = cube.dimensions.z;

	glm::vec3 position = cs.position;
	glm::vec3 vdx = glm::vec3(dimensions.x, 0, 0);
	glm::vec3 vdy = glm::vec3(0, dimensions.y, 0);
	glm::vec3 vdz = glm::vec3(0, 0, dimensions.z);

	glm::vec3 vx = glm::vec3(1, 0, 0);
	glm::vec3 vy = glm::vec3(0, 1, 0);
	glm::vec3 vz = glm::vec3(0, 0, 1);

    // front
	faces[0] = new Rectangle2D(CoordSystem::fromVectors(cs, vdy + vdx, -vx, -vy), dx, dy);
	// back
	faces[1] = new Rectangle2D(CoordSystem::fromVectors(cs, vdz, vy, vx), dy, dx);
    // up
	faces[2] = new Rectangle2D(CoordSystem::fromVectors(cs, vdy, vx, vz), dx, dz);
	// down
	faces[3] = new Rectangle2D(CoordSystem::fromVectors(cs, glm::vec3(0.f), vx, vz), dx, dz);
    // left
	faces[4] = new Rectangle2D(CoordSystem::fromVectors(cs, vdx, vz, vy), dz, dy);
    // right
	faces[5] = new Rectangle2D(CoordSystem::fromVectors(cs, glm::vec3(0.f), vy, vz), dy, dz);

    // Successeurs = faces
    int i = 0;
    for (std::list<CSymbol *>::iterator it = successors.begin(); it != successors.end(); it++, i++) {
        grammar.resolve(*it, context, *(faces[i]));
    }
}

