#include <Path.hpp>
#include <Grammar.hpp>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>


Path::Path(){
	value = 0.f;
}

// DEBUG: Affichage d'un ligne brisée
void Path::createPathRenderable(Context &context){
	std::vector<glm::vec3> bottom;
	std::vector<glm::vec3> top;
	float display_height = 20.f;
	for (int i = 0; i < points.size(); i++){
		bottom.push_back(glm::vec3(points[i].x, context.terrain->getHeight(points[i].x, points[i].y), points[i].y));
		top.push_back(glm::vec3(points[i].x, context.terrain->getHeight(points[i].x, points[i].y) + display_height, points[i].y));
	}

	glm::vec2 tc = glm::vec2();
	MeshRenderable *mr = new MeshRenderable();

	for (int i = 1; i < points.size(); i++){
		mr->addTriangle(bottom[i - 1], bottom[i], top[i], tc, tc, tc);
		mr->addTriangle(bottom[i - 1], top[i], top[i - 1], tc, tc, tc);
	}

	mr->setColor(glm::vec4(0, 0.5, 0.5, 0.5));
	context.renderables.push_back(mr);
}


class Compare {
public:
	Compare(CaseMatrix *mat) 
		: mat(mat) {}

	bool operator() (const glm::vec2 &p1, const glm::vec2 &p2) const {
		return ((*mat)(p1).score_H + (*mat)(p1).score_G > (*mat)(p2).score_H + (*mat)(p2).score_G);
	}

private:
	CaseMatrix *mat;
};

//Regarde si v est adjacent à un des point de targets
// Si oui, renvoie le points correspondant
// Si non, renvoie NULL
glm::vec2 *isAdjacent(glm::vec2 v, std::vector<glm::vec2> &targets, float gap){
	for (std::vector<glm::vec2>::iterator it = targets.begin(); it != targets.end(); it++){
		if (glm::distance(v, *it) <= gap){
			return &(*it);
		}
	}
	return NULL;
}

