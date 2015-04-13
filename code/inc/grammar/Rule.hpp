#ifndef _RULE_
#define _RULE_

#include "ListConstraints.hpp"
#include "Constraint.hpp"
#include "SymbolTable.hpp"
#include <list>

// Forward defs
class Grammar;
struct Context;
class Shape;

/*
 * Classe définissant une règle de la grammaire
 * Une règle est de la forme suivante
 * i: predecessor: cond -> [operation] successor: prob;
 */
class Rule
{
public:
    Rule();
    Rule(int i, Symbol const *predecessor);

    /* Accesseurs à l'index. */
	int getIndex() const;

	/* Mise à jour de l'index de la règle */
	//void setIndex(int i_);

    /* Accesseurs au predecesseur. */
    Symbol const *getPredecessor() const;

    /* Rajout d'un successeur dans la liste des successeurs. */
	void addSuccessor(Symbol const *successor);

    /* Mise à jour du prédécesseur de la règle */
	void setPredecessor(Symbol const *predecessor);

    /* Affiche la règle dans le terminal. */
    void printRule() const;

    /* Mise à jour du poids de la règle */
    void setWeight (float weight);

    float getWeight () const;

	//
	// Ajout d'une contrainte
    void addConstraint (Constraint *c);

    void setListConstraints (ListConstraints *l);
	
	//
	// Vérifie les contraintes 
	// Retourne true si les contraintes sont vérifiées
	// false sinon, et la règle ne sera pas exécutée
    bool verifyConstraints (Shape const& shape, Context &ct);

	//
	// Exécution de la règle
	// Grammar: pointeur vers la grammaire
	// Context: pointeur vers le contexte d'exécution
	// Shape: forme manipulée
	virtual void execute(Grammar &grammar, Context &context, Shape &shape) = 0;

protected:
    //
	// poids
	float weight;

	//
	// index
    int index;

    //
	// Prédécesseur
    Symbol const *predecessor;

    //
	// Successeurs
    std::list<Symbol const*> successors;

    //
    // conditions
    ListConstraints *lc;
};

#endif
