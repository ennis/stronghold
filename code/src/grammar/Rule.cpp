#include "Rule.hpp"
#include <iostream>

Rule::Rule(int i, Symbol const *predecessor) : weight(10), index(i), predecessor(predecessor), lc(NULL)
{
}

Rule::Rule() : weight(10), index(-1), predecessor(NULL), lc(NULL)
{
}

int Rule::getIndex() const 
{
	return index;
}

void Rule::setPredecessor(Symbol const *pred)
{
    predecessor = pred;
}

void Rule::setWeight (float weight) {
    this->weight = weight;
}

Symbol const *Rule::getPredecessor() const
{
  return predecessor;
}

float Rule::getWeight () const {
    return weight;
}


void Rule::addSuccessor(Symbol const *successor)
{
	successors.push_back(successor);
}

void Rule::printRule() const {
	std::list<Symbol const *>::const_iterator it = successors.begin();

	std::cout << index << ": " << getPredecessor()->name << " -> ";
	while (it != successors.end()) {
		std::cout << (*it)->name << " ";
		it++;
	}
    if (weight != -1) {
        std::cout << " : " << weight << std::endl;
    }
	std::cout << ";" << std::endl;
}

void Rule::addConstraint (Constraint *c) {
    lc->addConstraint(c);
}

bool Rule::verifyConstraints (Shape const& shape, Context &ct) {
    if (lc == NULL)
        return true;

    return (lc->verifyConstraints(shape, ct));
}

void Rule::setListConstraints (ListConstraints *l) {
    lc = l;
}

