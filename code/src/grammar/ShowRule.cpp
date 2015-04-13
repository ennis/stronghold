#include <ShowRule.hpp>
#include <Polygon2D.hpp>
#include <Trapezoid2D.hpp>
#include <PolygonRenderable.hpp>
#include <Grammar.hpp>
#include <Rampart.hpp>
#include <Cuboid.hpp>
#include <MeshRenderable.hpp>
#include <Random.hpp>
#include <Prism.hpp>
#include <CylinderRenderable.hpp>
#include <TextureManager.hpp>
#include <Tetrahedron.hpp>
#include <Cone.hpp>
#include <iostream>
#include <string>
#include "Intersections.hpp"

void ShowRule::setTexture(std::string const &tex)
{
	texture = TextureManager::Instance().loadTexture(tex.c_str());
}

void ShowRule::execute(Grammar &grammar, Context &context, Shape &shape)
{
	// XXX oui, il est possible de rajouter une méthode virtuelle aux volumes
	// qui va créer le renderable associé
	// MAIS ça ne devrait pas être la responsabilité du volume de se dessiner
	// (principe de 'separation of concerns')
	// du coup, switch & cast

	if (shape.getShapeType() == ST_Polygon) {
		createPolygonRenderable(static_cast<Polygon2D const&>(shape), context);
		//std::clog << "ShowRule: Polygon2D\n";
	}
	else if (shape.getShapeType() == ST_Trapezoid) {
		createTrapezoidRenderable(static_cast<Trapezoid2D const&>(shape), context);
		//std::clog << "ShowRule: Trapezoid2D\n";
	}
	else if (shape.getShapeType() == ST_Rampart) {
		createRampartRenderable(static_cast<Rampart const&>(shape), context);
		//std::clog << "ShowRule: Rampart\n";
	}
	else if (shape.getShapeType() == ST_Prism) {
		createPrismRenderable(static_cast<Prism const&>(shape), context);
		//std::clog << "ShowRule: Prism\n";
	}
	else if (shape.getShapeType() == ST_Cylinder) {
		createCylinderRenderable(static_cast<Cylinder const&>(shape), context);
		//std::clog << "ShowRule: Cylinder\n";
	}
	else if (shape.getShapeType() == ST_Cuboid) {
		createCuboidRenderable(static_cast<Cuboid const&>(shape), context, texture);
		//std::clog << "ShowRule: Cuboid\n";
	}
	else if (shape.getShapeType() == ST_Rectangle) {
		createRectangleRenderable(static_cast<Rectangle2D const&>(shape), context);
		//std::clog << "ShowRule: Rectangle2D\n";
	}
	else if (shape.getShapeType() == ST_Tetrahedron) {
		createTetrahedronRenderable(static_cast<Tetrahedron const&>(shape), context);
		//std::clog << "ShowRule: Tetrahedron\n";
	}
	else if (shape.getShapeType() == ST_Cone) {
		createConeRenderable(static_cast<Cone const&>(shape), context);
		//std::clog << "ShowRule: Cone\n";
	}
	else {
		std::clog << "ShowRule: unknown shape type\n";
	}
}

void createPolygonRenderable(Polygon2D const &poly2D, Context &context)
{
	PolygonRenderable *pr = new PolygonRenderable;
	pr->setPoints(poly2D.getPoints());
	context.renderables.push_back(pr);
}

void createTrapezoidRenderable(Trapezoid2D const &trapezoid, Context &context)
{
	PolygonRenderable *pr = new PolygonRenderable;
	CoordSystem const &cs = trapezoid.getCoordSystem();
	pr->addPoint(cs.transform(trapezoid.sommets[0]));
	pr->addPoint(cs.transform(trapezoid.sommets[1]));
	pr->addPoint(cs.transform(trapezoid.sommets[2]));
	pr->addPoint(cs.transform(trapezoid.sommets[3]));
	context.renderables.push_back(pr);
}

