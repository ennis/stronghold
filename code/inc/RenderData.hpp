#ifndef RENDERDATA_HPP
#define RENDERDATA_HPP

#include <OpenGL.hpp>
#include <Transform.hpp>
#include <Program.hpp>


struct RenderData
{
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	glm::vec3 sunDirection;
	glm::vec3 sunColor;
	glm::vec3 ambientColor;
	bool enableAmbientOcclusion;
	glm::vec2 viewportSize;
	glm::vec3 cameraPos;
};

void setShaderUniforms(Program *p, RenderData const &rd);

#endif
