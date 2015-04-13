#include "PolygonRenderable.hpp"
#include <sstream>

PolygonRenderable::PolygonRenderable(float height) : h(height), renderInited(false)
{}

PolygonRenderable::~PolygonRenderable()
{
	//  TODO do something
}

void PolygonRenderable::addPoint(glm::vec2 const &point)
{
	points.push_back(point);
}

void PolygonRenderable::setPoints(std::vector<glm::vec2> const &points)
{
	this->points = points;
}

void PolygonRenderable::render(RenderData const &rd)
{
	if (!renderInited) {
		initRender();
	}
	updateGeometry(); // Probleme ? car fait a chaque boucle de d'animation

	glm::mat4 mvp = rd.projMatrix * rd.viewMatrix;
	glm::vec4 color = glm::vec4(1.f, 0.f, 0.f, 0.5f);

	prog->use();
	prog->uniformMatrix4fv("viewProjMatrix", mvp);
	prog->uniform4f("color", color);

	// enable blending
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	// draw faces
	mesh.draw();
	// draw contours
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	color = glm::vec4(1.f, 0.f, 0.f, 1.f);
	prog->uniform4f("color", color);
	mesh.draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask(GL_TRUE);
}

void PolygonRenderable::initRender()
{
	// load shaders
	prog = ShaderManager::Instance().loadProgram("shaders/polygon.vert.glsl", "shaders/polygon.frag.glsl");
	renderInited = true;
}

void PolygonRenderable::updateGeometry()
{
	std::vector<glm::vec3> geom;

	for (int i = 0; i < points.size() - 1; ++i) {
		// generate quad
		glm::vec2 &p1 = points[i];
		glm::vec2 &p2 = points[i + 1];
		// 1st tri
		geom.push_back(glm::vec3(p1.x, 0, p1.y));
		geom.push_back(glm::vec3(p2.x, 0, p2.y));
		geom.push_back(glm::vec3(p1.x, h, p1.y));
		// 2nd tri
		geom.push_back(glm::vec3(p1.x, h, p1.y));
		geom.push_back(glm::vec3(p2.x, 0, p2.y));
		geom.push_back(glm::vec3(p2.x, h, p2.y));
	}
	// last
	glm::vec2 &p1 = points[points.size()-1];
	glm::vec2 &p2 = points[0];
	// 1st tri
	geom.push_back(glm::vec3(p1.x, 0, p1.y));
	geom.push_back(glm::vec3(p2.x, 0, p2.y));
	geom.push_back(glm::vec3(p1.x, h, p1.y));
	// 2nd tri
	geom.push_back(glm::vec3(p1.x, h, p1.y));
	geom.push_back(glm::vec3(p2.x, 0, p2.y));
	geom.push_back(glm::vec3(p2.x, h, p2.y));

	// update mesh
	mesh.setNumVertices(geom.size());
	mesh.setPosition(glm::value_ptr(*geom.data()));
	// TODO normales?
}