void createRampartRenderable(Rampart const &rampart, Context &context)
{
	// create geometry for the rampart
	glm::vec3 v[8];

	Rectangle2D const &rect = rampart.base;
	CoordSystem const &cs = rampart.getCoordSystem();

	v[0] = cs.transform(glm::vec3(0, rampart.base_ha, 0));
	v[1] = cs.transform(glm::vec3(rect.width, rampart.base_hb, 0));
	v[2] = cs.transform(glm::vec3(0, rampart.base_ha, rect.height));
	v[3] = cs.transform(glm::vec3(rect.width, rampart.base_hb, rect.height));

	v[4] = cs.transform(glm::vec3(0, rampart.base_ha + rampart.wall_ha, 0));
	v[5] = cs.transform(glm::vec3(rect.width, rampart.base_hb + rampart.wall_hb, 0));
	v[6] = cs.transform(glm::vec3(0, rampart.base_ha + rampart.wall_ha, rect.height));
	v[7] = cs.transform(glm::vec3(rect.width, rampart.base_hb + rampart.wall_hb, rect.height));

	glm::vec2 tc = glm::vec2();

	MeshRenderable *mr = new MeshRenderable();

	// bottom
	mr->addTriangle(v[0], v[1], v[2], tc, tc, tc);
	mr->addTriangle(v[1], v[3], v[2], tc, tc, tc);
	// top
	mr->addTriangle(v[4], v[6], v[5], tc, tc, tc);
	mr->addTriangle(v[5], v[6], v[7], tc, tc, tc);

	// right
	mr->addTriangle(v[1], v[5], v[3], tc, tc, tc);
	mr->addTriangle(v[5], v[7], v[3], tc, tc, tc);
	// left
	mr->addTriangle(v[0], v[2], v[4], tc, tc, tc);
	mr->addTriangle(v[4], v[2], v[6], tc, tc, tc);
	// front
	mr->addTriangle(v[0], v[4], v[1], tc, tc, tc);
	mr->addTriangle(v[4], v[5], v[1], tc, tc, tc);
	// back
	mr->addTriangle(v[6], v[2], v[3], tc, tc, tc);
	mr->addTriangle(v[6], v[3], v[7], tc, tc, tc);

	context.renderables.push_back(mr);
}

void createCuboidRenderable(Cuboid const &cuboid, Context &context, Texture *tex)
{
	// create geometry for the rampart
	glm::vec3 v[8];

	CoordSystem const &cs = cuboid.getCoordSystem();

	v[0] = cs.transform(glm::vec3(0, 0, 0));
	v[1] = cs.transform(glm::vec3(cuboid.dimensions.x, 0, 0));
	v[2] = cs.transform(glm::vec3(0, 0, cuboid.dimensions.z));
	v[3] = cs.transform(glm::vec3(cuboid.dimensions.x, 0, cuboid.dimensions.z));

	v[4] = cs.transform(glm::vec3(0, cuboid.dimensions.y, 0));
	v[5] = cs.transform(glm::vec3(cuboid.dimensions.x, cuboid.dimensions.y, 0));
	v[6] = cs.transform(glm::vec3(0, cuboid.dimensions.y, cuboid.dimensions.z));
	v[7] = cs.transform(glm::vec3(cuboid.dimensions.x, cuboid.dimensions.y, cuboid.dimensions.z));

	glm::vec2 tc = glm::vec2();

	//Texture *rockTex = TextureManager::Instance().loadTexture("textures/wall_c.png");
	//Texture *rockBumpTex = TextureManager::Instance().loadTexture("textures/wall_n.png");
	if (tex != NULL) {
		context.renderBatch.beginBatch(tex);
	} else {
		// default texture
		Texture *rockTex = TextureManager::Instance().loadTexture("textures/wall_c.png");
		Texture *rockBumpTex = TextureManager::Instance().loadTexture("textures/wall_n.png");
		context.renderBatch.beginBatch(rockTex, rockBumpTex);
	}
	// bottom
	context.renderBatch.addTriangle(v[0], v[2], v[3]);
	context.renderBatch.addTriangle(v[0], v[3], v[1]);
	// top
	context.renderBatch.addTriangle(v[4], v[6], v[5]);
	context.renderBatch.addTriangle(v[5], v[6], v[7]);

	// right
	context.renderBatch.addTriangle(v[1], v[5], v[7]);
	context.renderBatch.addTriangle(v[1], v[7], v[3]);
	// left
	context.renderBatch.addTriangle(v[0], v[6], v[4]);
	context.renderBatch.addTriangle(v[0], v[2], v[6]);
	// front
	context.renderBatch.addTriangle(v[0], v[4], v[1]);
	context.renderBatch.addTriangle(v[4], v[5], v[1]);
	// back
	context.renderBatch.addTriangle(v[6], v[2], v[3]);
	context.renderBatch.addTriangle(v[6], v[3], v[7]);

	//context.renderBatch.setColor(getRandomColor());
}

