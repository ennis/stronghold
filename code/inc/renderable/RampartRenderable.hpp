#ifndef RAMPART_RENDERABLE
#define RAMPART_RENDERABLE

#include <Renderable.hpp>
#include <RenderData.hpp>
#include <ShaderManager.hpp>
#include <Mesh.hpp>
#include <Trapezoid2D.hpp>
#include <Texture.hpp>

#include <vector>
#include <string>

class RampartRenderable : public Renderable{
public:
	RampartRenderable(Trapezoid2D const &base_,
		float base_ha_, float base_hb_,
		float wall_ha_, float wall_hb_);

	~RampartRenderable() {
        delete prog;
    }

	//
	// Rendu
	void render(RenderData const &data);
private:
	Trapezoid2D base;
	float base_ha;
	float base_hb;
	float wall_ha;
	float wall_hb;

	Mesh mesh;
	Program *prog;

	glm::vec3 vertices[8];
	void CreateVertices();

	Texture * texture;
};

#endif
