#version 400

layout(location = 0) in vec3 vertex_position;
uniform mat4 proj, rotview;
out vec3 texcoords;

void main () {
	texcoords = vertex_position;
	//gl_Position = proj * rotview * vec4 (vertex_position, 1.0);
	gl_Position = proj * rotview * vec4 (vertex_position, 1.0);
	//gl_Position = vec4(vertex_position, 1.0);
}
