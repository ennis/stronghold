#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <OpenGL.hpp>
#include <Transform.hpp>

#define NO_TRACE

class Camera
{
public:
	void init(float fov, float aspectRatio, float zNear, float zFar);
	void update(float dt);

	glm::mat4 projectionMatrix();
	glm::mat4 viewMatrix();

	// static vectors
	static const glm::vec3 Front;
	static const glm::vec3 Right;
	static const glm::vec3 Up;

    static Camera& Instance() {
        static Camera singleton;
        return singleton;
	}

	Transform transform;

private:
	void updateViewport(glm::vec2 const &size);

	float theta;
	float phi;
	float zoom;

	// perspective
	float nearPlane;
	float farPlane;
	float aspectRatio;
	float fov;

	glm::vec2 lastMousePos;
};

#endif
