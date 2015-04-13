#include "StrongholdSeedRule.hpp"
#include "Grammar.hpp"
#include "Polygon2D.hpp"
#include <stdexcept>
#include <algorithm>
#include <set>
#include <cmath>
#include <iostream>

int StrongholdSeedRule::N = 6;
float StrongholdSeedRule::threshold = 5.0f;
float StrongholdSeedRule::step = 0.5f;

// Algorithme de Graham
// Transcription de : http://en.wikipedia.org/wiki/Graham_scan
int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0; // collinéaire
    return (val > 0)? 1: 2; // non-trigo / trigo
}

static bool glm_equal (glm::vec2 p1, glm::vec2 p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

class CompareConvexHull {
    public:
        CompareConvexHull (glm::vec2 p0) : p0(p0) {};

        bool operator() (glm::vec2 p1, glm::vec2 p2) {
            int o = orientation(p0, p1, p2);
            if (o == 0)
                return (glm::distance(p0, p2) >= glm::distance(p0, p1) );

            return (o == 2);
        }

    private:
        glm::vec2 p0;
};

static std::vector<glm::vec2> convex_hull_2D (std::vector<glm::vec2> & points) {

    int n = points.size();

    // 1. Trouver le point le plus en bas (y le plus petit)
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;

        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // 2. Echanger le premier point avec le point trouvé précédemment
    glm::vec2 tmp;
    tmp = points[0];
    points[0] = points[min];
    points[min] = tmp;

    // 3. Trier les points suivant l'angle qu'ils font avec le premier
    glm::vec2 p0 = points[0];
    CompareConvexHull cmp(p0);
    std::sort(points.begin(), points.end(), cmp);

    // 4. Vecteur contenant les points de l'enveloppe convexe
    std::vector<glm::vec2> out(points.begin(), points.begin() + 3);

    for (int i = 3; i < n; i++) {
        // On enlève les points qui tournent dans le sens non trigo
        while (out.size() >= 2 && orientation(out[out.size()-2], out[out.size()-1], points[i]) != 2) {
            out.erase(out.end() - 1);
        }
        out.push_back(points[i]);
    }

    return out;
}

void repulse (std::vector<glm::vec2> & in,
              float factor) {
    for (int i = 0; i < in.size(); i++) {
        for (int j = 0; j < in.size(); j++) {
            if (j != i) {
                in[i].x -= 10.0f * factor / (glm::distance(in[i], in[j]) * glm::distance(in[i], in[j]));
                in[i].y -= 10.0f * factor / (glm::distance(in[i], in[j]) * glm::distance(in[i], in[j]));
            }
        }
    }
}

void StrongholdSeedRule::execute(Grammar &grammar, Context &context, Shape & shape) {
    if (successors.size() != 1) {
        throw std::runtime_error("stronghold_seed operation must have only one successor");
    }

    Polygon2D poly;

    // Forme initiale
    for (int i = 0; i < faces; i++) {
        glm::vec2 pt;
        pt.x = center.x + radius * cos(2 * i * M_PI / faces);
        pt.y = center.y + radius * sin(2 * i * M_PI / faces);

        poly.addPoint(pt);
    }
    std::vector<glm::vec2> points = poly.getPoints();

    // Evolution :
    // -> on commence par discrétiser chaque face du polygone
    // Equation des faces : ax + by + c = 0
    std::vector<glm::vec3> eqFaces(faces);
    for (int i = 0; i < faces; i++) {
        glm::vec2 p1 = points[i];
        glm::vec2 p2 = points[(i + 1) % faces];

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
    glm::vec2 **discreteFaces;
    discreteFaces = new glm::vec2 *[faces];
    for (int i = 0; i < faces; i++) {
        discreteFaces[i] = new glm::vec2[N + 2];
    }

    for (int i = 0; i < faces; i++) {
        float h = (points[(i + 1) % faces].x - points[i].x) / N;

        for (int j = 0; j <= N + 1; j++) {
            // On discrétise en x
            if (j == 0) {
                discreteFaces[i][j] = points[i];
            } else if (j == N + 1) {
                discreteFaces[i][j] = points[(i + 1) % faces];
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

    // -> pour chaque point, on regarde dans la direction de la normale sortante à ce point
    // jusqu'à tomber sur un point dont la hauteur est différente significativement
    std::vector<glm::vec2> final_points;
    for (int i = 0; i < faces; i++) {
        for (int j = 0; j < N; j++) {
            // Hauteur au niveau du point courant
            float height = context.terrain->getHeight(discreteFaces[i][j]);
            // Droite orthogonale
            glm::vec3 droiteN;
            droiteN.x = eqFaces[i].y;
            droiteN.y = -eqFaces[i].x;
            droiteN.z = -droiteN.x * discreteFaces[i][j].x - droiteN.y * discreteFaces[i][j].y;

            glm::vec2 pt = discreteFaces[i][j];
            do {
                if (droiteN.y == 0) {
                    pt.y += step;
                    pt.x = -droiteN.z / droiteN.x;
                } else {
                    pt.x += step;
                    pt.y = (-droiteN.z - droiteN.x * pt.x) / droiteN.y;
                }
            } while  (abs(height - context.terrain->getHeight(pt)) < threshold);

            std::cout << "point : " << pt.x << "," << pt.y << std::endl;

            final_points.push_back(pt);
        }
    }

    // Libérations
    for (int i = 0; i < faces; i++) {
        delete [] discreteFaces[i];
    }
    delete [] discreteFaces;

    // On enlève les points égaux
    /* final_points.erase(std::unique(final_points.begin(), final_points.end(), glm_equal), final_points.end()); */
    /* // On applique une force de répulsion pour éviter que les points ne soient trop rapprochés */
    /* repulse(final_points, 10.0f); */

    // Construction du nouveau polygone
    std::vector<glm::vec2> pointsPoly = convex_hull_2D(final_points);
    Polygon2D polyf;

    for (std::vector<glm::vec2>::iterator it = pointsPoly.begin(); it != pointsPoly.end(); it++) {
        polyf.addPoint(*it);
    }

    grammar.resolve(*successors.begin(), context, polyf);
}

