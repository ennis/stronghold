#version 150

in vec3 fnormal;

out vec4 outFragColor;

uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 ambientColor;

uniform vec4 color;


void main () {
	vec3 L = sunDirection;
	vec3 N = fnormal;

    float cosTheta = max(dot(N, -L), 0.0);
	outFragColor = color * (vec4(ambientColor, 1.f) + vec4(sunColor, 1.f) * cosTheta);
	outFragColor.a = 1.f;
}

