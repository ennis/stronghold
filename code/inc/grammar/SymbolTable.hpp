#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <map>
#include <string>

enum SymbolType {
	ST_NonTerminal,
	ST_Terminal
};

//
// Symbole: un nom avec un type associé 
// TODO symboles prédéfinis
class Terminal;
struct Symbol
{
	std::string name;
	// XXX en fait, les seuls symboles que l'utilisateur pourra définir sont les non-terminaux...
	SymbolType type;
};

typedef Symbol const CSymbol;

//
// Table des symboles
// Fortement inspirée de ce qu'on avait fait pour le projet GL
// Plus pratique que comparer des strings
class SymbolTable
{
public:
	//
	// Ajout d'un symbole dans la table des symboles
	// Si le symbole est déjà présent, renvoie l'objet correspondant
	CSymbol *addSymbol(const char *nt, SymbolType symType);

	//
	// Obtention d'un symbole
	// throw runtime_error si symbole non trouvé
	CSymbol *getSymbol(const char *sym) const;

	// Débogage: affichages des symboles
	void printSymbols() const;

private:
	std::map<std::string, CSymbol*> symbolTable;
};

#endif