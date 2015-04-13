#include "Rampart.hpp"

#include <vector>

Rampart::Rampart(Rectangle2D const &base_,
	float base_ha_, float base_hb_,
	float wall_ha_, float wall_hb_) : Shape(ST_Rampart), base(base_)
{
	coordSystem = base_.getCoordSystem();
	base_ha = base_ha_;
	base_hb = base_hb_;
	wall_ha = wall_ha_;
	wall_hb = wall_hb_;
}
