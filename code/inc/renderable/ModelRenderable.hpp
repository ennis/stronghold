#ifndef MODELRENDERABLE_HPP
#define MODELRENDERABLE_HPP

#include "Renderable.hpp"
#include <RenderData.hpp>
#include <ShaderManager.hpp>
#include "Model.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <Shape.hpp>

class ModelRenderable : public Renderable {
public:
    ModelRenderable (CoordSystem const &cs, float scale, Model *model, Texture *texture = NULL) : 
		position(cs.getFinalPosition()), rotation(cs.getFinalRotation()), 
		scale(scale), renderInited(false), model(model), prog(NULL), texture(texture)
	{
		initRender();
    }

    ~ModelRenderable () {
    }

    void initRender();

    void render(RenderData const &data);

private:
	glm::vec3 position;
	glm::quat rotation;
	float scale;

	bool renderInited;
	Model *model;
    Program *prog;
	Texture *texture;
};

#endif

