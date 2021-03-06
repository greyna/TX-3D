#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 per_vertex_text_coord;

uniform mat4 model, view, proj;

out vec3 position_eye, normal_eye; // position and normal in eye space
out vec2 text_coord;

void main() {
	text_coord = per_vertex_text_coord;

	position_eye = vec3 (view * model * vec4 (vertex_position, 1.0));
	normal_eye = normalize(vec3 (view * model * vec4 (vertex_normal, 0.0)));
	
	gl_Position = proj * vec4 (position_eye, 1.0);
}
