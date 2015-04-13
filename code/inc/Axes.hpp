#ifndef AXES_HPP
#define AXES_HPP

#include <Mesh.hpp>
#include <ShaderManager.hpp>
#include <RenderData.hpp>

class Axes
{
public:
	Axes();
	void init();
	void render(RenderData &data);

private:
	Program *prog;
	Mesh mesh;
};


#endif