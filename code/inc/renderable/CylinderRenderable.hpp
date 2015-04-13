#ifndef CYLINDER_RENDERABLE_HPP
#define CYLINDER_RENDERABLE_HPP

#include <Renderable.hpp>
#include <Cylinder.hpp>
#include <Renderable.hpp>
#include <RenderData.hpp>
#include <ShaderManager.hpp>
#include <Mesh.hpp>

#include <vector>
#include <string>

class CylinderRenderable : public Renderable {
private:
	Cylinder base;
	bool renderInited;

	void initRender();
	void updateGeometry();

	Mesh mesh;
	Program *prog;

public:
	CylinderRenderable(Cylinder base);
	~CylinderRenderable();
	void render(RenderData const &data);

};
#endif
