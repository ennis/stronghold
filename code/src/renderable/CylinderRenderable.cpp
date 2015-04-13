#include "CylinderRenderable.hpp"
#include <cmath>

CylinderRenderable::CylinderRenderable(Cylinder cylinder) : renderInited(false)
{
	base = cylinder;
}

CylinderRenderable::~CylinderRenderable()
{
}


void CylinderRenderable::initRender()
{
	// load shaders
	prog = ShaderManager::Instance().loadProgram("shaders/polygon.vert.glsl", "shaders/polygon.frag.glsl");
	renderInited = true;
}

void CylinderRenderable::updateGeometry() {

	std::vector<glm::vec3> geom;
	glm::vec3 bla = base.getCircle().getCoordSystem().position;
	double h = base.getHeight();
	double r = base.getCircle().getRadius();
    int nbSegments = Circle2D::nbSegments;

	for (int i = 0; i <nbSegments; ++i) {
		// lower base
		geom.push_back(bla);
		geom.push_back(glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments),bla.y,bla.z +  r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// upper base
		geom.push_back(glm::vec3(bla.x, bla.y + h, bla.z));
		geom.push_back(glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin(i * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// 1st tri
		geom.push_back(glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin(i * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// 2nd tri
		geom.push_back(glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		geom.push_back(glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
	}

	// update mesh
	mesh.setNumVertices(geom.size());
	mesh.setPosition(glm::value_ptr(*geom.data()));
	// TODO normales?
}

void CylinderRenderable::render(RenderData const &rd)
{
	if (!renderInited) {
		initRender();
	}
	updateGeometry(); 

	glm::mat4 mvp = rd.projMatrix * rd.viewMatrix;
	glm::vec4 color = glm::vec4(1.f, 1.f, 0.f, 0.5f);

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
	glDepthMask(GL_TRUE);
}
