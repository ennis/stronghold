#include <CityRoads.hpp>
#include <Cuboid.hpp>
#include <Shape.hpp>
#include <FieldFunction.hpp>
#include <GradientFunction.hpp>
#include <DistanceToPointsFunction.hpp>
#include <ManhattanDistanceToPointsFunction.hpp>
#include <DiagonalDistanceToPointsFunction.hpp>
#include <BorderFunction.hpp>
#include <PathReuseFunction.hpp>
#include <Grammar.hpp>
#include <TextureManager.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>


void CityRoads::create(Context &context){
	Cuboid door = context.strongholds[context.currentStronghold].walls[context.strongholds[context.currentStronghold].door_wall];
	CoordSystem cs = door.getCoordSystem();
	door_pos = cs.transform(glm::vec2(door.dimensions.x / 2, 0));

	glm::vec2 size = context.terrain->getSize();
	board = new CaseMatrix(size.x, size.y);
	step = 5;

	createMainRoads(context);
	createRoadsToHouses(context);

	createPathTreeFromFrequencies(context);
	initRender(context);
}

	
void CityRoads::createMainRoads(Context &context){
	glm::vec2 dungeon_pos(context.strongholds[context.currentStronghold].dungeon_pos);

	Polygon2D base = context.strongholds[context.currentStronghold].base;
	std::vector<glm::vec2> corners = base.getPoints();


	/* Fonctions de coût de deplacement */
	std::list<FieldFunction *> main_road_functions;
	GradientFunction gradf(context.terrain, true, 3, 6.f, 1.f);
	main_road_functions.push_front(&gradf);
	BorderFunction borderf(&base, 0.f, 1000000.f);
	main_road_functions.push_front(&borderf);
	/* Reutilisation de chemins */
	PathReuseFunction pathrf(board, 30, 0.5f);
	main_road_functions.push_front(&pathrf);

	// CHEMIN entre la porte et le donjon
	/* Fonction d'heuristique */
	ManhattanDistanceToPointsFunction distf(getCut(dungeon_pos, step), 1, 2.5f, 2.5f, 1.f);
	/* Calcul du chemin porte->donjon et mise à jour de la matrice de frequentation*/
	computeMinCostPath(board,step, door_pos, dungeon_pos, context, &distf, main_road_functions);

	// CHEMINS entre le donjon et les tours (les coins en attendant)
	// CHEMINS entre la porte et les tours (les coins en attendant)
	for (std::vector<glm::vec2>::const_iterator it = corners.begin(); it != corners.end(); it++){
		/* Fonction d'heuristique */
		ManhattanDistanceToPointsFunction distf_corner(getCut(*it, step), 1, 2.5f, 2.5f, 1.f);

		/* Calcul des chemins et mise à jour de la matrice de frequentation */
		computeMinCostPath(board, step, dungeon_pos, *it, context, &distf_corner, main_road_functions);
		computeMinCostPath(board, step, door_pos, *it, context, &distf_corner, main_road_functions);
	}

	// CHEMINS entre des tours consecutives: chemin de ronde
	int nbTowers = corners.size();
	for (int k = 0; k < nbTowers; k++){
		/* Fonction d'heuristique */
		ManhattanDistanceToPointsFunction distf_corner_k(getCut(corners[(k + 1) % nbTowers], step), 1, 2.5f, 2.5f, 1.f);
		/* Calcul de chemin */
		computeMinCostPath(board, step, corners[k], corners[(k + 1) % nbTowers], context, &distf_corner_k, main_road_functions);
	}

}


// Touver le point de targets le plus proche de v (en distance euclidienne)
// (Targets non-nul)
glm::vec2 findClosest(glm::vec2 v, std::vector<glm::vec2> targets){
	float current_distance;
	glm::vec2 closest = targets.front();
	float min_distance=glm::distance(v,closest);

	for (std::vector<glm::vec2>::const_iterator it = targets.begin() +1; it != targets.end(); it++){
		current_distance = glm::distance(v, *it);

		if (current_distance < min_distance){
			min_distance = current_distance;
			closest = *it;
		}
	}

	return closest;
}


// TODO: si il existe un chemin assez proche, on n'en cree pas de nouveau
void CityRoads::createRoadsToHouses(Context &context){
	std::vector<BuildingSeed> &buildings_seeds = context.strongholds[context.currentStronghold].seeds;
	std::vector<glm::vec2> houses;
	for (int k = 0; k < buildings_seeds.size(); k++){
		houses.push_back(buildings_seeds[k].location);
	}

	/* Fonction de coût de deplacement */
	std::list<FieldFunction *> road_functions;
	GradientFunction gradf(context.terrain, true, 3, 6.f, 1.f);
	road_functions.push_front(&gradf);
	BorderFunction borderf(&(context.strongholds[context.currentStronghold].base), 0.f, 1000000.f);
	road_functions.push_front(&borderf);
	PathReuseFunction pathf(board, 30, 0.5f);
	road_functions.push_front(&pathf);

	//float threshold = 5.f;
	for (int k = 0; k < houses.size(); k++){
		// On crée une nouvelle heuristique pour le point que l'on souhaite rejoindre
		ManhattanDistanceToPointsFunction distf(getCut(houses[k],step), 1, 2.5f, 2.5f, 1.f);

		// On calcule le chemin qui va de la porte à la maison
		Path *path = computeMinCostPath(board, step, door_pos, houses[k], context, &distf, road_functions);

		// On enregistre le chemin pour ces maisons
		buildings_seeds[k].closest_road_point = path->points.back();
	}
}

