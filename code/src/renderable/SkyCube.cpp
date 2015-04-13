#include "SkyCube.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"


SkyCube::SkyCube() {
  /*tex_back = TextureManager::Instance().loadTexture("textures/calm_back.jpg", false);
  tex_front = TextureManager::Instance().loadTexture("textures/calm_front.jpg", false);
  tex_left = TextureManager::Instance().loadTexture("textures/calm_left.jpg", false);
  tex_right = TextureManager::Instance().loadTexture("textures/calm_right.jpg", false);
  tex_top = TextureManager::Instance().loadTexture("textures/calm_top.jpg", false);*/
}


SkyCube::~SkyCube() {

}

/* A face of the skyCube, seen from the inside
 *  A(0,1) ______ B(1,1)
 *        |\     | 
 *        |  \   |
 *        |    \ |
 * C(0,0) |______\ D(1,0)
 *
 */


void SkyCube::init() {
	prog = ShaderManager::Instance().loadProgram("shaders/skybox.vert.glsl", "shaders/skybox_gen.frag.glsl");
	
	static const float vertices[] = {
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};

	cube.setNumVertices(6*6);
	cube.setPosition(vertices);
}


void SkyCube::render(RenderData const &data)
{
	prog->use();
	setShaderUniforms(prog, data);

	// follow camera
	glm::mat4 model = glm::scale(glm::translate(data.cameraPos), glm::vec3(150.f));
	prog->uniformMatrix4fv("modelMatrix", model);

	//glDepthMask(GL_FALSE);
	glCullFace(GL_FRONT);
	cube.draw();
	//glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
}


