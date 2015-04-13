#include "Grammar.hpp"
#include <stdexcept>
#include <DummyShape.hpp>
#include "Random.hpp"
#include <iostream>


Grammar::Grammar() {
}

Grammar::~Grammar() {
  //TODO
}

void Grammar::clear()
{
	rules.clear();
}

void Grammar::addRule(int i, Rule *rule)
{
	std::pair<std::map<int, Rule*>::iterator, bool> ins = 
		rules.insert(std::pair<int, Rule*>(i, rule));
	if (!ins.second) {
		// règle déjà définie
		throw std::runtime_error("Rule already defined");
	}
}

bool Grammar::ruleExists(int i) const 
{
    std::map<int, Rule*>::const_iterator it = rules.find(i);
    return it != rules.end();
}

Rule &Grammar::getRule(int i)
{
	std::map<int, Rule*>::const_iterator it = rules.find(i);
	if (it == rules.end()) {
		throw std::runtime_error("Rule not found");
	}

	return *it->second;
}

void Grammar::printRules()
{
	std::map<int, Rule*>::const_iterator it = rules.begin();

	while (it != rules.end()) {
		it->second->printRule();
		it++;
	}
}

CSymbol *Grammar::addSymbol(const char *nt, SymbolType symType)
{
	return symbolTable.addSymbol(nt, symType);
}

CSymbol *Grammar::getSymbol(const char *sym) const
{
	return symbolTable.getSymbol(sym);
}

void Grammar::match(CSymbol *pred, std::vector<Rule*>& matching, std::vector<float>& law)
{
	std::map<int, Rule*>::const_iterator it = rules.begin();
    float totalWeight = 0;

	while (it != rules.end()) {
		if (it->second->getPredecessor() == pred) {
			matching.push_back(it->second);
            totalWeight += it->second->getWeight();
            law.push_back(it->second->getWeight());
		}
		++it;
	}

    for (int i = 0; i < law.size(); i++) {
        law[i] /= totalWeight;
    }
}

void Grammar::resolve(CSymbol *pred, Context &context, Shape &shape)
{
	std::vector<Rule*> matching;
    std::vector<float> law;

	match(pred, matching, law);
	if (matching.empty()) {
		std::clog << "*** Warning: no production available for " << pred->name << '\n';
		return;
	}

	// remove all rules not satisfying the conditions
	for (std::vector<Rule*>::iterator it = matching.begin(); it != matching.end(); ) {
		if (!(*it)->verifyConstraints(shape, context)) {
			it = matching.erase(it);
		}
		else {
			++it;
		}
	}

    Rule *next;

    if (matching.size() == 1) {
        next = *(matching.begin());
    } else {
        int numNext = GRandom.Simulation(law);

        next = matching[numNext];
    }

    if (next->verifyConstraints(shape, context)) {
        next->execute(*this, context, shape);
    }
}

void Grammar::generate(const char *axiom, Context &context)
{
	CSymbol *axiomSym = getSymbol(axiom);
	// Volume initial?
	DummyShape init;
	resolve(axiomSym, context, init);
}

