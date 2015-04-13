#version 150

in vec3 position;
in vec3 normal;

out vec3 fnormal;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main () {
    fnormal = normal;
    gl_Position = projMatrix * viewMatrix * vec4(position, 1.0f);
}

