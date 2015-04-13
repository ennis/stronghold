#include "ModelRenderable.hpp"

void ModelRenderable::render(RenderData const &data) 
{
	prog->use();
	setShaderUniforms(prog, data);

	glm::mat4 modelMatrix = glm::translate(position) * glm::toMat4(rotation) * glm::scale(glm::vec3(scale));
	prog->uniformMatrix4fv("modelMatrix", modelMatrix);

	if (texture) {
		texture->setActive(0);
		prog->uniform1i("diffuseMap", 0);
		prog->uniform1i("hasBumpMap", 0);
	}

	model->getMesh().draw();
}

void ModelRenderable::initRender()
{
	// load shaders
	prog = ShaderManager::Instance().loadProgram("shaders/batch.vert.glsl", "shaders/batch.frag.glsl");
	renderInited = true;
}
