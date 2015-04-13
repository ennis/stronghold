#include "DoorGeneration.hpp"
#include "Terrain.hpp"
#include "Rectangle2D.hpp"
#include "Grammar.hpp"

#include <algorithm>
#include <limits>
#include <iostream>

void generateDoor(Context &context,
                  std::vector<Cuboid> const &walls,
                  int &selectedWall) {
    int n = walls.size();
    std::vector<float> scores(n);

    float r = 100.0f; // Rayon du disque utilisé pour le critère de platitude
    int N = 3; // Nombre de points de la subdivision

    // Calcul du score de chaque mur
    // On prend des points sur les bords des murs
    for (int j = 0; j < n; j++) {
        CoordSystem cs = walls[j].getCoordSystem();
        glm::vec3 dim = walls[j].dimensions;
        Rectangle2D mur(cs.position, cs.rotation, dim.x, dim.z);

        glm::vec3 v[4];
        v[0] = cs.transform(glm::vec3(0, 0, 0));
        v[1] = cs.transform(glm::vec3(mur.width, 0, 0));
        v[2] = cs.transform(glm::vec3(0, 0, mur.height));
        v[3] = cs.transform(glm::vec3(mur.width, 0, mur.height));

        std::vector<std::pair<glm::vec2, glm::vec2> > points(4);
        points[0].first.x = v[0].x;
        points[0].first.y = v[0].z;
        points[0].second.x = v[1].x;
        points[0].second.y = v[1].z;

        points[1].first = points[0].first;
        points[1].second.x = v[2].x;
        points[1].second.y = v[2].z;

        points[2].first = points[0].second;
        points[2].second.x = v[3].x;
        points[2].second.y = v[3].z;

        points[3].first = points[1].second;
        points[3].second = points[2].second;

        for (int i = 0; i < 4; i++) {
            glm::vec2 pt = points[i].first;
            glm::vec2 step = (points[i].second - points[i].first);
            step.x /= N;
            step.y /= N;
            for (int k = 0; k < N; k++) {
                scores[j] += flatnessScoreWall(pt, r, context);
                pt += step;
            }
        }
    }

    // On trouve le minimum
    float score_min = std::numeric_limits<float>::max();
    int index_min;

    for (int i = 0; i < n; i++) {
        if (scores[i] < score_min) {
            score_min = scores[i];
            index_min = i;
        }
    }

    selectedWall = index_min;
}

