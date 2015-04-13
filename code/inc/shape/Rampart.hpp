#ifndef RAMPART_HPP
#define RAMPART_HPP

#include <Shape.hpp>
#include <Trapezoid2D.hpp>
#include <RenderData.hpp>
#include <Mesh.hpp>
#include "Rectangle2D.hpp"

//
// Un rampart (une face de la muraille)
// Concrètement, c'est presque un prisme à base rectangulaire
// sauf que les deux extrémités du mur peuvent ne pas être à la même hauteur
//
class Rampart : public Shape
{
public:
	Rampart(Rectangle2D const &base_,
			float base_ha_, float base_hb_,
			float wall_ha_, float wall_hb_);

	Rampart() : Shape(ST_Rampart) {}

	Rectangle2D base;
	float base_ha;
	float base_hb;
	float wall_ha;
	float wall_hb;

	/*   P0,P4 ____________ P1,P5
	*	  	  /            \                      
	*	     /              \                     
	*	    /                \                    
	*  A   /                  \   B     
	*	  /                    \                  
	*    /                      \                 
	*   /                        \                
	*  /__________________________\      
	* P3,P7                       P2,P6
	*
	*	  _______
	*    |  ^    \________    
	* A  |  |              \_______  
	*    |  | wall_ha             |       ^
	*    |  |                     |  B    | wall_hb
	*    |  v         ____________|       v
	*    |___________/           ^
	*     ^                      |
	*     | base_ha              |  base_hb
	*     v                      v
	*  _____________________________  height 0
	*/
};

#endif