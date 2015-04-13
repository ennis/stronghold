#ifndef _GRAMMAR_
#define _GRAMMAR_

#include "Rule.hpp"
#include "SymbolTable.hpp"
#include "Renderable.hpp"
#include "MeshRenderable.hpp"
#include "Terrain.hpp"
#include "Polygon2D.hpp"
#include "Path.hpp"
#include "SeedGeneration.hpp"
#include <CityRoads.hpp>

#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>

class Cuboid;

struct Stronghold {
	Polygon2D base;
	CityRoads *roads;
    std::vector<Cuboid> walls;
	std::vector<BuildingSeed> seeds;
    int door_wall;
    glm::vec2 dungeon_pos;
};

//
// Contexte d'exécution
//
struct Context
{
	// liste d'objets renderables
	std::list<Renderable*> renderables;
	// Ici: mettre des trucs 

	// batch renderable
	MeshRenderable renderBatch;

	// - référence vers la (les?) heightmap pour la vérification des contraintes
	// - autres?
	Terrain *terrain;

	int currentStronghold;
    // Liste de forteresses
    std::vector<Stronghold> strongholds;
};


/*
 * Classe définissant la grammaire
 */
class Grammar
{
public:
	Grammar();
	~Grammar();

	//
	// Supprime l'ensemble des règles
	void clear();

	//
	// Ajout d'une règle avec l'index i dans le dictionnaire des règles
	// NB: l'instance de la classe grammar devient propriétaire de la règle 
	// (i.e. elle est en charge d'appeler delete sur la règle)
	void addRule(int i, Rule *rule);

	//
	// Retourne true si la règle d'index i a été définie, false sinon
	bool ruleExists(int i) const;
	
	//
	// Retourne une référence vers la règle d'index i.
	// Lance un exception si la règle d'index i n'existe pas
	Rule &getRule(int i);

	//
	// Débogage: affichage des règles
	void printRules();

	//
	// Ajout d'un symbole dans la table des symboles
	// Si le symbole est déjà présent, renvoie l'objet correspondant
	CSymbol *addSymbol(const char *nt, SymbolType symType);

	//
	// Obtention d'un symbole
	// throw runtime_error si symbole non trouvé
	CSymbol *getSymbol(const char *sym) const;

	//
	// Obtention de la table des symboles
	// (plutôt: utiliser les deux méthodes ci-dessus)
	SymbolTable &getSymbolTable() {
		return symbolTable;
	}

	//
	// Point d'entrée de la génération
	void generate(const char *axiom, Context &context);

	//
	// A partir d'un symbole non-terminal, séléctionne la règle suivante 
	// à exécuter et l'exécute, en lui passant le volume en paramètre.
	// (NB: non déterministe)
	//
	// TODO si on veut une exécution pas-à-pas, il faudrait effectuer les 
	// dérivations en largeur d'abord (breadth-first search)
	void resolve(CSymbol *pred, Context &context, Shape &shape);

  void setRules ();

private:
	SymbolTable symbolTable;

	//
	// la table des règles
	//
	// Note: les règles non déterministes ont le même prédecesseur
	//
	// Par exemple:
	// P -> wall
	// P -> tower
	//
	// Ici: les deux règles auront le même poids et auront la même chance d'être sélectionnés
	// Lors de l'exécution, il faudra donc trouver toutes les règles ayant le même prédécesseur dans la table
	// et en choisir une au hasard
	// XXX Une autre solution aurait été de séparer 'Rule' et 'Successeurs', mais je [Alex] trouvais que ça compliquait
	// la grammaire et les classes. Qu'en pensez-vous?
	std::map<int, Rule*> rules;

	//
	// Obtention de la liste de toutes les règles ayant 'pred' comme prédécesseur
	void match(CSymbol *pred, std::vector<Rule*>& matching, std::vector<float>& law);
};

#endif
