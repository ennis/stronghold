#version 150

in vec2 ftexcoord;
in vec3 fposition;	// world-space coordinates
in vec3 fnormal;
out vec4 outFragColor;

uniform sampler2D texFlat;
uniform sampler2D texSlope;
uniform int enableAmbientOcclusion;
uniform sampler2D ambientOcclusion;

uniform sampler2D texFlatBump;
uniform sampler2D texSlopeBump;

uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 ambientColor;

const float tex_scale_slope = 1.f/100.f;
const float tex_scale_flat = 1.f/1.f;

void main() 
{
	// Determine the blend weights for the 3 planar projections.  
// N_orig is the vertex-interpolated normal vector.  
	vec3 blend_weights = abs(fnormal);   // Tighten up the blending zone:  
	blend_weights = (blend_weights - 0.5);  
	blend_weights = max(blend_weights, 0);      // Force weights to sum to 1.0 (very important!)  
	blend_weights /= vec3(blend_weights.x + blend_weights.y + blend_weights.z );  
	
	vec4 blended_color; // .w hold spec value  
	vec3 blended_bump_vec;  

		// Compute the UV coords for each of the 3 planar projections.  
	// tex_scale (default ~ 1.0) determines how big the textures appear.  
	vec2 coord1 = fposition.yz * tex_scale_slope;  
	vec2 coord2 = fposition.zx * tex_scale_flat;  
	vec2 coord3 = fposition.xy * tex_scale_slope;  
	// This is where you would apply conditional displacement mapping.  
	//if (blend_weights.x > 0) coord1 = . . .  
	//if (blend_weights.y > 0) coord2 = . . .  
	//if (blend_weights.z > 0) coord3 = . . .  
	// Sample color maps for each projection, at those UV coords.  
	vec4 col1 = texture(texSlope, coord1);  
	vec4 col2 = texture(texFlat, coord2);  
	vec4 col3 = texture(texSlope, coord3);  
	// Sample bump maps too, and generate bump vectors.  
	// (Note: this uses an oversimplified tangent basis.)  
	vec2 bumpFetch1 = texture(texSlopeBump, coord1).xy - 0.5;
	vec2 bumpFetch2 = texture(texFlatBump, coord2).xy - 0.5;
	vec2 bumpFetch3 = texture(texSlopeBump, coord3).xy - 0.5;
	vec3 bump1 = vec3(0, bumpFetch1.x, bumpFetch1.y);  
	vec3 bump2 = vec3(bumpFetch2.y, 0, bumpFetch2.x);  
	vec3 bump3 = vec3(bumpFetch3.x, bumpFetch3.y, 0);  
	// Finally, blend the results of the 3 planar projections.  
	blended_color = col1.xyzw * blend_weights.xxxx +  
	                col2.xyzw * blend_weights.yyyy +  
	                col3.xyzw * blend_weights.zzzz;
    vec3 blended_bump = bump1 * blend_weights.xxx +  
					   bump2 * blend_weights.yyy +  
					   bump3 * blend_weights.zzz;  

	// Apply bump vector to vertex-interpolated normal vector.  
	vec3 N_for_lighting = normalize(fnormal + blended_bump); 

	vec3 L = sunDirection;
	vec3 N = N_for_lighting;

	float cosTheta = max(dot(N, -L), 0.0);

	if (enableAmbientOcclusion == 1) {
		float ao = texture(ambientOcclusion, ftexcoord).r;
		outFragColor = ao * blended_color * (vec4(ambientColor, 1.f) + vec4(sunColor, 1.f) * cosTheta);
	} else {
		outFragColor = blended_color * (vec4(ambientColor, 1.f) + vec4(sunColor, 1.f) * cosTheta);
	}

	//outFragColor = vec4(fnormal, 1.f);
	outFragColor.a = 1.f;

	//outFragColor = vec4((normalize(fnormal)+vec3(1.f)) / 2, 1.f);
}

