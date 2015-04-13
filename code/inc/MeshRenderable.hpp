#ifndef MESHRENDERABLE_HPP
#define MESHRENDERABLE_HPP

#include <Renderable.hpp>
#include <Mesh.hpp>
#include <Texture.hpp>
#include <vector>
#include <map>
#include <iosfwd>
#include <Shape.hpp>

class MeshRenderable : public Renderable
{
public:
	MeshRenderable();
	~MeshRenderable();

	// TODO textures
	void setColor(glm::vec4 const &color);

	//
	// NULL if no bump map
	void beginBatch(Texture *texture, Texture * texBumpMap = NULL);
	void endBatch();

	//
	// Ajoute une face
	void addTriangle(
			glm::vec3 const &p1, glm::vec3 const &p2, glm::vec3 const &p3,
			glm::vec2 const &t1, glm::vec2 const &t2, glm::vec2 const &t3);

	//
	// Ajoute une face
	// et calcule automatiquement les coordonnées de texture à partir de la position
	void addTriangle(
		glm::vec3 const &p1, glm::vec3 const &p2, glm::vec3 const &p3, CoordSystem const &cs = CoordSystem());


	//
	// Rendu
	void render(RenderData const &data);

	//
	// export vers OBJ
	void exportToOBJ(std::ostream &os);

	void clear();

private:
	void initRender();

	struct VertexBatch
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texcoords;
		// finalized mesh
		Mesh mesh;
		// couleur unie
		glm::vec4 color;
	};

	struct VertexBatchIndex
	{
		Texture *diffuseMap;
		Texture *bumpMap;

		inline bool operator<(VertexBatchIndex const &v2) const
		{
			return (diffuseMap < v2.diffuseMap) || (diffuseMap == v2.diffuseMap && bumpMap < v2.bumpMap);
		}
	};


	// batch by texture/bump map pair
	std::map<VertexBatchIndex, VertexBatch*> batches;
	VertexBatch *currentBatch;

	bool renderInited;
	Mesh mesh;
	Program *prog;
};

#endif