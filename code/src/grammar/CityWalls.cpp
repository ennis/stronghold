#include <CityWalls.hpp>
#include <cmath>
#include <Polygon2D.hpp>
#include <Rampart.hpp>
#include <Grammar.hpp>
#include <algorithm>
#include <Rectangle2D.hpp>
#include <Prism.hpp>
#include <Cuboid.hpp>
#include <DoorGeneration.hpp>
#include <iostream>

// Trouver les extrema locaux dans le vecteur d'entrée, avec un pas de discretisation step
// Renvoie un vecteur d'abscisse des extrema locaux
std::vector<int> find_extrema(std::vector<float> input, float step){
	std::vector<float> derivative(input.size() - 2);
	for (int i = 1; i < input.size() - 1; i++){
		derivative[i-1] = (input[i + 1] - input[i - 1]) / (2.0f * step);
	}

	std::vector<int> res;
	float threshold = 0.04; // empirique
	for (int i = 0; i < derivative.size(); i++){
		if (std::abs(derivative[i]) < threshold
			&& derivative[i]!=0.0f) { // bricolage
			res.push_back(i+1);
		}
	}

	return res;
}

void CityWalls::execute(Grammar &grammar, Context &context, Shape &shape)
{
	if (shape.getShapeType() != ST_Polygon) {
		// La règle ne s'applique que sur un polygone
		throw std::runtime_error("CityWalls can only be used on Polygon primitives");
	}
	
	Polygon2D &poly = static_cast<Polygon2D&>(shape);
	std::vector<glm::vec2> const &points = poly.getPoints();
	const int n = points.size();
	std::vector<Rectangle2D> walls(n);
	std::vector<Prism> corners(n);

	context.strongholds.push_back(Stronghold());
	context.currentStronghold = context.strongholds.size() - 1;
	context.strongholds[context.currentStronghold].base = poly;

	// stockage des cotés: vecteur orienté points[i]-> points[i+1]
	std::vector<glm::vec2> sides;
	for (int i = 0; i < n; i++){
		sides.push_back(glm::vec2(points[(i+1)%n] - points[i]));
	}

	//============================================
	// génération des murs et des coins
	for (int i = 0; i < n; ++i) {
		//murs
		glm::vec3 vector3d = glm::vec3(sides[i].x, 0, sides[i].y);
		float length = glm::length(vector3d); //calcul de la longueur
		vector3d = glm::normalize(vector3d); //calcul de la rotation du mur
		CoordSystem cs = CoordSystem::fromVectors(glm::vec3(points[i].x, 0, points[i].y),
				vector3d, glm::vec3(-vector3d.z,0,vector3d.x));
		walls[i] = Rectangle2D(cs.position, cs.rotation, length, width);

		//coins
		Polygon2D corner_polyg2D;
		corner_polyg2D.addPoint(points[i]);
		glm::vec2 side_next_rectangle = glm::normalize(glm::vec2(-sides[i].y, sides[i].x));
		corner_polyg2D.addPoint(points[i]+width*side_next_rectangle);
		glm::vec2 side_prec_rectangle = glm::normalize(glm::vec2(-sides[((i-1)+n)%n].y, sides[((i-1)+n)%n].x));
		corner_polyg2D.addPoint(points[i]+ width * side_prec_rectangle);
		corners[i] = Prism(corner_polyg2D, height, glm::vec3(points[i].x, context.terrain->getHeight(points[i]), points[i].y));
        corners[i].tower = true;
	}


	// Séparation des Rectangle2D en fonctions de la pente
	std::vector<Rectangle2D> split_walls;
	float length_min_section = 5.0; //Taille minimum d'un section de muraille

	//============================================
	// Pour chaque Rectangle2D, on va chercher la liste des points de pente brusque
	// Et subdiviser les Rectangle2D sur ces points
	for (int i = 0; i < n; i++) {
		glm::vec2 side_direction = glm::normalize(sides[i]);
		float side_length = glm::length(sides[i]);
		std::vector<float> laplacians;
		
		for (float s = 0.0; s < side_length; s ++){
			laplacians.push_back(context.terrain->getSmoothLaplacian(points[i] + s * side_direction));
		}
		std::vector<int> extrema = find_extrema(laplacians, 1.0);
		//TRACE
		//std::clog << "extrema.size(): " << extrema.size() << std::endl; 

		if (extrema.size()>0 && side_length >= length_min_section){ //On ajoute les rectangles splités dans la nouvelle liste de Rectangle2D
			float length;
			// On change la position des points de section
			// afin d'avoir des morceaux de taille > length_min_section
			// On insert aussi les extremités au debut et à la fin
			std::vector<int>::iterator ite = extrema.begin();
			extrema.insert(ite, 0);
			std::vector<float> section_points;
			section_points.push_back(0.f);
			float last_extrema = (float)extrema.front();
			for (int j = 1; j < extrema.size(); j++){
				length = extrema[j] - extrema[j-1];
				if (length >= length_min_section){
					section_points.push_back(extrema[j]);
					last_extrema = extrema[j];
				}
			}
			section_points.push_back(side_length);
			// Attention, on ne considere pas ici la longueur du dernier morceau

			//TRACE
			//std::clog << "On ajuste les longueurs: " << section_points.size() -2 << std::endl;

			// Creation des rectangles
			CoordSystem csys = walls[i].getCoordSystem();
			for (int j = 1; j < section_points.size(); j++){
				length = section_points[j] - section_points[j-1];
				split_walls.push_back(
					Rectangle2D(csys.position + glm::vec3(side_direction.x, 0, side_direction.y)*section_points[j-1],
								csys.rotation, length, width));
			}
		}
		else{ // On n'a pas de split à faire
			split_walls.push_back(walls[i]);
		}
	}


	// Offset des Hauteurs des Ramparts (initialisées à 0.0)
	std::vector<float> walls_height_offset(split_walls.size(),0.0); 
	float max_slope = 0.6f; // en valeur absolue

	// Gestion de la pente :
	//		Si la pente est trop elevée (en valeur absolue)
	//		on l'adoucie en ajustant la hauteur des murs
	float current_slope;
	float current_length;
	for (int i = 0; i < split_walls.size(); ++i) {
		CoordSystem csys = split_walls[i].getCoordSystem();
		CoordSystem csys_next = split_walls[(i + 1) % (split_walls.size())].getCoordSystem();
		current_length = glm::length(csys_next.position - csys.position);
		float terrain_height_A = context.terrain->getHeight(csys.position.x, csys.position.z);
		float terrain_height_B = context.terrain->getHeight(csys_next.position.x, csys_next.position.z);
		float height_A = terrain_height_A + walls_height_offset[i];
		float height_B = terrain_height_B + walls_height_offset[(i + 1) % (split_walls.size())];
		current_slope = abs(height_B - height_A) / current_length;
		//std::cout << "current_slope= " << current_slope << std::endl; // TRACE

		if (current_slope > max_slope){
			if (height_A > height_B){ // ça descend trop
				walls_height_offset[(i + 1) % (split_walls.size())] = height_A - terrain_height_B - current_length * max_slope;
			}
			else{ // ça monte trop
				walls_height_offset[i] = height_B - terrain_height_A - current_length * max_slope;
			}
		}
	}
	
	//============================================
	// Création des objets Ramparts
	std::vector<Cuboid> ramparts;
	for (int i = 0; i < split_walls.size(); ++i) {
		CoordSystem csys = split_walls[i].getCoordSystem();
		CoordSystem csys_next = split_walls[(i + 1) % (split_walls.size())].getCoordSystem();

		float skirt = 50.f;
		float h1 = context.terrain->getHeight(csys.position.x, csys.position.z) - skirt
			+ walls_height_offset[i];
		float h2 = context.terrain->getHeight(csys_next.position.x, csys_next.position.z) - skirt
			+ walls_height_offset[(i + 1) % (split_walls.size())];
		float d = glm::distance(csys.position, csys_next.position);

		csys.position.y = h1;
		csys.shear = (h2 - h1) / d;

		context.strongholds[context.currentStronghold].walls.push_back(Cuboid(csys, glm::vec3(d, height + skirt, width)));
		ramparts.push_back(Cuboid(csys, glm::vec3(d, height + skirt, width)));
	}

	//============================================
	// Selection du rampart qui va avoir une porte
	int doorWallIndex = 0;
	generateDoor(context, ramparts, doorWallIndex);
    context.strongholds[context.currentStronghold].door_wall = doorWallIndex;

	//============================================
	// Execution des successeurs
    if (successors.size() != 4) {
		throw std::runtime_error("CityWalls operation must have only four successors");
    }

	// Murs intérieurs, coins, intérieur, mur d'entrée
	std::list<CSymbol*>::const_iterator itSucc = successors.begin();

	// Murs intérieurs
	CSymbol *succ = *itSucc++;
	for (int i = 0; i < ramparts.size(); ++i) {
		if (i != doorWallIndex)
			grammar.resolve(succ, context, ramparts[i]);
	}
	
	// Coins
	CSymbol *cornerSucc = *itSucc++;
	for (std::vector<Prism>::iterator it = corners.begin(); it != corners.end(); ++it) {
		grammar.resolve(cornerSucc, context, *it);
	}
	
	// Intérieur
	CSymbol *innerPolygonSucc = *itSucc++;
	grammar.resolve(innerPolygonSucc, context, poly);

	// Door wall
	CSymbol *doorWallSucc = *itSucc++;
	grammar.resolve(doorWallSucc, context, ramparts[doorWallIndex]);
}