glm::vec4 getRandomColor()
{
	glm::vec4 mix = glm::vec4(1.f, 1.f, 1.f, 1.f);
	float red = GRandom.RandomNumber(0, 1);
	float green = GRandom.RandomNumber(0, 1);
	float blue = GRandom.RandomNumber(0, 1);
	glm::vec4 color = glm::vec4(red, green, blue, 1.f);

	// mix the color
	return 0.5f * (color + mix);
}

void createPrismRenderable(Prism const &prism, Context &context) {
	// create geometry for the rampart
	std::vector<glm::vec2> points = prism.poly.getPoints();
	int nb_points_base = prism.poly.size();
	std::vector<glm::vec3> v_bottom;
	std::vector<glm::vec3> v_top;
	CoordSystem const &cs = prism.getCoordSystem();
	for (int i = 0; i < nb_points_base; i++){
		v_bottom.push_back(cs.transform(glm::vec3(points[i].x, 0, points[i].y)));
		v_top.push_back(cs.transform(glm::vec3(points[i].x, prism.height, points[i].y)));
	}

	glm::vec2 tc = glm::vec2();
	MeshRenderable *mr = new MeshRenderable();

	// bottom
	/*for (int i = 0; i < nb_points_base; i++){
		mr->addTriangle(prism.poly[0], prism.poly[1], prism.poly[2], tc, tc, tc);
	}*/

	// top

	// sides
	for (int i = 0; i < nb_points_base; i++){
		mr->addTriangle(v_bottom[i], v_bottom[(i + 1) % nb_points_base], v_top[(i + 1) % nb_points_base],
						tc, tc, tc);
		mr->addTriangle(v_bottom[i], v_top[(i + 1) % nb_points_base], v_top[i],
						tc, tc, tc);
	}

	mr->setColor(glm::vec4(0,1,0,1.0));
	context.renderables.push_back(mr);
}

