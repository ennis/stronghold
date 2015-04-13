#include "ListConstraints.hpp"

void ListConstraints::addConstraint (Constraint *c) {
    constraints.push_back(c);
}

bool ListConstraints::verifyConstraints (Shape const& shape, Context &ct) {
    std::list<Constraint *>::iterator it;
    bool res = true;

    for (it = constraints.begin(); it != constraints.end(); it++) {
        res = res && (*it)->verify(shape, ct);
    }

    return res;
}
