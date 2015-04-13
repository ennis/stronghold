#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <Shape.hpp>
#include <SplitEnums.hpp>
#include <vector>

class Cuboid : public Shape
{
public:
	Cuboid() : Shape(ST_Cuboid)
	{}

	Cuboid(CoordSystem const &cs, glm::vec3 const &dimensions) :
		Shape(ST_Cuboid, cs),
		dimensions(dimensions)
	{}


	Cuboid(glm::vec3 const &position, glm::quat const &rotation, glm::vec3 const &dimensions) :
		Shape(ST_Cuboid, position, rotation),
		dimensions(dimensions)
	{}


	// split, en n sous-volumes, de mêmes tailles
	void splitEqually(SplitDirection direction, int numDivisions, std::vector<Cuboid> &out) const;

	// split en ratio.size() sous-volumes, avec proportions données
	void splitProportions(SplitDirection direction, std::vector<float> const &ratios, std::vector<Cuboid> &out) const;

	// split en repettant un liste de longueur un nombre n de fois
	// si n vaut -1, on repette autant de fois que possible
	// On split au debut, milieu ou fin de la longueur selon laquelle on split
	void splitRepeat(SplitDirection direction, std::vector<float> const &lengths, int nb_repeat, SplitAlignment alignment, std::vector<Cuboid> &out) const;

	glm::vec3 dimensions;
};

#endif

