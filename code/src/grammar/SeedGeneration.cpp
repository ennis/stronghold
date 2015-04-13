#include "SeedGeneration.hpp"
#include "Random.hpp"
#include "ShowRule.hpp"
#include "Terrain.hpp"
#include "Cuboid.hpp"
#include "Grammar.hpp"
#include "Polygon2D.hpp"
#include <iostream>
#include <glm/gtx/intersect.hpp>
#include "Intersections.hpp"


Rectangle2D generateBuilding(Context& context, BuildingSeed bs, glm::vec2 dim) {
    glm::vec3 pos = glm::vec3(bs.location.x, context.terrain->getHeight(bs.location), bs.location.y);
    CoordSystem cs;
    cs.position = pos;
    Rectangle2D rect(cs, dim.x, dim.y);

    return rect;
}


// génère un point au hasard dans un polygone
static glm::vec2 randomPoint(Polygon2D const &poly)
{
	glm::vec2 a, b;
	poly.getBounds(a, b);

	float x;
	float y;
	
	do {
		// algo de rejet
		x = GRandom.RandomNumber(a.x, b.x);
		y = GRandom.RandomNumber(a.y, b.y);
	} while (!poly.inside_polygon(glm::vec2(x, y)));

	return glm::vec2(x, y);
}

static float calculateAttractionPotential(
	glm::vec2 const &position,
	std::vector<BuildingSeed> const &particles)
{
	float acc = 0.f;
	const int n = particles.size();

	for (int i = 0; i < n; ++i) {
		float d = glm::distance(position, particles[i].location);
		//std::clog << "dist " << d << '\n';
		if (d > 0.01f)
			acc += -10.f * (powf(10.0f / d, 6) - powf(10.0f / d, 4));
	}

	return acc;
}

static void debugSeeds(Context &context, Stronghold &stronghold, std::vector<BuildingSeed> const &seeds)
{
	for (std::vector<BuildingSeed>::const_iterator it = seeds.begin(); it != seeds.end(); ++it) {
		BuildingSeed const &s = *it;
		Cuboid c = Cuboid(
			glm::vec3(s.location.x, context.terrain->getHeight(s.location), s.location.y),
			glm::vec3(1.f, 1.f, 1.f));
		createCuboidRenderable(c, context);
	}
}


void generateBuildingSeeds(Context &context, Stronghold &stronghold, Polygon2D const &inner, std::vector<BuildingSeed> &out)
{
	// habitations
	const int numSeeds = 5000;
	//const float attractionThreshold = 10000;
	float threshold = 0.95f;

	std::vector<BuildingSeed> &seeds = stronghold.seeds;
	
	// test LJ
	for (int i = 0; i < numSeeds; ++i) {
		// new seed
		BuildingSeed s;
		s.location = randomPoint(inner);
		s.attraction = calculateAttractionPotential(s.location, seeds);
		s.flatness = flatnessScoreWall(s.location, 4.0f, context);

		// 
		// Si le terrain n'est pas assez plat, on rejette la graine
		if (s.flatness > 0.3f) {
			continue;
		}

		//
		// La maison est trop proche d'autres habitations: rejet
		if (s.attraction < 0.f) {
			// reject
			continue;
		}

        // TODO: autres formes
        Rectangle2D house = generateBuilding(context, s, glm::vec2(7, 7));
        bool accepted = true;

        for (int j = 0; j < seeds.size(); j++) {
            Rectangle2D house2 = generateBuilding(context, seeds[j], glm::vec2(7, 7));
            if (intersect_houses(house, house2)) {
                accepted = false;
                break;
            }
        }

        if (!accepted) {
            continue;
        }


		// si la l'attraction n'est pas suffisante, il y a quand même une chance pour qu'un nouveau bâtiment apparaisse
		if (s.attraction < 0.5f) {
			if (GRandom.RandomNumber(0, 1) > threshold) {
				seeds.push_back(s);
			}
		} else {
			//
			// La graine a passé le test de 'platitude' et a une force d'attraction suffisante 
			seeds.push_back(s);				
		}


	}

	std::clog << "Created " << seeds.size() << " seeds\n";

	/* debugSeeds(context, stronghold, seeds); */

	// filtrage
}
