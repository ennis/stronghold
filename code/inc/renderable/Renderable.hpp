#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <OpenGL.hpp>
#include <RenderData.hpp>

//
// Repr�sente un objet qui pourra �tre dessin� � l'�cran.
// L'id�e: on fait un premi�re passe sur la grammaire qui cr�� des objets
// de type 'Renderable' et les met dans une liste
// Ensuite, on appelle la m�thode 'render' sur chaque �l�ment de la liste
//
// Les �l�ments 'Renderable' sont:
// Tous les Terminals:
//		- Box
//		- Wall
//		- OBJModel
//
// Le terrain
// La skybox
// Les volumes manipul�s 
//
// NB: Les non-terminaux ne sont pas cens�s �tre dessin�s (les volumes associ�s oui, cependant)
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
