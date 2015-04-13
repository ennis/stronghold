#ifndef _SELECTFACESRULE_HPP_
#define _SELECTFACESRULE_HPP_

#include "Rule.hpp"

// Règle permetttant de sélectionner les faces d'un cube
// Les successeurs doivent être dans l'ordre suivant :
// Faces+X
// Faces-X
// Faces+Y
// Faces-Y
// Faces+Z
// Faces-Z
class SelectFacesRule : public Rule {
    public:
        SelectFacesRule (int index, CSymbol *pred) : Rule(index, pred) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);
};

#endif

