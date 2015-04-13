#ifndef POLYGONRENDERABLE_HPP
#define POLYGONRENDERABLE_HPP

#include <Renderable.hpp>
#include <RenderData.hpp>
#include <ShaderManager.hpp>
#include <Mesh.hpp>

#include <vector>
#include <string>

//
// class Renderable pour les Polygon2D
// Affiche un prisme transparent sur la parcelle délimitée par les points
//
class PolygonRenderable : public Renderable
{
public:
	PolygonRenderable(float height = 100.0f);
	~PolygonRenderable();
	
	void addPoint(glm::vec2 const &point);

	//
	// Replacement des points du polygone
    void setPoints(std::vector<glm::vec2> const &points);
    
	//
	// Rendu
	void render(RenderData const &data);

private:
    float h;
    bool renderInited;

    void initRender();
    void updateGeometry(); 

	Mesh mesh;
    Program *prog;

    std::vector<glm::vec2> points;
};

#endif
