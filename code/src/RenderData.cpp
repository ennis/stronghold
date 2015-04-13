#include <RenderData.hpp>

void setShaderUniforms(Program *p, RenderData const &rd)
{
	p->uniformMatrix4fv("viewMatrix", rd.viewMatrix);
	p->uniformMatrix4fv("projMatrix", rd.projMatrix);
	p->uniform3f("sunDirection", rd.sunDirection);
	p->uniform3f("sunColor", rd.sunColor);
	p->uniform3f("ambientColor", rd.ambientColor);
	p->uniform2f("viewportSize", rd.viewportSize);
	p->uniform3f("cameraPos", rd.cameraPos);
}