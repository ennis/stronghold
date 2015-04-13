#ifndef _MAKEVILLAGERULE_HPP_
#define _MAKEVILLAGERULE_HPP_

#include "Rule.hpp"
#include <Shape.hpp>

// Création d'un village à l'intérieur de la forteresse
// Successeurs :
// -> Donjon
// -> Parcelles
class MakeVillageRule : public Rule {
    public:
        MakeVillageRule (int index, CSymbol *pred);

        void execute(Grammar &grammar, Context &context, Shape &shape);
};

#endif

