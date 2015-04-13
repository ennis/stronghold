#version 150
in vec3 position;
out vec3 fposition;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

// passthrough shader
void main() {
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    fposition =  (modelMatrix * vec4(position, 1.0)).xyz;
}