Path *computeMinCostPath(CaseMatrix *board, int step, glm::vec2 start, glm::vec2 goal, Context &context,
						FieldFunction * heuristic_function, std::list<FieldFunction *> cost_function_list)
{
	glm::vec2 first_rally= getCut(start, step); // point de depart sur la grille
	glm::vec2 last_rally = getCut(goal, step); // point d'arrivée, non connu à l'avance

	int width = context.terrain->getSize().x;
	int height = context.terrain->getSize().y;
	
	/* Declaration de la closed_list et de l'open_list */
	std::list<glm::vec2> closed_list;
	Compare comp(board);
	std::vector<glm::vec2> open_list; // rangés du mois au plus prioritaire
	open_list.push_back(first_rally);

	/* Initialisation au point de départ */
	glm::vec2 current_pos = first_rally;
	(*board)(current_pos).score_G = 0.f;
	(*board)(current_pos).score_H = heuristic_function->eval(current_pos);

	/* Pour la sauvegarde d'un bon successeur à la case courante */
	bool good_successor_found = false;
	glm::vec2 good_successor;
	float min_scoreF;
	
	/* Liste des cases voisines accessibles */
	std::vector<glm::vec2> neighbors;
	float score_G_temp = 0.f;
	std::vector<bool> neighbors_in_closed_list;
	std::vector<bool> neighbors_in_open_list;
	bool goal_reached = false;
	int nb_loop = 0;
	/* --------- BOUCLE PRINCIPALE --------- */
	while (!open_list.empty()){
		if (good_successor_found){
			// Si on a sauvegardé un bon successeur, on l'utilise
			current_pos = good_successor;
		}
		else{
			current_pos = open_list.back();
			open_list.pop_back();
		}
		good_successor_found = false;
		good_successor = glm::vec2(-1, -1);
		closed_list.push_front(current_pos);
		min_scoreF = (*board)(current_pos).getScoreF();

		neighbors = context.terrain->getReachableNeighbors(current_pos.x, current_pos.y, step);
		/* Condition de sortie */
		if (std::find(neighbors.begin(), neighbors.end(), last_rally) != neighbors.end()){
			(*board)(last_rally).parent = current_pos;
			goal_reached = true;
			break;
		}

		neighbors_in_closed_list.resize(neighbors.size());
		neighbors_in_open_list.resize(neighbors.size());

		for (int k = 0; k < neighbors.size(); k++){
			// calcul du score_G du voisin traité
			score_G_temp = (*board)(current_pos).score_G;

			for (std::list<FieldFunction *>::const_iterator fit = cost_function_list.begin(); fit != cost_function_list.end(); fit++)
			{
				score_G_temp += (*fit)->eval(neighbors[k]);
				;
			}

			// On regarde si le voisin appartient aux listes ouvertes et fermées
			neighbors_in_closed_list[k] = (std::find(closed_list.begin(), closed_list.end(), neighbors[k]) != closed_list.end());
			neighbors_in_open_list[k] = (std::find(open_list.begin(), open_list.end(), neighbors[k]) != open_list.end());

			if (neighbors_in_closed_list[k] || neighbors_in_open_list[k]){ // Case déja trouvée auparavant
				if (score_G_temp < (*board)(neighbors[k]).score_G){
					// Mise a jour : on a trouvé un meilleur acces a ce voisin
					(*board)(neighbors[k]).parent = current_pos;
					(*board)(neighbors[k]).score_G = score_G_temp;
				}
			}
			else{ // Case jamais trouvée auparavant
				(*board)(neighbors[k]).score_H = heuristic_function->eval(neighbors[k]);
				(*board)(neighbors[k]).parent = current_pos;
				(*board)(neighbors[k]).score_G = score_G_temp;
			}

			// On regarde si le voisin courrant est un bon successeur
			// On cherche à obtenir le meilleur bon successeur possible
			if (!neighbors_in_closed_list[k]
				&& (*board)(neighbors[k]).getScoreF() < min_scoreF){
				min_scoreF = (*board)(neighbors[k]).getScoreF();
				good_successor_found = true;
				good_successor = neighbors[k];
			}
		}

		// Ajout dans l'open_list
		for (int k = 0; k < neighbors.size(); k++){
			if (!neighbors_in_closed_list[k] && !neighbors_in_open_list[k]){
				// Ajout seulement si c'est une case jamais visitée, 
				// et que l'on ne la sauvegarde pas pour le tour suivant
				if (!good_successor_found || (neighbors[k] != good_successor)){
					open_list.push_back(neighbors[k]);
				}
			}
		}

		//std::cout << "TRACE: open_list.size(): " << open_list.size() << std::endl; // TRACE

		// On trie l'open list
		// Cela est necessaire seulement si on pas réussi à trouvé un bon successeur
		if (!good_successor_found){
			std::sort(open_list.begin(), open_list.end(), comp);
		//std::cout << "TRACE: tri effectué" << std::endl;  // TRACE
		}
		nb_loop++; // pour une trace
	} /* Fin de la boucle principale */

	/* std::cout << "TRACE: Path computed. Number of loops: " << nb_loop << std::endl;  // TRACE */

	if (!goal_reached){
		return NULL;
	}


	// On cree le chemin le plus court en "remontant les parents" depuis la position finale
	Path *path = new Path();

	current_pos = last_rally;
	while (current_pos != first_rally){
		path->points.push_back(current_pos);
		(*board)(current_pos).frequency++;

		current_pos = (*board)(current_pos).parent;
	}
	path->points.push_back(first_rally);
	(*board)(first_rally).frequency++;

	std::reverse(path->points.begin(), path->points.end());
	return path;
}


PathTree *recursiveFindContainer(PathTree* tree, glm::vec2 v, bool *found){
	if (tree == NULL || tree->path == NULL){
		return NULL;
	}
	for (std::vector<glm::vec2>::const_iterator path_it = tree->path->points.begin();
		path_it != tree->path->points.end(); path_it++){
		if (*path_it == v){
			*found = true;
			return tree;
		}
	}
	for (std::vector<PathTree *>::const_iterator child_ite = tree->children.begin();
		child_ite != tree->children.end(); child_ite++){
		PathTree *res = recursiveFindContainer(*child_ite, v, found);
		if (found){
			return res;
		}
	}
	return NULL;
}


PathTree * findBestRoadContainingPoint(PathTree* tree, glm::vec2 v){
	bool found = false;
	return recursiveFindContainer(tree, v, &found);
}

glm::vec2 getCut(glm::vec2 v, int step){
	int vix = (int)v.x;
	int viy = (int)v.y;
	int v2x = vix - (vix % step);
	int v2y = viy - (viy % step);

	return glm::vec2(v2x, v2y);
}
