#ifndef _HOUSESRULE_HPP_
#define _HOUSESRULE_HPP_

#include "Rule.hpp"

// Règle générant un nombre de maisons donné dans un polygone
// On suppose que le polygone est convexe
class HousesRule : public Rule {
    public:
        HousesRule(int index, CSymbol *pred) : Rule(index, pred), num(0) 
		{
        }

        void addDimension (glm::vec3 const& dim);
        void setNum (int num);

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        std::list<glm::vec3> dimensions;
        int num;
};

#endif

