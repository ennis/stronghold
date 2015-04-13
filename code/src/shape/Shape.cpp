#include "Shape.hpp"


CoordSystem::CoordSystem(
CoordSystem const &parent_,
glm::vec3 const &position_,
glm::quat const &rotation_,
float shear_) : position(position_), rotation(rotation_), shear(shear_), parent(&parent_)
{}

CoordSystem::CoordSystem(
	glm::vec3 const &position_,
	glm::quat const &rotation_,
	float shear_) : position(position_), rotation(rotation_), shear(shear_), parent(NULL)
{}


glm::vec3 CoordSystem::shearVector(glm::vec3 const& v) const
{
	glm::vec3 result;
	result.x = v.x;
	result.y = shear * v.x + v.y;
	result.z = v.z;
	return result;
}

// on pourrait utiliser des coordonnées homogènes. A voir
// vecteurs
glm::vec2 CoordSystem::rotate(glm::vec2 const &v) const {
	glm::vec3 tmp = rotation * glm::vec3(v.x, 0, v.y);
	glm::vec2 r = glm::vec2(tmp.x, tmp.z);
	if (parent != NULL) {
		return parent->rotate(r);
	}
	else  {
		return r;
	}
}

glm::vec3 CoordSystem::rotate(glm::vec3 const &v) const {
	glm::vec3 r = rotation * shearVector(v);
	if (parent != NULL) {
		return parent->rotate(r);
	}
	else  {
		return r;
	}
}

// points 
glm::vec2 CoordSystem::transform(glm::vec2 const &p) const {
	glm::vec3 tmp = position + rotation * glm::vec3(p.x, 0, p.y);
	glm::vec2 r = glm::vec2(tmp.x, tmp.z);
	if (parent != NULL) {
		return parent->transform(r);
	}
	else {
		return r;
	}
}

glm::vec3 CoordSystem::transform(glm::vec3 const &p) const {
	glm::vec3 r = position + rotation * shearVector(p);
	if (parent != NULL) {
		return parent->transform(r);
	}
	else {
		return r;
	}
}

glm::quat CoordSystem::getFinalRotation() const {
	if (parent) {
		return rotation * parent->getFinalRotation();
	}
	else {
		return rotation;
	}
}

glm::vec3 CoordSystem::getFinalPosition() const {
	if (parent) {
		return position + parent->getFinalPosition();
	}
	else {
		return position;
	}
}

CoordSystem CoordSystem::transform(CoordSystem const & cs) const {
	return CoordSystem(cs, position, rotation, shear);
}

CoordSystem CoordSystem::fromParent(
	CoordSystem const &cs,
	glm::vec3 const &position_,
	glm::quat const &rotation_,
	float shear_)
{
	return CoordSystem(cs, position_, rotation_, shear_);
}

CoordSystem CoordSystem::fromVectors(
	glm::vec3 const &position,
	glm::vec3 const &u0,
	glm::vec3 const &v0,
	float shear)
{
	CoordSystem cs;
	cs.position = position;

	glm::vec3 n0 = glm::cross(v0, u0);
	glm::mat3 t = glm::mat3(u0, n0, v0);
	cs.rotation = glm::quat(t);

	cs.shear = shear;
	cs.parent = NULL;

	return cs;
}

CoordSystem CoordSystem::fromVectors(
	CoordSystem const &parent,
	glm::vec3 const &position,
	glm::vec3 const &u0,
	glm::vec3 const &v0,
	float shear)
{
	CoordSystem cs = fromVectors(position, u0, v0, shear);
	cs.parent = &parent;
	return cs;
}
