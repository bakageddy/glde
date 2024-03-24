#version 330 core

in vec3 vertex_output;
out vec4 frag_color;

void main() {
	float d = length(vertex_output);
	d = abs(d) * exp(-length(vertex_output));
	d = 0.2 / d;
	frag_color = vec4(
					d,
					d,
					d,
					1.0f
				);
}
