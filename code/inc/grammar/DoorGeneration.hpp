#ifndef DOORGENERATION_HPP
#define DOORGENERATION_HPP

#include <Rule.hpp>
#include <Cuboid.hpp>
#include <vector>

// 
//
void generateDoor(
	Context &context,
	std::vector<Cuboid> const &walls,
	int &selectedWall);


#endif