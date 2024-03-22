#version 330 core

layout (location = 0) in vec3 pos;

uniform float angle;

out vec3 vertex_output;

void main() {
	float shift = radians(angle);
	vec3 transform;
	transform.x = (pos.x * cos(shift)) - (pos.y * sin(shift));
	transform.y = (pos.x * sin(shift)) - (pos.y * cos(shift));
	transform.x *= 1.5;
	transform.y *= 1.5;
	gl_Position = vec4(transform, 1.0f);
	vertex_output = transform;
}
