#version 150
in vec3 position;

uniform mat4 viewProjMatrix;

void main()
{
    gl_Position = viewProjMatrix * vec4(position, 1.f);
}