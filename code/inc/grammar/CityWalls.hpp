#ifndef CITYWALLS_HPP
#define CITYWALLS_HPP

#include <stdexcept>
#include <Rule.hpp>
#include <Shape.hpp>
#include <Rampart.hpp>
#include <Cuboid.hpp>

//
// Règle de création de la muraille
// Prend un polygone2D et génère des Cuboid
class CityWalls : public Rule
{
public:
	CityWalls(int index, CSymbol *pred, float width = 5.f, float height = 20.f) :
		Rule(index, pred), width(width), height(height)
	{}


	void execute(Grammar &grammar, Context &context, Shape &volume);

private:
	
	void generateWalls(
		Context &context,
		std::vector<Rampart> const &ramparts,
		std::vector<Cuboid> &walls,
		std::vector<Cuboid> &wallsTop);
	
	//
	// épaisseur des murs
	float width;

	//
	// hauteur des murs
	float height;

};

#endif
