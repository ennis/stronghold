#include "RoofRule.hpp"
#include "Circle2D.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Rectangle2D.hpp"
#include "Tetrahedron.hpp"
#include "Cuboid.hpp"
#include "Grammar.hpp"
#include <stdexcept>

void RoofRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    // La forme de base est soit un cylindre, soit un cuboïde
    switch (shape.getShapeType()) {
        case ST_Cylinder:
            {
                Cylinder const& cylinder = static_cast<Cylinder const&>(shape);
                double r = cylinder.getCircle().getRadius();
                double h = cylinder.getHeight();
                glm::vec3 center = cylinder.getCircle().getCoordSystem().position;
                glm::vec3 hCenter(center.x, center.y + h, center.z);
                Circle2D base(r, hCenter);
                Cone cone(base, hCenter, height);

                // Successeurs
                if (successors.size() != 2) {
                    throw std::runtime_error("build_roof operation must have only two successors");
                }

                // Roof
                CSymbol *succ = *successors.begin();
                grammar.resolve(succ, context, cone);

                // Base
                succ = *(++(successors.begin()));
                grammar.resolve(succ, context, shape);
            }
            break;

        case ST_Cuboid:
            {
                Cuboid const& cube = static_cast<Cuboid const&>(shape);
                CoordSystem cs = cube.getCoordSystem();
                glm::vec3 dim = cube.dimensions;
                glm::vec3 origTetra = cs.position + glm::vec3(0, dim.y, 0);
                Rectangle2D base(origTetra, cs.rotation, dim.x, dim.z);
                Tetrahedron tetra(base, origTetra, height);

                // Successeurs
                if (successors.size() != 2) {
                    throw std::runtime_error("build_roof operation must have only two successors");
                }

                // Roof
                CSymbol *succ = *successors.begin();
                grammar.resolve(succ, context, tetra);

                // Base
                succ = *(++(successors.begin()));
                grammar.resolve(succ, context, shape);
            }
            break;

        default:
            throw std::runtime_error("Build_roof can only be used on Cylinder or Cuboid primitives");
            break;
    }
}
