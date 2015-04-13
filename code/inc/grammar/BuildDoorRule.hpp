#ifndef _BUILDDOORRULE_HPP_
#define _BUILDDOORRULE_HPP_

#include <stdexcept>
#include <Rule.hpp>
#include <Shape.hpp>

// Règle permettant de construire une porte sur le mur approprié
// Wall -> build_door wallSideLeft wallSideRight doorWallUp doorWallDown
class BuildDoorRule : public Rule {
    public:
        BuildDoorRule (int index, CSymbol *pred) : Rule(index, pred) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
};

#endif

