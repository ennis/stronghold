#include <Camera.hpp>
#include <Window.hpp>
#include <iostream>
#include <glm/gtx/transform.hpp>

const glm::vec3 Camera::Front = glm::vec3(0, 0, 1);
const glm::vec3 Camera::Right = glm::vec3(1, 0, 0);
const glm::vec3 Camera::Up = glm::vec3(0, 1, 0);

void Camera::init(float fov_, float aspectRatio_, float zNear_, float zFar_)
{
    theta  = 0.f;
    phi = 0.f;
    zoom = 1.f;

	transform.move(glm::vec3(0, 0, 1)).rotate(3.1415f,glm::vec3(0, 1, 0));
	fov = fov_;
	aspectRatio = aspectRatio_;
	nearPlane = zNear_;
	farPlane = zFar_;

}

void Camera::updateViewport(glm::vec2 const &size)
{
	aspectRatio = size.x / size.y;
}

glm::mat4 Camera::projectionMatrix()
{
	return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt(
		transform.position,
		transform.position + transform.rotation * Camera::Front,
		transform.rotation * Camera::Up);
}

void Camera::update(float dt)
{
	using namespace glm;

	int width, height;
	glfwGetWindowSize(GMainWindow, &width, &height);
	updateViewport(glm::vec2(static_cast<float>(width), static_cast<float>(height)));

	// handle mouse input
	double cursorX, cursorY;
	glfwGetCursorPos(GMainWindow, &cursorX, &cursorY);
	float dx = (float(cursorX) - lastMousePos.x) / 100.f;
	float dy = (float(cursorY) - lastMousePos.y) / 100.f;

	
	if (abs(dx) > abs(dy)) { 
	  dy = 0; 
	} else { 
	  dx = 0; 
	}
	

	bool mouseRight = glfwGetMouseButton(GMainWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
	bool mouseLeft = glfwGetMouseButton(GMainWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

	if ((mouseRight || mouseLeft) && (dx != 0 || dy != 0))
	{
		float orbit_dist = distance(transform.position, vec3(0, 0, 0));

		phi += dy;
		theta -= dx;
		vec3 up = vec3(0, 1, 0);
		vec3 right = angleAxis(theta, up) * vec3(1, 0, 0);
		transform.rotation = angleAxis(phi, right) * angleAxis(theta, up);

#ifndef NO_TRACE
		std::clog << "phi=" << phi << ", theta=" << theta << '\n';
#endif

		//if (mouseLeft) {
			// orbit mode
		//	transform.position = transform.rotation * vec3(0, 0, -orbit_dist);
			//	}
	}

	lastMousePos.x = static_cast<float>(cursorX);
	lastMousePos.y = static_cast<float>(cursorY);
	//std::clog << "lastMouseX=" << lastMousePos.x << ", lastMouseY" << lastMousePos.y<< '\n';

	vec3 front = transform.rotation * Camera::Front;
	vec3 right = transform.rotation * Camera::Right;

	// handle keyboard input
	int keyW = glfwGetKey(GMainWindow, GLFW_KEY_W);
	int keyA = glfwGetKey(GMainWindow, GLFW_KEY_A);
	int keyS = glfwGetKey(GMainWindow, GLFW_KEY_S);
	int keyD = glfwGetKey(GMainWindow, GLFW_KEY_D);
	int keyR = glfwGetKey(GMainWindow, GLFW_KEY_R);
	int keyT = glfwGetKey(GMainWindow, GLFW_KEY_T);

	dt = dt * 100.f;

	if (keyW == GLFW_PRESS) {
		transform.position += dt * front;
	}
	else if (keyS == GLFW_PRESS) {
		transform.position -= dt * front;
	}
	if (keyA == GLFW_PRESS) {
		transform.position += dt * right;
	}
	else if (keyD == GLFW_PRESS) {
		transform.position -= dt * right;
	}

	if (keyR == GLFW_PRESS) {
		transform.position = glm::vec3();
		transform.rotation = glm::quat();
	}
	if (keyT == GLFW_PRESS) {
	  transform.position = glm::vec3(500,500,500);
	  transform.rotation = glm::quat();
	}
		
}
