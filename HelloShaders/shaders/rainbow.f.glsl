#version 330 core

in vec3 vertex_output;
out vec4 frag_color;

void main() {
	frag_color = vec4(
					sin(vertex_output.x) * vertex_output.x,
					cos(vertex_output.y) * vertex_output.y,
					tan(vertex_output.y + vertex_output.x) * vertex_output.z,
					1.0f
				);
}