void createCylinderRenderable(Cylinder const &cylinder, Context &context)
{
	glm::vec3 bla = cylinder.getCircle().getCoordSystem().position;
	double h = cylinder.getHeight();
	double r = cylinder.getCircle().getRadius();
	int nbSegments = Circle2D::nbSegments;

	Texture *tex = TextureManager::Instance().loadTexture("textures/dungeon_walls.jpg");

	context.renderBatch.beginBatch(tex);

	for (int i = 0; i <nbSegments; ++i) {
		// lower base
		context.renderBatch.addTriangle(
			bla,
			glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// upper base
		context.renderBatch.addTriangle(
			glm::vec3(bla.x, bla.y + h, bla.z),
			glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin(i * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// 1st tri
		context.renderBatch.addTriangle(
			glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin(i * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
		// 2nd tri
		context.renderBatch.addTriangle(
			glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos(i * 2 * M_PI / nbSegments), bla.y, bla.z + r*sin(i * 2 * M_PI / nbSegments)),
			glm::vec3(bla.x + r*cos((i + 1) * 2 * M_PI / nbSegments), bla.y + h, bla.z + r*sin((i + 1) * 2 * M_PI / nbSegments)));
	}

	context.renderBatch.endBatch();
}

void createRectangleRenderable(Rectangle2D const &rect, Context &context)
{
	CoordSystem const &cs = rect.getCoordSystem();
	glm::vec2 tc(0.f, 0.f);

	glm::vec3 v[4];
	v[0] = cs.transform(glm::vec3(0, 0, 0));
	v[1] = cs.transform(glm::vec3(rect.width, 0, 0));
	v[2] = cs.transform(glm::vec3(0, 0, rect.height));
	v[3] = cs.transform(glm::vec3(rect.width, 0, rect.height));

	// bottom
	// essai essai essai
	Texture *rockTex = TextureManager::Instance().loadTexture("textures/wall_c.png");
	Texture *rockBumpTex = TextureManager::Instance().loadTexture("textures/wall_n.png");
	context.renderBatch.beginBatch(rockTex, rockBumpTex);
	context.renderBatch.addTriangle(v[0], v[2], v[3]);
	context.renderBatch.addTriangle(v[0], v[3], v[1]);
	context.renderBatch.endBatch();
}

void createTetrahedronRenderable(Tetrahedron const &tetra, Context &context) {
    CoordSystem cs = tetra.base.getCoordSystem();
    glm::vec2 orig = glm::vec2(cs.position.x, cs.position.z);
    glm::vec3 v[4];
    v[0] = cs.transform(glm::vec3(0, 0, 0));
    v[1] = cs.transform(glm::vec3(tetra.base.width, 0, 0));
    v[2] = cs.transform(glm::vec3(0, 0, tetra.base.height));
	v[3] = cs.transform(glm::vec3(tetra.base.width, 0, tetra.base.height));

	Texture *tex = TextureManager::Instance().loadTexture("textures/roof_texture.jpg");
	context.renderBatch.beginBatch(tex);

    // Intersection of the two diagonals of the rectangle
    glm::vec3 eq1, eq2;
    eq1 = compute_eq(v[0], v[3]);
    eq2 = compute_eq(v[1], v[2]);
    float D = eq1.x * eq2.y - eq2.x * eq1.y;
    float xi = (eq2.z * eq1.y - eq1.z * eq2.y) / D;
    float yi = (eq2.x * eq1.z - eq1.x * eq2.z) / D;

    glm::vec3 inter;
    inter.x = xi;
    inter.z = yi;
    inter.y = tetra.height + cs.position.y;

	context.renderBatch.beginBatch(tex);

    // Base
	context.renderBatch.addTriangle(v[0], v[1], v[2]);
	context.renderBatch.addTriangle(v[1], v[2], v[3]);

    // Sides
	context.renderBatch.addTriangle(v[0], inter, v[1]);
	context.renderBatch.addTriangle(v[0], v[2], inter);
	context.renderBatch.addTriangle(v[1], inter, v[3]);
	context.renderBatch.addTriangle(v[2], v[3], inter);

	context.renderBatch.endBatch();
}

void createConeRenderable(Cone const &cone, Context &context) {
    CoordSystem cs = cone.base.getCoordSystem();
    int nbSegments = cone.base.nbSegments;
    double r = cone.base.getRadius();
    glm::vec3 center = cs.position;
    glm::vec3 hCenter(center.x, center.y + cone.height, center.z);

	Texture *tex = TextureManager::Instance().loadTexture("textures/roof_texture.jpg");
	context.renderBatch.beginBatch(tex);

    // Base
    for (int i = 0; i < nbSegments; i++) {
        glm::vec3 ri(center.x + r * cos(2 * i * M_PI / nbSegments),
                     center.y,
                     center.z + r * sin(2 * i * M_PI / nbSegments));
        glm::vec3 rip1(center.x + r * cos(2 * (i + 1) * M_PI / nbSegments),
                       center.y,
                       center.z + r * sin(2 * (i + 1) * M_PI / nbSegments));
		context.renderBatch.addTriangle(center, rip1, ri);
    }

    // Sides
    for (int i = 0; i < nbSegments; i++) {
        glm::vec3 ri(center.x + r * cos(2 * i * M_PI / nbSegments),
                     center.y,
                     center.z + r * sin(2 * i * M_PI / nbSegments));
        glm::vec3 rip1(center.x + r * cos(2 * (i + 1) * M_PI / nbSegments),
                       center.y,
                       center.z + r * sin(2 * (i + 1) * M_PI / nbSegments));
		context.renderBatch.addTriangle(hCenter, rip1, ri);
    }

	context.renderBatch.endBatch();
}

