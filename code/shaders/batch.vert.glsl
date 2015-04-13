#version 150

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 fnormal;
out vec2 ftexcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main () {
    fnormal = normal;
    ftexcoord = texcoord;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}

