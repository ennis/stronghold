#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <OpenGL.hpp>
#include <RenderData.hpp>

//
// Représente un objet qui pourra être dessiné à l'écran.
// L'idée: on fait un première passe sur la grammaire qui créé des objets
// de type 'Renderable' et les met dans une liste
// Ensuite, on appelle la méthode 'render' sur chaque élément de la liste
//
// Les éléments 'Renderable' sont:
// Tous les Terminals:
//		- Box
//		- Wall
//		- OBJModel
//
// Le terrain
// La skybox
// Les volumes manipulés 
//
// NB: Les non-terminaux ne sont pas censés être dessinés (les volumes associés oui, cependant)
//
//
class Renderable
{
public:
	virtual void render(RenderData const &data) = 0;

    virtual ~Renderable () {};
	// TODO bounding box?
};

#endif
