#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform2.hpp>

//
// Repr�sente les diff�rents types de formes manipul�s lors de l'ex�cution des r�gles
//
enum ShapeType
{
	ST_Polygon,
	ST_Prism,
	ST_Cuboid,
	ST_Trapezoid,
	ST_Trapezoid3D,
	ST_Rampart,
	ST_Dummy,
	ST_Circle,
	ST_Rectangle,
	ST_Cylinder,
    ST_Model,
    ST_Tetrahedron,
    ST_Cone,
	ST_Point
};

class Shape;

struct CoordSystem
{
	CoordSystem(
		CoordSystem const &parent_,
		glm::vec3 const &position_,
		glm::quat const &rotation_,
		float shear_);

	CoordSystem(
		glm::vec3 const &position_ = glm::vec3(),
		glm::quat const &rotation_ = glm::quat(),
		float shear_ = 0.f);

	glm::vec3 position;
	// plus compact qu'une matrice3x3
	glm::quat rotation;
	// coef. cisaillement (XZ)
	float shear;
	// parent
	CoordSystem const *parent;

	glm::vec3 shearVector(glm::vec3 const& v) const;

	// on pourrait utiliser des coordonn�es homog�nes. A voir
	// vecteurs
	glm::vec2 rotate(glm::vec2 const &v) const;

	glm::vec3 rotate(glm::vec3 const &v) const;

	// points 
	glm::vec2 transform(glm::vec2 const &p) const;

	glm::vec3 transform(glm::vec3 const &p) const;

	glm::quat getFinalRotation() const;

	glm::vec3 getFinalPosition() const;

	CoordSystem transform(CoordSystem const & cs) const;

	static CoordSystem fromParent(
		CoordSystem const &cs,
		glm::vec3 const &position_ = glm::vec3(),
		glm::quat const &rotation_ = glm::quat(),
		float shear_ = 0.f);

	// TODO inverse
	static CoordSystem fromVectors(
		glm::vec3 const &position,
		glm::vec3 const &u0,
		glm::vec3 const &v0,
		float shear = 0.f);

	static CoordSystem fromVectors(
		CoordSystem const &parent,
		glm::vec3 const &position,
		glm::vec3 const &u0,
		glm::vec3 const &v0,
		float shear = 0.f);

};

//
// Repr�sente une forme manipul�e par les r�gles de la grammaire
//
class Shape
{
protected:
	// r�f�rentiel du volume (position et orientation)
	//
	// Chaque forme a un r�f�rentiel qui lui est propre. 
	// Voir chaque sous-classe pour savoir comment il est d�fini

	// transformation local->world
	CoordSystem coordSystem;

	// type de la forme
	ShapeType shapeType;

public:
	Shape(ShapeType volumeType_,
		CoordSystem const &cs) : shapeType(volumeType_), coordSystem(cs)
	{
		index = 0;
	}

	Shape(ShapeType volumeType_,
		glm::vec3 const &position_ = glm::vec3(),
		glm::quat const &rotation_ = glm::quat()) : shapeType(volumeType_), coordSystem(position_, rotation_)
	{
        index = 0;
        tower = false;
    }
	
	//
	// Obtient le type de la forme
	// 
	// Utile pour les op�rations ne s'ex�cutant que sur certains types de formes
	ShapeType getShapeType() const {
		return shapeType;
	}

	CoordSystem const &getCoordSystem() const
	{
		return coordSystem;
	}

	CoordSystem &getCoordSystem()
	{
		return coordSystem;
	}

    // Flags pour les contraintes
    int index;
    bool tower;
};

#endif
