#version 410
in vec3 color;
in vec2 texUV;
in vec3 world_pos;
in vec3 world_normal;

uniform vec3 light_pos;
uniform vec3 eye_pos;

out vec4 frag_color;

uniform sampler2D myTextureSampler;

const float lightIntensity = 10.0;

void main() {
	
	vec3 L = normalize(light_pos - world_pos);
	vec3 V = normalize(eye_pos - world_pos);

	float dist = length(light_pos - world_pos);

	float diffuse = max(0, dot(L, world_normal));
	vec3 diffuseOut = (diffuse * color) / (dist*dist);
	
	diffuseOut = diffuseOut*lightIntensity;
	vec3 result = diffuseOut;

	//frag_color = texture(myTextureSampler, texUV)* vec4(result, 1.0);
	
	frag_color = vec4(result, 1.0);
}