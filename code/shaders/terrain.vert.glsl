#version 150
//layout (location = 0) in vec2 position;
in vec2 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform sampler2D heightmap;
uniform vec2 offset;	// tile coordinates
uniform vec2 size;

out vec2 ftexcoord;
out vec3 fposition;
out vec3 fnormal;
out vec3 fnormal_cs;
out vec3 fposition_cs;

const float scale = 200.f;
/*
	const double p=double(TS)*0.001;

	double dx,dy;

	dx=getTerrainHeight(x+p,y)-getTerrainHeight(x-p,y);
	dy=getTerrainHeight(x,y+p)-getTerrainHeight(x,y-p);

	t[6]=-dx;//norx
	t[7]=2.0*p;//nory
	t[8]=-dy;//norz
*/


float height(vec2 pos)
{
	return scale * texture(heightmap, pos/size).r;
}

void main()
{
	const float ddp = 1.0;
	const vec2 dx = vec2(1.0, 0);
	const vec2 dy = vec2(0, 1.0);

	// position in 0..1
	// offset = tile coords (chunks of 64 meters)
	vec2 pos = offset * 64 + position * 64;

	vec2 dd = vec2(height(pos+dx)-height(pos-dx),
					height(pos+dy)-height(pos-dy));

	vec3 normal = normalize(vec3(-dd.x, 2.0*ddp, -dd.y));

	vec4 disp = vec4(pos.x, height(pos), pos.y, 1.f);
    gl_Position = projMatrix * viewMatrix * modelMatrix * disp;
    mat4 modelView = viewMatrix * modelMatrix;

    fposition = (modelMatrix * disp).xyz;
    fnormal = normal;
	ftexcoord = pos/size;

    fposition_cs = (viewMatrix * modelMatrix * disp).xyz;
    fnormal_cs = normalize((viewMatrix * modelMatrix * vec4(normal, 0.f)).xyz);
}

