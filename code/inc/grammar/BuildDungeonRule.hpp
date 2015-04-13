#ifndef _BUILDDUNGEONRULE_HPP_
#define _BUILDDUNGEONRULE_HPP_

#include <stdexcept>
#include <Rule.hpp>
#include <Shape.hpp>

// Règle permettant de construire un donjon à l'emplacement spécifié
class BuildDungeonRule : public Rule {
    public:
        BuildDungeonRule (int index, CSymbol *pred, float radius = 10.0f, float height = 100.0f) : Rule(index, pred), radius(radius), height(height) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        float radius;
        float height;
};

#endif

