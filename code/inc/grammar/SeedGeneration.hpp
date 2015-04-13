#ifndef SEEDGENERATION_HPP
#define SEEDGENERATION_HPP

// g�n�ration des graines d'habitations

#include <Rule.hpp>
#include <vector>

struct Context;
struct Stronghold;
class Polygon2D;

struct BuildingSeed
{
	glm::vec2 location;
	float attraction;
	float flatness;
	// point appartenant � une route le plus proche
	glm::vec2 closest_road_point;
};

// g�n�ration de seeds
void generateBuildingSeeds(Context &context, Stronghold &stronghold, Polygon2D const &inner, std::vector<BuildingSeed> &out);

#endif