//
// Convertit un rampart en cuboides
void CityWalls::generateWalls(
	Context &context, 
	std::vector<Rampart> const &ramparts, 
	std::vector<Cuboid> &walls, 
	std::vector<Cuboid> &wallsTop)
{
	for (std::vector<Rampart>::const_iterator it = ramparts.begin(); it != ramparts.end(); ++it)
	{
		Rampart const &rampart = *it;
		Rectangle2D const &base = rampart.base;

		float ha = rampart.base_ha + rampart.wall_ha;
		float hb = rampart.base_hb + rampart.wall_hb;

		// extract middle cuboid
		float h1 = std::min(rampart.base_ha, rampart.base_hb);
		float h2 = std::min(ha, hb);
		walls.push_back(
			Cuboid(
				rampart.getCoordSystem().position + glm::vec3(0, h1, 0),
				rampart.getCoordSystem().rotation,
				glm::vec3(base.width, h2 - h1, base.height)));

		// discretisation des sections inclinées

		// haut
		CoordSystem cs;
		cs.position = rampart.getCoordSystem().position + glm::vec3(0, h2, 0);
		cs.rotation = rampart.getCoordSystem().rotation;

		//float step = base.width / numDivisions;
		//int nb = numDivisions;
		float step = 5;
		int nb = base.width / step;

		for (int i = 0; i < nb; ++i) {
			// height
			float height = std::max(0.f, ha - hb) + i * (hb - ha) / nb;
			Cuboid top = Cuboid(cs.position, cs.rotation, glm::vec3(step, height, base.height));

			// set constraint flags
			top.index = i + 1;

			wallsTop.push_back(top);
			cs.position += cs.rotate(glm::vec3(step, 0, 0));
		}
	}
}