void CityRoads::recursiveCreatePathTree(Context &context, PathTree *tree, glm::vec2 pos, glm::vec2 pos_prec){
	(*board)(pos).found = true;
	tree->path->points.push_back(pos);

	int nb_new = 0, nb_old = 0;
	glm::vec2 loop_point; // pour faire la liaison dans une boucle

	// Continuer de construire le chemin en cours
	std::vector<glm::vec2> neighbors = context.terrain->getReachableNeighbors(pos.x, pos.y, step);
	for (int k = 0; k < neighbors.size(); k++){
		int freq = (*board)(neighbors[k]).frequency;
		if (freq != 0) {
			if (!((*board)(neighbors[k]).found)){
				nb_new++;
				// Si ce voisin a été utilisé lors du pathfinding
				// et s'il n'a pas déja été trouvé par recursiveCreatePathTree
				if (freq == (*board)(pos).frequency){
					recursiveCreatePathTree(context, tree, neighbors[k], pos);
					break;
				}
				else{
					//Creation de nouveau chemin
					PathTree *child_tree = new PathTree();
					child_tree->path = new Path();
					child_tree->path->value = freq;

					// linking dans l'arbre
					child_tree->parent = tree;
					tree->children.push_back(child_tree);

					// pour la liasion au niveau du tracé
					child_tree->path->points.push_back(pos);

					recursiveCreatePathTree(context, child_tree, neighbors[k], pos);
				}
			}
			else{ // point deja traité avant
				nb_old++;
				if (freq != (*board)(pos_prec).frequency){
					loop_point = neighbors[k];
				}
			}
		}
	}

	if (nb_new == 0 && nb_old > 1){
		tree->path->points.push_back(loop_point);
	}
}

void CityRoads::createPathTreeFromFrequencies(Context &context){
	//glm::vec2 start = getCut(door_pos, step);
	glm::vec2 dungeon_pos(context.strongholds[context.currentStronghold].dungeon_pos);
	glm::vec2 start = getCut(dungeon_pos, step);

	roads = new PathTree();
	roads->path = new Path();
	roads->path->value = (*board)(start).frequency;

	// point de depart : porte
	recursiveCreatePathTree(context, roads, start, start);
}


void CityRoads::initRender(Context &context)
{
	if (renderInited)
		return;

	roadTexture = TextureManager::Instance().loadTexture("textures/stone-texture.jpg");
	renderPath(context, roads);
}

void CityRoads::render(RenderData const &data)
{
	mesh.render(data);
}


static inline float halfAngle(glm::vec2 const &v1, glm::vec2 const &v2)
{
	return glm::orientedAngle(
		glm::normalize(v2),
		glm::normalize(v1)) / 2;
}

void CityRoads::renderPath(Context &context, PathTree * tree)
{
	// render current path
	if (tree == NULL || tree->path == NULL) {
		return;
	}

	// 
	Path &p = *(tree->path);

	std::vector<float> angles(p.points.size());
	angles[0] = M_PI / 2.f;

	// generate angles
	for (int i = 1; i < p.points.size() - 1; ++i) {
		glm::vec2 p1 = p.points[i - 1];
		glm::vec2 p2 = p.points[i];
		glm::vec2 p3 = p.points[i + 1];
		glm::vec2 d1 = glm::normalize(p1 - p2);
		glm::vec2 d2 = glm::normalize(p3 - p2);
		glm::vec2 n1 = glm::normalize(d1 + d2);

		angles[i] = halfAngle(d1, d2);
	}
	angles[p.points.size()-1] = M_PI / 2.f;

	// generate polygons
	for (int i = 0; i < p.points.size() - 1; ++i) {
		glm::vec2 p1 = p.points[i];
		glm::vec2 p2 = p.points[i + 1];
		glm::vec2 d = glm::normalize(p2 - p1);
		glm::vec2 n = glm::vec2(-d.y, d.x);

		float h = std::min(p.value / 5.f, 2.f);

		glm::vec2 pA = p1 + h / tanf(angles[i]) * d + h * n;
		glm::vec2 pB = p1 - h / tanf(angles[i]) * d - h * n;
		glm::vec2 pC = p2 - h / tanf(angles[i+1]) * d + h * n;
		glm::vec2 pD = p2 + h / tanf(angles[i+1]) * d - h * n;

		glm::vec3 pAA = glm::vec3(pA.x, context.terrain->getHeight(pA) + 1.f, pA.y);
		glm::vec3 pBB = glm::vec3(pB.x, context.terrain->getHeight(pB) + 1.f, pB.y);
		glm::vec3 pCC = glm::vec3(pC.x, context.terrain->getHeight(pC) + 1.f, pC.y);
		glm::vec3 pDD = glm::vec3(pD.x, context.terrain->getHeight(pD) + 1.f, pD.y);

		// generate triangles
		mesh.beginBatch(roadTexture);
		mesh.addTriangle(pAA, pCC, pBB);
		mesh.addTriangle(pBB, pCC, pDD);
		mesh.endBatch();
	}

	for (std::vector<PathTree*>::const_iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		renderPath(context, (*it));
	}
}
