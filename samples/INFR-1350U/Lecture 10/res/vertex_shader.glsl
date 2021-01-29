#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 outcolor;
out vec2 texUV;

uniform mat4 MVP;

void main() {
	outcolor = vertex_color;
	texUV = vertex_uv;

	gl_Position = MVP * vec4(vertex_pos, 1.0);
	
}
	