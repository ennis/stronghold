#version 150
in vec3 fcolor;

out vec4 outFragColor;

void main() {
	outFragColor = vec4(fcolor, 1.f);
}

