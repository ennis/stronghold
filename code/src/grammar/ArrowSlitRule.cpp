#include "ArrowSlitRule.hpp"
#include "Grammar.hpp"
#include "Cuboid.hpp"
#include "TextureManager.hpp"

#include <iostream>
#include <stdexcept>

ArrowSlitRule::ArrowSlitRule (int index, CSymbol *pred) : Rule(index, pred) {
    slitTexture = TextureManager::Instance().loadTexture("textures/slit.jpg");
}

void ArrowSlitRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Cuboid) {
        // La règle ne s'applique que sur un cuboide
        throw std::runtime_error("make_arrow_slit can only be used on Cuboid primitives");
    }

    // Dimensions of the arrow slit
    float height = 7.0f;
    float width = 2.0f;

    Cuboid& mur = static_cast<Cuboid&>(shape);
    glm::vec3 dim = mur.dimensions;
    CoordSystem cs = mur.getCoordSystem();

	if (dim.x < 10.f) {
		// do not generate an arrow slit if the wall not large enough
		CSymbol *succ = *(successors.begin());
		grammar.resolve(succ, context, mur);
		return;
	}

    // Repère en haut du mur
    glm::vec3 vx = glm::vec3(1, 0, 0);
    glm::vec3 vz = glm::vec3(0, 0, 1);
    CoordSystem newCs = CoordSystem::fromVectors(cs, glm::vec3(0, dim.y, 0), vx, vz);
    glm::vec3 pos = newCs.position;

    // On avance la meurtrière pour qu'elle soit devant le mur
    pos.z += dim.z + 1;

    glm::vec3 posTerrain = newCs.transform(newCs.position);
    float diff = posTerrain.y - context.terrain->getHeight(glm::vec2(posTerrain.x, posTerrain.z));
    float h = dim.y + 10;

    // Coordonées de la meurtrière dans le repère locale
    glm::vec3 p1(pos.x, pos.y - h, pos.z);
    glm::vec3 p2(pos.x,  pos.y - h - height, pos.z);

    glm::vec3 p3(pos.x + width, pos.y - h, pos.z);
    glm::vec3 p4(pos.x + width,  pos.y - h - height, pos.z);

    // Coordonnées de textures
    glm::vec2 t1(0, 1);
    glm::vec2 t2(0, 0);
    glm::vec2 t3(1, 1);
    glm::vec2 t4(1, 0);

    p1 = newCs.transform(p1);
    p2 = newCs.transform(p2);
    p3 = newCs.transform(p3);
    p4 = newCs.transform(p4);

    // Texture
    context.renderBatch.beginBatch(slitTexture);
        context.renderBatch.addTriangle(p1, p2, p3, t1, t2, t3);
        context.renderBatch.addTriangle(p3, p2, p4, t3, t2, t4);
    context.renderBatch.endBatch();

    // Successeur
    if (successors.size() != 1) {
        throw std::runtime_error("make_arrow_slit operation must have only one successor");
    }

    CSymbol *succ = *(successors.begin());

    grammar.resolve(succ, context, mur);
}

