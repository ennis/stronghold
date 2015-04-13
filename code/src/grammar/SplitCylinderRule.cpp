#include "SplitCylinderRule.hpp"
#include "Cylinder.hpp"
#include "Grammar.hpp"
#include <stdexcept>

void SplitCylinderRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Cylinder) {
        // La règle ne s'applique que sur un cylindre
        throw std::runtime_error("split_cylinder can only be used on Cylinder primitives");
    }

    Cylinder const& cylinder = static_cast<Cylinder const&>(shape);

    std::vector<Cylinder> cylinders;
    cylinder.split(numDivisions, cylinders);

    // Successeurs
    if (successors.size() != 1) {
        throw std::runtime_error("split_cylinder operation must have only one successor");
    }

    CSymbol *succ = *successors.begin();
    for (std::vector<Cylinder>::iterator it = cylinders.begin();
         it != cylinders.end();
         it++) {
        grammar.resolve(succ, context, *it);
    }
}

