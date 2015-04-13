#include "Cuboid.hpp"

void Cuboid::splitEqually(SplitDirection direction, int numDivisions, std::vector<Cuboid> &out) const
{
	float dim;
	glm::vec3 dir;
	glm::vec3 size;

	switch (direction) {
	case Split_X:
		dim = dimensions.x;
		dir = glm::vec3(1, 0, 0);
		size = glm::vec3(dim / numDivisions, dimensions.y, dimensions.z);
		break;
	case Split_Y:
		dim = dimensions.y;
		dir = glm::vec3(0, 1, 0);
		size = glm::vec3(dimensions.x, dim / numDivisions, dimensions.z);
		break;
	case Split_Z:
		dim = dimensions.z;
		dir = glm::vec3(0, 0, 1);
		size = glm::vec3(dimensions.x, dimensions.y, dim / numDivisions);
		break;
	}

	float step = dim / numDivisions;

	for (int i = 0; i < numDivisions; ++i) {
		// repère de base
		CoordSystem cs = CoordSystem::fromParent(coordSystem);
		// décalage
		cs.position += i * step * dir;
		Cuboid cuboid(cs, size);
		cuboid.index = i;
		out.push_back(cuboid);
	}

}

void Cuboid::splitProportions(SplitDirection direction, std::vector<float> const &ratios, std::vector<Cuboid> &out) const
{
	float dim;
	glm::vec3 dir;
	glm::vec3 size;

	switch (direction) {
	case Split_X:
		dim = dimensions.x;
		dir = glm::vec3(1, 0, 0);
		size = glm::vec3(0.0f, dimensions.y, dimensions.z);
		break;
	case Split_Y:
		dim = dimensions.y;
		dir = glm::vec3(0, 1, 0);
		size = glm::vec3(dimensions.x, 0.0f, dimensions.z);
		break;
	case Split_Z:
		dim = dimensions.z;
		dir = glm::vec3(0, 0, 1);
		size = glm::vec3(dimensions.x, dimensions.y, 0.0f);
		break;
	}

	float sum = 0.0;
	for (std::vector<float>::const_iterator it = ratios.begin(); it != ratios.end(); it++){
		sum += *it;
	}

	float current_sum = 0.0;
	int index = 0;
	for (std::vector<float>::const_iterator it = ratios.begin(); it != ratios.end(); it++) {
		// repère de base
		CoordSystem cs = CoordSystem::fromParent(coordSystem);

		// décalage
		cs.position += dim * (current_sum / sum) * dir;
		switch (direction) {
		case Split_X:
			size.x = dim * (*it / sum);
			break;
		case Split_Y:
			size.y = dim * (*it / sum);
			break;
		case Split_Z:
			size.z = dim * (*it / sum);
			break;
		}
		Cuboid cuboid(cs,  size);
		cuboid.index = index++;
		out.push_back(cuboid);

		current_sum += *it;
	}
}

