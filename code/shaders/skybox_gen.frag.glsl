#version 150

in vec2 ftexcoord;
in vec3 fposition;	// world-space coordinates

out vec4 outFragColor;

uniform mat4 viewMatrix;
uniform vec3 cameraPos;
uniform vec2 viewportSize;
uniform vec3 sunColor;
uniform vec3 sunDirection;
const vec3 skyColor = vec3(
	0.18867780436772762, 
	0.4978442963618773, 
	0.6616065586417131);


void main() 
{
	// eye ray in world space
	vec3 ray = normalize(fposition - cameraPos);

	// 
	float ext = pow(max(0, dot(ray, -sunDirection)), 2);
	float scatter = smoothstep(0.1, 0.3, (1.f - sunDirection.y) / 2);
	vec3 color = sunColor * ext + skyColor * scatter;
	outFragColor = vec4(color, 1.f);
}

