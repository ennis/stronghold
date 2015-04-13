#ifndef _SKYCUBE_
#define _SKYCUBE_

#include "Program.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Renderable.hpp"


class SkyCube : public Renderable
{
 public:
	 SkyCube();
	~SkyCube();

	void init();
	void render(RenderData const &data);

private:
	 Program *prog;
	 Mesh cube;

	 /*Transform transform;
	 GLfloat size;
	 Mesh *mesh;
	 Program *program;

	 Texture * tex_back;
	 Texture * tex_front;
	 Texture * tex_left;
	 Texture * tex_right;
	 Texture * tex_top;
	 //Texture * tex_bottom;*/
};

#endif