void Cuboid::splitRepeat(SplitDirection direction, std::vector<float> const &lengths, int nb_repeat, SplitAlignment alignment, std::vector<Cuboid> &out) const
{
	float dim;
	glm::vec3 dir;
	glm::vec3 size;
	int index = 0;

	switch (direction) {
	case Split_X:
		dim = dimensions.x;
		dir = glm::vec3(1, 0, 0);
		size = glm::vec3(0.0, dimensions.y, dimensions.z);
		break;
	case Split_Y:
		dim = dimensions.y;
		dir = glm::vec3(0, 1, 0);
		size = glm::vec3(dimensions.x, 0.0, dimensions.z);
		break;
	case Split_Z:
		dim = dimensions.z;
		dir = glm::vec3(0, 0, 1);
		size = glm::vec3(dimensions.x, dimensions.y, 0.0f);
		break;
	}

	// Calcul de la longueur total utilisée
	float total_length_used = 0.0;
	int ind = 0;
	int current_loop = 0; // pour les repetitions
	while (total_length_used + lengths[ind] <= dim){
		if (current_loop == nb_repeat){
			break;
		}

		total_length_used += lengths[ind];
		ind++;
		if (ind == lengths.size()){
			current_loop++;
			ind = 0;
		}
	}
	
	// Calcul de l'abscisse du debut du splitting sur le Cuboid
	float begin_splitting;
	switch (alignment) {
	case Split_Begin:
		begin_splitting = 0.0;
		break;
	case Split_Middle:
		begin_splitting = (dim - total_length_used) / 2.0f;
		break;
	case Split_End:
		begin_splitting = dim - total_length_used;
		break;
	}

	if (total_length_used != dim){
		// Creation d'un cuboid au debut de la repetition si il y a de la marge
		if (alignment == Split_End){
			CoordSystem cs = CoordSystem::fromParent(coordSystem);
			switch (direction) {
			case Split_X:
				size.x = dim - total_length_used;
				break;
			case Split_Y:
				size.y = dim - total_length_used;
				break;
			case Split_Z:
				size.z = dim - total_length_used;
				break;
			}
			Cuboid cuboid(cs, size);
			cuboid.index = index++;
			out.push_back(cuboid);
		}
		if (alignment == Split_Middle){
			CoordSystem cs = CoordSystem::fromParent(coordSystem);
			switch (direction) {
			case Split_X:
				size.x = (dim - total_length_used) / 2.0f;
				break;
			case Split_Y:
				size.y = (dim - total_length_used) / 2.0f;
				break;
			case Split_Z:
				size.z = (dim - total_length_used) / 2.0f;
				break;
			}
			Cuboid cuboid(cs, size);
			cuboid.index = index++;
			out.push_back(cuboid);
		}
	}

	/* SPLITTING PRINCIPAL */

	float length_used = 0.0;
	ind = 0;
	current_loop = 0; // pour les repetitions
	while (length_used + lengths[ind] <= dim){
		if (current_loop == nb_repeat){
			break;
		}
		
		// repère de base
		CoordSystem cs = CoordSystem::fromParent(coordSystem);

		// décalage
		cs.position += (begin_splitting + length_used) * dir;
		switch (direction) {
		case Split_X:
			size.x = lengths[ind];
			break;
		case Split_Y:
			size.y = lengths[ind];
			break;
		case Split_Z:
			size.z = lengths[ind];
			break;
		}
		Cuboid cuboid(cs, size);
		cuboid.index = index++;
		out.push_back(cuboid);

		length_used += lengths[ind];
		ind++;
		if (ind == lengths.size()){
			current_loop++;
			ind = 0;
		}
	}

	if (total_length_used != dim){
		// Creation d'un cuboid à la fin de la repetition si il y a de la marge
		if (alignment == Split_Begin){
			CoordSystem cs = CoordSystem::fromParent(coordSystem);
			cs.position += total_length_used * dir;
			switch (direction) {
			case Split_X:
				size.x = dim - total_length_used;
				break;
			case Split_Y:
				size.y = dim - total_length_used;
				break;
			case Split_Z:
				size.z = dim - total_length_used;
				break;
			}
			Cuboid cuboid(cs, size);
			cuboid.index = index++;
			out.push_back(cuboid);
		}
		if (alignment == Split_Middle){
			CoordSystem cs = CoordSystem::fromParent(coordSystem);
			cs.position += (total_length_used + ((dim - total_length_used) / 2.f)) * dir;
			switch (direction) {
			case Split_X:
				size.x = (dim - total_length_used) / 2.0f;
				break;
			case Split_Y:
				size.y = (dim - total_length_used) / 2.0f;
				break;
			case Split_Z:
				size.z = (dim - total_length_used) / 2.0f;
				break;
			}
			Cuboid cuboid(cs, size);
			cuboid.index = index++;
			out.push_back(cuboid);
		}
	}

}
