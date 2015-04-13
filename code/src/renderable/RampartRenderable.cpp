#include "RampartRenderable.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

RampartRenderable::RampartRenderable(Trapezoid2D const &base_,
	float base_ha_, float base_hb_,
	float wall_ha_, float wall_hb_){
	Trapezoid2D base = base_;
	float base_ha = base_ha_;
	float base_hb = base_hb_;
	float wall_ha = wall_ha_;
	float wall_hb = wall_hb_;

	CreateVertices();
	this->texture = TextureManager::Instance().loadTexture("textures/brick_wall.jpg");

	// load shaders
	prog = ShaderManager::Instance().loadProgram("shaders/polygon.vert.glsl",
		"shaders/polygon.frag.glsl");
}

void RampartRenderable::CreateVertices(){
	vertices[0] = glm::vec3(base.sommets[0].x, base_ha, base.sommets[0].y);
	vertices[1] = glm::vec3(base.sommets[1].x, base_hb, base.sommets[1].y);
	vertices[2] = glm::vec3(base.sommets[2].x, base_hb, base.sommets[2].y);
	vertices[3] = glm::vec3(base.sommets[3].x, base_ha, base.sommets[3].y);
	vertices[4] = glm::vec3(base.sommets[0].x, base_ha + wall_ha, base.sommets[0].y);
	vertices[5] = glm::vec3(base.sommets[1].x, base_ha + wall_hb, base.sommets[1].y);
	vertices[6] = glm::vec3(base.sommets[2].x, base_ha + wall_hb, base.sommets[2].y);
	vertices[7] = glm::vec3(base.sommets[3].x, base_ha + wall_ha, base.sommets[3].y);

	// tableau de vertices pour le mesh
	std::vector<glm::vec3> geom;
	// tableau d'indices
	std::vector<glm::ivec3> indices;
	// tableau de coordonnée de textures pour le mesh
	std::vector<glm::vec2> texcoords;
	// tableau de normales
	//std::vector<glm::vec3> normals;

	//bottom : vertices
	geom.push_back(vertices[0]);
	geom.push_back(vertices[1]);
	geom.push_back(vertices[2]);
	geom.push_back(vertices[3]);
	//bottom : textures
	texcoords.push_back(glm::vec2(1, 1));
	texcoords.push_back(glm::vec2(0, 1));
	texcoords.push_back(glm::vec2(0, 0));
	texcoords.push_back(glm::vec2(1, 0));
	//bottom: indices 2 triangles
	indices.push_back(glm::ivec3(0, 3, 1));
	indices.push_back(glm::ivec3(1, 3, 2));

	//top: vertices
	geom.push_back(vertices[4]);
	geom.push_back(vertices[5]);
	geom.push_back(vertices[6]);
	geom.push_back(vertices[7]);
	//top: textures
	texcoords.push_back(glm::vec2(0, 1));
	texcoords.push_back(glm::vec2(1, 1));
	texcoords.push_back(glm::vec2(1, 0));
	texcoords.push_back(glm::vec2(0, 0));
	//top: indices 2 triangles
	indices.push_back(glm::ivec3(4, 7, 5));
	indices.push_back(glm::ivec3(5, 7, 6));

	//all sides but one
	for (int i = 0; i < 4 - 1; ++i) {
		//vertices
		geom.push_back(vertices[i]);
		geom.push_back(vertices[i + 1]);
		geom.push_back(vertices[i + 4]);
		geom.push_back(vertices[i + 4 + 1]);
		//texturecoords
		texcoords.push_back(glm::vec2(1, 0));
		texcoords.push_back(glm::vec2(0, 0));
		texcoords.push_back(glm::vec2(1, 1));
		texcoords.push_back(glm::vec2(0, 1));
		//indices 2 triangles
		indices.push_back(glm::ivec3(8 + i * 4,
			8 + i * 4 + 1,
			8 + i * 4 + 2));
		indices.push_back(glm::ivec3(8 + i * 4 + 2,
			8 + i * 4 + 1,
			8 + i * 4 + 3));
	}
	//last side: vertices
	geom.push_back(vertices[0]); // indice 8 + 3*4
	geom.push_back(vertices[3]); // indice 8 + 3*4 + 1
	geom.push_back(vertices[4]); // indice 8 + 3*4 + 2
	geom.push_back(vertices[7]); // indice 8 + 3*4 + 3
	//texturecoords
	texcoords.push_back(glm::vec2(0, 0));
	texcoords.push_back(glm::vec2(1, 0));
	texcoords.push_back(glm::vec2(0, 1));
	texcoords.push_back(glm::vec2(1, 1));
	//last side: indices 2 triangles
	indices.push_back(glm::ivec3(8 + 3 * 4 + 1,
		8 + 3 * 4,
		8 + 3 * 4 + 3));
	indices.push_back(glm::ivec3(8 + 3 * 4 + 3,
		8 + 3 * 4,
		8 + 3 * 4 + 2));

	//Remplissage du mesh
	mesh.setNumVertices(geom.size());
	mesh.setPosition(glm::value_ptr(*geom.data()));
	mesh.setTexCoord(glm::value_ptr(*texcoords.data()));
	mesh.setIndices(glm::value_ptr(*indices.data()), 24);


	//TODO: ajouter les normales
}

void RampartRenderable::render(RenderData const &data){
	glm::mat4 mvp = data.projMatrix * data.viewMatrix;
	glm::vec4 color = glm::vec4(1.f, 0.f, 0.f, 0.5f);

	prog->use();
	prog->uniformMatrix4fv("viewProjMatrix", mvp);
	prog->uniform4f("color", color);


	/* Bind texture */
	this->texture->bind();

	/* Draw faces */
	mesh.draw();
}