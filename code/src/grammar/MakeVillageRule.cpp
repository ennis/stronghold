#include "MakeVillageRule.hpp"
#include "Polygon2D.hpp"
#include "Grammar.hpp"
#include <stdexcept>
#include <iostream>
#include <SeedGeneration.hpp>
#include "BuildingGeneration.hpp"

MakeVillageRule::MakeVillageRule (int index, CSymbol *pred) : Rule(index, pred) {
}

static glm::vec2 find_dungeon (Polygon2D const& poly,
                               Context &context) {
    std::vector<glm::vec2> points = poly.getPoints();
    int n = points.size();
    // Hypothèse: polygone est convexe => barycentre dedans
    glm::vec2 centroid;
    for (int i = 0; i < n; i++) {
        centroid += points[i];
    }
    centroid /= n;
    /* std::cout << "centroid : " << centroid.x << ", " << centroid.y << std::endl; */

    // Equation des faces : ax + by + c = 0
    std::vector<glm::vec3> eqFaces(n);
    for (int i = 0; i < n; i++) {
        glm::vec2 p1 = points[i];
        glm::vec2 p2 = points[(i + 1) % n];

        if (p1.x == p2.x) {
            eqFaces[i].x = 1;
            eqFaces[i].y = 0;
            eqFaces[i].z = -p1.x;
        } else {
            eqFaces[i].x = (p2.y - p1.y) / (p2.x - p1.x);
            eqFaces[i].y = -1;
            eqFaces[i].z = p1.y - eqFaces[i].x * p1.x;
        }
    }

    // Discrétisation
    int N = 6;
    glm::vec2 **discreteFaces;
    discreteFaces = new glm::vec2 *[n];
    for (int i = 0; i < n; i++) {
        discreteFaces[i] = new glm::vec2[N + 2];
    }

    for (int i = 0; i < n; i++) {
        float h = (points[(i + 1) % n].x - points[i].x) / N;

        for (int j = 0; j <= N + 1; j++) {
            if (j == 0) {
                discreteFaces[i][j] = points[i];
            } else if (j == N + 1) {
                discreteFaces[i][j] = points[(i + 1) % n];
            } else {
                float x;
                float y;

                if (eqFaces[i].y == 0) {
                    x = -eqFaces[i].z / eqFaces[i].x;
                    y = points[i].y + h * j;
                } else {
                    x = points[i].y + h * j;
                    y = (-eqFaces[i].z - eqFaces[i].x * x) / eqFaces[i].y;
                }

                discreteFaces[i][j] = glm::vec2(x, y);
            }
        }
    }

    glm::vec2 orig_dungeon;
    float cur_height = -1;
    float step = 0.1f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < N; j++) {
            // Hauteur au niveau du point courant
            float height = context.terrain->getHeight(discreteFaces[i][j]);
            // Droite orthogonale
            glm::vec3 droiteN;
            droiteN.x = eqFaces[i].y;
            droiteN.y = -eqFaces[i].x;
            droiteN.z = -droiteN.x * discreteFaces[i][j].x - droiteN.y * discreteFaces[i][j].y;

            // Normale intérieure
            if (glm::dot(glm::vec2(-droiteN.y, droiteN.x), centroid) >= 0) {
                droiteN.x = -droiteN.x;
                droiteN.y = -droiteN.y;
                droiteN.z = -droiteN.x * discreteFaces[i][j].x - droiteN.y * discreteFaces[i][j].y;
            }

            glm::vec2 pt = discreteFaces[i][j];
            while (poly.inside_polygon(pt)) {
                if (droiteN.y == 0) {
                    pt.y += step;
                    pt.x = -droiteN.z / droiteN.x;
                } else {
                    pt.x += step;
                    pt.y = (-droiteN.z - droiteN.x * pt.x) / droiteN.y;
                }

                //std::cout << "hauteur : " << context.terrain->getHeight(pt) << std::endl;
                //std::cout << "point : " << pt.x << " " << pt.y << std::endl;
                if (context.terrain->getHeight(pt) > cur_height) {
                    orig_dungeon = pt;
                    cur_height = context.terrain->getHeight(pt);
                }
            }
        }
    }

    // Libérations
    for (int i = 0; i < n; i++) {
        delete [] discreteFaces[i];
    }
    delete [] discreteFaces;

    return orig_dungeon;
}

void MakeVillageRule::execute(Grammar &grammar, Context &context, Shape &shape) {
    if (shape.getShapeType() != ST_Polygon) {
        // La règle ne s'applique que sur un polygone
        throw std::runtime_error("make_village can only be used on Prism primitives");
    }

    Polygon2D const& poly = static_cast<Polygon2D const&>(shape);

    // Donjon
    // On place le donjon au point le plus haut dans le polygone
    glm::vec2 orig_dungeon = find_dungeon(poly, context);
    /* std::cout << "Dungeon" << std::endl; */
    /* std::cout << orig_dungeon.x << std::endl; */
    /* std::cout << orig_dungeon.y << std::endl; */
    context.strongholds[context.currentStronghold].dungeon_pos = orig_dungeon;
    Polygon2D dungeon;
    dungeon.addPoint(orig_dungeon);

	// current stronghold
	Stronghold &s = context.strongholds[context.currentStronghold];

	// Building seeds
	generateBuildingSeeds(context, s, poly, s.seeds);

    // Chemin principal
	s.roads = new CityRoads;
	s.roads->create(context);
	context.renderables.push_back(s.roads);

    // Génération des maisons
    std::vector<Cuboid> houses;
    generateBuildings(context, s.seeds, houses);

    // Successeurs
    if (successors.size() != 2) {
        throw std::runtime_error("make_village can only have two successors");
    }

    // Donjon
    CSymbol *succ = *(successors.begin());
    grammar.resolve(succ, context, dungeon);

    // Maisons
    succ = *(++(successors.begin()));
    for (std::vector<Cuboid>::iterator it = houses.begin();
         it != houses.end(); it++) {
        grammar.resolve(succ, context, *it);
    }
}

