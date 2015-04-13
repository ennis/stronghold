#ifndef TRAPEZOID2D_HPP
#define TRAPEZOID2D_HPP

/*      P1 ____________ P2
*	  	  /            \                      ^
*	     /              \                     |                    
*	    /                \                    |
* a->  /------------------\<-b                |                    
*	  /                    \                  | hauteur
*    /                      \                 |                    
*   /<-aAngle                \                |
*  /__________________________\<-bAngle       v                    
* P4                         P3
*/


#include <Shape.hpp>

class Trapezoid2D: public Shape {
public:
    glm::vec2 a, b;
    double aAngle, bAngle;
    double hauteur;
    glm::vec2 sommets[4];
    Trapezoid2D();
    Trapezoid2D(glm::vec2 const &a, glm::vec2 const &b, double aA, double bA, double height);
};

#endif
