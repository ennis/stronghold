#include "BuildDoorRule.hpp"
#include "Grammar.hpp"
#include "Cuboid.hpp"
#include <iostream>

void BuildDoorRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Cuboid) {
        throw std::runtime_error("build_door only works on Cuboid primitives!");
    }

	Cuboid const &doorWall = static_cast<Cuboid const&>(shape);

	// création de la géométrie de la porte

	// TODO selon door_pos
	std::vector<Cuboid> outSplit;
	doorWall.splitEqually(Split_X, 3, outSplit);
	Cuboid &wallSideLeft = outSplit[0];
	Cuboid &wallSideRight = outSplit[2];

	std::vector<Cuboid> splitTerrain;
	std::vector<float> hauteur(2);
	glm::vec3 pos = outSplit[1].getCoordSystem().position;
	
	glm::vec2 posbis = glm::vec2(pos.x,pos.z);

	hauteur[0] = context.terrain->getHeight(posbis)/(outSplit[1].dimensions.y + doorWall.getCoordSystem().position.y);
	
	//Grosse truande !!!
	if (hauteur[0] >= 1)
		hauteur[1] = hauteur[0] - 1;
	else
		hauteur[1] = 1 - hauteur[0];
	outSplit[1].splitProportions(Split_Y, hauteur, splitTerrain);

	std::vector<Cuboid> splitDoor;
	splitTerrain[1].splitEqually(Split_Y, 2, splitDoor);
	Cuboid &doorUp = splitDoor[1];
	Cuboid &doorDown = splitDoor[0];

	std::list<CSymbol*>::const_iterator it = successors.begin();

	CSymbol *wallSideLeftSucc = *it++;
	CSymbol *wallSideRightSucc = *it++;
	CSymbol *doorWallUpSucc = *it++;
	CSymbol *doorWallDownSucc = *it++;

	grammar.resolve(wallSideLeftSucc, context, wallSideLeft);
	grammar.resolve(wallSideRightSucc, context, wallSideRight);
	grammar.resolve(doorWallUpSucc, context, doorUp);
	grammar.resolve(doorWallDownSucc, context, doorDown);
    
}

