#include <Axes.hpp>
#include <glm/gtc/type_ptr.hpp>

Axes::Axes() 
{}

void Axes::init() {

	static const float lines[6][3] = {
		// X
		{ 0.0f, 0.0f, 0.0f }, 
		{ 1000.0f, 0.0f, 0.0f },
		// Y
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1000.0f, 0.0f }, 
		// Z
		{ 0.0f, 0.0f, 0.0f }, 
		{ 0.0f, 0.0f, 1000.0f }, 
	};

	static const float line_colors[6][3] = {
		{ 1.f, 0.0f, 0.0f }, 
		{ 1.f, 0.0f, 0.0f },
		{ 0.f, 1.0f, 0.0f },
		{ 0.f, 1.0f, 0.0f },
		{ 0.f, 0.0f, 1.0f },
		{ 0.f, 0.0f, 1.0f }
	};

	prog = ShaderManager::Instance().loadProgram("shaders/axes.vert.glsl", "shaders/axes.frag.glsl");
	mesh.setMeshMode(GL_LINES);
	mesh.setNumVertices(6);
	mesh.setPosition(lines[0]);
	mesh.setColor(line_colors[0]);
}

void Axes::render(RenderData &data)
{
	prog->use();
	prog->uniformMatrix4fv("projMatrix", data.projMatrix);
	prog->uniformMatrix4fv("viewMatrix", data.viewMatrix);

	glLineWidth(3.f);
	mesh.draw();
	glLineWidth(1.f);
}