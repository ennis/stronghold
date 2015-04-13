#version 150

in vec3 fnormal;
in vec2 ftexcoord;

out vec4 outFragColor;

// texture arrays
uniform sampler2D diffuseMap;
uniform sampler2D bumpMap;
uniform int hasDiffuseMap;
uniform int hasBumpMap;

// lighting
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 ambientColor;

const float tex_scale = 1.f/5.f;


// shadowmap ? 


void main () {
	vec2 bump;
	if (hasBumpMap == 1) {
		bump = texture(bumpMap, tex_scale * ftexcoord).xy - 0.5;
	} else {
		bump = vec2(0,0);
	}
	// TODO tangent basis
	vec3 up = vec3(0, 1, 0);
	vec3 T = normalize(cross(fnormal, up));
	vec3 S = normalize(cross(fnormal, T));
	vec3 vbump = bump.x * T + bump.y * S;
    vec3 N = normalize(fnormal + vbump); 
	vec3 L = sunDirection;

    float cosTheta = max(dot(N, -L), 0.0);

    vec4 color = texture(diffuseMap, tex_scale * ftexcoord);
    //vec4 color = vec4(ftexcoord.x, ftexcoord.y, 0, 1);

	outFragColor = color * (vec4(ambientColor, 1.f) + vec4(sunColor, 1.f) * cosTheta);
	outFragColor.a = 1.f;
}

