#ifndef _STRONGHOLDSEEDRULE_HPP_
#define _STRONGHOLDSEEDRULE_HPP_

#include "Rule.hpp"
#include "Shape.hpp"

class StrongholdSeedRule : public Rule {
    public:
        StrongholdSeedRule (int index, CSymbol *pred, glm::vec2 center, int faces, float radius) : Rule(index, pred), center(center), faces(faces), radius(radius) {
        };

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        glm::vec2 center;
        int faces;
        float radius;

        static int N;
        static float threshold;
        static float step;
};

#endif

