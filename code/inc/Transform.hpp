#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	Transform() :
	scaling(1.f, 1.f, 1.f),
	position(0.f, 0.f, 0.f),
	rotation(1.f, 0.f, 0.f, 0.f)
	{}

	Transform &scale(float v) {
		scaling = glm::vec3(v);
		return *this;
	}

	Transform &translate(glm::vec3 const &v) {
		position = v;
		return *this;
	}

	Transform &move(glm::vec3 const &v) {
		position += v;
		return *this;
	}

	Transform &rotate(float angle, glm::vec3 const &axis) {
		rotation = glm::angleAxis(angle, axis);
		return *this;
	}

	glm::mat4 toMatrix() {
		return glm::translate(position) * glm::scale(scaling) * glm::toMat4(rotation);
	}

	glm::vec3 scaling;
	glm::vec3 position;
	glm::quat rotation;
};

#endif