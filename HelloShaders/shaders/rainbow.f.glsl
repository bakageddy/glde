#version 330 core

in vec3 vertex_output;
out vec4 frag_color;

void main() {
	frag_color = vec4(vertex_output, 0.0f, 1.0f);
}
