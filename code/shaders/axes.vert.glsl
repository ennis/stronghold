#version 150
in vec3 position;
in vec3 color;

out vec3 fcolor;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * viewMatrix * vec4(position, 1.f);
	fcolor = color;
}