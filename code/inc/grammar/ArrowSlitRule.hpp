#ifndef _ARROWSLITRULE_HPP_
#define _ARROWSLITRULE_HPP_

#include "Rule.hpp"
#include "Shape.hpp"
#include "Random.hpp"
#include "Texture.hpp"

class ArrowSlitRule : public Rule {
    public:
        ArrowSlitRule (int index, CSymbol *pred);

        void execute(Grammar &grammar, Context &context, Shape &shape);

    private:
        Texture *slitTexture;
};

#endif

